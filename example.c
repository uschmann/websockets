#include <stdio.h>
#include <arpa/inet.h>

#include "ws.h"

int main(int argc , char *argv[])
{
	char text [1024];
	int socketId = 0;
	struct sockaddr_in server;
	socketId = socket(AF_INET , SOCK_STREAM , 0);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (connect(socketId , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        return 2;
    }

	wsConnect(socketId, "/");
    while(1) {
    	gets(text);
    	wsSendText(socketId, text);
    	wsReceiveText(socketId, text, 1024);
    	puts(text);
    }

    return 0;
}