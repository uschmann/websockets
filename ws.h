 



 struct WsFrame {
	uint8_t finFlag;
	uint8_t maskingFlag;
	uint8_t opcode;
	uint64_t payloadLenght;
	uint32_t maskingMap;
	uint8_t *payload;
};