#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define ERROR -1

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Использование: %s <IP-адрес> <порт сервера> <порт клиента-монитора> <количество клиентов>\n",
           argv[0]);
    return 1;
  }
srand(time(NULL));
  int serverSocket, portNum, nBytes;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in serverAddr, clientAddr, monitorAddr;
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

  if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) ==
      ERROR) {
    perror("Error binding socket");
    return 1;
  }

  memset(&monitorAddr, 0, sizeof(monitorAddr));
  monitorAddr.sin_family = AF_INET;
  monitorAddr.sin_port = htons(atoi(argv[3]));
  if (inet_pton(AF_INET, argv[1], &(monitorAddr.sin_addr)) <= 0) {
    perror("Invalid address or address not supported");
    return 1;
  }

  printf("Сервер начал работу\n");

  addr_size = sizeof(monitorAddr);
  if ((nBytes = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
                         (struct sockaddr *)&monitorAddr, &addr_size)) ==
      ERROR) {
    perror("Error receiving message");
    return 1;
  }
  int num_clients = atoi(argv[4]);
  for (int i = 1; i <= num_clients; i++) {
    addr_size = sizeof(clientAddr);
    if ((nBytes = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
                           (struct sockaddr *)&clientAddr, &addr_size)) ==
        ERROR) {
      perror("Error receiving message");
      return 1;
    }

    buffer[nBytes] = '\0';

    char ticketNumber[10];
    sprintf(ticketNumber, "%d", rand() % 1000);
    if (sendto(serverSocket, ticketNumber, strlen(ticketNumber), 0,
               (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, "Отправилен билет номер ", strlen("Отправилен билет номер "),
               0, (struct sockaddr *)&monitorAddr,
               sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, ticketNumber, strlen(ticketNumber), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, " студенту ", strlen(" студенту "), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    char num[10];
    sprintf(num, "%d", i);
    if (sendto(serverSocket, num, strlen(num), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, "\n", strlen("\n"), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    addr_size = sizeof(clientAddr);
    if ((nBytes = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
                           (struct sockaddr *)&clientAddr, &addr_size)) ==
        ERROR) {
      perror("Error receiving message");
      return 1;
    }

    buffer[nBytes] = '\0';

    if (sendto(serverSocket, "Получен ответ на номер ",
               strlen("Получен ответ на номер "), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, ticketNumber, strlen(ticketNumber), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, " от студента ", strlen(" от студента "), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, num, strlen(num), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, "\n", strlen("\n"), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    int grade = (rand() % 10) + 1;
    char grade_str[3];
    snprintf(grade_str, sizeof(grade_str), "%d", grade);

    if (sendto(serverSocket, grade_str, strlen(grade_str), 0,
               (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, "Отправлена оценка ", strlen("Отправлена оценка "), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, grade_str, strlen(grade_str), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, " студенту ", strlen(" студенту "), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, num, strlen(num), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
    if (sendto(serverSocket, "\n", strlen("\n"), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }
  }
  if (sendto(serverSocket, "stop", strlen("stop"), 0,
               (struct sockaddr *)&monitorAddr, sizeof(monitorAddr)) == ERROR) {
      perror("Error sending message");
      return 1;
    }

  close(serverSocket);

  return 0;
}