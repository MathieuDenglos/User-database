#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include <exception>

class window;

struct action_canceled : public std::exception
{
private:
    const char *what_message = "action canceled, cannot continue";

public:
    action_canceled(const char *what_message) : what_message(what_message) {}
    action_canceled() = default;

    const char *what() const noexcept { return what_message; }
};

class dialog
{
public:
    //All the methods to make different Gtk::MessageDialog appear
    static void error_dialog(window &win, std::string main, std::string secondary);
    static void about_dialog(window &win);
    static bool auto_save_dialog(window &win);
    static std::string open_file_dialog(window &win);
    static std::string save_file_dialog(window &win);
};

#endif //DIALOG_H