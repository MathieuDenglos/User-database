#include "warn_menu.h"
#include "user_list.h"
#include "warn_list.h"
#include "dialog.h"
#include "window.h"

warn_menu::warn_menu(window &win) : assistant(win)
{
    //creates all the items of the menu and add them to the menu
    Gtk::MenuItem *item;

    item = Gtk::manage(new Gtk::MenuItem("_Add warn", true));
    item->signal_activate().connect(sigc::bind<window &>(sigc::mem_fun(*this, &warn_menu::on_add_warn),
                                                         win));
    append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Edit warn", true));
    item->signal_activate().connect(sigc::bind<window &, warn_list &>(sigc::mem_fun(*this, &warn_menu::on_edit_warn),
                                                                      win, win.get_warn_list()));
    append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Remove warn", true));
    item->signal_activate().connect(sigc::bind<window &, warn_list &>(sigc::mem_fun(*this, &warn_menu::on_remove_warn),
                                                                      win, win.get_warn_list()));
    append(*item);

    //make the menu accessible only from the user treeview
    accelerate(win.get_warn_list());

    //all the signal handlers related to the assistant
    assistant.signal_apply().connect(sigc::bind<window &, warn_list &>(sigc::mem_fun(*this, &warn_menu::on_assistant_apply),
                                                                       win, win.get_warn_list()));

    assistant.signal_cancel().connect(sigc::mem_fun(*this,
                                                    &warn_menu::on_assistant_cancel));

    assistant.signal_close().connect(sigc::mem_fun(*this,
                                                   &warn_menu::on_assistant_cancel));

    show_all();
}

void warn_menu::on_add_warn(window &win)
{
    //tells the on_assistant_apply() that we're in add mode and launch the assistant
    edit_ID = 0;
    assistant.launch_assistant(win);
}

void warn_menu::on_edit_warn(window &win, warn_list &warns)
{
    if (warns.get_model()->children().size() != 0)
    {
        //tells the on_assistant_apply() that we're in edit mode and launch the assistant
        Gtk::TreeModel::Row warn_row = *(warns.get_selection()->get_selected());
        edit_ID = warn_row.get_value(warns.get_columns().ID);
        edit_warn_ID = warn_row.get_value(warns.get_columns().warn_ID);
        assistant.launch_assistant(win,
                                   warn_row[warns.get_columns().ID],
                                   warn_row[warns.get_columns().warn_ID],
                                   warn_row[warns.get_columns().reason]);
    }
    else
        dialog::error_dialog(win, "no warns to edit",
                             "please add warns before editing any");
}

void warn_menu::on_remove_warn(window &win, warn_list &warns)
{
    if (warns.get_model()->children().size() != 0)
    {
        //grab the selected warn and remove it
        long long ID = (*warns.get_selection()->get_selected())[warns.get_columns().ID];
        long long warn_ID = (*warns.get_selection()->get_selected())[warns.get_columns().warn_ID];

        //close the assistant if it's open in edit mode with the user
        if (edit_ID == ID && warn_ID == edit_warn_ID)
            on_assistant_cancel();

        //remove the warn (issues with .erase() method and invalid iters)
        Gtk::TreeModel::Children::iterator iter = warns.get_model()->children().begin();
        while ((iter->get_value(warns.get_columns().ID) != ID || iter->get_value(warns.get_columns().warn_ID) != warn_ID) && iter != warns.get_model()->children().end())
            iter++;

        if (iter != warns.get_model()->children().end())
            warns.get_model()->erase(iter);
    }
    else
        dialog::error_dialog(win, "no warns to remove",
                             "please add warns before removing them");
}

void warn_menu::on_assistant_apply(window &win, warn_list &warns)
{
    //Grab the data that the warnentered in the user_assistant
    std::string temp_username, temp_reason;
    long long temp_ID;
    int temp_warn_ID;
    assistant.get_result(temp_username,
                         temp_ID,
                         temp_warn_ID,
                         temp_reason);

    //Before adding/editing thewarn we need to verify :
    //If in add mode : that the use doesn't already exists
    //If in edit mode : that the new (if new) ID entered isn't the same as another user
    bool copy = false;
    if (!(edit_ID != 0 &&
          temp_ID == warns.get_selection()->get_selected()->get_value(warns.get_columns().ID) &&
          temp_warn_ID == warns.get_selection()->get_selected()->get_value(warns.get_columns().warn_ID)))
    {
        Gtk::TreeModel::Children warn_children = warns.get_model()->children();
        for (Gtk::TreeModel::Children::iterator warn_iter = warn_children.begin();
             warn_iter != warn_children.end();
             ++warn_iter)
            if (temp_ID == (*warn_iter)[warns.get_columns().ID] &&
                temp_warn_ID == (*warn_iter)[warns.get_columns().warn_ID])
                copy = true;
    }

    //If it's not the case create/edit the warn
    if (!copy)
    {
        Gtk::TreeModel::Row row = (edit_ID != 0) ? *(warns.get_selection()->get_selected()) : *(warns.get_model()->append());

        row[warns.get_columns().username] = temp_username;
        if (edit_ID != 0)
            row = *(warns.get_selection()->get_selected());
        row[warns.get_columns().ID] = temp_ID;
        if (edit_ID != 0)
            row = *(warns.get_selection()->get_selected());
        row[warns.get_columns().warn_ID] = temp_warn_ID;
        if (edit_ID != 0)
            row = *(warns.get_selection()->get_selected());
        row[warns.get_columns().reason] = temp_reason;
    }
    else if (edit_ID != 0)
    {
        dialog::error_dialog(win, "Couldn't edit warn, ID and warn ID already existing",
                             "Please try again");
    }
    else
    {
        dialog::error_dialog(win, "Couldn't add warn, the warn already exist",
                             "Please try again with another warn");
    }
    edit_ID = 0;
}

void warn_menu::on_assistant_cancel()
{
    edit_ID = 0;
    assistant.hide();
}

long long &warn_menu::get_edited_ID()
{
    return edit_ID;
}

warn_assistant &warn_menu::get_assistant()
{
    return assistant;
}