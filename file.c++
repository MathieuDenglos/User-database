#include "file.h"
#include "dialog.h"
#include "window.h"

#include <fstream>
#include <sstream>
#include <iomanip>

bool file::file_opener(window &win)
{
    //This program will return true if it is the first opened somewhat correctly
    //Grab the filename using the Gtk::FileChooserDialog, and open the selected text file
    std::string file_path;
    file_path = dialog::open_file_dialog(win);
    std::ifstream file(file_path);

    //First lets verify if the file is opened.
    if (!file.is_open())
    {
        if (file_path != "ND")
        {
            //If it doesn't detect any files, even after the users selection, show the error message
            dialog::error_dialog(win, "Error while loading the file",
                                 "Couldn't find the selected file, please select a valid file or create a new list");
        }
        return false;
    }
    else
    {
        //verify if the file have the correct format by comparing the third line
        //This verification is used in order to prevent biased data from entering
        //Note that this system is not flawless, however for our use it's going to be enough
        std::string line;
        for (int i = 0; i < 3; i++)
            std::getline(file, line);
        if (line != "─────────────────────────────────────────────────────────────────────────────────────────────────────")
        {
            //If not of the correct format, show a Gtk::MessageDialog in error mode stating the issue, and create a new blank list if it's the first file
            if (files_path.size() == 0)
            {
                dialog::error_dialog(win, "This file doesn't have the right format",
                                     "Please select a file with the correct format, a new blank list is created");
                return true;
            }
            else
            {
                dialog::error_dialog(win, "This file doesn't have the right format",
                                     "Please select a file with the correct format");
                return false;
            }
        }
        else if (verify_file(file_path))
        {
            //If the file already got opened, show a Gtk::MessageDialog in error mode stating the issue
            dialog::error_dialog(win, "This file was already opened previously",
                                 "No data to grab, try opening another file");
            return false;
        }
        else
        {
            //If true, adds the data to the userlist and the warnlist
            std::string line, temp;
            std::stringstream information;
            int add_users, add_warns;

            //Comes back to the begining of the document
            file.seekg(0);

            //First, lets collect the amount of user we'll potentially add
            //This information is always (in a correct file) stored as the second element of line 1
            std::getline(file, line);
            information.str(line);
            information >> temp;
            information >> add_users;

            //Skip the next 2 lines, only here for readability
            for (int i = 0; i < 2; i++)
                std::getline(file, line);

            //Then grab all datas around the users
            for (int i = 1; i <= add_users; i++)
            {
                std::getline(file, line);
                win.get_user_list().grab_data(line);
            }

            //Skip the empty line between the userlist and the warnlist
            std::getline(file, line);

            //Now lets collect the amount of warns to potentially add like we did for the usercount
            std::getline(file, line);
            information.str(line);
            information >> temp;
            information >> add_warns;

            //Skip the next 2 lines, only here for readability
            for (int i = 0; i < 2; i++)
                std::getline(file, line);

            //Then grab all datas around the warns, similarily to the user data
            for (int i = 1; i <= add_warns; i++)
            {
                std::getline(file, line);
                win.get_warn_list().grab_data(line, win.get_user_list());
            }

            //adds the file to the files list and close it
            files_path.push_back(file_path);
            file.close();

            //tells the user that the file isn't saved yet
            win.change_title(false);

            if (files_path.size() == 1)
            {
                if (add_users != 0 && win.get_user_list().get_model()->children().size() == 0)
                    dialog::error_dialog(win, "This file is empty",
                                         "opening a new list");
                return true;
            }
            else
                return false;
        }
    }
}

void file::file_saver(window &win, bool saveas)
{
    //Grab the filename using the Gtk::FileChooserDialog, and open/create the text file, to save data in the file
    std::string file_path;
    file_path = (!saveas && is_output_file()) ? output_file : dialog::save_file_dialog(win);
    std::ofstream file(file_path);

    //First lets verify if the file is open.
    if (!file.is_open())
    {
        if (file_path != "ND")
        {
            //If it doesn't detect any files, even after the users selection (file_name != "ND")
            dialog::error_dialog(win, "Error creating the file",
                                 "Please try again");
        }
    }
    else
    {
        //If the file is open, we will now save the file with the correct format
        std::string line;

        //Clear the file
        file.clear();

        //Add the presentation to the file (vital in for the file to be opened another time and for lisibility)
        file << "───────────────────────────────────────────  " << std::setw(3) << std::right << std::setfill('0') << win.get_user_list().get_model()->children().size() << " members  ───────────────────────────────────────────" << std::endl;
        file << "Username                          tag  ID       birthday : Month     Day  Intro  Review  warns   " << std::endl;
        file << "─────────────────────────────────────────────────────────────────────────────────────────────────────" << std::endl;

        //Now create a long string with all the user information
        //The vector of long long is used to store all ID of the warns, to count the amount of warn of each users
        file << win.get_user_list().save_user_list(win.get_warn_list()) << std::endl;

        //Adding presentation again for lisibility
        file << "──────────────────────────────  " << std::setw(3) << std::right << std::setfill('0') << win.get_warn_list().get_model()->children().size() << " warns  ─────────────────────────────" << std::endl;
        file << "WarnID  Username                          ID                   Reason   " << std::endl;
        file << "────────────────────────────────────────────────────────────────────────" << std::endl;

        //Create a long string with all the warn data
        file << win.get_warn_list().save_warn_list() << std::endl;

        //If the file was never opened before, add it to the files_path vector
        if (!verify_file(file_path))
            files_path.push_back(file_path);

        output_file = file_path;
        win.output_file = file_path;

        file.close();

        //Tells the user that the file is saved
        win.change_title(true);
    }
}

void file::auto_saver(window &win)
{
    //Grab the filename using the Gtk::FileChooserDialog, and open/create the text file, to save data in the file
    std::ofstream file(win.output_file);

    //If the file is open, we will now save the file with the correct format
    std::string line;

    //Clear the file
    file.clear();

    //Add the presentation to the file (vital in for the file to be opened another time and for lisibility)
    file << "───────────────────────────────────────────  " << std::setw(3) << std::right << std::setfill('0') << win.get_user_list().get_model()->children().size() << " members  ───────────────────────────────────────────" << std::endl;
    file << "Username                          tag  ID       birthday : Month     Day  Intro  Review  warns   " << std::endl;
    file << "─────────────────────────────────────────────────────────────────────────────────────────────────────" << std::endl;

    //Now create a long string with all the user information
    //The vector of long long is used to store all ID of the warns, to count the amount of warn of each users
    file << win.get_user_list().save_user_list(win.get_warn_list()) << std::endl;

    //Adding presentation again for lisibility
    file << "──────────────────────────────  " << std::setw(3) << std::right << std::setfill('0') << win.get_warn_list().get_model()->children().size() << " warns  ─────────────────────────────" << std::endl;
    file << "WarnID  Username                          ID                   Reason   " << std::endl;
    file << "────────────────────────────────────────────────────────────────────────" << std::endl;

    //Create a long string with all the warn data
    file << win.get_warn_list().save_warn_list() << std::endl;

    file.close();
}

bool file::verify_file(std::string file_path)
{
    //grabs each file_path, compare them to the one sent and if it finds the same one return true
    //otherwise return false and continue the process
    for (std::string comparison : files_path)
        if (comparison == file_path)
            return true;

    return false;
}