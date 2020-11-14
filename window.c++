#include "window.h"
#include "dialog.h"
#include "file.h"

window::window() : Gtk::ApplicationWindow(),
                   box(Gtk::ORIENTATION_VERTICAL, 12),

                   user_box(Gtk::ORIENTATION_HORIZONTAL, 0),
                   user_frame("user list"),
                   users(*this),
                   user_separator(""),

                   warn_box(Gtk::ORIENTATION_HORIZONTAL, 0),
                   warn_frame("warn list"),
                   warns(*this),
                   warn_separator(""),

                   birthdays(*this)
{

    //first we'll add the user_list
    user_scrolledwindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    user_scrolledwindow.add(users);
    user_box.pack_start(user_scrolledwindow, Gtk::PACK_SHRINK);
    user_box.pack_start(user_separator, Gtk::PACK_EXPAND_WIDGET);
    user_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    user_frame.set_shadow_type(Gtk::SHADOW_NONE);
    user_frame.add(user_box);
    box.pack_start(user_frame, Gtk::PACK_EXPAND_WIDGET);

    //Now we'll add the warn_list
    warn_scrolledwindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    warn_scrolledwindow.add(warns);
    warn_box.pack_start(warn_scrolledwindow, Gtk::PACK_SHRINK);
    warn_box.pack_start(warn_separator, Gtk::PACK_EXPAND_WIDGET);
    warn_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    warn_frame.set_shadow_type(Gtk::SHADOW_NONE);
    warn_frame.add(warn_box);
    box.pack_start(warn_frame, Gtk::PACK_SHRINK);

    //add all the elements to the window
    add(box);

    //sets the application logo
    property_icon() = Gdk::Pixbuf::create_from_file("share/app_logo.png");

    //sets the default side and add a signal ahndlers to detect each time the window is resized, to resize correctly the visible widgets
    signal_check_resize().connect(sigc::mem_fun(*this, &window::on_resize));
    set_default_size(420, 300);
    set_size_request(420, 50);
}

void window::on_resize()
{
    //Resize the warn_frame, so it either take 1/5 of the page or show all the elements
    //For this we're using a mix of normal equations and boolean equations
    warn_frame.set_size_request(420, -1 * !user_frame.is_visible() +
                                         user_frame.is_visible() * ((get_height() / 5) * (get_height() / 5 < (24 * warns.get_model()->children().size() + 24)) +
                                                                    (24 * warns.get_model()->children().size() + 24) * (get_height() / 5 >= (24 * warns.get_model()->children().size() + 24))));
}

void window::set_user_visibility(bool state)
{
    //make the user_frame visible or not and change the minimum window size
    user_frame.set_visible(state);
    set_size_request(420, 10 + 40 * (!user_frame.is_visible() && !warn_frame.is_visible()) +
                              352 * (user_frame.is_visible()) +
                              78 * (warn_frame.is_visible()));

    //We now remove and reintroduce the warn_list in  order for it to take the correct space
    //Gtk::PACK_EXPAND_WIDGET if the user_list isn't visible, so the warn_list take the entire space
    //Gtk::PACK_SHRINK if the the user_list is visible, so the warn_list takes space following the ration 1/5
    box.remove(warn_frame);
    if (user_frame.is_visible())
        box.pack_start(warn_frame, Gtk::PACK_SHRINK);
    else
        box.pack_start(warn_frame, Gtk::PACK_EXPAND_WIDGET);
}

void window::set_warn_visibility(bool state)
{
    //make the warn_frame visible or not and change the minimum window size
    warn_frame.set_visible(state);
    set_size_request(420, 10 + 40 * (!user_frame.is_visible() && !warn_frame.is_visible()) +
                              352 * (user_frame.is_visible()) +
                              78 * (warn_frame.is_visible()));
}

void window::change_title(bool saved)
{
    set_title("User database");
    if (!saved && auto_save_mode_on)
        file::auto_saver(*this);
    else if (!saved)
        set_title("User database (unsaved)");
}

user_list &window::get_user_list()
{
    return users;
}

warn_list &window::get_warn_list()
{
    return warns;
}

birthday_window &window::get_birthday_window()
{
    return birthdays;
}