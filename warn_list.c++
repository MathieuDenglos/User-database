#include "warn_list.h"
#include "user_list.h"
#include "dialog.h"

#include <iomanip>
#include <gtkmm/treemodelsort.h>

warn_list::warn_list(window &win) : menu(win)
{
    //Create the model for the treeview
    warn_model = Gtk::ListStore::create(w_columns);
    set_model(warn_model);

    //Create the sort model so that the treeview is always sorted by usernames
    Glib::RefPtr<Gtk::TreeModelSort> sort = Gtk::TreeModelSort::create(warn_model);
    sort->set_sort_column(w_columns.username, Gtk::SORT_ASCENDING);
    set_model(sort);

    //add the visible columns to the treeview (as non editable for now)
    append_column("username", w_columns.username);
    append_column_numeric("warn ID", w_columns.warn_ID, "%03d");
    append_column("reason", w_columns.reason);
    show_all();
}

bool warn_list::on_button_press_event(GdkEventButton *button_event)
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

void warn_list::grab_data(std::string line, user_list &users)
{
    std::stringstream information(line), temp_string;

    //First we need to verify if the warn doesn't already exist.
    //A warn exist if the ID and warn_ID are the same as another warn
    //Warns will also be removed if the user isn't found
    bool copy = false;
    long long temp_ID;
    int temp_warn_ID;
    information >> temp_warn_ID;
    information.ignore(38);
    information >> temp_ID;

    //To grab the username we will look through the userlist (ID is generalized, username is not)
    //This method is used to prevent ambiguity in term of the usernames
    std::string username = users.get_username(temp_ID);

    //Now lets verify if the warn is already on the list
    //We go out of the while loop either if we found a copy or reach the end
    Gtk::TreeModel::Children::iterator warn_iter = warn_model->children().begin();
    while (warn_iter != warn_model->children().end() && copy == false)
    {
        copy = (temp_ID == (*warn_iter)[w_columns.ID] && temp_warn_ID == (*warn_iter)[w_columns.warn_ID]);
        warn_iter++;
    }

    if (username != "ND" && !copy)
    {
        //if the user with the warn still exists and the warn doesn't already exist
        //create a new row and fill it with the necessary data
        Gtk::TreeModel::Row row = *(warn_model->append());

        row[w_columns.ID] = temp_ID;
        row[w_columns.username] = username;
        row[w_columns.warn_ID] = temp_warn_ID;
        information.ignore(2);
        temp_string << information.rdbuf();
        row[w_columns.reason] = temp_string.str();
    }
}

void warn_list::remove_warns(long long ID, int index /*= 0*/)
{
    //erase all the warns with the ID sent
    if (menu.get_edited_ID() == ID && index == 0)
        menu.on_assistant_cancel();

    Gtk::TreeModel::Children::iterator iter = warn_model->children().begin();

    ///move the iter to the correct spot
    for (int i = 0; i < index; i++)
        iter++;

    while (iter != warn_model->children().end() && (ID != iter->get_value(w_columns.ID)))
    {
        index++;
        iter++;
    }
    index++;

    if (iter != warn_model->children().end())
    {
        remove_warns(ID, index);
        warn_model->erase(iter);
    }
}

void warn_list::modify_warns(long long old_ID, long long new_ID, std::string new_username)
{
    //modify all the warns with the old ID, with the new ID and new username
    Gtk::TreeModel::Children children = warn_model->children();
    for (Gtk::TreeModel::Children::iterator warn_iter = children.begin(); warn_iter != children.end(); ++warn_iter)
    {
        if (old_ID == (*warn_iter)[w_columns.ID])
        {
            //Change the username and the ID with the new ones
            (*warn_iter)[w_columns.ID] = new_ID;
            (*warn_iter)[w_columns.username] = new_username;
        }
    }
}

std::string warn_list::save_warn_list()
{
    //Create normalized lines based off of the warns data and returns it
    //It'll then be outputed in the file
    std::stringstream text;

    Gtk::TreeModel::Children children = warn_model->children();
    for (Gtk::TreeModel::Children::iterator warn_iter = children.begin();
         warn_iter != children.end();
         ++warn_iter)
        text << std::setw(3) << std::right << std::setfill('0') << warn_iter->get_value(w_columns.warn_ID) << "     "
             << std::setw(33) << std::left << std::setfill(' ') << warn_iter->get_value(w_columns.username) << " "
             << std::setw(20) << std::left << std::setfill(' ') << warn_iter->get_value(w_columns.ID) << " "
             << std::left << std::setfill(' ') << warn_iter->get_value(w_columns.reason) << std::endl;

    return text.str();
}

void warn_list::change_visibility()
{
    //remove the columns and reinsert visible ones in edit mode (if activated)
    if (get_n_columns() == 3)
        remove_column(*get_column(2));
    else
        append_column("reason", w_columns.reason);
}