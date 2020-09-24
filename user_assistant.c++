#include "user_assitant.h"
#include "window.h"

#include <algorithm>

user_assistant::user_assistant(window &win) : page1_box(Gtk::ORIENTATION_VERTICAL, 12),
                                              tag_ID_box(Gtk::ORIENTATION_HORIZONTAL, 24),
                                              username_frame("Username"),
                                              tag_frame("Tag"),
                                              tag_adjustment(Gtk::Adjustment::create(1000.0, 0000.0, 9999.0, 1.0, 10.0, 0.0)),
                                              tag_spin(tag_adjustment),
                                              ID_frame("ID"),

                                              page2_box(Gtk::ORIENTATION_VERTICAL, 12),
                                              birthday_box(Gtk::ORIENTATION_HORIZONTAL, 24),
                                              birthday_check("Birthday given ?"),
                                              month_frame("month"),
                                              day_frame("day"),
                                              day_adjustment(Gtk::Adjustment::create(1.0, 1.0, 31.0, 1.0, 10.0, 0.0)),
                                              day_spin(day_adjustment),

                                              page3_box(Gtk::ORIENTATION_VERTICAL, 12),
                                              introduction_check("introduction"),
                                              review_check("review")
{
    //constructor for the user assistant, addes the different elements of the different pages
    set_transient_for(win);
    set_border_width(12);
    set_default_size(420, 260);
    set_resizable(false);

    //setup page 1 (username, tag, ID)
    username_entry.set_max_length(32);
    username_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    username_frame.set_shadow_type(Gtk::SHADOW_NONE);
    username_frame.add(username_entry);

    tag_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    tag_frame.set_shadow_type(Gtk::SHADOW_NONE);
    tag_frame.add(tag_spin);

    ID_entry.set_max_length(18);
    ID_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    ID_frame.set_shadow_type(Gtk::SHADOW_NONE);
    ID_frame.add(ID_entry);

    tag_ID_box.pack_start(tag_frame, Gtk::PACK_SHRINK);
    tag_ID_box.pack_end(ID_frame, Gtk::PACK_SHRINK);
    page1_box.pack_start(username_frame, Gtk::PACK_SHRINK);
    page1_box.pack_start(tag_ID_box, Gtk::PACK_SHRINK);
    append_page(page1_box);
    set_page_title(*get_nth_page(0), "General information");
    set_page_type(page1_box, Gtk::ASSISTANT_PAGE_INTRO);

    //setup page 2 (birthday)
    month_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    month_frame.set_shadow_type(Gtk::SHADOW_NONE);
    month_frame.add(month_combo);

    day_spin.set_size_request(45, -1);
    day_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    day_frame.set_shadow_type(Gtk::SHADOW_NONE);
    day_frame.add(day_spin);

    birthday_box.pack_start(month_frame, Gtk::PACK_SHRINK);
    birthday_box.pack_start(day_frame, Gtk::PACK_SHRINK);
    birthday_box.pack_start(birthday_separator, Gtk::PACK_EXPAND_WIDGET);
    page2_box.pack_start(birthday_check, Gtk::PACK_SHRINK);
    page2_box.pack_start(birthday_box, Gtk::PACK_SHRINK);
    append_page(page2_box);
    set_page_title(*get_nth_page(1), "Birthday information");
    set_page_complete(page2_box, true);

    //setup page 3 (other)
    page3_box.pack_start(introduction_check, Gtk::PACK_SHRINK);
    page3_box.pack_start(review_check, Gtk::PACK_SHRINK);
    append_page(page3_box);
    set_page_title(*get_nth_page(2), "Other information");
    set_page_complete(page3_box, true);
    set_page_type(page3_box, Gtk::ASSISTANT_PAGE_CONFIRM);

    //signal handlers to make the assistant functional
    username_entry.signal_changed().connect(sigc::mem_fun(*this,
                                                          &user_assistant::on_username_changed));
    ID_entry.signal_changed().connect(sigc::mem_fun(*this,
                                                    &user_assistant::on_ID_changed));
    birthday_check.signal_clicked().connect(sigc::mem_fun(*this,
                                                          &user_assistant::on_birthday_given));
    month_combo.signal_changed().connect(sigc::mem_fun(*this,
                                                       &user_assistant::on_month_changed));

    show_all_children();
}

