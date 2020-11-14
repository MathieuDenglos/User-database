#ifndef USER_DATATREEVIEW_H
#define USER_DATATREEVIEW_H

#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <string>
#include <array>

#include "user_menu.h"
#include "user_columns.h"

class window;
class warn_list;

enum class USER_DATA
{
    USERNAME,
    TAG,
    ID,
    BIRTHDAY,
    OTHER,
};

class user_list : public Gtk::TreeView
{
private:
    //elements for the treeview
    user_columns u_columns;
    Glib::RefPtr<Gtk::ListStore> user_model;

    //popu menu to interact woth the data
    user_menu menu;

    //override that is activated each time a button is pressed on the treeview
    bool on_button_press_event(GdkEventButton *button_event) override;

public:
    //Constructor
    user_list(window &win);

    //Method related to the data
    void grab_data(std::string line);
    std::string save_user_list(warn_list &warns);
    std::string get_username(long long ID);

    //Method related to visibility
    void change_visibility(USER_DATA visibility, std::array<bool, 4> status, bool add);

    //getters
    [[nodiscard]] user_columns &get_columns() noexcept { return u_columns; }
    [[nodiscard]] Glib::RefPtr<Gtk::ListStore> &get_model() noexcept { return user_model; }
};

#endif //USER_DATATREEVIEW_H