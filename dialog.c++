#include "dialog.h"
#include "window.h"

#include <gtkmm/dialog.h>

void dialog::error_dialog(window &win, std::string main, std::string secondary)
{
    Gtk::MessageDialog error_dialog(main, false,
                                    Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
    error_dialog.set_transient_for(win);
    error_dialog.set_secondary_text(secondary);
    error_dialog.run();
}

void dialog::about_dialog(window &win)
{
    Gtk::MessageDialog about_dialog("Program made by Mathieu Denglos", false,
                                    Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
    about_dialog.set_transient_for(win);
    about_dialog.set_secondary_text("Contact me on Linkedin if help is needed");
    about_dialog.run();
}

bool dialog::auto_save_dialog(window &win)
{
    Gtk::MessageDialog error_dialog("Do you want to activate auto save mode", false,
                                    Gtk::MESSAGE_INFO, Gtk::BUTTONS_YES_NO);
    error_dialog.set_transient_for(win);
    error_dialog.set_secondary_text("This can still be activated later");
    const int result = error_dialog.run();

    if (result == Gtk::RESPONSE_YES)
        return true;
    else
        return false;
}

std::string dialog::open_file_dialog(window &win)
{
    //Create the file chooser dialog in open mode
    Gtk::FileChooserDialog open_dialog("Openfile", Gtk::FILE_CHOOSER_ACTION_OPEN);
    open_dialog.set_transient_for(win);

    //add the buttons for the selection (Select to confirm and anything else to cancel)
    open_dialog.add_button("Select", Gtk::RESPONSE_OK);
    open_dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);

    //add a filter for .txt types (only file extension supported for the programm)
    Glib::RefPtr<Gtk::FileFilter> text_filter = Gtk::FileFilter::create();
    text_filter->set_name("text files");
    text_filter->add_mime_type("text/plain");
    open_dialog.add_filter(text_filter);

    //Run the file opener dialog
    const int result = open_dialog.run();

    //If Select button pressed return file_path, otherwise return ND
    if (result == Gtk::RESPONSE_OK)
        return open_dialog.get_filename();
    else
        throw action_canceled("file opening canceled");
}

std::string dialog::save_file_dialog(window &win)
{
    //Create the file chooser dialog in save mode
    Gtk::FileChooserDialog save_dialog("title", Gtk::FILE_CHOOSER_ACTION_SAVE);
    save_dialog.set_transient_for(win);

    //add the buttons for the selection (Select to confirm and anything else to cancel)
    save_dialog.add_button("Select", Gtk::RESPONSE_OK);
    save_dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);

    //add a filter for .txt types
    Glib::RefPtr<Gtk::FileFilter> text_filter = Gtk::FileFilter::create();

    //run the file opener dialog
    const int result = save_dialog.run();

    //if Select button pressed return file_path, otherwise return ND
    if (result == Gtk::RESPONSE_OK)
        return save_dialog.get_filename();
    else
        throw action_canceled("file saving canceled");
}