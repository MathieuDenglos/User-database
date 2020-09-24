#ifndef WARNTREEVIEW_H
#define WARNTREEVIEW_H

#include "warn_columns.h"
#include "warn_menu.h"

#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <string>

class window;
class user_list;

class warn_list : public Gtk::TreeView
{
private:
    //elements for the treeview
    warn_columns w_columns;
    Glib::RefPtr<Gtk::ListStore> warn_model;

    //popu menu to interact woth the data
    warn_menu menu;

    //override that is activated each time a button is pressed on the treeview
    bool on_button_press_event(GdkEventButton *button_event) override;

public:
    //constructor
    warn_list(window &win);

    //Method related to the data
    void grab_data(std::string line, user_list &users);
    void remove_warns(long long ID, int index = 0);
    void modify_warns(long long old_ID, long long new_ID, std::string new_username);
    std::string save_warn_list();

    //Method related to the visibility/editability of the treeview
    void change_visibility();

    //getters
    warn_columns &get_columns();
    Glib::RefPtr<Gtk::ListStore> &get_model();
};

#endif //WARNTREEVIEW_H