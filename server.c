#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "othello.h"

void die_with_error(char *error_msg) {
	printf("%s", error_msg);
	exit(-1);
}

int size = 8;

void board_init() {
	char board[size][size];

	printf("\n ");
	for (int i = 0; i < size; i++)
		printf(" %d", i);

	printf("\n");
	for (int i = 0; i < size; i++) {
		printf("%d", i);
		for (int j = 0; j < size; j++) {
			board[i][j] = '+';
			printf(" %c", board[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[]) {
	int server_sock, client_sock, port_no, client_size, n;

	struct sockaddr_in server_addr, client_addr;
	struct sockaddr_in {
		short   sin_family; /* must be AF_INET */
		u_short sin_port;
		struct  in_addr sin_addr;
		char    sin_zero[8]; /* Not used, must be zero */
	};

	if (argc < 2) {
		printf("Usage: %s port_no", argv[0]);
		exit(1);
	}

	// Create a socket for incoming connections
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0)
		die_with_error("Error: socket() Failed.");

	// Bind socket to a port
	bzero((char *) &server_addr, sizeof(server_addr));
	port_no = atoi(argv[1]);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_no);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		die_with_error("Error: bind() Failed.");

	// Mark the socket so it will listen for incoming connections
	listen(server_sock,5);
	//printf("Wating for connections!\n");

	// Accept new connection
	client_size = sizeof(client_addr);
	client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_size);
	if (client_sock < 0)
		die_with_error("Error: accept() Failed.");

	char sbuffer[256];
	char rbuffer [256];

	//Client successfully connected
	printf("\n");
	printf("################################\n");
	printf("###### Welcome to OTHELLO ######\n");
	printf("################################\n");
	printf("\n");
	
	initialize_game ();
	generateInitialPieces ();

	printboard ();

	//closing connection
	close(client_sock);
	close(server_sock);
	return 0;
}