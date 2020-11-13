#include "user_search.h"
#include "window.h"
#include "user_list.h"
#include "warn_list.h"

user_search::user_search(window &win) : general_box(Gtk::ORIENTATION_VERTICAL, 12),

                                        search_box(Gtk::ORIENTATION_HORIZONTAL, 6),
                                        search_label("search by :"),
                                        username_box(Gtk::ORIENTATION_HORIZONTAL, 0),
                                        username_frame("username"),
                                        username_separator(""),
                                        username_separator_box(Gtk::ORIENTATION_VERTICAL, 0),
                                        username_frame_separator(""),
                                        ID_box(Gtk::ORIENTATION_HORIZONTAL, 0),
                                        ID_frame("enter ID"),
                                        ID_separator(""),
                                        ID_separator_box(Gtk::ORIENTATION_VERTICAL, 0),
                                        ID_frame_separator(""),
                                        birthday_frame("birthday"),
                                        birthday_box(Gtk::ORIENTATION_VERTICAL, 12),
                                        birthday_info_box(Gtk::ORIENTATION_HORIZONTAL, 12),
                                        day_adjustment(Gtk::Adjustment::create(1.0, 1.0, 31.0, 1.0, 10.0, 0.0)),
                                        day_spin(day_adjustment),
                                        birthday_options_box(Gtk::ORIENTATION_HORIZONTAL, 20),
                                        month_check("search by month"),
                                        day_check("search by day"),
                                        other_box(Gtk::ORIENTATION_VERTICAL, 8),
                                        introduction_check("introduction"),
                                        review_check("review"),

                                        user_box(Gtk::ORIENTATION_HORIZONTAL, 6),
                                        user_separator(""),
                                        user_frame("user list"),
                                        no_users_label("No users found"),

                                        warn_box(Gtk::ORIENTATION_HORIZONTAL, 6),
                                        warn_separator(""),
                                        warn_frame("warn list"),
                                        no_warns_label("The following users have no warns"),

                                        search_buttonbox(Gtk::ORIENTATION_HORIZONTAL),
                                        search_button("search"),
                                        close_buttonbox(Gtk::ORIENTATION_HORIZONTAL),
                                        close_button("close")
{
    options_model = Gtk::ListStore::create(options_columns);
    search_type.set_model(options_model);

    Gtk::TreeModel::Row row = *(options_model->append());
    row[options_columns.option_name] = "Username";
    row[options_columns.option] = USER_DATA::USERNAME;

    row = *(options_model->append());
    row[options_columns.option_name] = "ID";
    row[options_columns.option] = USER_DATA::ID;

    row = *(options_model->append());
    row[options_columns.option_name] = "Birthday";
    row[options_columns.option] = USER_DATA::BIRTHDAY;

    row = *(options_model->append());
    row[options_columns.option_name] = "Intro/review";
    row[options_columns.option] = USER_DATA::OTHER;

    search_type.pack_start(options_columns.option_name);
    search_box.pack_start(search_label, Gtk::PACK_SHRINK);
    search_box.pack_start(search_type, Gtk::PACK_SHRINK);
    general_box.pack_start(search_box, Gtk::PACK_SHRINK);
    //username
    username_entry.set_max_length(32);
    username_box.pack_start(username_entry, Gtk::PACK_SHRINK);
    username_box.pack_start(username_separator, Gtk::PACK_EXPAND_WIDGET);
    username_separator_box.pack_start(username_box, Gtk::PACK_SHRINK);
    username_separator_box.pack_start(username_frame_separator, Gtk::PACK_EXPAND_WIDGET);
    username_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    username_frame.set_shadow_type(Gtk::SHADOW_NONE);
    username_frame.add(username_separator_box);
    username_frame.set_size_request(-1, 90);
    general_box.pack_start(username_frame, Gtk::PACK_SHRINK);
    //ID
    ID_entry.set_max_length(18);
    ID_box.pack_start(ID_entry, Gtk::PACK_SHRINK);
    ID_box.pack_start(ID_separator, Gtk::PACK_EXPAND_WIDGET);
    ID_separator_box.pack_start(ID_box, Gtk::PACK_SHRINK);
    ID_separator_box.pack_start(ID_frame_separator, Gtk::PACK_EXPAND_WIDGET);
    ID_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    ID_frame.set_shadow_type(Gtk::SHADOW_NONE);
    ID_frame.add(ID_separator_box);
    ID_frame.set_size_request(-1, 90);
    general_box.pack_start(ID_frame, Gtk::PACK_SHRINK);
    //birthday
    day_spin.set_size_request(45, -1);
    birthday_info_box.pack_start(month_combo, Gtk::PACK_SHRINK);
    birthday_info_box.pack_start(day_spin, Gtk::PACK_SHRINK);
    birthday_options_box.pack_start(month_check, Gtk::PACK_SHRINK);
    birthday_options_box.pack_start(day_check, Gtk::PACK_SHRINK);
    birthday_box.pack_start(birthday_info_box, Gtk::PACK_SHRINK);
    birthday_box.pack_start(birthday_options_box, Gtk::PACK_SHRINK);
    birthday_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    birthday_frame.set_shadow_type(Gtk::SHADOW_NONE);
    birthday_frame.add(birthday_box);
    birthday_frame.set_size_request(-1, 90);
    general_box.pack_start(birthday_frame, Gtk::PACK_SHRINK);
    //other (introduction/review)
    other_box.pack_start(introduction_check, Gtk::PACK_SHRINK);
    other_box.pack_start(review_check, Gtk::PACK_SHRINK);
    other_box.set_size_request(-1, 90);
    general_box.pack_start(other_box, Gtk::PACK_SHRINK);

    //search button
    search_buttonbox.set_layout(Gtk::BUTTONBOX_START);
    search_buttonbox.add(search_button);
    general_box.pack_start(search_buttonbox, Gtk::PACK_SHRINK);

    //user list
    users_visible_model = Gtk::ListStore::create(users_visible_columns);
    users_visible_treeview.set_model(users_visible_model);
    users_visible_treeview.append_column("username", users_visible_columns.username);
    users_visible_treeview.append_column_numeric("tag", users_visible_columns.tag, "%04d");
    users_visible_treeview.append_column_numeric("ID", users_visible_columns.ID, "%18lld");
    users_visible_treeview.append_column("birthday", users_visible_columns.birthday_month);
    users_visible_treeview.append_column_numeric("", users_visible_columns.birthday_day, "%02d");
    users_visible_treeview.append_column("intro", users_visible_columns.introduction);
    users_visible_treeview.append_column("review", users_visible_columns.review);
    user_scrolledwindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    user_scrolledwindow.add(users_visible_treeview);
    user_box.pack_start(user_scrolledwindow, Gtk::PACK_SHRINK);
    user_box.pack_start(user_separator, Gtk::PACK_EXPAND_WIDGET);
    user_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    user_frame.set_shadow_type(Gtk::SHADOW_NONE);
    user_frame.add(user_box);
    general_box.pack_start(user_frame, Gtk::PACK_EXPAND_WIDGET);
    general_box.pack_start(no_users_label, Gtk::PACK_START, 0);

    //warn list
    warns_visible_model = Gtk::ListStore::create(warns_visible_columns);
    warns_visible_treeview.set_model(warns_visible_model);
    warns_visible_treeview.append_column("username", warns_visible_columns.username);
    warns_visible_treeview.append_column_numeric("warn ID", warns_visible_columns.warn_ID, "%03d");
    warns_visible_treeview.append_column("reason", warns_visible_columns.reason);
    warn_scrolledwindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    warn_scrolledwindow.add(warns_visible_treeview);
    warn_box.pack_start(warn_scrolledwindow, Gtk::PACK_SHRINK);
    warn_box.pack_start(warn_separator, Gtk::PACK_EXPAND_WIDGET);
    warn_frame.set_label_align(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    warn_frame.set_shadow_type(Gtk::SHADOW_NONE);
    warn_frame.add(warn_box);
    general_box.pack_start(warn_frame, Gtk::PACK_SHRINK);
    general_box.pack_start(no_warns_label, Gtk::PACK_START, 0);

    //Button to close the page
    close_buttonbox.set_layout(Gtk::BUTTONBOX_END);
    close_buttonbox.add(close_button);
    general_box.pack_end(close_buttonbox, Gtk::PACK_SHRINK);

    set_transient_for(win);
    add(general_box);

    //signal handlers
    search_button.signal_clicked().connect(sigc::bind<user_list &, warn_list &>(
        sigc::mem_fun(*this, &user_search::on_search),
        win.get_user_list(), win.get_warn_list()));
    close_button.signal_clicked().connect(sigc::mem_fun(*this,
                                                        &user_search::on_close));
    search_type.signal_changed().connect(sigc::mem_fun(*this,
                                                       &user_search::on_type_changed));
    ID_entry.signal_changed().connect(sigc::mem_fun(*this,
                                                    &user_search::on_ID_changed));
    month_combo.signal_changed().connect(sigc::mem_fun(*this,
                                                       &user_search::on_month_changed));
    month_check.signal_clicked().connect(sigc::mem_fun(*this,
                                                       &user_search::on_month_activated));
    day_check.signal_clicked().connect(sigc::mem_fun(*this,
                                                     &user_search::on_day_activated));

    show_all_children();
}

void user_search::on_type_changed()
{
    //hide the 4 options filing frames and show the correct one
    username_frame.hide();
    ID_frame.hide();
    birthday_frame.hide();
    other_box.hide();

    //Depending on the choice selected, hide and show the correct elements
    switch ((*search_type.get_active())[options_columns.option])
    {
    case USER_DATA::USERNAME:
        search_button.set_sensitive(true);
        username_frame.show();
        break;
    case USER_DATA::ID:
        on_ID_changed();
        ID_frame.show();
        break;
    case USER_DATA::BIRTHDAY:
        search_button.set_sensitive(true);
        birthday_frame.show();
        break;
    case USER_DATA::OTHER:
        search_button.set_sensitive(true);
        other_box.show();
        break;
    }
}

void user_search::on_ID_changed()
{
    long long ID = 0;
    //first we use the std::stoi to grab the value from the string
    //we catch potential errors if the value entered is not a valid value
    try
    {
        ID = std::stoll(ID_entry.get_text());
    }
    catch (std::invalid_argument const &e)
    {
        search_button.set_sensitive(false);
        return;
    }
    catch (std::out_of_range const &e)
    {
        search_button.set_sensitive(false);
        return;
    }

    //If it's over 1.0e18, it is a valid ID and the user can search
    search_button.set_sensitive(ID >= 100000000000000000);
}

void user_search::on_month_changed()
{
    //when the month is changed, change the maximum day value to prevent invalid Birthday
    day_adjustment->set_upper((*month_combo.get_active())[month_combo.get_months_columns().days]);
    if (day_adjustment->get_upper() < day_adjustment->get_value())
        day_adjustment->set_value(day_adjustment->get_upper());
}

void user_search::on_month_activated()
{
    //Make the combobox sensitive or not
    //Also used for the search to determine whether the month is a factor of selection
    month_combo.set_sensitive(month_check.get_active());
}

void user_search::on_day_activated()
{
    //Make the spinbutton sensitive or not
    //Also used for the search to determine whether the day is a factor of selection
    day_spin.set_sensitive(day_check.get_active());
}

void user_search::on_search(user_list &users, warn_list &warns)
{
    //This method is called when the search button is closed and works in multiple steps
    //First lets clear both warn and user treeview (from older search elements)
    users_visible_model->clear();
    warns_visible_model->clear();

    //Now lets hide all elements (The visible elements depend on the search results)
    user_frame.hide();
    no_users_label.hide();
    warn_frame.hide();
    no_warns_label.hide();

    //Now for both the user list and the warn list, we need to grab :
    //The children (to update the list each time and go through all the elements)
    //as well as a row to add elements to the visible user/warn treeview
    //and an old row to grab the data
    //Note that for the warn list we either grab the list or an empty list, depending on whether we'll show it or not
    //this improve speed greatly by preventing unnecessary checking
    Gtk::TreeModel::Children user_list_children = users.get_model()->children();
    Gtk::TreeModel::Row user_new_row;
    Gtk::TreeModel::Children warn_list_children = ((*search_type.get_active())[options_columns.option] == USER_DATA::BIRTHDAY || (*search_type.get_active())[options_columns.option] == USER_DATA::OTHER)
                                                      ? warns_visible_model->children()
                                                      : warns.get_model()->children();
    Gtk::TreeModel::Row warn_new_row;

    //Now lets look at all the elements of the userlist and verify if they follow the search conditions
    for (Gtk::TreeModel::Children::iterator user_iter = user_list_children.begin();
         user_iter != user_list_children.end();
         ++user_iter)
    {
        if (search_conditions(*user_iter, users))
        {
            //if so, crate a new row to the treeview and copy the data
            user_new_row = *(users_visible_model->append());
            copy_user_row(user_new_row, *user_iter, users);

            //Also verifies if the user have warns (if we don't need to show the list, the warnlist will be an empty list and this step skipped)
            //                                     (because warn_list_children.begin() == warn_list_children.end())
            for (Gtk::TreeModel::Children::iterator warn_iter = warn_list_children.begin(); warn_iter != warn_list_children.end(); ++warn_iter)
            {
                if ((*user_iter)[users.get_columns().ID] == (*warn_iter)[warns.get_columns().ID])
                {
                    //IF the user have warns, create a new row to the warn treview and copy the data
                    warn_new_row = *(warns_visible_model->append());
                    copy_warn_row(warn_new_row, *warn_iter, warns);
                }
            }
        }
    }

    //Now depending on the search type and whether elements got found
    //Either show the list, a label stating that nothing got found or nothing
    switch (search_type.get_active()->get_value(options_columns.option))
    {
    case USER_DATA::USERNAME:
    case USER_DATA::ID:
        if (warns_visible_model->children().size() != 0)
        {
            warn_frame.show();
            warn_frame.set_size_request(420, 26 * (warns_visible_model->children().size() + 1) * (warns_visible_model->children().size() < 3) +
                                                 120 * (warns_visible_model->children().size() >= 3));
        }
        else
            no_warns_label.show();
    default:
        if (users_visible_model->children().size() != 0)
            user_frame.show();
        else
            no_users_label.show();
    }

    //change the size of the search window so the information fits nicely
    set_size_request(420, 200 +
                              40 * no_users_label.is_visible() +
                              (32 * (users_visible_model->children().size() + 1) * (users_visible_model->children().size() < 8) +
                               280 * (users_visible_model->children().size() >= 8)) *
                                  user_frame.is_visible() +
                              40 * no_warns_label.is_visible() +
                              (26 * (warns_visible_model->children().size() + 1) * (warns_visible_model->children().size() < 3) +
                               120 * (warns_visible_model->children().size() >= 3)) *
                                  warn_frame.is_visible());
}

bool user_search::search_conditions(Gtk::TreeModel::Row row, user_list &users)
{
    //Grabs the useful data (USER_DATA type for speed ; upper_* to prevent cap sensibility)
    std::string upper_username = row.get_value(users.get_columns().username);
    std::transform(upper_username.begin(), upper_username.end(), upper_username.begin(), ::toupper);
    std::string upper_search = username_entry.get_text();
    std::transform(upper_search.begin(), upper_search.end(), upper_search.begin(), ::toupper);
    USER_DATA type = search_type.get_active()->get_value(options_columns.option);

    //returns depending on the search factors if the user follows them
    return (
        (
            //If option in username verify if the letters entered on the entry are in the begining of the username (case sensitive)
            type == USER_DATA::USERNAME && !upper_username.rfind(upper_search, 0)) ||
        (
            //If option in ID verify if the ID entered is the same as the one of the user
            type == USER_DATA::ID && row.get_value(users.get_columns().ID) == std::stoll(ID_entry.get_text())) ||
        (
            //If option in birthday, verify what we're searching for (month, day, both, none) and verify if the user birthday enter the category
            type == USER_DATA::BIRTHDAY &&
            ((
                 !month_check.get_active() && row.get_value(users.get_columns().birthday_month) != "ND") ||
             (month_check.get_active() && row.get_value(users.get_columns().birthday_month) == month_combo.get_active()->get_value(month_combo.get_months_columns().month))) &&
            ((
                 !day_check.get_active() && row.get_value(users.get_columns().birthday_day) != 0) ||
             (day_check.get_active() && row.get_value(users.get_columns().birthday_day) == day_adjustment->get_value()))) ||
        (
            //If option in other, verify what we're asking for and if the user follow this
            type == USER_DATA::OTHER &&
            !(
                (
                    introduction_check.get_active() && !row.get_value(users.get_columns().introduction)) ||
                (review_check.get_active() && !row.get_value(users.get_columns().review)))));
}

void user_search::copy_user_row(Gtk::TreeModel::Row &receiving_row, Gtk::TreeModel::Row giving_row, user_list &users)
{
    //Just copies the data from the giving row to the receiving row
    receiving_row[users_visible_columns.username] = giving_row.get_value(users.get_columns().username);
    receiving_row[users_visible_columns.tag] = giving_row.get_value(users.get_columns().tag);
    receiving_row[users_visible_columns.ID] = giving_row.get_value(users.get_columns().ID);
    receiving_row[users_visible_columns.birthday_month] = giving_row.get_value(users.get_columns().birthday_month);
    receiving_row[users_visible_columns.birthday_day] = giving_row.get_value(users.get_columns().birthday_day);
    receiving_row[users_visible_columns.introduction] = giving_row.get_value(users.get_columns().introduction);
    receiving_row[users_visible_columns.review] = giving_row.get_value(users.get_columns().review);
}

void user_search::copy_warn_row(Gtk::TreeModel::Row &receiving_row, Gtk::TreeModel::Row giving_row, warn_list &warns)
{
    //Just copies the data from the giving row to the receiving row
    receiving_row[warns_visible_columns.username] = giving_row.get_value(warns.get_columns().username);
    receiving_row[warns_visible_columns.ID] = giving_row.get_value(warns.get_columns().ID);
    receiving_row[warns_visible_columns.warn_ID] = giving_row.get_value(warns.get_columns().warn_ID);
    receiving_row[warns_visible_columns.reason] = giving_row.get_value(warns.get_columns().reason);
}

void user_search::on_close()
{
    hide();
}

void user_search::launch_search()
{
    //reset all the values and show the window
    set_size_request(420, 200);
    search_type.set_active(0);
    on_type_changed();
    //username
    username_entry.set_text("");
    //ID
    ID_entry.set_text("0");
    //birthday
    month_combo.set_active(months::get_date().first);
    on_month_changed();
    day_adjustment->set_value(months::get_date().second);
    month_check.set_active(true);
    month_combo.set_sensitive(true);
    day_check.set_active(true);
    day_spin.set_sensitive(true);
    //other (introduction/review)
    introduction_check.set_active(false);
    review_check.set_active(false);

    //userlist
    user_frame.hide();
    no_users_label.hide();
    //warnlist
    warn_frame.hide();
    no_warns_label.hide();

    search_button.set_sensitive(true);
    show();
}