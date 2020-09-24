#include "birthday_window.h"
#include "dialog.h"
#include "window.h"

#include <algorithm>

birthday_window::birthday_window(window &win) : birthday_box(Gtk::ORIENTATION_VERTICAL, 0),
                                                birthday_label(""),
                                                close_button("Close window")
{
    //Gtk::Label with the birthday_list
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
    std::string text;
    //First lets look through all the users that gave their birthday, for this we have 2 options :
    //Either look at the month (defined as "ND" if not given)
    //Look at the day (defined as 0 if not given), which is what we'll do for speed improvment.
    //int lenght = users.get_birthday_list(win, birthdays);
    int max_lenght = fill_birthday_list(win, users);

    //Now lets verify if this list contains at least one element, if it does we need to :
    //reorder the list depending on the birthdays (using the rankdate)
    //create the string with all the information
    //Send it to the window and create it
    if (birthdays.size() != 0)
    {
        set_size_request(240, (birthdays.size() < 30) * birthdays.size() * 20 + 600 * (birthdays.size() >= 30));
        std::string text = convert_data(max_lenght);
        birthday_label.set_text(text);
        show();
    }
    else
    {
        dialog::error_dialog(win, "No birthday list to show",
                             "Please insert birthdays in order to make a list visible");
    }
}

int birthday_window::fill_birthday_list(window &win,
                                        user_list &users)
{
    //used to store the lenght of the longest username, for better displaying
    int max_lenght = 0;

    birthday_list temp;
    int rankdate;
    std::string temp_str;
    Gtk::TreeModel::Children children = users.get_model()->children();

    for (Gtk::TreeModel::Children::iterator user_iter = children.begin();
         user_iter != children.end();
         ++user_iter)
    {
        if ((*user_iter)[users.get_columns().birthday_day] != 00)
        {
            //First let's create the rankdate, which consist of a value between 0101 and 1231
            //First 2 numbers contain the month and last 2 contains the day
            //This will later be used to sort the username by their birthdate
            temp_str = (*user_iter)[users.get_columns().birthday_month];
            std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), ::toupper);
            rankdate = months::get_map().at(temp_str) * 100 + (*user_iter)[users.get_columns().birthday_day];

            if (rankdate <= 1231 || rankdate <= 0101)
            {
                //Now lets complete the temp birthday_information class with all the needed data
                temp.set_info(rankdate,
                              (*user_iter)[users.get_columns().username],
                              temp_str,
                              (*user_iter)[users.get_columns().birthday_day]);
                birthdays.push_back(temp);

                //And we finish by looking at if the username is the longest (using boolean equations for optimisation)
                max_lenght = (user_iter->get_value(users.get_columns().username).size() > max_lenght) ? user_iter->get_value(users.get_columns().username).size() : max_lenght;
            }
            else
            {
                temp_str = "Error while grabbing the birthday of : ";
                temp_str.append((*user_iter)[users.get_columns().username]);
                dialog::error_dialog(win, temp_str,
                                     "user skipped, please verify its birthday");
            }
        }
    }
    return max_lenght;
}

std::string birthday_window::convert_data(int max_lenght)
{
    std::string text;
    //Sorting using the std::sort algorithm and using the comparison method (defined below)
    std::sort(birthdays.begin(), birthdays.end(), [](birthday_list &one, birthday_list &two) { return (one.rankdate < two.rankdate); });

    //Now lets create the string
    text.append("```\n");
    for (int i = 0; i < birthdays.size(); i++)
    {
        text.append(birthdays[i].get_line(max_lenght));
    }
    text.append("```\n");
    return text;
}

void birthday_window::on_close()
{
    //hide the window and clear the vector
    hide();
    birthdays.clear();
}