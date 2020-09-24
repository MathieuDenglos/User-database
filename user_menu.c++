#include "user_menu.h"
#include "user_list.h"
#include "warn_list.h"
#include "dialog.h"
#include "window.h"

user_menu::user_menu(window &win) : assistant(win), search(win)
{
    //creates all the items of the menu and add them to the menu
    Gtk::MenuItem *item;

    item = Gtk::manage(new Gtk::MenuItem("_Search user", true));
    item->signal_activate().connect(sigc::bind<user_list &>(sigc::mem_fun(*this, &user_menu::on_search_user),
                                                            win.get_user_list()));
    append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Add user", true));
    item->signal_activate().connect(sigc::mem_fun(*this, &user_menu::on_add_user));
    append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Edit user", true));
    item->signal_activate().connect(sigc::bind<window &, user_list &>(sigc::mem_fun(*this, &user_menu::on_edit_user),
                                                                      win, win.get_user_list()));
    append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Remove user", true));
    item->signal_activate().connect(sigc::bind<user_list &, window &>(sigc::mem_fun(*this, &user_menu::on_remove_user),
                                                                      win.get_user_list(), win));
    append(*item);

    //make the menu accessible only from the user treeview
    accelerate(win.get_user_list());

    //all the signal handlers related to the assistant
    assistant.signal_apply().connect(sigc::bind<window &, user_list &, warn_list &>(sigc::mem_fun(*this, &user_menu::on_assistant_apply),
                                                                                    win, win.get_user_list(), win.get_warn_list()));

    assistant.signal_cancel().connect(sigc::mem_fun(*this, &user_menu::on_assistant_cancel));

    assistant.signal_close().connect(sigc::mem_fun(*this, &user_menu::on_assistant_cancel));

    show_all();
}

void user_menu::on_search_user(user_list &users)
{
    //launch the search window
    search.launch_search();
}

void user_menu::on_edit_user(window &win, user_list &users)
{
    //tells the on_assistant_apply() that we're in edit mode and launch the assistant
    if (users.get_model()->children().size() != 0)
    {
        Gtk::TreeModel::Row user_row = *(users.get_selection()->get_selected());
        edit_ID = user_row[users.get_columns().ID];
        assistant.launch_assistant(user_row[users.get_columns().username],
                                   user_row[users.get_columns().tag],
                                   user_row[users.get_columns().ID],
                                   user_row[users.get_columns().birthday_month],
                                   user_row[users.get_columns().birthday_day],
                                   user_row[users.get_columns().introduction],
                                   user_row[users.get_columns().review]);
    }
    else
        dialog::error_dialog(win, "No users to edit",
                             "add users before editing any");
}

void user_menu::on_add_user()
{
    //tells the on_assistant_apply() that we're in add mode and launch the assistant
    edit_ID = 0;
    assistant.launch_assistant();
}

void user_menu::on_remove_user(user_list &users, window &win)
{
    if (users.get_model()->children().size() != 0)
    {
        //grab the selected user and remove it with all its warns
        long long ID = (*users.get_selection()->get_selected())[users.get_columns().ID];

        //close the assistant if it's open in edit mode with the user
        if (ID == edit_ID)
            on_assistant_cancel();

        //remove the warns
        win.get_warn_list().remove_warns(ID);

        //remove the user (issues with .erase() method and invalid iters)
        Gtk::TreeModel::Children::iterator iter = users.get_model()->children().begin();
        while (iter->get_value(users.get_columns().ID) != ID && iter != users.get_model()->children().end())
            iter++;

        if (iter != users.get_model()->children().end())
            users.get_model()->erase(iter);

        //tells the user that he needs to save the file
        win.change_title(false);
    }
    else
        dialog::error_dialog(win, "No users to remove",
                             "add users before removing them");
}

void user_menu::on_assistant_apply(window &win, user_list &users, warn_list &warns)
{
    //Grab the data that the user entered in the user_assistant
    std::string temp_username, temp_month;
    int temp_tag, temp_day;
    long long temp_ID;
    bool temp_intro, temp_review;
    assistant.get_result(temp_username,
                         temp_tag,
                         temp_ID,
                         temp_month,
                         temp_day,
                         temp_intro,
                         temp_review);

    //Before adding/editing the user we need to verify :
    //If in add mode : that the use doesn't already exists
    //If in edit mode : that the new (if new) ID entered isn't the same as another user
    Gtk::TreeModel::Children::iterator user_iter = users.get_model()->children().begin();
    if (!(edit_ID != 0 && temp_ID == edit_ID))
        while (user_iter != users.get_model()->children().end() && (*user_iter)[users.get_columns().ID] != edit_ID)
            user_iter++;

    //We now have an iterator, if this iterator is valid or the size of the treeview is 0, it means the new user is valid
    if (users.get_model()->children().size() == 0 || user_iter != users.get_model()->children().end())
    {
        Gtk::TreeModel::Row row;
        //Either create a new row (if in add mode) or find the edited row (we could get the selection but it might change during editing)
        if (edit_ID != 0)
            row = *user_iter;
        else
            row = *(users.get_model()->append());

        if (edit_ID != 0)
            warns.modify_warns(row[users.get_columns().ID], temp_ID, temp_username);

        row[users.get_columns().username] = temp_username;
        if (edit_ID != 0)
            row = *(users.get_selection()->get_selected());
        row[users.get_columns().tag] = temp_tag;
        if (edit_ID != 0)
            row = *(users.get_selection()->get_selected());
        row[users.get_columns().ID] = temp_ID;
        if (edit_ID != 0)
            row = *(users.get_selection()->get_selected());
        row[users.get_columns().birthday_month] = temp_month;
        if (edit_ID != 0)
            row = *(users.get_selection()->get_selected());
        row[users.get_columns().birthday_day] = temp_day;
        if (edit_ID != 0)
            row = *(users.get_selection()->get_selected());
        row[users.get_columns().introduction] = temp_intro;
        if (edit_ID != 0)
            row = *(users.get_selection()->get_selected());
        row[users.get_columns().review] = temp_review;

        //tells the user (by changing the app title) that he needs to save the file
        win.change_title(false);
    }
    else if (edit_ID != 0)
    {
        dialog::error_dialog(win, "Couldn't edit user, ID already existing",
                             "Please try again");
    }
    else
    {
        dialog::error_dialog(win, "Couldn't add user, the user already exist",
                             "Please try again with another user");
    }
    edit_ID = 0;
}

void user_menu::on_assistant_cancel()
{
    edit_ID = 0;
    assistant.hide();
}