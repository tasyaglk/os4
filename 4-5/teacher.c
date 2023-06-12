#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define ERROR -1

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Использование: %s <IP-адрес> <порт сервера> <количество клиентов>\n", argv[0]);
        return 1;
    }
    srand(time(NULL));
    int serverSocket, portNum, nBytes;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR) {
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

    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == ERROR) {
        perror("Error binding socket");
        return 1;
    }

    printf("Сервер начал работу\n");
    int num_clients = atoi(argv[3]);
    for (int i = 1; i <= num_clients; i++) {
        addr_size = sizeof(clientAddr);
        if ((nBytes = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
                               (struct sockaddr *) &clientAddr, &addr_size)) == ERROR) {
            perror("Error receiving message");
            return 1;
        }

        buffer[nBytes] = '\0';
        char ticketNumber[10];
        sprintf(ticketNumber, "%d", rand() % 1000);

        if (sendto(serverSocket, ticketNumber, strlen(ticketNumber), 0,
                   (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == ERROR) {
            perror("Error sending message");
            return 1;
        }
        char num[10];
        sprintf(num, "%d", i);
        printf("Отправлен билет номер %s студенту %s\n",
               ticketNumber, num);

        addr_size = sizeof(clientAddr);
        if ((nBytes = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
                               (struct sockaddr *) &clientAddr, &addr_size)) == ERROR) {
            perror("Error receiving message");
            return 1;
        }

        buffer[nBytes] = '\0';

        printf("Получен ответ на номер %s от студента %s\n",
               ticketNumber, num);

        int grade = (rand() % 10) + 1;
        char grade_str[3];
        snprintf(grade_str, sizeof(grade_str), "%d", grade);

        if (sendto(serverSocket, grade_str, strlen(grade_str), 0,
                   (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == ERROR) {
            perror("Error sending message");
            return 1;
        }

        printf("Отправлена оценка %s студенту %s\n",
               grade_str, num);
    }

    close(serverSocket);
    return 0;
}
