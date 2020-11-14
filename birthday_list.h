#ifndef BIRTHDAY_LIST_H
#define BIRTHDAY_LIST_H

#include <string>

class birthday_list
{
private:
    //useful user informations
    std::string username;
    std::string birthday_month;
    int birthday_day;
    int rankdate;

public:
    //constructor
    birthday_list(int rankdate,
                  std::string username,
                  std::string month,
                  int day);

    //maximum username lenght
    static int max_lenght;

    //getter for the rankdate : for the sort
    [[nodiscard]] const int get_rankdate_const() const noexcept { return this->rankdate; }

    //operator overload to convert a user's data into a stringstream
    friend std::stringstream &operator<<(std::stringstream &, const birthday_list &);
};

#endif //BIRTHDAY_LIST_H