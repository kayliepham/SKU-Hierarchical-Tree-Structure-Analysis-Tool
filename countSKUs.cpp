//
// Name: Kaylie Pham
// RedID: 828129478
//

#include <iostream>
#include "SKUTree.h"
#include <fstream>

int main(int argc, char **argv) {
    // check if correct number of arguments are passed (3)
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << "<skuchart-file> <test-file>" << std::endl;
        return 1; // exit program
        }

    // store files into strings
    const char* SKUChart_file = argv[1];
    const char* test_file = argv[2];

    std::ifstream SKUChart_file_stream(SKUChart_file); // open SKU chart file for reading
    std::ifstream test_file_stream(test_file); // open test file for reading

    // if SKUChart file is non-existent, print error message
    if (!SKUChart_file_stream.is_open()) {
        std::cout << "Unable to open <<skuchart.txt>>" << std::endl;
        return 1; // exit program, error occurred
    }

    /// if test file is non-existent, print error message
    if (!test_file_stream.is_open()) {
        std::cout << "Unable to open <<testfile.txt>>" << std::endl;
        return 1; // exit program, error occurred
    }

    // get internal levels from first line of SKU Chart file
    std::string internal_level_line = "";
    std::getline(SKUChart_file_stream, internal_level_line);

    // get max children per level from second line of SKU chart file
    std::string max_children_line = "";
    std::getline(SKUChart_file_stream, max_children_line);

    // create Tree object with internal level and max children info
    SKUTree* SKUChart = new SKUTree(internal_level_line, max_children_line);

    //read SKU ID Paths from SKU chart file and add SKUNodes to the ree
    std::string SKUPath = "";
    while (std::getline(SKUChart_file_stream, SKUPath)) {
        SKUChart->addSKU(SKUPath.c_str());
    }

    // read SKU ID Paths from test file and output number of SKU Nodes below current node
    std::string SKUPath_test = "";
    while (std::getline(test_file_stream, SKUPath_test)) {
            unsigned int node_count = 0;
            SKUChart->countNumOfSKUsInSKUChart(SKUPath_test, node_count);
            std::cout << SKUPath_test << " " << node_count << std::endl;
    }

    return 0; // exit program
}



