#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFSIZE 1024
#define MAX_RETRY_TIMES 10

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serverAddress;
    char buf[BUFSIZE];
    int nbytes;

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <server IP> <port> <msg>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Create a socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    /* Prepare the server address */
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &serverAddress.sin_addr) <= 0)
    {
        perror("inet_pton error");
        exit(EXIT_FAILURE);
    }


    int retry = 0;
    socklen_t len = sizeof(serverAddress);
    while (retry < MAX_RETRY_TIMES)
    {        
        // 傳送訊息到 Server
        snprintf(buf, BUFSIZE, argv[3]);
        if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }

        printf("Send done times : %d\t", ++retry);
        
        // 等待 Server 的回應
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = (500000 << (retry - 1)) <= 8000000 ? (500000 << (retry - 1)) : 8000000;
        printf("Wait %d ms\t", timeout.tv_usec/1000);

        int result = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
        if (result == 0)
        {
            printf("Timeout times : %d\n", retry);
            continue;
        }

        // 接收 Server 的訊息
        if ((nbytes = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&serverAddress, &len)) < 0)
        {
            perror("recvfrom");
            continue;
        }

        buf[nbytes] = '\0';
        printf("\n\nReceived from server: %s\n", buf);
        break;
    }

    if (retry >= MAX_RETRY_TIMES)
    {
        printf("Failed to receive response from server after %d times\n", retry);
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    return 0;
}