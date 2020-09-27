#include "toolbar.h"

#include <gtkmm/settings.h>

int main(int argc, char *argv[])
{
//Set the environment variable for Gtk to not show any window decoration
#ifdef _WIN32
    putenv("GTK_CSD=0");
#endif

    //Create the application
    Glib::RefPtr<toolbar> application = toolbar::create();

    //Change the settings for the selected theme and font
    auto a = Gtk::Settings::get_default();
    a->property_gtk_theme_name() = "Windows10";
    a->property_gtk_font_name() = "Segoe UI 9";

    // Start the application, showing the initial window, and return when all window are closed
    const int status = application->run(argc, argv);
    return status;
}