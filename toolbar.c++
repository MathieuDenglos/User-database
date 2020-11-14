#include "toolbar.h"
#include "dialog.h"

#include <string>
#include <fstream>
#include <iostream>

toolbar::toolbar() : Gtk::Application("org.gtkmm.example")
{
    //The toolbar constructor is only used to set the name of the application
    Glib::set_application_name("User database");
}

Glib::RefPtr<toolbar> toolbar::create()
{
    //The create method is called by the main function
    //It returns a Glib::RefPtr (smart pointer) of the toolbar class
    return Glib::RefPtr<toolbar>(new toolbar());
}

void toolbar::on_startup()
{
    //This method is called on the startup of the application
    //It is used to create the toolbar

    //Call the base class implementation:
    Gtk::Application::on_startup();

    //Layout the actions in a menubar and an application menu
    //This will contain the main elements
    toolbar_builder = Gtk::Builder::create();

    //Grab the toolbar info from toolbar-ui.xml
    std::ifstream file("toolbar-ui.xml");

    //If the file is not detected
    if (!file.is_open())
    {
        dialog::error_dialog(*win, "Couldn't build the tool bar",
                             "toolbar-ui.xml is unfindable");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string ui_info(buffer.str());

    //Add the XML part of the code in the application
    try
    {
        toolbar_builder->add_from_file("toolbar-ui.xml");
    }
    catch (const Glib::Error &ex)
    {
        //Can't show the error dialog dialog yet (win* not created yet)
        std::cerr << "couldn't make the toolbar" << ex.what() << std::endl;
    }

    //Get the menubar and the app menu, and add them to the application:
    Glib::RefPtr<Glib::Object> object = toolbar_builder->get_object("appmenu");
    Glib::RefPtr<Gio::Menu> menu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    set_menubar(menu);

    //make the actions the following option available for the user at the begining
    //If add_action(); method not called, the options will be visible but not selectionable
    add_action("openfile",
               sigc::mem_fun(*this, &toolbar::on_open_file));
    add_action("about",
               sigc::mem_fun(*this, &toolbar::on_about));
    add_action("newfile",
               sigc::mem_fun(*this, &toolbar::on_new_file));
}

void toolbar::on_activate()
{
    //Show the window with the treeview (userlist/warnlist)
    //we only call it for the window because it stores everything, multiple elements can be called
    //The window will be stored in the toolbar, the access to the different datas will therefore be easier
    win = new window();

    //Make sure that the application runs for as long this window is still open
    add_window(*win);

    //Delete the window when it is hidden, to prevent memory leak.
    win->signal_hide().connect(sigc::bind<Gtk::Window *>(
        sigc::mem_fun(*this, &toolbar::on_window_hide), win));

    //Show the window
    win->show_all();

    //We need to hide both frames, we'll make them visible once we have data to show
    win->set_user_visibility(false);
    win->set_warn_visibility(false);
}

void toolbar::on_window_hide(Gtk::Window *window)
{
    //delete all the elements
    delete window;
}

//All the signal handlers for the files managment (contained in the file.c++)
void toolbar::on_open_file()
{
    //Call the file_opener method first to grab the data from a file
    //Then change the visibility settings depending on if users/warns/a file got added
    bool visibility = files.file_opener(*win);
    visibility_settings(visibility);
}

void toolbar::on_new_file()
{
    visibility_settings(true);
}

void toolbar::on_add_file()
{
    //Call the file_opener method first to grab the data from a file
    //Then change the visibility settings depending on if users/warns/a file got added
    bool visibility = files.file_opener(*win);
    visibility_settings(visibility);
}

void toolbar::visibility_settings(bool visibility)
{
    //If the list got its first element : Make the treeview appear to the user,
    //activate the elements of the userlist and warnlist section of the toolbar
    //Make all the menu options visible
    //Ask if the user want to turn on autosave mode
    if (visibility)
    {
        //First activate all the menu options for the userlist
        toolbar_user_list = add_action_bool("showuserlist",
                                            sigc::mem_fun(*this, &toolbar::on_show_user_list), true);
        toolbar_tag = add_action_bool("tag",
                                      sigc::mem_fun(*this, &toolbar::on_tag), true);
        toolbar_ID = add_action_bool("id",
                                     sigc::mem_fun(*this, &toolbar::on_ID), true);
        toolbar_birthday = add_action_bool("birthday",
                                           sigc::mem_fun(*this, &toolbar::on_birthday), true);
        toolbar_other = add_action_bool("other",
                                        sigc::mem_fun(*this, &toolbar::on_other), true);

        //Then activate all the menu options for the warnlist
        toolbar_warn_list = add_action_bool("showwarnlist",
                                            sigc::mem_fun(*this, &toolbar::on_show_warn_list), true);
        toolbar_reason = add_action_bool("reason",
                                         sigc::mem_fun(*this, &toolbar::on_reason), true);

        //Finish by showing everything to the user
        win->set_user_visibility(true);
        win->set_warn_visibility(true);

        //add the save option, the add file and remove the open file and blank file option
        add_action("savefile", sigc::bind<bool>(
                                   sigc::mem_fun(*this, &toolbar::on_save_file), false));
        add_action("savefileas", sigc::bind<bool>(
                                     sigc::mem_fun(*this, &toolbar::on_save_file), true));
        add_action("addfile",
                   sigc::mem_fun(*this, &toolbar::on_add_file));
        add_action("birthdaylist",
                   sigc::mem_fun(*this, &toolbar::on_show_birthday));
        auto_save = add_action_bool("autosave",
                                    sigc::mem_fun(*this, &toolbar::on_auto_save), false);
        remove_action("openfile");
        remove_action("newfile");

        //Ask the user whether they want to activate auto save
        if (dialog::auto_save_dialog(*win))
        {
            files.file_saver(*win, false);
            auto_save->change_state(files.is_output_file());
            win->auto_save_mode_on = files.is_output_file();
        }
    }
}

void toolbar::on_save_file(bool saveas)
{
    //A signal handler used to save the file
    files.file_saver(*win, saveas);
}

void toolbar::on_auto_save()
{
    //gets the current states, verify if an output files already exist, if not open the file saver dialog
    //invert the status, except if the chosen output file wasn't correct
    bool state = false;
    toolbar_tag->get_state(state);
    files.file_saver(*win, false);
    state = !state;
    toolbar_tag->change_state(state && files.is_output_file());
    win->auto_save_mode_on = (state && files.is_output_file());
}

//User treeview visibility settings (menu bar section)
void toolbar::on_show_user_list()
{
    bool state = false;
    //Get the actual state and send it to the .change_visibility() method
    //This will invert the state of the button (which is not automatic)
    //And will show/hide the treeview
    toolbar_user_list->get_state(state);
    state = !state;
    toolbar_user_list->change_state(state);
    win->set_user_visibility(state);

    //Now will verify if the treeview is visible, and will show/hide the proper options
    if (state)
    {
        toolbar_tag->get_state(state);
        toolbar_tag = add_action_bool("tag",
                                      sigc::mem_fun(*this, &toolbar::on_tag), state);
        toolbar_ID->get_state(state);
        toolbar_ID = add_action_bool("id",
                                     sigc::mem_fun(*this, &toolbar::on_ID), state);
        toolbar_birthday->get_state(state);
        toolbar_birthday = add_action_bool("birthday",
                                           sigc::mem_fun(*this, &toolbar::on_birthday), state);
        toolbar_other->get_state(state);
        toolbar_other = add_action_bool("other",
                                        sigc::mem_fun(*this, &toolbar::on_other), state);
    }
    else
    {
        remove_action("tag");
        remove_action("id");
        remove_action("birthday");
        remove_action("other");
    }
}

void toolbar::on_tag()
{
    //Similar as the on_showuserlist but with the tag
    bool state = false;
    toolbar_tag->get_state(state);
    state = !state;
    toolbar_tag->change_state(state);
    win->get_user_list().change_visibility(USER_DATA::TAG, get_user_status(), state);
}

void toolbar::on_ID()
{
    //Similar as the on_showuserlist but with the ID
    bool state = false;
    toolbar_ID->get_state(state);
    state = !state;
    toolbar_ID->change_state(state);
    win->get_user_list().change_visibility(USER_DATA::ID, get_user_status(), state);
}

void toolbar::on_birthday()
{
    //Similar as the on_showuserlist but with the birthday
    bool state = false;
    toolbar_birthday->get_state(state);
    state = !state;
    toolbar_birthday->change_state(state);
    win->get_user_list().change_visibility(USER_DATA::BIRTHDAY, get_user_status(), state);
}

void toolbar::on_other()
{
    //Similar as the on_showuserlist but with the misc elements (introduction/review)
    bool state = false;
    toolbar_other->get_state(state);
    state = !state;
    toolbar_other->change_state(state);
    win->get_user_list().change_visibility(USER_DATA::OTHER, get_user_status(), state);
}

std::array<bool, 4> toolbar::get_user_status()
{
    bool visible;
    std::array<bool, 4> status = {false, false, false, false};

    toolbar_tag->get_state(status[0]);
    toolbar_ID->get_state(status[1]);
    toolbar_birthday->get_state(status[2]);
    toolbar_other->get_state(status[3]);
    return status;
}

//Warn treeview visibility settings (menu bar section)
void toolbar::on_show_warn_list()
{
    //Get the actual state and send it to the .set_warn_visibility() method
    //This will invert the state of the button (which is not automatic)
    //And will show/hide the treeview
    bool state = false;
    toolbar_warn_list->get_state(state);
    state = !state;
    win->set_warn_visibility(state);
    toolbar_warn_list->change_state(state);

    if (state)
    {
        toolbar_reason->get_state(state);
        toolbar_reason = add_action_bool("reason",
                                         sigc::mem_fun(*this, &toolbar::on_reason), state);
    }
    else
        remove_action("reason");
}

void toolbar::on_reason()
{
    //Similar as the on_showuserlist but with the misc elements (introduction/review)
    bool reason = false;
    toolbar_reason->get_state(reason);
    reason = !reason;
    win->get_warn_list().change_visibility();
    toolbar_reason->change_state(reason);
}

//Other section
void toolbar::on_show_birthday()
{
    //Call the method to show the birthday window (stored in win)
    win->get_birthday_window().show_birthday_window(*win, win->get_user_list());
}

void toolbar::on_about()
{
    //call the program to show the about dialog with basic infos about the app
    dialog::about_dialog(*win);
}