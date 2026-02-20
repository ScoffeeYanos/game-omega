#include "comm.h"

#include <array>
#include <limits>

namespace comm
{
namespace
{

constexpr std::size_t header_size = sizeof(uint16_t) + sizeof(uint32_t);

void write_u16_be(uint8_t* out, uint16_t value)
{
    out[0] = static_cast<uint8_t>((value >> 8) & 0xFFU);
    out[1] = static_cast<uint8_t>(value & 0xFFU);
}

void write_u32_be(uint8_t* out, uint32_t value)
{
    out[0] = static_cast<uint8_t>((value >> 24) & 0xFFU);
    out[1] = static_cast<uint8_t>((value >> 16) & 0xFFU);
    out[2] = static_cast<uint8_t>((value >> 8) & 0xFFU);
    out[3] = static_cast<uint8_t>(value & 0xFFU);
}

uint16_t read_u16_be(const uint8_t* in)
{
    return static_cast<uint16_t>((static_cast<uint16_t>(in[0]) << 8) |
                                 static_cast<uint16_t>(in[1]));
}

uint32_t read_u32_be(const uint8_t* in)
{
    return (static_cast<uint32_t>(in[0]) << 24) |
           (static_cast<uint32_t>(in[1]) << 16) |
           (static_cast<uint32_t>(in[2]) << 8) |
           static_cast<uint32_t>(in[3]);
}

} // namespace

TcpConnection::TcpConnection(tcp::socket socket)
    : socket_(std::move(socket))
{
}

bool TcpConnection::is_open() const
{
    return socket_.is_open();
}

void TcpConnection::close()
{
    boost::system::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_both, ec);
    socket_.close(ec);
}

void TcpConnection::send(const Message& message)
{
    if (message.payload.size() > std::numeric_limits<uint32_t>::max())
    {
        throw std::runtime_error("Message payload exceeds 32-bit size limit");
    }

    std::array<uint8_t, header_size> header{};
    write_u16_be(header.data(), message.type);
    write_u32_be(header.data() + sizeof(uint16_t), static_cast<uint32_t>(message.payload.size()));

    boost::asio::write(socket_, boost::asio::buffer(header));
    if (!message.payload.empty())
    {
        boost::asio::write(socket_, boost::asio::buffer(message.payload));
    }
}

Message TcpConnection::receive()
{
    std::array<uint8_t, header_size> header{};
    boost::asio::read(socket_, boost::asio::buffer(header));

    Message message;
    message.type = read_u16_be(header.data());

    const uint32_t payload_size = read_u32_be(header.data() + sizeof(uint16_t));
    message.payload.resize(payload_size);

    if (payload_size > 0)
    {
        boost::asio::read(socket_, boost::asio::buffer(message.payload));
    }

    return message;
}

tcp::socket& TcpConnection::socket()
{
    return socket_;
}

TcpServer::TcpServer(boost::asio::io_context& io_context, uint16_t port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
}

TcpConnection TcpServer::accept()
{
    tcp::socket socket(acceptor_.get_executor());
    acceptor_.accept(socket);
    return TcpConnection(std::move(socket));
}

TcpClient::TcpClient(boost::asio::io_context& io_context)
    : io_context_(io_context)
{
}

TcpConnection TcpClient::connect(const std::string& host, uint16_t port)
{
    tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(host, std::to_string(port));

    tcp::socket socket(io_context_);
    boost::asio::connect(socket, endpoints);

    return TcpConnection(std::move(socket));
}

} // namespace comm