//August 2024
//Deletes all files that are not .JPG in the folder

//NOTE: Change folder location before running in line 32

#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void deleteNonJpgFiles(const fs::path& directory) {
    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_directory(entry.status())) {
                // Recursively enter directories
                deleteNonJpgFiles(entry.path());
            } else if (fs::is_regular_file(entry.status())) {
                // Check if the file extension is not ".jpg"
                if (entry.path().extension() != ".JPG") {
                    fs::remove(entry.path());
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General exception: " << e.what() << std::endl;
    }
}

int main() {
    std::string rootDirectory = "D:/CONVERTED_DATA";

    deleteNonJpgFiles(rootDirectory);

    return 0;
}