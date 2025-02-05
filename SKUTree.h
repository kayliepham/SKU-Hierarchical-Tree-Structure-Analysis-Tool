//
// Name: Kaylie Pham
// RedID: 828129478
//

#ifndef SKUTREE_H
#define SKUTREE_H

#include <string>
#include <vector>


class SKUTree {
public:
    struct SKUNode {
        int max_num_children; // stores maximum possible child nodes, given internal levels
        SKUNode **children; // pointer to array of child nodes
        const char* SKUId; // stores SKU ID Path
        SKUNode() : children(nullptr), max_num_children(0), SKUId(nullptr) {} //SKUNode constructor
    };

    SKUTree(); // SKUTree constructor
    SKUTree(std::string levels, const std::string& max_children);

    int internalLevels;
    SKUNode* root;  // creates root node (every tree has root node "s")
    std::vector<int> max_children_per_level;  // vector storing max children for nodes at each level

    void set_internalLevels(std::string internal_levels);   // sets the number of internal levels
    void set_max_children_per_level(const std::string& max_children);   // sets vector of maximum children per level
    int get_max_children_per_level(const char* SKUPath);    // returns maximum children given a SKU path

    bool addSKU(const char* SKUPath);   // adds nodes to tree, returning true if successful
    SKUNode* findSKU(SKUNode* current_node, const char* SKUPath);   // returns node if found, returns nullptr if not

    void countNumOfSKUsInSKUChart(std::string SKUPath,unsigned int &count); // increments count for each node

};

#endif //SKUTREE_H
