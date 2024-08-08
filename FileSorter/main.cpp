//July 2024
//Made for Margolian Dental

//Converting of: Apteryx XrayVision to Carestream Dental Imaging

//Convert from "STEPWEDGEOP5_17925" format to
//  ABELDent/DATA/NOTES/PN#/##/##IMGS format

//Take last 5 numbers from original format and split into 3 parts,
//coresponding to the folder they will be placed into

//Notes: ENSURE CHANGE OF FOLDER LOCATIONS LOCATED IN INT MAIN BEFORE RUNNING
#include <iostream>
#include <filesystem>
#include <string>
#include <unordered_set>

namespace fs = std::filesystem;

// Copy directory contents recursively
// done this way so if folders are inside other folders they will all be accounted for
void copyDirectoryRecursively(const fs::path& source, const fs::path& destination) {
    try {
        // Create the destination directory if it doesn't exist
        if (!fs::exists(destination)) {
            fs::create_directories(destination);
        }

        // Iterate over the contents of the source directory
        for (const auto& entry : fs::directory_iterator(source)) {
            const auto& path = entry.path();
            auto destPath = destination / path.filename();

            if (fs::is_directory(path)) {
                // Recursively copy subdirectory
                copyDirectoryRecursively(path, destPath);
            } else if (fs::is_regular_file(path)) {
                // Copy file
                try {
                    fs::copy_file(path, destPath, fs::copy_options::overwrite_existing);
                } catch (const fs::filesystem_error& e) {
                    std::cerr << "Error copying file " << path << ": " << e.what() << std::endl;
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void copyFilesIntoFolders(const std::string& sourcePath, const std::string& destinationPath, const std::string& duplicatePath) {
    try {
        // Check if source path exists and is a directory
        if (!fs::exists(sourcePath) || !fs::is_directory(sourcePath)) {
            std::cerr << "Source path does not exist or is not a directory." << std::endl;
            return;
        }

        // Create destination directory if it doesn't exist
        if (!fs::exists(destinationPath)) {
            fs::create_directories(destinationPath);
        }

        // To avoid creating the same directories multiple times, use a set to keep track of created paths
        std::unordered_set<std::string> createdPaths;

        // Iterate over folders in the source directory
        for (const auto& entry : fs::directory_iterator(sourcePath)) {
            const auto& path = entry.path();
            if (fs::is_directory(path)) {
                std::string folderName = path.filename().string();
                if (folderName.length() >= 5) {
                    // Extract relevant digits
                    std::string fifthLastDigit = "PN" + folderName.substr(folderName.length() - 5, 1);
                    std::string thirdFourthLastDigits = folderName.substr(folderName.length() - 4, 2);
                    std::string lastTwoDigits = folderName.substr(folderName.length() - 2) + "IMGS";

                    // Create the nested folder structure
                    std::string nestedFolderPath = destinationPath + "/" + fifthLastDigit + "/" + thirdFourthLastDigits + "/" + lastTwoDigits;

                    // Check if the last ##IMGS folder exists
                    if (fs::exists(nestedFolderPath)) {
                        std::cout << "Path location already exists: " << nestedFolderPath << std::endl;
                        // Create directory in duplicatePath and copy the entire folder
                        std::string newDuplicatePath = duplicatePath + "/" + folderName;
                        if (!fs::exists(newDuplicatePath)) {
                            fs::create_directories(newDuplicatePath);
                        }
                        copyDirectoryRecursively(path, newDuplicatePath);
                    } else {
                        // Create directories if they haven't been created yet
                        fs::create_directories(nestedFolderPath);

                        // Copy directory contents recursively
                        copyDirectoryRecursively(path, nestedFolderPath);
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
    //Converting from this folder (change everything inside the " ")
    std::string sourceFolder = "C:/Users/Dental/Desktop/BackUPFullData"; // Replace with your source directory path

    //Copying into this folder (change everything inside the " ")
    std::string destinationFolder = "D:/COPIED_DATA"; // Replace with your destination directory path

    //Copying all folders with duplicate numbers into this folder
    std::string duplicateFolder = "D:/Duplicate_Number_Present"; // Replace with your destination directory path

    copyFilesIntoFolders(sourceFolder, destinationFolder, duplicateFolder);

    return 0;
}
