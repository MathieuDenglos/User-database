#ifndef WINDOW_H
#define WINDOW_H

#include "user_list.h"
#include "warn_list.h"
#include "birthday_window.h"

#include <gtkmm.h>

class window : public Gtk::ApplicationWindow
{
private:
    //All the necessary widgets for the window
    Gtk::Box box;

    //All the widgets for the warn treeview
    Gtk::Frame warn_frame;
    Gtk::Box warn_box;
    Gtk::Label warn_separator;
    Gtk::ScrolledWindow warn_scrolledwindow;
    warn_list warns;

    //All the widgets for the user treeview
    Gtk::Frame user_frame;
    Gtk::Box user_box;
    Gtk::Label user_separator;
    Gtk::ScrolledWindow user_scrolledwindow;
    user_list users;

    //birthday window
    birthday_window birthdays;

    void on_resize();

public:
    //Constructor
    window();

    //defines the visibility of the user treeview and the warn treview
    void set_user_visibility(bool state);
    void set_warn_visibility(bool state);

    //methoid to change the title depending on the save status
    void change_title(bool saved);

    //Getters functions for the warnlist and userlist
    warn_list &get_warn_list();
    user_list &get_user_list();
    birthday_window &get_birthday_window();
};

#endif //WINDOW.H