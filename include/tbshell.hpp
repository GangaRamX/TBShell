#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include "./socket.hpp"

#define PATH_LEN 256
#define CMD_LEN 512
#define LOG_LEN 4096


class tbsh {
private:
    int m_client;
    char m_pwd[PATH_LEN];
    Server *m_server_p = nullptr;
    void print(std::shared_ptr<char> msg, bool pr_log = true)
    {
        char *pwd = msg.get();
        snprintf(m_pwd, PATH_LEN, "%s", pwd);
        if (pr_log) {
            char *log = pwd + PATH_LEN;
            printf("%s", log);
        }
        printf("%s > ", m_pwd);
    }
    void pr_pwd()
    {
        printf("%s > ", m_pwd);
    }

    void get_cmd(char *cmd)
    {
        int count = 0;
        while (true) {
            char c = getchar();
            if (c != '\n' && c != EOF && count < 512) {
                if (cmd[count - 1] == ' ' && c == ' ')
                    continue;
                cmd[count++] = c;
            }
            else
                break;
        }
        if (cmd[count-1] == ' ')
            cmd[count-1] = '\0';
    }

    int valid(char *cmd) {
        if ((strncmp(cmd, "ping ", 4) == 0) ||
            (strncmp(cmd, "top", 4) == 0))
            return 0;
        else
            return 1;
    }

public:
    tbsh(const char* ip, int port)
    {
        m_server_p = new Server(ip, port);
        m_client = m_server_p->connect_client();
        if (m_client < 0) {
            std::cout << "Unable to connect client!\n";
            exit(-1);
        }
    }

    void start()
    {
        char cmd[CMD_LEN];
        /* First message PWD is expected */
        std::shared_ptr<char> msg = msg_receive(m_client);
        if (msg.get() == nullptr) {
            std::cout << "Unable to start shell!\n";
            return;
        }
        else {
            print(msg, false);
        }
        /* Non ending loop */
        while (1) {
            memset(cmd, 0, CMD_LEN);
            get_cmd(cmd);
            if (cmd[0] < 48) {
                pr_pwd();
                continue;
            }
            if (!valid(cmd)) {
                std::cout << "Command not allowed\n";
                continue;
            }

            if (msg_send(m_client, cmd, CMD_LEN) < 0) {
                std::cout << "Unable to send command\n";
            }

            std::shared_ptr<char> msg = msg_receive(m_client);
            if (msg.get() == nullptr) {
                std::cout << "Client error!\n";
            }
            print(msg);
        }

    }

    ~tbsh()
    {
        delete m_server_p;
    }
};

class tbshcd {
private:
    Client *m_client;
    char m_pwd[PATH_LEN];
    std::vector<char> log_buff;

public:
    tbshcd(const char* server_ip, int port)
    {
        m_client = new Client(server_ip, port);
        log_buff.resize(LOG_LEN);
        memset(log_buff.data(), 0, LOG_LEN);
    }

    void start()
    {
        char line[LOG_LEN];
        char *log_ptr;
        int free_space;
        /* Send PWD as first message */
        if (getcwd(log_buff.data(), PATH_LEN) == NULL) {
            std::cout << "System Error!\n";
            return;
        }

        int size = msg_send(m_client->get_sockfd(), log_buff.data(), PATH_LEN);
        if (size != PATH_LEN) {
            std::cout << "Send Error!\n";
            return;
        }
        /* Non ending loop */
        while (1) {
            log_ptr = log_buff.data() + PATH_LEN;
            free_space = LOG_LEN - PATH_LEN;
            memset(log_buff.data(), 0, LOG_LEN);
            std::shared_ptr<char> cmd = msg_receive(m_client->get_sockfd());
            if (cmd.get() == nullptr) {
                std::cout << "Failed to receive command!\n";
                continue;
            }
            if (strncmp(cmd.get(), "cd ", 3) == 0) {
                if (chdir(&cmd.get()[3])) {
                    sprintf(log_ptr, "Path doesn't exist!\n");
                    free_space -= strlen(log_ptr);
                }
                else {
                    free_space -= 1;
                }
            }
            else {
                FILE *fp = popen(cmd.get(), "r");
                if (!fp) {
                    std::cout << "Unable to execute command!\n";
                }

                while (fgets(line, LOG_LEN, fp) != NULL) {
                    if (free_space <= 0) {
                        std::cout << "No space left in log buffer.\n";
                        continue;
                    }
                    strncat(log_ptr, line, free_space);
                    log_ptr += std::min(free_space, (int)strlen(line));
                    free_space -= std::min(free_space, (int)strlen(line));
                }
                pclose(fp);
            }
            if (getcwd(log_buff.data(), PATH_LEN) == NULL) {
                std::cout << "System Error!\n";
                return;
            }
            auto msg_len = LOG_LEN - free_space;
            size = msg_send(m_client->get_sockfd(), log_buff.data(), msg_len);
            if (size != msg_len) {
                std::cout << "Log send Error!\n";
                continue;
            }
        }
    }

    ~tbshcd()
    {
        delete m_client;
    }
};
