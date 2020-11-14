#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "window.h"
#include "file.h"

#include <gtkmm.h>
#include <array>

class toolbar : public Gtk::Application
{
protected:
    //constructor accessed by the create static function
    toolbar();

    //Overrides of default initializer, to make the toolbar and other elements of the window
    void on_startup() override;
    void on_activate() override;

    //All the element that will appear on the window apart from the toolbar
    //Everything that will appear in the main window is contained in the window class
    window *win;
    void on_window_hide(Gtk::Window *window);

    //create the Gtk::builder, necessary for the toolbar
    Glib::RefPtr<Gtk::Builder> toolbar_builder;

    //everything related to the file section
    void on_open_file();
    void on_add_file();
    void on_new_file();
    Glib::RefPtr<Gio::SimpleAction> auto_save;
    void on_auto_save();
    void visibility_settings(bool visibility);
    void on_save_file(bool saveas);
    file files;

    //everything related to the user list (used to show or not the Gtk::Column/Gtk::TreeView)
    Glib::RefPtr<Gio::SimpleAction> toolbar_user_list;
    Glib::RefPtr<Gio::SimpleAction> toolbar_tag;
    Glib::RefPtr<Gio::SimpleAction> toolbar_ID;
    Glib::RefPtr<Gio::SimpleAction> toolbar_birthday;
    Glib::RefPtr<Gio::SimpleAction> toolbar_other;

    void on_show_user_list();
    void on_tag();
    void on_ID();
    void on_birthday();
    void on_other();
    std::array<bool, 4> get_user_status();

    //everything related to the warn list (used to show or not the Gtk::Column/Gtk::TreeView)
    Glib::RefPtr<Gio::SimpleAction> toolbar_warn_list;
    Glib::RefPtr<Gio::SimpleAction> toolbar_reason;

    void on_show_warn_list();
    void on_reason();

    //other section of the programm (containing the showbirthdaylist and the about)
    void on_show_birthday();
    void on_about();

public:
    //called to create the application
    static Glib::RefPtr<toolbar> create();
};

#endif //TOOLBAR_H