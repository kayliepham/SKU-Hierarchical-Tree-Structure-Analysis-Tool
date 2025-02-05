//
// Name: Kaylie Pham
// RedID: 828129478
//

#include "SKUTree.h"
#include <sstream>
#include <cstring>
#include <iostream>

// initializes SKUTree attributes
SKUTree::SKUTree() : internalLevels(0), root(nullptr), max_children_per_level(0) {}

// initializes SKUTree attributes given internal levels and maximum children per level
SKUTree::SKUTree(std::string levels, const std::string& max_children) {
    // stores values for internal levels and max children
    set_max_children_per_level(max_children);
    set_internalLevels(levels);

    // allocate memory for root node
    root = new SKUNode();
    root->SKUId = strdup("s"); // root/head node has a SKUId of "s'
    int index_of_root_children = 0; // the max children of the root node is stored in index 0
    root->max_num_children = max_children_per_level[index_of_root_children];
    root->children = nullptr; // initialize children nodes to nullptr
}

// sets the max children per level vector given a string
void SKUTree::set_max_children_per_level(const std::string& max_children) {
    std::stringstream ss(max_children);
    int num = 0; // reads integers from string stream; 0 is a placeholder
    while (ss >> num) { // if it is a number, add to vector
        this->max_children_per_level.push_back(num);
    }
}

// sets the value for internal levels given a string
void SKUTree::set_internalLevels(std::string internal_levels) {;
    std::string internal_levels_string = "internalLevels=";
    size_t start_position_to_erase = internal_levels.find(internal_levels_string); // find the index of level statement
    int len_of_levels_string = internal_levels_string.length(); // 15 is the length pf "internalLevels="
    internal_levels.erase(start_position_to_erase, len_of_levels_string); // erases "internalLevels="
    this->internalLevels = std::stoi(internal_levels); // converts string to int and stores value
}

// returns max number of children given a path, depending on length of path
int SKUTree::get_max_children_per_level(const char* SKUPath) {
    // if length is 1, we need index 0; if length is 3, we need index 1; if length is 5, we need index 2...and so on
    int length_of_path = strlen(SKUPath);
    int children_index = length_of_path - (length_of_path / 2) - 1; // index = length - (length / 2) - 1
    return max_children_per_level[children_index];  // return max children value

}
// adds SKU Nodes to tree; for ex: if we have s_#_$, we need to check: s, s_#, s_#_$ in this order
bool SKUTree::addSKU(const char* SKUPath) {

    size_t current_length = 3; // starting with 3 because we already have root node s, so we need s_# first (len=3)
    int max_length = internalLevels + internalLevels + 1; // max length of path, accounting for "_" and "s" as well

    // while current length does not exceed maximum possible length and the path length
    while (strlen(SKUPath) >= current_length && current_length <= max_length) {

        // path for parent node and current node
        char parent_path[max_length] = "";
        char current_path[max_length] = "";
        int len_parent_path = current_length - 2; // parent node's path contains 2 less characters than current node's
        int new_character_index = current_length - 1; // last index of current node we are looking at
        int amt_to_add = 2; // after checking current node, add 2 (# and "_") to current_length to get next path

        strncpy(parent_path, SKUPath, len_parent_path);
        parent_path[len_parent_path] = '\0'; // end string
        strncpy(current_path, SKUPath, current_length);
        current_path[current_length] = '\0';   // end string

        SKUNode* parent_node = findSKU(root, parent_path); // get parent node of current path
        if (parent_node == nullptr) { return true; } // if parent node is invalid, return

        // store rightmost character (index) of current path as an integer
        char new_character = SKUPath[new_character_index];
        int int_new_char = new_character - '0'; // store as integer

        // if new index is out of range (larger than max), return and stop adding nodes
        if (int_new_char >= parent_node->max_num_children) {
            return true;
        }

        // if parent node children are not initialized yet, initialize to array of double pointers
        if (parent_node->children == nullptr) {
            parent_node->children = new SKUNode*[parent_node->max_num_children];
            for (int i = 0; i < parent_node->max_num_children; i++) { // initialize each child to nullptr
                parent_node->children[i] = nullptr;
            }
        }
        // if node does not already exist, create node and add it to parent node's children array
        if (parent_node->children[int_new_char] == nullptr) {
            SKUNode* new_node = new SKUNode();
            new_node->SKUId = strdup(current_path);
            new_node->max_num_children = get_max_children_per_level(current_path);
            new_node->children = nullptr;  // Initialize child pointer to nullptr
            parent_node->children[int_new_char] = new_node; // Assign the new node to the parent node's children array
        }
        current_length += amt_to_add; // increase the length of the path by 2, accounting for "_" as well
    }
    return true; // SKU added nodes successfully
}

// find and return the SKU node in the SKU tree based on a SKU path
SKUTree::SKUNode* SKUTree::findSKU(SKUNode* current_node, const char *SKUPath) {

    // return nullptr if current node is empty, if tree is empty, or if node cannot be found in the tree
    if (current_node == nullptr) {
        return nullptr;
    }

    // check if the current node's SKU ID path is equal to the SKUPath of the node we're finding
    if (strcmp(current_node->SKUId, SKUPath) == 0) {
        return current_node; // if equal, return node (node is found)
    }

    // if current node has children, loop through the child nodes
    if (current_node->children != nullptr) {
        for (int i = 0; i < current_node->max_num_children; i++) {
            SKUNode* child_node = current_node->children[i];
            if (child_node != nullptr) { // if child node is not null, recursively find and compare child node to SKUPath
                SKUNode* result = findSKU(child_node, SKUPath);
                if (result != nullptr) {
                    return result; // if node is found, return the node
                }
            }
        }
    }
    // If SKUPath wasn't found, return nullptr
    return nullptr;
}
// increments count for each node below the current node (node that has the following SKUPath)
void SKUTree::countNumOfSKUsInSKUChart(std::string SKUPath, unsigned int &count) {
    SKUNode* node_curr = findSKU(root,SKUPath.c_str()); // find the node that has the SKUPath

    // if the current node with the following SKUPath does not exist, return
    if (node_curr == nullptr) {
        return;
    }
    //count current node
    count++;

    //recursively count SKU children nodes
    if (node_curr->children != nullptr) {
        for (int i = 0; i < node_curr->max_num_children; i++) {
            if (node_curr->children[i] != nullptr) {
                countNumOfSKUsInSKUChart(node_curr->children[i]->SKUId, count);
            }
        }
    }
}

