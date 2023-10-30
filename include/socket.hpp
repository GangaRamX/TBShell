#include <iostream>
#include <cstdio>
#include <memory>

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#define BUFFSIZE 4096
class Socket {
protected:
    int sockfd;

public:
    Socket() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            std::cerr << "Socket creation failed...\n";
            exit(0);
        }
        else
            std::cout << "Socket successfully created..\n";
    }

    ~Socket() {
        close(sockfd);
    }

    int get_sockfd() {
        return sockfd;
    }
};

class Client : public Socket {
private:
    struct sockaddr_in servaddr;

    int Connect(const char* ip, int port)
    {
        servaddr.sin_family = AF_INET;
        if (inet_pton(AF_INET, ip, &servaddr.sin_addr) == 0) {
            std::cout << "Invalid IP address!\n";
            return -1;
        }

        servaddr.sin_port = htons(port);
        std::cout << "waiting to connect server: " << ip <<":" << port << std::endl;

        while (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
            usleep(50000);
        }
        return 0;
    }

public:
    Client(const char* server_ip, int port)
    {
        if (Connect(server_ip, port)) {
            std::cout << "Failed to connect server!";
            exit(-1);
        }
        else
            std::cout << "Connected to server";
    }

    ~Client()
    {
        std::cout << "Client Destroyed";
    }
};

class Server : public Socket {
private:
    struct sockaddr_in servaddr;

public:
    Server(const char* ip, int port)
    {
        servaddr.sin_family = AF_INET;
        if (inet_pton(AF_INET, ip, &servaddr.sin_addr) == 0) {
            std::cout << "Invalid IP address.\n";
            exit(-1);
        }
        servaddr.sin_port = htons(port);
        if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
            perror("Address binding failed\n");
            exit(-1);
        }
    }

    int connect_client()
    {
        struct sockaddr_in client_addr;
        int client_sock;
        int err = listen(sockfd, 1);
        if ( err < 0) {
            return err;
        }

        /* accept */
        int socklen = sizeof(client_addr);
        client_sock = accept(sockfd, (struct sockaddr*) &client_addr, (socklen_t*) &socklen);
        if (client_sock >= 0) {
            std::cout << "Connected to " << std::string(inet_ntoa(client_addr.sin_addr)) << std::endl;
        }
        return client_sock;
     }

    ~Server()
    {
        std::cout << "Server Destroyed";
    }
};

 inline unsigned msg_send(int fd, char* message, unsigned len)
 {
    return send(fd, message, len, 0);
 }

inline std::shared_ptr<char> msg_receive(int fd)
{
    struct pollfd fds[1];
    unsigned nbytes = 0;
    fds[0].fd = fd;
    fds[0].events = POLLIN;

    int timeout = -1; /* infinite wait */
    int ret = poll(fds, 1, timeout);
    if (ret <= 0) {
        /* timeout and error */
        return std::shared_ptr<char>(nullptr);
    } else {
        if (fds[0].revents & POLLIN) {
            if (ioctl(fd, FIONREAD, &nbytes) < 0) {
                return std::shared_ptr<char>(nullptr);
            }
        }
    }

    std::shared_ptr<char> buffer(new char[nbytes], std::default_delete<char[]>());
    unsigned size = (unsigned) recv(fd, (void*)buffer.get(), nbytes, 0);
    if (size < nbytes) {
        std::cout << "Unexpected error!\n";
    }
    return buffer;
}
