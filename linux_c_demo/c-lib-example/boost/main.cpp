#include <iostream>
#include <string>
#include <boost/regex.hpp>

int main(int argc, char const *argv[])
{
    boost::regex email_regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    std::string email;
    std::cout << "enter an email address: ";
    std::cin >> email;

    if (boost::regex_match(email, email_regex))
    {
        std::cout << "valid email address !" << std::endl;
        boost::smatch matches;
        if (boost::regex_match(email, matches, email_regex))
        {
            std::cout << "userame: " << matches[0] << std::endl;
        }
    }
    else
    {
        std::cout << "invalid email address" << std::endl;
    }

    return 0;
}
