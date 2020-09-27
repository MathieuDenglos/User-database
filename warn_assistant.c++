#include "warn_assistant.h"
#include "window.h"
#include "dialog.h"

#include <sstream>

warn_assistant::warn_assistant(window &win) : page1_box(Gtk::ORIENTATION_VERTICAL, 12),
                                              user_box(Gtk::ORIENTATION_HORIZONTAL, 24),
                                              user_frame("Username"),
                                              ID_frame("ID"),
                                              warn_ID_frame("Warn ID"),
                                              warn_ID_box(Gtk::ORIENTATION_HORIZONTAL, 0),
                                              warn_ID_adjustment(Gtk::Adjustment::create(0.0, 0.0, 999.0, 1.0, 10.0, 0.0)),
                                              warn_ID_spin(warn_ID_adjustment),
                                              warn_ID_separator("")
{
    //constructor for the user assistant, addes the different elements of the different pages
    set_transient_for(win);
    set_border_width(12);
    set_default_size(420, 300);
    set_resizable(false);

    //setup page 1 (everything : user, warn_ID, reason)
    user_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    user_frame.set_shadow_type(Gtk::SHADOW_NONE);
    ID_entry.set_max_length(18);
    ID_entry.set_size_request(120, -1);
    ID_entry.set_sensitive(false); //Not modifiable (changed with the user_combo)
    ID_entry.set_text("ND");       //For the launch_assistant() to detect the first launch
    ID_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    ID_frame.set_shadow_type(Gtk::SHADOW_NONE);
    ID_frame.add(ID_entry);
    user_box.pack_start(user_frame, Gtk::PACK_SHRINK);
    user_box.pack_start(ID_frame, Gtk::PACK_SHRINK);
    page1_box.pack_start(user_box, Gtk::PACK_SHRINK);
    warn_ID_spin.set_size_request(65, -1);
    warn_ID_box.pack_start(warn_ID_spin, Gtk::PACK_SHRINK);
    warn_ID_box.pack_start(warn_ID_separator, Gtk::PACK_EXPAND_WIDGET);
    warn_ID_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    warn_ID_frame.set_shadow_type(Gtk::SHADOW_NONE);
    warn_ID_frame.add(warn_ID_box);
    page1_box.pack_start(warn_ID_frame, Gtk::PACK_SHRINK);
    reason_buffer = Gtk::TextBuffer::create();
    reason_text.set_buffer(reason_buffer);
    reason_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_START);
    reason_frame.set_shadow_type(Gtk::SHADOW_IN);
    reason_frame.add(reason_text);
    page1_box.pack_start(reason_frame, Gtk::PACK_EXPAND_WIDGET);

    append_page(page1_box);
    set_page_title(*get_nth_page(0), "warn information");

    set_page_type(page1_box, Gtk::ASSISTANT_PAGE_CONFIRM);

    //signal handlers to make the assistant functional
    user_combo.signal_changed().connect(sigc::mem_fun(*this,
                                                      &warn_assistant::on_user_selected));

    warn_ID_adjustment->signal_value_changed().connect(sigc::mem_fun(*this,
                                                                     &warn_assistant::on_info_changed));

    show_all_children();
}

void warn_assistant::on_user_selected()
{
    //When a user is selected, change the text on the ID_entry and verify if user can go to next page
    ID_entry.set_text(std::to_string((*user_combo.get_active())[u_columns.ID]));
    on_info_changed();
}

void warn_assistant::on_info_changed()
{
    //verify if the warn_assistant is complete and if the user can click on apply
    set_page_complete(page1_box, (warn_ID_adjustment->get_value() != 0));
}

void warn_assistant::launch_assistant(window &win,
                                      long long ID /*= 0*/,
                                      int warn_ID /*= 0*/,
                                      std::string reason /*= ""*/)
{
    //if it's the first time the assistant is launched
    if (ID_entry.get_text() == "ND")
    {
        user_combo.set_model(win.get_user_list().get_model());
        user_combo.pack_start(u_columns.username);
        user_frame.add(user_combo);
        user_combo.show();
    }

    //if the assistant is called in add mode, sets defaults values, otherwise sets the value sent
    if (ID == 0)
    {
        set_title("add warn");
        user_combo.set_active(0);
        ID_entry.set_text(std::to_string((*win.get_user_list().get_model()->children().begin())[win.get_user_list().get_columns().ID]));
        warn_ID_adjustment->set_value(0);
        reason_buffer->set_text("");
        on_info_changed();
        show();
    }
    else
    {
        set_title("edit warn");

        //First we need to verify if the user exists and if so, we need to return the index of said user
        //For this we'll move in the treemodel until we find the user
        int count = 0;
        Gtk::TreeModel::Children::iterator user_iter = win.get_user_list().get_model()->children().begin();
        while (user_iter != win.get_user_list().get_model()->children().end() && (*user_iter)[win.get_user_list().get_columns().ID] != ID)
        {
            user_iter++;
            count++;
        }

        if (count != win.get_user_list().get_model()->children().size())
        {
            //sets all the value depoending on what got sent
            user_combo.set_active(count);
            warn_ID_adjustment->set_value(warn_ID);
            reason_buffer->set_text(reason);

            //To change the ID and make the apply button selectable
            on_user_selected();
            show();
        }
        else
        {
            dialog::error_dialog(win, "The warn you're trying to edit doesn't coïncide to any user",
                                 "Please remove it");
        }
    }
}

void warn_assistant::get_result(std::string &username,
                                long long &ID,
                                int &warn_ID,
                                std::string &reason)
{
    //Grab the results from the Gtk::Assistant
    username = user_combo.get_active()->get_value(u_columns.username);
    std::stringstream temp_ID(ID_entry.get_text());
    temp_ID >> ID;
    warn_ID = warn_ID_adjustment->get_value();
    reason = reason_buffer->get_text();
}