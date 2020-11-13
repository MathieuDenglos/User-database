#ifndef MONTHS_H
#define MONTHS_H

#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <gtkmm/combobox.h>
#include <map>
#include <utility>

class months : public Gtk::ComboBox
{
private:
    //create the month columns with all the months, and the number of day they have
    //this is used for a combobox to select the birth month
    class columns : public Gtk::TreeModel::ColumnRecord
    {
    public:
        columns()
        {
            add(month);
            add(days);
        }

        Gtk::TreeModelColumn<std::string> month;
        Gtk::TreeModelColumn<int> days;
    };
    columns months_columns;
    Glib::RefPtr<Gtk::ListStore> months_model;

public:
    //constructor
    months();

    //map used to convert the month (std::string) to a interger (int) from 1 to 12
    //This map is used to create the rankdate and sort users by their birthday
    static const std::map<std::string, int> months_map;

    //getters for the date and the model and colums
    static const std::pair<int, int> get_date();
    const columns &get_months_columns() const { return months_columns; }
};

#endif //MONTHS_H