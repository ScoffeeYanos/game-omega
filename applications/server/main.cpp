#include <iostream>
#include <thread>
#include <boost/asio/io_context.hpp>
#include "common/comm/comm.h"
#include "game/util/connector.h"
#include "game/world/world.h"
namespace
{
    constexpr uint16_t    kHandshakeType=1;
    constexpr uint16_t    kWelcomeType=2;
    constexpr uint16_t    kPort=40000;
}// namespace
int main()
{
    World world;
    std::cout<<"Server initialized with "<<world.kChunkCount<<" chunks."<<std::endl;
    Connector connector;
    if(!connector.init(kPort))
    {
        std::cout<<"Error initializing the connector"<<std::endl;
    }
    return 0;
}
