// gmshparser.h

#ifndef MSHREADER_H
#define MSHREADER_H

#include <string>
#include <vector>

// Define structs to hold different data sections
struct Node {
    int tag;
    double x, y, z;
};

struct Element {
    int tag;
    std::vector<int> nodeTags;
};

struct PhysicalGroup {
    int dimension;
    int tag;
    std::string name;
};

struct Partition {
    int tag;
    int partition;
};

struct PeriodicLink {
    int entityDim;
    int entityTag;
    int entityTagMaster;
    std::vector<std::pair<int, int>> correspondingNodes;
};

struct GhostElement {
    int tag;
    int partitionTag;
    std::vector<int> ghostPartitionTags;
};

// Function to parse .msh file and return parsed entities
std::tuple<std::vector<Node>, std::vector<Element>, std::vector<PhysicalGroup>, std::vector<Partition>, std::vector<PeriodicLink>, std::vector<GhostElement>> parse_msh_file(const std::string& filename);

#endif // GMSHPARSER_H
