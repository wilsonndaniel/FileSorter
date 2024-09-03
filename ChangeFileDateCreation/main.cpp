//August 2024
//Changes the file creation date to match with the one
//listed in the name of the file

//If an error occurs with a files name not matching the proper
//format the creation date will be changed to January 1, 2023

//NOTE: Change the folder location before running
#include <iostream>
#include <filesystem>
#include <windows.h>
#include <string>
#include <regex>

namespace fs = std::filesystem;

// Function to parse the date from the file name (only the date part "08152023")
bool parseDateFromFilename(const std::string& filename, SYSTEMTIME& st) {
    std::regex pattern(R"(_(\d{8})_)");  // Matches "_08152023_" part of the filename
    std::smatch matches;

    if (std::regex_search(filename, matches, pattern) && matches.size() == 2) {
        std::string datePart = matches[1].str();  // Extracts the "08152023" part
        st.wMonth = std::stoi(datePart.substr(0, 2));
        st.wDay = std::stoi(datePart.substr(2, 2));
        st.wYear = std::stoi(datePart.substr(4, 4));
        st.wHour = 12;  // Set to noon to avoid any issues with daylight saving time
        st.wMinute = 0;
        st.wSecond = 0;
        st.wMilliseconds = 0;
        return true;
    }
    return false;
}

// Function to set the creation time of a file
void setFileCreationTime(const fs::path& filePath, const SYSTEMTIME& st) {
    HANDLE hFile = CreateFileA(
        filePath.string().c_str(),  // Using narrow string version
        FILE_WRITE_ATTRIBUTES,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        FILETIME ft, localFt;
        SystemTimeToFileTime(&st, &ft);
        LocalFileTimeToFileTime(&ft, &localFt); // Convert local FILETIME to UTC
        if (!SetFileTime(hFile, &localFt, NULL, NULL)) {
            std::cerr << "Failed to set file time for: " << filePath << std::endl;
        }
        CloseHandle(hFile);
    } else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
}

// Recursive function to process files in directories
void processFiles(const fs::path& directory) {
    SYSTEMTIME defaultTime = { 2023, 1, 1, 1, 1, 0, 0, 0 };  // January 1, 2023, 12:00 PM

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_directory(entry.status())) {
                // Recursively process subdirectories
                processFiles(entry.path());
            } else if (fs::is_regular_file(entry.status())) {
                SYSTEMTIME st;
                if (!parseDateFromFilename(entry.path().filename().string(), st)) {
                    std::cerr << "Error with file name format: "
                              << entry.path() << std::endl;
                    st = defaultTime;
                }
                setFileCreationTime(entry.path(), st);
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General exception: " << e.what() << std::endl;
    }
}

int main() {
    std::string rootDirectory = "D:/CONVERTED_DATA/COPIED_DATA";
    processFiles(rootDirectory);

    return 0;
}