#ifndef DIALOG_H
#define DIALOG_H

#include <string>

class window;

class dialog
{
    public :
        //All the methods to make different Gtk::MessageDialog appear
        static void error_dialog(window &win, std::string main, std::string secondary);
        static void about_dialog(window &win);
        static std::string open_file_dialog(window &win);
        static std::string save_file_dialog(window &win);
};

#endif //DIALOG_H