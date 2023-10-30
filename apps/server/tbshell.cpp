#include <iostream>
#include <cstdio>
#include "./tbshell.hpp"

int main(int argc, char *argv[]) {
    if (argc != 3) {
       std::cout << "Invalid arguement.\n";
       std::cout << "Please specify server_ip followed by port number.\n";
       return 0;
    }
    tbsh shell(argv[1], atoi(argv[2]));
    shell.start();
    return 0;
}
