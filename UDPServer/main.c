#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8888
#define BUFSIZE 1024

int main(int argc, char *argv[])
{

    // if (argc > 1)
    // {
    //     clock_t delay = atoi(argv[1]) * CLOCKS_PER_SEC;
    //     clock_t start = clock();
    //     printf("Delay %d s\n", delay / CLOCKS_PER_SEC);
    //     while (clock() - start < delay);
    //     // sleep(delay);
    // }

    int sockfd;
    struct sockaddr_in server, client;

    // 建立 socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // 設置 server 的資訊
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(server.sin_zero, 0, sizeof(server.sin_zero));

    // 綁定 socket 到本機的指定 port
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    printf("Listening on port %d\n", PORT);

    // 接收資料
    while (1)
    {
        socklen_t len = sizeof(struct sockaddr);
        char buf[BUFSIZE];

        int n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&client, &len);
        if (n == -1)
        {
            perror("recvfrom");
            exit(1);
        }

        buf[n] = '\0';
        printf("Received from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Data: %s\n", buf);

        // 回傳資料給 client
        if (sendto(sockfd, buf, n, 0, (struct sockaddr *)&client, len) == -1)
        {
            perror("sendto");
            exit(1);
        }
    }

    return 0;
}