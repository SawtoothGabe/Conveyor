#include <Connection.hpp>
#include <iostream>
#include <Server.hpp>

namespace conv
{
    Server::Server(asio::io_context& context)
        :
        m_context(context),
        m_acceptor(context, tcp::endpoint(tcp::v4(), 25565))
    {
        MakeAccept();

        m_serverThread = std::jthread(
        [this]
        {
           m_context.run();
        });

        while (true)
        {
            std::string command;
            std::getline(std::cin, command);

            if (command == "stop")
            {
                m_context.stop();
                break;
            }
        }
    }

    void Server::MakeAccept()
    {
        auto connection = std::make_shared<Connection>(m_context);

        m_acceptor.async_accept(connection->GetSocket(), std::bind(&Server::HandleAccept, this, // NOLINT(*-avoid-bind)
            connection, asio::placeholders::error));
    }

    void Server::HandleAccept(const std::shared_ptr<Connection>& connection, const std::error_code& error)
    {
        if (!error)
        {
            connection->Start();
            m_clients.push_back(connection);
        }

        MakeAccept();
    }
}