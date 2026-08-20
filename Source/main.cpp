#include <print>

#include <IO/Logger.hpp>

#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

class Connection
{
public:
    explicit Connection(asio::io_context& context)
        :
        m_context(context),
        m_socket(context)
    {}

    tcp::socket& GetSocket()
    {
        return m_socket;
    }

    void Start()
    {
        LOG_INFO("Client connected");
    }
private:
    asio::io_context& m_context;
    tcp::socket m_socket;
};

class Server
{
public:
    explicit Server(asio::io_context& context)
        :
        m_context(context),
        m_acceptor(context, tcp::endpoint(tcp::v4(), 25565))
    {
        StartAccept();
    }
private:
    void StartAccept()
    {
        auto connection = std::make_shared<Connection>(m_context);

        m_acceptor.async_accept(connection->GetSocket(), std::bind(&Server::HandleAccept, this, // NOLINT(*-avoid-bind)
            connection, asio::placeholders::error));
    }

    void HandleAccept(const std::shared_ptr<Connection>& connection, const std::error_code& error)
    {
        if (!error)
            connection->Start();

        StartAccept();
    }

    asio::io_context& m_context;
    tcp::acceptor m_acceptor;
};

int main()
{
    asio::io_context context;
    Server server(context);

    try
    {
        context.run();
    }
    catch (std::exception& e)
    {
        std::println(stderr, "{}", e.what());
    }

    return 0;
}