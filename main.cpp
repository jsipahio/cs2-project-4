//
//  main.cpp
//  Profiler
//
//  Created by Jonathan Maletic.
//  Copyright 2025 Kent State University. All rights reserved.
//  Spring 2025
//  srcML 1.0
//
//  Modified by:
//
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "ASTree.hpp"

// Simple function to exercise/test copy-ctor, dtor, swap, assignment.
//
void testCopyAssign(srcML p) {
    std::cout << "The orginal code: " <<std::endl;
    std::cout << "------------------------------------------------" <<std::endl;
    std::cout << p;   //Should print out same code as printed in main.
    std::cout << "------------------------------------------------" <<std::endl;
    std::cout << "No code: " <<std::endl;
    p = srcML();      //Should print out nothing.
    std::cout << p;
    std::cout << "------------------------------------------------" <<std::endl;
}


// Input:  One or more srcML files, main must be first file
// Output: One or more cpp files (with p- prefix) that are insturmented
//
// Example: profile main.cpp.xml utils.cpp.xml
// Results: p-main.cpp p-utlis.cpp
//
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Input file(s) are required." << std::endl;
        std::cerr << "       The main must be the first argument followed by ";
        std::cerr << "any other .cpp files.  For example:" << std::endl;
        std::cerr << "profiler main.cpp.xml file1.cpp.xml file2.cpp.xml";
        std::cerr << std::endl << std::endl;
        return(1);
    }

    srcML                     code;           //Source code to be profiled.
    std::vector<std::string>  inputName;      //Input file names (foo.cpp.xml)
    std::vector<std::string>  fileName;       //File names       (foo.cpp)
    std::vector<std::string>  profileName;    //Profile names    (foo_cpp)

    for (int i = 1; i < argc; ++i) {
        std::string temp;
        temp = argv[i];                                   //Get foo.cpp.xml
        inputName.push_back(temp);                        //Put in list
        temp = temp.substr(0, temp.find(".xml"));         //Remove .xml
        fileName.push_back(temp);                         //Put in list
        std::replace(temp.begin(), temp.end(), '.', '_'); //convert . to _
        profileName.push_back(temp);                      //Put in list
    }

    std::ifstream inFile(inputName[0].c_str());    //Read in the main
    inFile >> code;
    inFile.close();

    code.insertMainHeader(profileName, fileName);  //Add in main header
    code.insertMainReport(profileName);            //Add in the report
    code.insertFunctionCount(profileName[0]);      //Count funciton invocations
    code.insertLineCount(profileName[0]);          //Count line invocations

    std::string outFileName;
    outFileName = "p-" + fileName[0];
    std::ofstream outFile(outFileName.c_str());
    outFile << code << std::endl;
    outFile.close();

    for (unsigned i = 1; i < inputName.size(); ++i) { //Read rest of the files
        inFile.open(inputName[i].c_str());
        inFile >> code;
        inFile.close();

        code.insertFileHeader(profileName[i]);      //Add in file header info
        code.insertFunctionCount(profileName[i]);   //Count funciton invocations
        code.insertLineCount(profileName[i]);       //Count line invocations

        outFileName = "p-" + fileName[i];
        outFile.open(outFileName.c_str());
        outFile << code << std::endl;
        outFile.close();
    }

    return 0;
}
