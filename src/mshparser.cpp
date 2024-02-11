#include <iostream>
#include <string>
#include <fstream>

int main()
{
    // Read specified msh file
    std::string filename = "ressource/example_5p14.msh";

    std::cout << filename << std::endl;

    std::ifstream infile(filename); 

    std::string line;
    while (std::getline(infile, line))
    {
        std::cout << line << std::endl;
    }
    
    
    return 0;
}