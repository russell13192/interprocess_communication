// The operation code could be enums
typedef enum {
	CREATE,
	UPDATE,
	DELETE
} OPCODE;

// The structure which contains the data to be synchronized can be:
typedef struct_msg_body {
	char destination[16];
	char mask;
	char gateway_ip[16];
	char oif[32];
} msg_body_t;

// The final msg that needs to be synced from the routing table manager process to all clients
// should operation code and body

typedef struct_sync_msg {
	OPCODE op_code;
	msg_body_t msg_body;
} sync_msg_t;