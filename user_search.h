#ifndef USER_DATA_SEARCH_H
#define USER_DATA_SEARCH_H

#include "user_columns.h"
#include "warn_columns.h"
#include "months.h"

#include <gtkmm/window.h>
#include <gtkmm/liststore.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/combobox.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/spinbutton.h>

class window;
enum class USER_DATA;
class user_list;
class warn_list;

class user_search : public Gtk::Window
{
private:
    //class used for the combobox of the option selection
    class columns : public Gtk::TreeModel::ColumnRecord
    {
    public:
        columns()
        {
            add(option_name);
            add(option);
        }

        Gtk::TreeModelColumn<std::string> option_name;
        Gtk::TreeModelColumn<USER_DATA> option;
    };
    columns options_columns;
    Glib::RefPtr<Gtk::ListStore> options_model;

    Gtk::Box general_box;

    //search configuration
    Gtk::Box search_box;
    Gtk::Label search_label;
    Gtk::ComboBox search_type;
    //username
    Gtk::Box username_box;
    Gtk::Frame username_frame;
    Gtk::Entry username_entry;
    Gtk::Label username_separator;
    Gtk::Box username_separator_box;
    Gtk::Label username_frame_separator;
    //ID
    Gtk::Box ID_box;
    Gtk::Frame ID_frame;
    Gtk::Entry ID_entry;
    Gtk::Label ID_separator;
    Gtk::Box ID_separator_box;
    Gtk::Label ID_frame_separator;
    //birthday
    Gtk::Frame birthday_frame;
    Gtk::Box birthday_box;
    Gtk::Box birthday_info_box;
    months month_combo;
    Glib::RefPtr<Gtk::Adjustment> day_adjustment;
    Gtk::SpinButton day_spin;
    Gtk::Box birthday_options_box;
    Gtk::CheckButton month_check;
    Gtk::CheckButton day_check;
    //other (introduction/review)
    Gtk::Box other_box;
    Gtk::CheckButton introduction_check;
    Gtk::CheckButton review_check;

    //userlist
    Gtk::Box user_box;
    Gtk::ScrolledWindow user_scrolledwindow;
    Gtk::Frame user_frame;
    Gtk::Label user_separator;
    user_columns users_visible_columns;
    Glib::RefPtr<Gtk::ListStore> users_visible_model;
    Gtk::TreeView users_visible_treeview;
    Gtk::Label no_users_label;

    //warnlist
    Gtk::Box warn_box;
    Gtk::ScrolledWindow warn_scrolledwindow;
    Gtk::Frame warn_frame;
    Gtk::Label warn_separator;
    warn_columns warns_visible_columns;
    Glib::RefPtr<Gtk::ListStore> warns_visible_model;
    Gtk::TreeView warns_visible_treeview;
    Gtk::Label no_warns_label;

    //close and search button
    Gtk::ButtonBox search_buttonbox;
    Gtk::Button search_button;
    Gtk::ButtonBox close_buttonbox;
    Gtk::Button close_button;

    //signal handlers
    void on_ID_changed();
    void on_type_changed();
    void on_month_changed();
    void on_month_activated();
    void on_day_activated();
    void on_search(user_list &users, warn_list &warns);
    void on_close();

    //Methods called by on-search() used to make the code cleaner
    void copy_user_row(Gtk::TreeModel::Row &receiving_row, Gtk::TreeModel::Row giving_row, user_list &users);
    void copy_warn_row(Gtk::TreeModel::Row &receiving_row, Gtk::TreeModel::Row giving_row, warn_list &warns);
    bool search_conditions(Gtk::TreeModel::Row row, user_list &users);

public:
    //constructor
    user_search(window &win);

    //launcher, called to show the window
    void launch_search();
};

#endif //USER_DATA_SEARCH_H