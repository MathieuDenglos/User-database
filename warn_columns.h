#ifndef WARN_COLUMNS_H
#define WARN_COLUMNS_H

#include <gtkmm/treemodel.h>

class warn_columns : public Gtk::TreeModel::ColumnRecord
{
public:
    //constructor
    warn_columns()
    {
        add(username);
        add(ID);
        add(warn_ID);
        add(reason);
    }

    //defines a type for each element
    Gtk::TreeModelColumn<std::string> username;
    Gtk::TreeModelColumn<long long> ID;
    Gtk::TreeModelColumn<int> warn_ID;
    Gtk::TreeModelColumn<std::string> reason;
};

#endif //WARN_COLUMNS_H