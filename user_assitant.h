#ifndef USER_ASSISTANT_H
#define USER_ASSISTANT_H

#include "months.h"

#include <gtkmm/assistant.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/frame.h>
#include <gtkmm/spinbutton.h>
#include <string>

class window;

class user_assistant : public Gtk::Assistant
{
private:
    //page 1 (username, tag, ID)
    Gtk::Box page1_box;
    Gtk::Box tag_ID_box;
    Gtk::Frame username_frame;
    Gtk::Entry username_entry;
    Gtk::Frame tag_frame;
    Glib::RefPtr<Gtk::Adjustment> tag_adjustment;
    Gtk::SpinButton tag_spin;
    Gtk::Frame ID_frame;
    Gtk::Entry ID_entry;
    bool ID_valid;
    void on_username_changed();
    void on_ID_changed();

    //page 2 (birthday)
    Gtk::Box page2_box;
    Gtk::Box birthday_box;
    Gtk::CheckButton birthday_check;
    Gtk::Frame month_frame;
    months month_combo;
    Gtk::Frame day_frame;
    Glib::RefPtr<Gtk::Adjustment> day_adjustment;
    Gtk::SpinButton day_spin;
    Gtk::Label birthday_separator;
    void on_birthday_given();
    void on_month_changed();

    //page 3 (introduction/review)
    Gtk::Box page3_box;
    Gtk::CheckButton introduction_check;
    Gtk::CheckButton review_check;

public:
    //constructor
    user_assistant(window &win);

    //method to set the values and launch the assistant
    void launch_assistant(std::string username = "",
                          int tag = 1000,
                          long long ID = 0,
                          std::string birthday_month = "ND",
                          int birthday_day = 0,
                          bool introduction = false,
                          bool review = false);

    //method to get the results from the assistant
    void get_result(std::string &username,
                    int &tag,
                    long long &ID,
                    std::string &birthday_month,
                    int &birthday_day,
                    bool &introduction,
                    bool &review);
};

#endif