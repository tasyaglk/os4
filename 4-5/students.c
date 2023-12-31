#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define ERROR -1

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <IP-адрес> <порт>\n", argv[0]);
        return 1;
    }
    int clientSocket, portNum, nBytes;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR) {
        perror("Error creating socket");
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &(serverAddr.sin_addr)) <= 0) {
        perror("Invalid address or address not supported");
        return 1;
    }

    strcpy(buffer, "get_ticket");
    if (sendto(clientSocket, buffer, strlen(buffer), 0,
               (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == ERROR) {
        perror("Error sending message");
        return 1;
    }
    addr_size = sizeof(serverAddr);
    if ((nBytes = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0,
                          (struct sockaddr *)&serverAddr, &addr_size)) == ERROR) {
        perror("Error receiving message");
        return 1;
    }
    buffer[nBytes] = '\0';
    printf("Получен билет: %s\n", buffer);

    if (sendto(clientSocket, buffer, strlen(buffer), 0,
               (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == ERROR) {
        perror("Error sending message");
        return 1;
    }
printf("Отправлен ответ на билет номер %s\n", buffer);
    addr_size = sizeof(serverAddr);
    if ((nBytes = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0,
                          (struct sockaddr *)&serverAddr, &addr_size)) == ERROR) {
        perror("Error receiving message");
        return 1;
    }

    buffer[nBytes] = '\0';
    printf("Получена оценка - %s\n", buffer);

    close(clientSocket);

    return 0;
}
