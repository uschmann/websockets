#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdint.h>
#include <fcntl.h> 
#include "ws.h"

const static char http_get[] = "GET %s HTTP/1.1\r\n";
const static char http_upgrade[] = "Upgrade: websocket\r\n";
const static char http_connection[] = "Connection: Upgrade\r\n";
const static char http_ws_key[] = "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n";
const static char http_ws_version[] = "Sec-WebSocket-Version: 13\r\n";

#define WS_OPCODE_CONTINUATION 0x00
#define WS_OPCODE_TEXT 0x01
#define WS_OPCODE_BINARY 0x02
#define WS_OPCODE_CONECTION_CLOSE 0x08
#define WS_OPCODE_PING 0x09
#define WS_OPCODE_PONG 0x0A

void wsConnect(int socketId, const char *path) {
	char *server_reply[2000];
	char buffer[300];

    sprintf(buffer, http_get, path);
    send(socketId, buffer, strlen(buffer) , 0);
    send(socketId, http_upgrade, strlen(http_upgrade) , 0);
    send(socketId, http_connection, strlen(http_connection) , 0);
    send(socketId, http_ws_key, strlen(http_ws_key) , 0);
    send(socketId, http_ws_version, strlen(http_ws_version) , 0);
    send(socketId, "\r\n", 2, 0);

    memset(server_reply, 0, 2000);
    recv(socketId, server_reply , 2000 , 0);
}

void wsInitFrame(WsFrame *frame) {
	frame->finFlag = 0;
	frame->maskingFlag = 0;
	frame->opcode = 0;
	frame->payloadLenght = 0;
	frame->maskingMap = 0;
	frame->payload = 0;
}

void wsCreateTextFrame(WsFrame *frame, const char *text) {
	frame->finFlag = 1;
	frame->maskingFlag = 1;
	frame->opcode = WS_OPCODE_TEXT;
	frame->payloadLenght = strlen(text);
	frame->maskingMap = 0x00000000;
	frame->payload = (uint8_t *)text;
}

void wsSendFrame(int socket_desc, WsFrame *frame) {
	uint8_t finOpcode = (frame->finFlag) ? frame->opcode | 0x80 : frame->opcode;
	uint8_t maskPayloadLength = (frame->maskingFlag) ? frame->payloadLenght | 0x80 : frame->payloadLenght;	// todo support extended payloadLength
	send(socket_desc , &finOpcode , 1 , 0);
	send(socket_desc , &maskPayloadLength , 1 , 0);
	send(socket_desc , &frame->maskingMap , 4 , 0);	
	send(socket_desc , frame->payload, frame->payloadLenght , 0);
}

void wsReceiveFrame(int socket, WsFrame *frame) {
	char inBuffer[256];
	int receivedTotal = 0;
	int receivedLength = 0;

	memset(inBuffer, 0, 256);
	// set socket to blocking to wait for the first available bytes
	fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) & ~O_NONBLOCK);
	while((receivedLength = recv(socket, inBuffer + receivedTotal, 2 - receivedTotal, 0)) > 0 && receivedTotal < 2) {
		// set socket to non blocking
   		fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK);
    	receivedTotal += receivedLength;
    }
    frame->finFlag = (inBuffer[0] & 0x80) >> 7;
    frame->opcode = inBuffer[0] & 0x0F;
    frame->maskingFlag = inBuffer[1] & 0x80;
    frame->payloadLenght = inBuffer[1] & 0x7F;	// Todo: support extended payload length


	while((receivedLength = recv(socket, inBuffer + receivedTotal, frame->payloadLenght + 2 - receivedTotal, 0)) > 0 && receivedTotal < frame->payloadLenght) {
    	receivedTotal += receivedLength;
    }

    memcpy(frame->payload, inBuffer + 2, receivedTotal - 2);
}

void wsSendText(int socket, const char *text) {
	WsFrame frame;
	wsInitFrame(&frame);
	wsCreateTextFrame(&frame, text);
    wsSendFrame(socket, &frame);
}

void wsReceiveText(int socket, char *buffer, int bufferSize) {
	WsFrame frame;
	wsInitFrame(&frame);
	memset(buffer, 0, bufferSize);
	frame.payload = (uint8_t *)buffer;
	wsReceiveFrame(socket, &frame);
}
