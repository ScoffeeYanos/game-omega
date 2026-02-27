#include "connector.h"

#include <iostream>
#include <boost/asio/io_context.hpp>

#include "common/comm/codes.h"
#include "common/comm/comm.h"
Connector::Connector(uint16_t port)
{
    boost::asio::io_context io_context;
    comm::TcpClient client(io_context);

    auto connection = client.connect("127.0.0.1", port);
    connection.send(comm::Message::from<uint32_t>(comm::codes::handshake, 0xC0DEF00DU));

    const auto response = connection.receive();
    if (response.type == 2)
    {
        const auto chunk_count = response.as<uint32_t>();
        std::cout << "Connected to server. Chunk count: " << chunk_count << std::endl;
    }
    connection.close();
}
