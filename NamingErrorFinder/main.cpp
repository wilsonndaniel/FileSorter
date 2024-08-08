//July 2024
//Use This After Running the FileSorter Code to find any specific outliers that were not sorted properly
//because of a naming error
//Example: A folder called PNL could have been created because of a naming erorr
//To find where this folder was created from this code will tell you the name of the folder with the 5th last digit you need

//Notes:
//Make sure to change line 32 & 47 as needed
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void findFoldersWithEAsFifthLastDigit(const std::string& sourcePath) {
    try {
        // Check if source path exists and is a directory
        if (!fs::exists(sourcePath) || !fs::is_directory(sourcePath)) {
            std::cerr << "Source path does not exist or is not a directory." << std::endl;
            return;
        }

        // Iterate over folders in the source directory
        for (const auto& entry : fs::directory_iterator(sourcePath)) {
            const auto& path = entry.path();
            if (fs::is_directory(path)) {
                std::string folderName = path.filename().string();
                if (folderName.length() >= 5) {
                    // Check if the 5th last digit is this character

                    //CHANGE AS NEEDED
                    if (folderName[folderName.length() - 5] == 'U') {
                        std::cout << "Folder with input as the 5th last digit: " << folderName << std::endl;
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    // Path to the source directory (change as needed)
    std::string sourceFolder = "C:/Users/Dental/Desktop/BackUPFullData";

    findFoldersWithEAsFifthLastDigit(sourceFolder);

    return 0;
}