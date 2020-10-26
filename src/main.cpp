#include "parameters.h"
#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <unistd.h>  
#include <filesystem>
#include "network_packages.h"

bool is_number(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

std::regex is_valid_regex(const std::string &s)
{
    try
    {
        std::regex re(s.c_str());
        return re;
    }
    catch(const std::regex_error& e)
    {
        std::cout << "Regex error: " << e.what() << "\n";
        exit(1);
    }
}


std::regex NetworkPackages::reg;
ThreadPool *NetworkPackages::pool;
bool NetworkPackages::is_regex_defined;
FILE *NetworkPackages::file;

int main(int argc, char **argv)
{
    if(getuid())
    {
        std::cout << "This project must be run as root !\n";
        exit(1);
    }

    int thread_count = 5;
    std::string regex{};
    std::regex re{};

    InputParser input(argc, argv);
    
    if(input.cmdOptionExists("-t"))
    {
        std::string thread_count_str = input.getCmdOption("-t");
        if(!is_number(thread_count_str))
        {
            std::cout << "Please enter valid thread count!\n";
            exit(1);
        }
        thread_count = std::stoi(thread_count_str);
    }
    NetworkPackages::is_regex_defined = false;
    if(input.cmdOptionExists("-r"))
    {
        std::string regex = input.getCmdOption("-r");
        re = is_valid_regex(regex);
        NetworkPackages::is_regex_defined = true;
    }

    if(thread_count > 10)
        thread_count = 10;
        
    ThreadPool *my_pool = new ThreadPool(thread_count);

    NetworkPackages::reg = re;
    NetworkPackages::pool = my_pool;
    NetworkPackages::start();
    
}