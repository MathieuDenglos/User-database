#include "birthday_window.h"
#include "dialog.h"
#include "window.h"

#include <algorithm>

birthday_window::birthday_window(window &win) : birthday_box(Gtk::ORIENTATION_VERTICAL, 0),
                                                birthday_label(""),
                                                close_button("Close window")
{
    //Add all the needed elements in the birthday window
    birthday_label.set_selectable(true);
    birthday_scrolledwindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    birthday_scrolledwindow.add(birthday_label);
    birthday_box.pack_start(birthday_scrolledwindow, Gtk::PACK_EXPAND_WIDGET);
    birthday_box.pack_end(close_button, Gtk::PACK_SHRINK);

    add(birthday_box);
    set_title("Users Birthday List");
    set_transient_for(win);

    //Signal handlers to hide the window
    close_button.signal_clicked().connect(sigc::mem_fun(*this,
                                                        &birthday_window::on_close));

    //show all the widgets, but not the window
    show_all_children();
}

void birthday_window::show_birthday_window(window &win,
                                           user_list &users)
{
    //First lets look through all the users that gave their birthday. For this we will
    //Look at the day (defined as 0 if not given), which is the fastest way.
    auto children = users.get_model()->children();
    for (auto user_iter = children.begin(); user_iter != children.end(); ++user_iter)
        if ((*user_iter)[users.get_columns().birthday_day] != 00)
        {
            std::string upper_month = (*user_iter)[users.get_columns().birthday_month];
            std::transform(upper_month.begin(), upper_month.end(), upper_month.begin(), ::toupper);
            //First let's create the rankdate, which consist of a value between 0101 and 1231 (MMDD)
            //This will later be used to sort the users by their birthdate
            int rankdate = months::months_map.at(upper_month) * 100 + (*user_iter)[users.get_columns().birthday_day];

            //Now lets complete the user_info birthday_information class with all the needed data
            birthday_list user_info(rankdate,
                                    (*user_iter)[users.get_columns().username],
                                    upper_month,
                                    (*user_iter)[users.get_columns().birthday_day]);
            birthdays.push_back(user_info);

            //And we finish by looking at if the username is the longest
            if (user_iter->get_value(users.get_columns().username).size() > birthday_list::max_lenght)
                birthday_list::max_lenght = user_iter->get_value(users.get_columns().username).size();
        }

    //Now verify if the for loop found any users with a birthday
    if (birthdays.size() != 0)
    {
        //sort each user by their rankdate to have an organized list
        std::sort(birthdays.begin(),
                  birthdays.end(),
                  [](birthday_list &one, birthday_list &two) { return one.get_rankdate_const() < two.get_rankdate_const(); });

        //change the size of the window to fit each username
        set_size_request(240, (birthdays.size() < 30) * birthdays.size() * 20 + 600 * (birthdays.size() >= 30));

        std::stringstream text;
        text << "```\n";
        for (const auto &user : birthdays)
            text << user;
        text << "```\n";
        birthday_label.set_text(text.str());
        birthdays.clear();
        show();
    }
    else
    {
        dialog::error_dialog(win, "No birthday list to show",
                             "none of the user gave their birthday");
    }
}