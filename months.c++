#include "months.h"

#include <chrono>
#include <ctime>

//Add the different elements to the map
const std::map<std::string, int> months::months_map = {{"JANUARY", 1},
                                                       {"FEBRUARY", 2},
                                                       {"MARCH", 3},
                                                       {"APRIL", 4},
                                                       {"MAY", 5},
                                                       {"JUNE", 6},
                                                       {"JULY", 7},
                                                       {"AUGUST", 8},
                                                       {"SEPTEMBER", 9},
                                                       {"OCTOBER", 10},
                                                       {"NOVEMBER", 11},
                                                       {"DECEMBER", 12}};

const std::pair<int, int> months::get_date()
{
    //grab the actual time of the system
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    //converts it to a tm, to grab the day and month
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm local = *localtime(&tt);

    //returns a pain with the month and the day
    return {local.tm_mon, local.tm_mday};
}

months::months()
{
    //First let's create the ListStore treemodel and assign it to our treeview
    months_model = Gtk::ListStore::create(months_columns);
    set_model(months_model);

    //Now lets create our 12 rows and add all the data
    Gtk::TreeModel::Row row = *(months_model->append());
    row[months_columns.month] = "January";
    row[months_columns.days] = 31;

    row = *(months_model->append());
    row[months_columns.month] = "February";
    row[months_columns.days] = 29;

    row = *(months_model->append());
    row[months_columns.month] = "March";
    row[months_columns.days] = 31;

    row = *(months_model->append());
    row[months_columns.month] = "April";
    row[months_columns.days] = 30;

    row = *(months_model->append());
    row[months_columns.month] = "May";
    row[months_columns.days] = 31;

    row = *(months_model->append());
    row[months_columns.month] = "June";
    row[months_columns.days] = 30;

    row = *(months_model->append());
    row[months_columns.month] = "July";
    row[months_columns.days] = 31;

    row = *(months_model->append());
    row[months_columns.month] = "August";
    row[months_columns.days] = 31;

    row = *(months_model->append());
    row[months_columns.month] = "September";
    row[months_columns.days] = 30;

    row = *(months_model->append());
    row[months_columns.month] = "October";
    row[months_columns.days] = 31;

    row = *(months_model->append());
    row[months_columns.month] = "November";
    row[months_columns.days] = 30;

    row = *(months_model->append());
    row[months_columns.month] = "December";
    row[months_columns.days] = 31;

    pack_start(months_columns.month);
}