#ifndef USER_DATAMENU_H
#define USER_DATAMENU_H

#include "user_assitant.h"
#include "user_search.h"

#include <gtkmm/menu.h>

class window;
class user_list;
class warn_list;

class user_menu : public Gtk::Menu
{
private:
    //Store whether we're editing or adding a user (used for on_assistant_apply())
    long long edit_ID;

    //Different signal handlers for the elements of the menu
    void on_search_user(window &win, user_list &users);
    void on_add_user();
    void on_edit_user(window &win, user_list &users);
    void on_remove_user(user_list &users, window &win);

    //user_search for on_search_user() and assistant for on_add_user() and on_edit_user()
    user_search search;
    user_assistant assistant;

    //signals handlers that gets activated at the end of the use of the user_assistant
    void on_assistant_apply(window &win, user_list &users, warn_list &warns);
    void on_assistant_cancel();

public:
    user_menu(window &win);
};

#endif