void user_assistant::on_username_changed()
{
    //verify if a username has been entered, if so, the user can go to page 2
    if (username_entry.get_text_length() > 0 && ID_valid)
        set_page_complete(page1_box, true);
    else
        set_page_complete(page1_box, false);
}

void user_assistant::on_ID_changed()
{
    long long ID;
    //first we use the std::stoi to grab the value from the string
    //we catch potential errors if the value entered is not a valid value
    try
    {
        ID = std::stoll(ID_entry.get_text());
    }
    catch (std::invalid_argument const &e)
    {
        ID_valid = false;
        return;
    }
    catch (std::out_of_range const &e)
    {
        ID_valid = false;
        return;
    }

    //If it's over 1.0e18, it is a valid ID
    if (ID >= 100000000000000000)
        ID_valid = true;
    else
        ID_valid = false;

    //verify with the other condition to possibly validate the page
    on_username_changed();
}

void user_assistant::on_birthday_given()
{
    //If the birthday is given, allow the user to enter it
    month_frame.set_sensitive(birthday_check.get_active());
    day_frame.set_sensitive(birthday_check.get_active());
}

void user_assistant::on_month_changed()
{
    //when the month is changed, change the maximum to prevent invalid Birthday
    day_adjustment->set_upper((*month_combo.get_active())[month_combo.get_months_columns().days]);
    if (day_adjustment->get_upper() < day_adjustment->get_value())
        day_adjustment->set_value(day_adjustment->get_upper());
}

void user_assistant::launch_assistant(std::string username /*= ""*/,
                                      int tag /*= 1000*/,
                                      long long ID /*= 0*/,
                                      std::string birthday_month /*= "ND"*/,
                                      int birthday_day /*= 0*/,
                                      bool introduction /*= false*/,
                                      bool review /*= false*/)
{
    //sets the basic information of the user
    username_entry.set_text(username);
    tag_adjustment->set_value(tag);
    ID_entry.set_text(std::to_string(ID));
    ID_valid = (ID != 0) ? true : false;

    //Verify if the conditions to go to page 2 are completed
    on_username_changed();

    //Change the birthdate depending on if it's given or not
    birthday_check.set_active(birthday_day != 0);
    on_birthday_given();
    if (birthday_day != 0)
    {
        set_title("add user");
        for (int i = 0; i < birthday_month.size(); i++)
            birthday_month[i] -= 32 * (birthday_month[i] >= 97 && birthday_month[i] <= 122);
        month_combo.set_active(months::get_map().at(birthday_month) - 1);
        day_adjustment->set_value(birthday_day);
    }
    else
    {
        set_title("edit user");
        month_combo.set_active(months::get_date().first - 1);
        day_adjustment->set_value(months::get_date().first);
    }

    //set the state for the introduction and review checkbox
    introduction_check.set_active(introduction);
    review_check.set_active(review);

    show();
}

void user_assistant::get_result(std::string &username,
                                int &tag,
                                long long &ID,
                                std::string &birthday_month,
                                int &birthday_day,
                                bool &introduction,
                                bool &review)
{
    //Grab the results from the Gtk::Assistant
    username = username_entry.get_text();
    tag = tag_adjustment->get_value();
    ID = std::stoll(ID_entry.get_text());
    if (birthday_check.get_active())
    {
        birthday_month = (*month_combo.get_active())[month_combo.get_months_columns().month];
        birthday_day = day_adjustment->get_value();
    }
    else
    {
        birthday_month = "ND";
        birthday_day = 00;
    }
    introduction = introduction_check.get_active();
    review = review_check.get_active();
}