#ifndef WARN_ASSISTANT_H
#define WARN_ASSISTANT_H

#include "user_columns.h"

#include <gtkmm/assistant.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/combobox.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/textview.h>
#include <string>

class window;

class warn_assistant : public Gtk::Assistant
{
private:
    //page 1 (everything : user, warn_ID, reason)
    Gtk::Box page1_box;
    Gtk::Box user_box;
    Gtk::Frame user_frame;
    user_columns u_columns;
    Gtk::ComboBox user_combo;
    Gtk::Frame ID_frame;
    Gtk::Entry ID_entry;
    Gtk::Frame warn_ID_frame;
    Gtk::Box warn_ID_box;
    Glib::RefPtr<Gtk::Adjustment> warn_ID_adjustment;
    Gtk::SpinButton warn_ID_spin;
    Gtk::Label warn_ID_separator;
    Gtk::Frame reason_frame;
    Glib::RefPtr<Gtk::TextBuffer> reason_buffer;
    Gtk::TextView reason_text;
    void on_user_selected();
    void on_info_changed();

public:
    //constructor
    warn_assistant(window &win);

    //method to set the values and launch the assistant
    void launch_assistant(window &win,
                          long long ID = 0,
                          int warn_ID = 0,
                          std::string reason = "");

    //method to get the results from the assistant
    void get_result(std::string &username,
                    long long &ID,
                    int &warn_ID,
                    std::string &reason);
};

#endif //WARN_ASSISTANT_H