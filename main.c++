#include "toolbar.h"

int main(int argc, char *argv[])
{
    //Set the environment variable for Gtk to not show any window decoration
    putenv("GTK_CSD=0");

    //Create the application 
    Glib::RefPtr<toolbar> application = toolbar::create();

    // Start the application, showing the initial window, and return when all window are closed
    const int status = application->run(argc, argv);
    return status;
}