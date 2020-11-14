#ifndef WARN_MENU_H
#define WARN_MENU_H

#include "warn_assistant.h"

#include <gtkmm/menu.h>

class window;
class user_list;
class warn_list;

class warn_menu : public Gtk::Menu
{
private:
    //Store whether we're editing or adding a warn (used for on_assistant_apply())
    long long edit_ID;
    int edit_warn_ID;

    //Different signal handlers for the elements of the menu
    void on_add_warn(window &win);
    void on_edit_warn(window &win, warn_list &warns);
    void on_remove_warn(window &win, warn_list &warns);

    //user_search for on_search_user() and assistant for on_add_user() and on_edit_user()
    warn_assistant assistant;

    //signals handlers that gets activated at the end of the use of the user_assistant
    void on_assistant_apply(window &win, warn_list &warns);

public:
    //constructor
    warn_menu(window &win);

    //method called when the assistant is closed
    void on_assistant_cancel();

    //getters
    [[nodiscard]] long long &get_edited_ID() noexcept { return edit_ID; }
    [[nodiscard]] warn_assistant &get_assistant() noexcept { return assistant; }
};

#endif //WARN_MENU_H