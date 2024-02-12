// gmshparser.cpp

#include "mshreader.h"
#include <fstream>
#include <sstream>

// Function to parse nodes from $Nodes section
std::vector<Node> parseNodes(std::ifstream& file) {
    std::vector<Node> nodes;
    int numEntityBlocks, numNodes;
    file >> numEntityBlocks >> numNodes;
    for (int i = 0; i < numEntityBlocks; ++i) {
        int entityDim, entityTag, parametric, numNodesInBlock;
        file >> entityDim >> entityTag >> parametric >> numNodesInBlock;
        for (int j = 0; j < numNodesInBlock; ++j) {
            Node node;
            file >> node.tag >> node.x >> node.y >> node.z;
            nodes.push_back(node);
        }
    }
    return nodes;
}

// Function to parse elements from $Elements section
std::vector<Element> parseElements(std::ifstream& file) {
    std::vector<Element> elements;
    int numEntityBlocks, numElements;
    file >> numEntityBlocks >> numElements;
    for (int i = 0; i < numEntityBlocks; ++i) {
        int entityDim, entityTag, elementType, numElementsInBlock;
        file >> entityDim >> entityTag >> elementType >> numElementsInBlock;
        for (int j = 0; j < numElementsInBlock; ++j) {
            int elementTag, numTags;
            file >> elementTag >> numTags;
            Element element;
            element.tag = elementTag;
            for (int k = 0; k < numTags; ++k) {
                int tag;
                file >> tag; // Ignoring tags for now
            }
            for (int k = 0; k < 4; ++k) { // Assuming 4 nodes per element
                int nodeTag;
                file >> nodeTag;
                element.nodeTags.push_back(nodeTag);
            }
            elements.push_back(element);
        }
    }
    return elements;
}

// Function to parse physical groups from $PhysicalNames section
std::vector<PhysicalGroup> parsePhysicalGroups(std::ifstream& file) {
    std::vector<PhysicalGroup> physicalGroups;
    int numPhysicalNames;
    file >> numPhysicalNames;
    for (int i = 0; i < numPhysicalNames; ++i) {
        int dimension, tag;
        std::string name;
        file >> dimension >> tag;
        std::getline(file >> std::ws, name); // Read the name (handles spaces)
        physicalGroups.push_back({dimension, tag, name});
    }
    return physicalGroups;
}

// Function to parse partitions from $PartitionedEntities section
std::vector<Partition> parsePartitions(std::ifstream& file) {
    std::vector<Partition> partitions;
    int numPartitions;
    file >> numPartitions;
    for (int i = 0; i < numPartitions; ++i) {
        int numGhostEntities;
        file >> numGhostEntities;
        for (int j = 0; j < numGhostEntities; ++j) {
            int ghostEntityTag, partition;
            file >> ghostEntityTag >> partition;
            partitions.push_back({ghostEntityTag, partition});
        }
    }
    return partitions;
}

// Function to parse periodic links from $Periodic section
std::vector<PeriodicLink> parsePeriodics(std::ifstream& file) {
    std::vector<PeriodicLink> periodics;
    int numPeriodicLinks;
    file >> numPeriodicLinks;
    for (int i = 0; i < numPeriodicLinks; ++i) {
        PeriodicLink periodic;
        file >> periodic.entityDim >> periodic.entityTag >> periodic.entityTagMaster;
        int numAffine;
        file >> numAffine;
        for (int j = 0; j < numAffine; ++j) {
            double value;
            file >> value; // Ignoring values for now
        }
        int numCorrespondingNodes;
        file >> numCorrespondingNodes;
        for (int j = 0; j < numCorrespondingNodes; ++j) {
            int nodeTag, nodeTagMaster;
            file >> nodeTag >> nodeTagMaster;
            periodic.correspondingNodes.push_back({nodeTag, nodeTagMaster});
        }
        periodics.push_back(periodic);
    }
    return periodics;
}

// Function to parse ghost elements from $GhostElements section
std::vector<GhostElement> parseGhostElements(std::ifstream& file) {
    std::vector<GhostElement> ghostElements;
    int numGhostElements;
    file >> numGhostElements;
    for (int i = 0; i < numGhostElements; ++i) {
        GhostElement ghostElement;
        file >> ghostElement.tag >> ghostElement.partitionTag;
        int numGhostPartitions;
        file >> numGhostPartitions;
        for (int j = 0; j < numGhostPartitions; ++j) {
            int ghostPartitionTag;
            file >> ghostPartitionTag;
            ghostElement.ghostPartitionTags.push_back(ghostPartitionTag);
        }
        ghostElements.push_back(ghostElement);
    }
    return ghostElements;
}

std::tuple<std::vector<Node>, std::vector<Element>, std::vector<PhysicalGroup>, std::vector<Partition>, std::vector<PeriodicLink>, std::vector<GhostElement>> parse_msh_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    std::vector<Node> nodes;
    std::vector<Element> elements;
    std::vector<PhysicalGroup> physicalGroups;
    std::vector<Partition> partitions;
    std::vector<PeriodicLink> periodics;
    std::vector<GhostElement> ghostElements;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "$Nodes") {
            nodes = parseNodes(file);
        } else if (keyword == "$Elements") {
            elements = parseElements(file);
        } else if (keyword == "$PhysicalNames") {
            physicalGroups = parsePhysicalGroups(file);
        } else if (keyword == "$PartitionedEntities") {
            partitions = parsePartitions(file);
        } else if (keyword == "$Periodic") {
            periodics = parsePeriodics(file);
        } else if (keyword == "$GhostElements") {
            ghostElements = parseGhostElements(file);
        }
    }

    return std::make_tuple(nodes, elements, physicalGroups, partitions, periodics, ghostElements);
}
