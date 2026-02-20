#pragma once
#include "connector.h"
#include <vector>
#include "common/comm/comm.h"
namespace util::connector
{
    constexpr std::size_t kMaxConnections=16;
    void broadcast_message(std::vector<comm::TcpConnection> &connections,
                           std::mutex &connections_mutex,const comm::Message &message);
    void handle_new_connection(comm::TcpConnection connection,
                               std::vector<comm::TcpConnection> &connections,
                               std::mutex &connections_mutex);
    void accept_connections_multi_thread(uint16_t port,std::atomic<bool> &running,comm::TcpServer &Tcpserver,
                                        std::vector<comm::TcpConnection> &connections,
                                        std::mutex &connections_mutex);
}
class Connector
{
public:
    Connector();
    bool init(uint16_t port);
    ~Connector();
private:
    std::vector<comm::TcpConnection> connections;
    std::mutex                       connections_mutex;
    std::atomic<bool>                running{true};
    boost::asio::io_context          io_context;
    std::unique_ptr<comm::TcpServer> server;
    std::thread                      accept_thread;
};
