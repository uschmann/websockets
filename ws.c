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