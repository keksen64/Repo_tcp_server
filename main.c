
#include <winsock2.h>//#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <time.h>
#include <pthread.h>

#define MAX 80
#define PORT 11001
#define SA struct sockaddr

// Function designed for chat between client and server.
void *handler(int *arg)
{
    char buff[MAX];
    int i;
    time_t now;
    int count = 0;
    int connfd = *arg;
    for(;;){
        memset(buff,0,MAX);
        i = recv(connfd, buff, sizeof(buff), 0);
        if(i<0){
            printf("closed channel");
            break;
        }
        printf("connfid - %d From client: %s\t", connfd, buff);
        strcpy(buff,"r23123123\t\n");
        send(connfd, buff, sizeof(buff),0);
        count++;
        if(count%10000==0){
            //time_t now;
            time(&now);
            // Преобразование в местный формат времени и вывод на стандартный вывод
            printf("%s", ctime(&now));
            printf("count %d\n", count);
        }
    }
}

void createWSAData(){
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup creation failed...\n");
        exit(0);
    }
}


int main()
{
    int sockfd, connfd, len, iResult;
    struct sockaddr_in servaddr, cli;
    pthread_t threadId;

    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup creation failed...\n");
        exit(0);
    }

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("%d", WSAGetLastError());
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    memset(&servaddr,0,sizeof(servaddr));
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("%d", WSAGetLastError());
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    //подготовка сервера окончена
    for(;;) {
        // Accept the data packet from client and verification
        //точка останова
        printf("STOP POINT\n");
        connfd = accept(sockfd, (SA *) &cli, &len);
        if (connfd < 0) {
            printf("%d", WSAGetLastError());
            printf("server accept failed...\n");
            exit(0);
        }
        //вызов обработчика
        printf("connfid - %d ", connfd);
        pthread_create(&threadId, NULL, (void *(*)(void *)) handler, &connfd);
        //handler(connfd);
    }
   // close(sockfd);
}
