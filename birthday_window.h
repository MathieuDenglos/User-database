#ifndef BIRTHDAY_WINDOW_H
#define BIRTHDAY_WINDOW_H

#include "birthday_list.h"

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <vector>

class window;
class user_list;

class birthday_window : public Gtk::Window
{
private:
    //Elements needed for the window
    Gtk::Box birthday_box;
    Gtk::ScrolledWindow birthday_scrolledwindow;
    Gtk::Label birthday_label;
    Gtk::Button close_button;

    //Element needed to store the datas about the users
    std::vector<birthday_list> birthdays;

    //Signal handlers
    void on_close();

public:
    //Constructor
    birthday_window(window &win);

    //Method to create the birthday_list with a user_list and show it on a separate window
    void show_birthday_window(window &win,
                              user_list &users);
};

#endif //BIRTHDAY_WINDOW_H