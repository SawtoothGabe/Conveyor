#include <print>
#include <Server.hpp>

int main()
{
    asio::io_context context;
    conv::Server server(context);

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