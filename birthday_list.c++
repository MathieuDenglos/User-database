#include "birthday_list.h"

#include <iostream>
#include <sstream>
#include <iomanip>

void birthday_list::set_info(int rank, std::string user, std::string month, int day)
{
    //grabs the useful data of the userlist and put it in the birthdaylist
    rankdate = rank;
    username = user;
    birthday_month = month;
    birthday_day = day;
}

std::string birthday_list::get_line(int lenght)
{
    std::stringstream line;

    //create the line with all the user information (username : day month)
    line << std::setw(lenght) << std::left << std::setfill(' ') << username << " : "
         << std::setw(2) << std::right << std::setfill('0') << birthday_day << " "
         << std::left << birthday_month << std::endl;

    //Now that the line is created, we just need to convert it to a string with .str() and return it
    return line.str();
}