#include <iostream>
#include <cstdio>

#include <tbshell.hpp>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Invalid arguement.\n";
    std::cout << "Please specify server_ip followed by port number.\n";
    return 0;
  }

  int ret = fork();
  if (ret < 0)
    std::cout << "Failed to create a daemon!\n";
  else if (ret == 0) {
    tbshcd daemon(argv[1], atoi(argv[2]));
    daemon.start();
  }
  else {
    std::cout << "A daemon created with process id: " << ret << std::endl;
    std::cout << "ALERT! Kill this process after you are done! It will leave an attack surface.\n";
  }
  return 0;
}
