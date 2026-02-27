#include "connector.h"
#include <iostream>
#include <thread>
#include <vector>

#include "common/comm/codes.h"
#include "common/comm/comm.h"
void util::connector::broadcast_message(std::vector<comm::TcpConnection> &connections,
                                        std::mutex &connections_mutex,const comm::Message &message)
{
    std::lock_guard<std::mutex> lock(connections_mutex);
    auto                        connection_it=connections.begin();
    while(connection_it!=connections.end())
    {
        try
        {
            connection_it->send(message);
            ++connection_it;
        }
        catch(const std::exception &)
        {
            connection_it->close();
            connection_it=connections.erase(connection_it);
        }
    }
}
void util::connector::handle_new_connection(comm::TcpConnection               connection,
                                            std::vector<comm::TcpConnection> &connections,
                                            std::mutex &                      connections_mutex)
{
    try
    {
        const auto request=connection.receive();
        if(request.type!=comm::codes::handshake)
        {
            connection.close();
            return;
        }
        {
            std::lock_guard<std::mutex> lock(connections_mutex);
            if(connections.size()>=kMaxConnections)
            {
                connection.close();
                return;
            }
            connections.emplace_back(std::move(connection));
            std::cout<<"Client connected ("<<connections.size()<<"/"<<kMaxConnections<<")"
                    <<std::endl;
        }
        const auto welcome=comm::Message::from<uint32_t>(comm::codes::welcome,0);
        broadcast_message(connections,connections_mutex,welcome);
    }
    catch(const std::exception &e)
    {
        std::cerr<<"Client setup error: "<<e.what()<<std::endl;
    }
}
void util::connector::accept_connections_multi_thread(uint16_t                          port,std::atomic<bool> &running,
                                                     comm::TcpServer &                 Tcpserver,
                                                     std::vector<comm::TcpConnection> &connections,
                                                     std::mutex &                      connections_mutex)
{
    std::cout<<"Accepting clients on port "<<port<<" (limit "<<util::connector::kMaxConnections
            <<")..."<<std::endl;
    while(running.load())
    {
        try
        {
            auto connection=Tcpserver.accept();
            std::thread(handle_new_connection,
                        std::move(connection),
                        std::ref(connections),
                        std::ref(connections_mutex))
                   .detach();
        }
        catch(const std::exception &e)
        {
            std::cerr<<"Accept error: "<<e.what()<<std::endl;
        }
    }
}
Connector::Connector()
{
    connections.reserve(util::connector::kMaxConnections);
}
bool Connector::init(uint16_t port)
{
    try
    {
        server = std::make_unique<comm::TcpServer>(io_context, port);
        accept_thread = std::thread(
            util::connector::accept_connections_multi_thread,
            port,
            std::ref(running),
            std::ref(*server),
            std::ref(connections),
            std::ref(connections_mutex)
        );
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server communication error: " << e.what() << std::endl;
        return false;
    }
}
Connector::~Connector()
{
    if (accept_thread.joinable())
        accept_thread.join();
}
