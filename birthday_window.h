#ifndef BIRTHDAY_WINDOW_H
#define BIRTHDAY_WINDOW_H

#include "birthday_list.h"

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <vector>
#include <string>

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

    //Method to grab all the users which gave their birthday and fill the vector birthdays
    int fill_birthday_list(window &win, user_list &users);

    //Method to grab all the data and convert it to a string
    std::string convert_data(int max_lenght);

    //Signal handlers
    void on_close();

public:
    //Constructor
    birthday_window(window &win);

    //Method to show the window
    void show_birthday_window(window &win, user_list &users);
};

#endif //BIRTHDAY_WINDOW_H