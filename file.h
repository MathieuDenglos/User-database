#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>

class window;

class file
{
private:
    //Vector containing all the files path
    std::vector<std::string> files_path;

    //String containing the output file
    std::string output_file;

    //Method called to verify if a file path already exists or not
    bool verify_file(std::string file_path);

public:
    //Methods called by the signal handlers in order to open a file and
    bool file_opener(window &win);
    void file_saver(window &win, bool saveas);
    static void auto_saver(window &win);

    //getters and setters
    [[nodiscars]] constexpr const bool is_output_file() const noexcept { return !output_file.empty(); }
};

#endif //FILE_H