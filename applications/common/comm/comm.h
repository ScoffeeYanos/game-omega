#pragma once

#include <boost/asio.hpp>

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace comm
{

using tcp = boost::asio::ip::tcp;

struct Message
{
    uint16_t type = 0;
    std::vector<uint8_t> payload;

    template <typename T>
    T as() const
    {
        static_assert(std::is_trivially_copyable<T>::value,
                      "Message::as<T>() requires trivially copyable types");
        if (payload.size() != sizeof(T))
        {
            throw std::runtime_error("Message payload size does not match requested type");
        }

        T value{};
        std::memcpy(&value, payload.data(), sizeof(T));
        return value;
    }

    template <typename T>
    static Message from(uint16_t message_type, const T& value)
    {
        static_assert(std::is_trivially_copyable<T>::value,
                      "Message::from<T>() requires trivially copyable types");

        Message message;
        message.type = message_type;
        message.payload.resize(sizeof(T));
        std::memcpy(message.payload.data(), &value, sizeof(T));
        return message;
    }
};

class TcpConnection
{
public:
    explicit TcpConnection(tcp::socket socket);

    TcpConnection(const TcpConnection&) = delete;
    TcpConnection& operator=(const TcpConnection&) = delete;
    TcpConnection(TcpConnection&&) noexcept = default;
    TcpConnection& operator=(TcpConnection&&) noexcept = default;

    [[nodiscard]] bool is_open() const;
    void close();

    void send(const Message& message);
    Message receive();

    [[nodiscard]] tcp::socket& socket();

private:
    tcp::socket socket_;
};

class TcpServer
{
public:
    TcpServer(boost::asio::io_context& io_context, uint16_t port);

    TcpConnection accept();

private:
    tcp::acceptor acceptor_;
};

class TcpClient
{
public:
    explicit TcpClient(boost::asio::io_context& io_context);

    TcpConnection connect(const std::string& host, uint16_t port);

private:
    boost::asio::io_context& io_context_;
};

} // namespace comm