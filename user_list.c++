#include "user_list.h"
#include "warn_list.h"
#include "window.h"

#include <algorithm>
#include <string>
#include <iomanip>

user_list::user_list(window &win) : menu(win)
{
    //Create the model for the treeview
    user_model = Gtk::ListStore::create(u_columns);
    set_model(user_model);

    //Create the sort model so that the treeview is always sorted by usernames
    Glib::RefPtr<Gtk::TreeModelSort> sort = Gtk::TreeModelSort::create(user_model);
    sort->set_sort_column(u_columns.username, Gtk::SORT_ASCENDING);
    set_model(sort);

    //add all the columns that are gonna be visible
    append_column("username", u_columns.username);
    append_column_numeric("tag", u_columns.tag, "%04d");
    append_column_numeric("ID", u_columns.ID, "%18lld");
    append_column("birthday", u_columns.birthday_month);
    append_column_numeric("", u_columns.birthday_day, "%02d");
    append_column("intro", u_columns.introduction);
    append_column("review", u_columns.review);

    //show everything
    show_all_children();
}

bool user_list::on_button_press_event(GdkEventButton *button_event)
{
    bool return_value = false;

    //Get the button pressed
    return_value = TreeView::on_button_press_event(button_event);

    //If the right click is pressed on the treeview, make the menu popup
    if ((button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3))
    {
        menu.popup(button_event->button, button_event->time);
    }
    return return_value;
}

void user_list::grab_data(std::string line)
{
    std::stringstream information(line);

    //First we need to verify if the user doesn't already exist.
    //A user exist if the ID is the same as another user
    bool copy = false;
    std::string temp_str;
    int temp_int;
    long long temp_ID;
    bool temp_bool;
    information >> temp_str;
    information >> temp_int;
    information >> temp_ID;

    //Now lets verify if the user is already on the list
    //We go out of the while loop either if we found a copy or reach the end
    Gtk::TreeModel::Children::iterator iter = user_model->children().begin();
    while (iter != user_model->children().end() && copy == false)
    {
        copy = (temp_ID == iter->get_value(u_columns.ID));
        iter++;
    }

    if (!copy)
    {
        //create a new row and fill it with the necessary data
        Gtk::TreeModel::Row row = *(user_model->append());
        //general information
        row[u_columns.username] = temp_str;
        row[u_columns.tag] = temp_int;
        row[u_columns.ID] = temp_ID;
        //birthday (month, day)
        information >> temp_str;
        row[u_columns.birthday_month] = temp_str;
        information >> temp_int;
        row[u_columns.birthday_day] = temp_int;

        //others (introduction, review)
        information >> temp_str;
        temp_bool = temp_str == "yes" ? true : false;
        row[u_columns.introduction] = temp_bool;
        information >> temp_str;
        temp_bool = temp_str == "yes" ? true : false;
        row[u_columns.review] = temp_bool;
    }
}

std::string user_list::save_user_list(warn_list &warns)
{
    std::stringstream text;
    int count;

    //looks through each users and add them to a string, to return them and later add them to the file
    Gtk::TreeModel::Children user_children = user_model->children();
    for (Gtk::TreeModel::Children::iterator user_iter = user_children.begin();
         user_iter != user_children.end();
         ++user_iter)
    {
        text << std::setw(33) << std::left << std::setfill(' ') << user_iter->get_value(u_columns.username) << " "
             << std::setw(4) << std::right << std::setfill('0') << user_iter->get_value(u_columns.tag) << " "
             << std::setw(19) << std::left << std::setfill('0') << user_iter->get_value(u_columns.ID) << " ";
        if ((*user_iter)[u_columns.birthday_day] == 0)
            text << "ND        00    ";
        else
            text << std::setw(9) << std::left << std::setfill(' ') << user_iter->get_value(u_columns.birthday_month) << " "
                 << std::setw(2) << std::right << std::setfill('0') << user_iter->get_value(u_columns.birthday_day) << "   ";
        if ((*user_iter)[u_columns.introduction])
            text << "yes    ";
        else
            text << "no     ";
        if ((*user_iter)[u_columns.review])
            text << "yes     ";
        else
            text << "no      ";

        //This part deals with the count of warns of the user, just go through the warn_list
        //and count the amount of identical ID
        count = 0;
        Gtk::TreeModel::Children warn_children = warns.get_model()->children();
        for (Gtk::TreeModel::Children::iterator warn_iter = warn_children.begin();
             warn_iter != warn_children.end();
             ++warn_iter)
            count += ((*user_iter)[u_columns.ID] == (*warn_iter)[warns.get_columns().ID]);

        text << std::setw(2) << std::right << std::setfill('0') << count << std::endl;
    }
    return text.str();
}

std::string user_list::get_username(long long ID)
{
    //Verifies each users and return its username if the ID is identical, else return ND
    Gtk::TreeModel::Children children = user_model->children();
    for (Gtk::TreeModel::Children::iterator iter = children.begin(); iter != children.end(); ++iter)
        if (iter->get_value(u_columns.ID) == ID)
            return iter->get_value(u_columns.username);

    return "ND";
}

void user_list::change_visibility(USER_DATA visibility, std::array<bool, 4> status, bool add)
{
    //This method is used to show and hide different columns and works in 2 parts

    //this first part either remove the column to remove if in remove mode
    //or else remove each columns until we reach the columns we want to add
    //We can't just use the .set_visible(bool); because we'd need to use Gtk::CellRenderer which are a pain
    switch (visibility)
    {
    case USER_DATA::TAG:
        if (!add)
        {
            remove_column(*get_column(get_n_columns() - (1 + 2 * status[3] + 2 * status[2] + status[1])));
            return;
        }
        else if (status[1])
        {
            remove_column(*get_column(get_n_columns() - (1 + 2 * status[3] + 2 * status[2])));
        }
    case USER_DATA::ID:
        if (!add)
        {
            remove_column(*get_column(get_n_columns() - (1 + 2 * status[3] + 2 * status[2])));
            return;
        }
        else if (status[2])
        {
            remove_column(*get_column(get_n_columns() - (2 + 2 * status[3])));
            remove_column(*get_column(get_n_columns() - (1 + 2 * status[3])));
        }
    case USER_DATA::BIRTHDAY:
        if (!add)
        {
            remove_column(*get_column(get_n_columns() - (2 + 2 * status[3])));
            remove_column(*get_column(get_n_columns() - (1 + 2 * status[3])));
            return;
        }
        else if (status[3])
        {
            remove_column(*get_column(get_n_columns() - 2));
            remove_column(*get_column(get_n_columns() - 1));
        }
    case USER_DATA::OTHER:
        if (!add)
        {
            remove_column(*get_column(get_n_columns() - 2));
            remove_column(*get_column(get_n_columns() - 1));
            return;
        }
    }

    //Now if we're in add mode add again all the columns that gotta be visible
    switch (visibility)
    {
    case USER_DATA::TAG:
        if (status[0])
            append_column_numeric("tag", u_columns.tag, "%04d");
    case USER_DATA::ID:
        if (status[1])
            append_column_numeric("ID", u_columns.ID, "%018lld");
    case USER_DATA::BIRTHDAY:
        if (status[2])
        {
            append_column("birthday", u_columns.birthday_month);
            append_column_numeric("", u_columns.birthday_day, "%02d");
        }
    case USER_DATA::OTHER:
        if (status[3])
        {
            append_column("intro", u_columns.introduction);
            append_column("review", u_columns.review);
        }
    }
}