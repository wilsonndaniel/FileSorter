// ConvertPatientIDsConsole.cpp
// Convert Patient IDs in an ipatient.dat file using a Conversion Map
// The Conversion Map is a two column file:
// 1st Column: Old/Existing Patient ID
// 2nd Column: New Patient ID
// 
// April 2023
// Created for conversion at: NORTH COLORADO HEALTH ALLIANCE - SUNRISE FAMILY
//


#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(void)
{
    // Define Constants to define the size of the Arrays that will hold the Conversion Map and ipatient.dat
    // These Define the maximum size of the files that can be read in for conversions
    const int maxRows = 160000; //The maximum number of possible rows in the Conversion Map (defines the Array size)
    const int maxCols = 2; //The maximum number of columns in the Conversion Map (always 2)
    const int maxiPatient = 30000; //The maximum number of possible rows in the ipatient.dat (defines the Array size)
    const int patientIDstart = 20; //The first position of the PatientID in the ipatient.dat file
    const int patientIDlength = 10; //The length of the PatientID field in the ipatient.dat file

    // Define the variables that will be set to the values of the actual Conversionmap.dat and ipatient.dat
    int totalRows = 0; //The total number of Rows in the Conversionmap.dat file being read
    int totaliPatient = 0; //The total number of Rows in the ipatient.dat file being read

    //Create the 2D Array to hold the ConversionMap[][] data on the Heap
    auto conversionMap = new string[maxRows][maxCols];

    //Create the 1D Array to hold the ipatient.data data one line at a time in ipatientline[] on the Heap
    auto ipatientline = new string[maxRows];

    int r = 0;  //counter for rows in the Conversion Map
    int c = 0;  //counter for columns in the Conversion Map
    int iline = 0; //counter for lines in the ipatient data
    int s = 0; //counter for successful conversions
    int f = 0; //counter for failed conversions
    int count = 0; //counter for number of lines in a file
    string line; //holder for contents read from lines of a file
    int match = 0; // flag to define when a match has been made with the Conversion Map

    //DISPLAY THE USAGE INFO
    cout << endl;
    cout << endl;
    cout << "################################################################" << endl;
    cout << "#                                                              #" << endl;
    cout << "#     ConvertPatientIDsConsole.exe                             #" << endl;
    cout << "#                                                              #" << endl;
    cout << "#     Patient ID Converion Tool                                #" << endl;
    cout << "#                                                              #" << endl;
    cout << "#     Copyright PatientGallery 2023                            #" << endl;
    cout << "#                                                              #" << endl;
    cout << "################################################################" << endl;
    cout << endl;
    cout << endl;
    cout << "This application runs at the command line and converts an existing ipatient.dat file" << endl;
    cout << "to a new file (ipatientnew.dat) with updated Patient IDs." << endl;
    cout << "Updated Patient IDs are supplied in the Conversion Map (Conversionmap.dat)." << endl;
    cout << endl;
    cout << "This is a useful tool in situations where a client has upgraded or changed their software" << endl;
    cout << "and the new sofware usage has forced a change in the Patient IDs." << endl;
    cout << endl;
    cout << "RELATED FILES:" << endl;
    cout << "  ConvertPatientIDsConsole.exe - This execuable that converts the ipatient.dat -> ipatientnew.dat" << endl;
    cout << "  ConversionMap.dat - The PatientID mapping file: 1st column=Old ID, 2nd column=New ID, no headers" << endl;
    cout << "                      Tab delimited columns. Make sure that the patient IDs are never longer than 10 digita." << endl;
    cout << "                      Keep the file shorter than 100,000 entries. If it is longer, break it up and run several" << endl;
    cout << "                      iterations of the conversion." << endl;
    cout << "  ipatient.dat - The original/old ipatient.dat file." << endl;
    cout << "  ipatientnew.dat - The new ipatient.dat file that is output with the converted Patient IDs." << endl;
    cout << "  goodConversions.log - The output log file showing all Patient IDs that have been converted successfully." << endl;
    cout << "  missingPatientIDS.log - The output log file with the list of Patient IDs that were not found in the Conversion Map." << endl;
    cout << "All files in the same directory.";
    cout << endl;
    cout << endl;
    cout << "PRESS <ENTER> TO CONVERT ipatient.dat to ipatientnew.dat using the supplied Conversionmap.dat." << endl;
    cin.get(); //wait for user input


    //DETERMINE HOW MANY LINES ARE IN EACH OF THE INPUT FILES: ConversionMap.dat and ipatient.dat

    // Open the ConversionMap.dat file for reading (1st column is the old Patient ID, 2nd column is the new Patient ID, space in between)
    ifstream conversionMapFile("ConversionMap.dat");
    // Check if the file is open, count the number of lines, or report that the file failed to open
    if (conversionMapFile.is_open())
    {
        while (conversionMapFile.peek() != EOF)
        {
            getline(conversionMapFile, line);
            count++;
        }
        totalRows = count ; //The total number of Rows in the Conversionmap.dat file being read
        cout << "Number of lines in the Conversionmap.dat file: " << totalRows << endl;
    }
    else
        cout << "Couldn't open the file\n";
 
    // Open the ipatient.dat file for reading (no writing necessary)
    ifstream ipatientFile("ipatient.dat");
    // Check if the file is open, count the number of lines, or report that the file failed to open
    count = 0;
    if (ipatientFile.is_open())
    {
        while (ipatientFile.peek() != EOF)
        {
            getline(ipatientFile, line);
            count++;
        }
        totaliPatient = count ; //The total number of Rows in the ipatient.dat file being read
        cout << "Number of lines in the ipatient.dat file: " << totaliPatient << endl;
    }
    else
        cout << "Couldn't open the file\n";
    

    //READ IN THE CONVERSION MAP
    cout << endl;
    cout << "Reading in the Patient ID Conversion Mapping File" << endl;
    cout << endl;

    // Clear the EOF for the Conversion Map file to start reading from the beginning again
    conversionMapFile.clear();
    conversionMapFile.seekg(0);
    
    //Defining the loop for getting input from the file
    for (r = 0; r < totalRows; r++) //Outer loop for rows
    {
        for (c = 0; c < maxCols; c++) //inner loop for columns
        {
            conversionMapFile >> conversionMap[r][c];  //Take input from file and put into mconversionMap[][] Array
            conversionMap[r][c].append(patientIDlength - conversionMap[r][c].length(), ' '); // Pad both entries in the row with spaces to the right to stretch to the full PatientID length
        }
    }


//    Display the Conversion Map Data, if you would like. 
//          for (r = 0; r < totalRows; r++)
//          {
//              cout << "Conversion Map: " << r << ": ";
//              for (c = 0; c < maxCols; c++)
//          {
//              cout << conversionMap[r][c] << "\t";
//          }
//          cout << endl;
//          }

    // Close the Conversion Map File - It has been read into the Array - no need to keep it open.
    conversionMapFile.close();

    cout << "The Patient ID Conversion Mapping File has been stored." << endl;
    cout << "There are a total of " << r << " Mapping Pairs." << endl;
    cout << endl;

    //    cout << "Now let's check that the data from the Conversion Map has been received. " << endl; // Checking that data was received
    //    cout << "Type a row: "; // Type a number and press enter
    //    r = 0;
    //   cin >> r; // Get user input from the keyboard
    //    cout << "Type a column: "; // Type a number and press enter
    //    c = 0;
    //   cin >> c; // Get user input from the keyboard
    //    cout << "For row: " << r << " and column:" << c << "  : "; // Type a number and press enter
    //    cout << conversionMap[r][c] << endl;

    //READ IN THE IPATIENT.DAT
    cout << "Reading the existing ipatient.dat file" << endl;
    cout << endl;

    // Clear the EOF for the ipatient.dat file to start reading from the beginning again
    ipatientFile.clear();
    ipatientFile.seekg(0);

    //Defining the loop for getting input from the ipatient.dat file

    for (iline = 0; iline < totaliPatient; iline++) //Loop for lines
    {

        getline(ipatientFile, ipatientline[iline]);
        //            cout << "ipatient.dat: " << "iline: " << iline << " " << ipatientline[iline].substr(0, 80) << endl; //Display this onscreen as it is being read in, if you like
    }

    //       for (iline = 0; iline < maxRows; iline++)
    //       {
    //           cout << iline << ipatientline[iline] << endl;
    //       }
    //       cout << endl;

    ipatientFile.close();

    cout << "The existing ipatient.dat file has been read for processing." << endl;
    cout << "There are a total of " << iline << " lines in the existing ipatient.dat file." << endl;
    cout << endl;


    // OPEN THE .DAT AND LOG FILES FOR STORING THE OUTPUT DATA
  
    // Open the new ipatientNew.dat file for writing
    ofstream ipatientFileNew("ipatientnew.dat", std::ios::trunc);

    if (!ipatientFileNew.is_open())
        cout << "Error opening file";

    // Open a new goodConversions.log file for writing successful conversions
      ofstream goodConversionsFile("goodConversions.log");

    if (!goodConversionsFile.is_open())
        cout << "Error opening file";
    // Write the title and date at the top of the log
    goodConversionsFile << "SUCCESSFUL CONVERSIONS LOG: " << endl;

    // Open a new missingPatientIDs.log file for writing entries that do not have corresponding PatientIDs in the Conversion Map
    ofstream missingPatientIDsFile("missingPatientIDs.log");

    if (!missingPatientIDsFile.is_open())
        cout << "Error opening file";
    // Write the title and date at the top of the log
    missingPatientIDsFile << "PATIENT RECORDS WITH MISSING PATIENT ID MAPPINGS (FAILED PATIENT ID CONVERSION) LOG: " << endl;


    //#####################################################
    //BEGIN CONVERTING PATIENT IDS USING THE CONVERSION MAP
    //#####################################################
  
    // Evaluate the existing lines of the ipatient.dat file for matches in the Conversion Map
    r = 0;
    iline = 0;
    for (iline = 0; iline < totaliPatient; iline++) //Loop through all lines of ipatientline[]
    {
        match = 0; // We have not matched yet - set match to NO(0)
//        cout << "Processing iline: " << iline << endl;
        for (r = 0; r < totalRows; r++) //Loop through all rows of conversionMap[][]
        {
            if (conversionMap[r][0].compare(ipatientline[iline].substr(patientIDstart, patientIDlength)) == 0) //Does the Conversion Map == the Patient ID in ipatientlinne[]?
            {
                //Report the match on screen if you would like (runs a lot slower if you do)
 //               cout << iline << " Match Found for PatientID: " + ipatientline[iline].substr(patientIDstart, patientIDlength);
 //               cout << "Converted to New ID: " << conversionMap[r][1] << "@ r: " << r;
 //               cout << endl;
                //Report the match in the goodConversions.log log file
                goodConversionsFile << s << " " << iline << " Match Found for PatientID: " + ipatientline[iline].substr(patientIDstart, patientIDlength);
                goodConversionsFile << "Converted to New ID: " << conversionMap[r][1] << "@ r: " << r;
                goodConversionsFile << endl;
                s++;
                // Replace the existing line of the ipatient Array with the new Conversion Map value using the replace() function
                ipatientline[iline].replace(patientIDstart, patientIDlength, conversionMap[r][1]);
                // Write the newly modified ipatient Array line into the ipatientnew.dat file
                ipatientFileNew << ipatientline[iline] << endl;
                r = totalRows; // We matched so move to the end of the conversionMap rows
                match = 1; // We matched so set match to YES(1)
            }
            else
            {
                // No Match. Do nothing. Move on to the next pair in the Conversion Map.
            }
        }
        if (match == 0) //We did not match in the loop through the whole Conversion Map : Report It! 
        {
            // The entire Conversion Map has been checked and no match for the PatientID was found (unless match=1) We are missing a Conversion ID for this patient
            // Report the missing PatientID on the screen (if you would like - runs much slower if you do)
 //           cout << iline << " No Match Found: "; // Reached end of ConversionMap[][] with no matches in ipatientlines[] REPORT!
 //           cout << "for existing Patient ID : " << ipatientline[iline].substr(patientIDstart, patientIDlength);
 //           cout << endl;
            // Report the missing PatientID in the missingPatientIDs.log file
            missingPatientIDsFile << f << " " << iline << " No Match Found: "; // Reached end of ConversionMap[][] with no matches in ipatientlines[] REPORT!
            missingPatientIDsFile << "for existing Patient ID : " << ipatientline[iline].substr(patientIDstart, patientIDlength);
            missingPatientIDsFile << endl;
            ipatientFileNew << ipatientline[iline] << endl; // Write the unmodified ipatient.dat line into the ipatientnew.dat file
            f++;
        }
        else // We matched during the run through the Conversion Map so no need to report anything
        {
        }
    }
     //#####################################################
     //END CONVERTING PATIENT IDS USING THE CONVERSION MAP
     //#####################################################

   
    cout << "The Conversion Map has been successfully processed. Please review the log files for conversion results." << endl;
    cout << endl;


    // CLOSE THE ipatientnew.dat FILE

    ipatientFileNew.close();

    cout << "The new ipatientnew.dat file has been stored." << endl;
    cout << "There are a total of " << iline << " lines written to the new ipatientnew.dat file." << endl;
    cout << endl;


    // CLOSE THE LOG FILES AND FINISH UP

    // Reporting on number of successful conversions:
    goodConversionsFile << "Successful Conversions: " << s << endl; // Reporting to log file
    cout << "Completed writing the goodConversions.log file." << endl;
    cout << "Successful Conversions: " << s << endl;
    cout << endl;

    goodConversionsFile.close();

    // Reporting on the number of failed conversions:
    missingPatientIDsFile << "Failed Conversions: " << f << endl; // Reporting to log file
    cout << "Completed writing of the missingPatientIDs.log file." << endl;
    cout << "Failed conversions (missing IDs in the Conversion Map): " << f << endl;
    cout << endl;

    missingPatientIDsFile.close();

    cout << "Patient ID Conversion Completed.";
    cout << endl;
    cout << endl;

    delete[] conversionMap;
    delete[] ipatientline;

    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
