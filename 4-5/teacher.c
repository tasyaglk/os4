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

    int serverSocket, portNum, nBytes;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    // Создание сокета
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR) {
        perror("Error creating socket");
        return 1;
    }

    // Заполнение структуры адреса сервера
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &(serverAddr.sin_addr)) <= 0) {
        perror("Invalid address or address not supported");
        return 1;
    }

    // Привязка сокета к IP и порту
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == ERROR) {
        perror("Error binding socket");
        return 1;
    }

    printf("Server started. Waiting for messages...\n");
    int num_clients = atoi(argv[3]);
    for (int i = 0; i < num_clients; i++) {
        // Ожидание получения запроса на билет от клиента
        addr_size = sizeof(clientAddr);
        if ((nBytes = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
                               (struct sockaddr *) &clientAddr, &addr_size)) == ERROR) {
            perror("Error receiving message");
            return 1;
        }

        buffer[nBytes] = '\0';

        // Генерация номера билета
        // Вместо этого может быть добавлена ваша логика генерации номера билета
        char ticketNumber[10];
        sprintf(ticketNumber, "%d", rand() % 1000);

        // Отправка номера билета клиенту
        if (sendto(serverSocket, ticketNumber, strlen(ticketNumber), 0,
                   (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == ERROR) {
            perror("Error sending message");
            return 1;
        }

        printf("Sent ticket number %s to %s:%d\n",
               ticketNumber, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // Ожидание получения ответа от клиента
        addr_size = sizeof(clientAddr);
        if ((nBytes = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
                               (struct sockaddr *) &clientAddr, &addr_size)) == ERROR) {
            perror("Error receiving message");
            return 1;
        }

        buffer[nBytes] = '\0';

        // Вывод сообщения о полученном ответе
        printf("Received answer for ticket number %s from %s:%d\n",
               ticketNumber, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // Пример генерации оценки
        // Вместо этого может быть добавлена ваша логика оценивания
        char grade[10];
        strcpy(grade, "A+");

        // Отправка оценки клиенту
        if (sendto(serverSocket, grade, strlen(grade), 0,
                   (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == ERROR) {
            perror("Error sending message");
            return 1;
        }

        printf("Sent grade %s to %s:%d\n",
               grade, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    }

    // Закрытие сокета
    close(serverSocket);

    return 0;
}
