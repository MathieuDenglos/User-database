#ifndef BIRTHDAY_LIST_H
#define BIRTHDAY_LIST_H

#include <string>

class birthday_list
{
private:
    //basic user components
    std::string username;
    std::string birthday_month;
    int birthday_day;

public:
    //The rankdate is an element used to class the users by birthday
    //This will be used later on to classify the users by their birthday
    int rankdate;

    //All the methods to get the users data and make a string to show
    std::string get_line(int lenght);
    void set_info(int rank, std::string user, std::string month, int day);
    static bool comparison(birthday_list &one, birthday_list &two);
};

#endif //BIRTHDAY_LIST_H