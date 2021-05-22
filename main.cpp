#include "CppServer/core/app.hpp"


int main(int, const char*[])
{
    App app;

    char chosenOption;
    std::cout << "###########################################\n";
    std::cout << "Welcome, stranger. Choose favorable option, please\n";
    std::cout << "1. Http Server\n";
    std::cout << "2. Chat server\n";
    std::cin >> chosenOption;
    switch (chosenOption)
    {
        case '1':{
            std::cout << "Running...\n";
            app.run_http_server();
            break;
        }

        case '2':
            std::cout << "Running...";
            break;
    }
}