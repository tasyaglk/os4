#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define ERROR -1

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Использование: %s <IP-адрес сервера> <порт сервера>\n", argv[0]);
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

  if (sendto(clientSocket, "get_messages", strlen("get_messages"), 0,
             (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == ERROR) {
    perror("Error sending request");
    return 1;
  }
  while (1) {
    addr_size = sizeof(serverAddr);
    if ((nBytes = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0,
                           (struct sockaddr *)&serverAddr, &addr_size)) ==
        ERROR) {
      perror("Error receiving messages");
      return 1;
    }

    buffer[nBytes] = '\0';
    if (strcmp(buffer, "stop") == 0) {
      break;
    }
    printf("%s", buffer);
  }

  close(clientSocket);

  return 0;
}