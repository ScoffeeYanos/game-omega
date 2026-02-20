#include <iostream>

#include "game/world/world.h"

int main()
{
    World world;

    std::cout << "Server initialized with " << world.kChunkCount << " chunks." << std::endl;
    return 0;
}