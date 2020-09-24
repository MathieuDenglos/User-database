#ifndef USER_COLUMNS_H
#define USER_COLUMNS_H

#include <gtkmm/treemodel.h>
#include <string>

class user_columns : public Gtk::TreeModel::ColumnRecord
{
public:
    //constructor that adds the different warns data to the columns
    user_columns()
    {
        add(username);
        add(tag);
        add(ID);
        add(birthday_month);
        add(birthday_day);
        add(introduction);
        add(review);
    }

    //defines a type for each element
    Gtk::TreeModelColumn<std::string> username;
    Gtk::TreeModelColumn<int> tag;
    Gtk::TreeModelColumn<long long> ID;
    Gtk::TreeModelColumn<std::string> birthday_month;
    Gtk::TreeModelColumn<int> birthday_day;
    Gtk::TreeModelColumn<bool> introduction;
    Gtk::TreeModelColumn<bool> review;
};

#endif //USER_COLUMNS_H