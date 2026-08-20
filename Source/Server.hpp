#pragma once

#include <Common/Asio.hpp>

namespace conv
{
    class Connection;
    class Server
    {
    public:
        explicit Server(asio::io_context& context);
    private:
        void MakeAccept();
        void HandleAccept(const std::shared_ptr<Connection>& connection,
            const std::error_code& error);

        asio::io_context& m_context;
        tcp::acceptor m_acceptor;

        std::jthread m_serverThread;

        std::vector<std::shared_ptr<Connection>> m_clients;
    };
}