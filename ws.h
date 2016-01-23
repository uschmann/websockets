struct WsFrame {
	uint8_t finFlag;
	uint8_t maskingFlag;
	uint8_t opcode;
	uint64_t payloadLenght;
	uint32_t maskingMap;
	uint8_t *payload;
};

void wsConnect(int socketId, const char *path);
void wsInitFrame(WsFrame *frame);
void wsCreateTextFrame(WsFrame *frame, const char *text);
void wsSendFrame(int socket_desc, WsFrame *frame);
void wsReceiveFrame(int socket, WsFrame *frame);
void wsSendText(int socket, const char *text);
void wsReceiveText(int socket, char *buffer, int bufferSize);