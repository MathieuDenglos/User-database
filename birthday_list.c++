#include "birthday_list.h"

#include <sstream>
#include <iomanip>

int birthday_list::max_lenght = 0;

birthday_list::birthday_list(int rankdate,
                             std::string username,
                             std::string month,
                             int day) : rankdate(rankdate),
                                        username(username),
                                        birthday_month(month),
                                        birthday_day(day) {}

std::stringstream &operator<<(std::stringstream &stream, const birthday_list &user)
{
    stream << std::setw(birthday_list::max_lenght) << std::left << std::setfill(' ') << user.username << " : "
           << std::setw(2) << std::right << std::setfill('0') << user.birthday_day << " "
           << std::left << user.birthday_month << std::endl;
}