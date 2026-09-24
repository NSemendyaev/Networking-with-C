#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/uio.h>

char *buf = "Hello World! My name is Nikita.";

int main(void)
{
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    int client = socket(AF_INET, SOCK_STREAM, 0);
    connect(client, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    write(client, buf, strlen(buf));
    close(client);
    return 0;
}