#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "othello.h"

#define SERVER 1
#define CLIENT 2

void die_with_error(char *error_msg) {
	printf("%s", error_msg);
	exit(-1);
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

	//send positions of player 1's initial tile pieces
	for (int i = 0; i < 2; i++) {
		bzero(sbuffer, 256);
		int a = p1.tileswpieces [i];
		sprintf (sbuffer, "%d", a);

		n = send(client_sock, sbuffer, sizeof(sbuffer), 0);
		if (n < 0)
			die_with_error("Error: send() Failed.");
	}

	//send positions of player 2's initial tile pieces
	for (int i = 0; i < 2; i++) {
		bzero(sbuffer, 256);
		int a = p2.tileswpieces [i];
		sprintf (sbuffer, "%d", a);

		n = send(client_sock, sbuffer, sizeof(sbuffer), 0);
		if (n < 0)
			die_with_error("Error: send() Failed.");
	}

	printboard ();
	sleep (1);

	while (!game_over()) {
		int ans_client;
		int ans_server;

		//SERVER'S TURN
		//-------------------------------------------------------
		printf("\nEnter the tile number where you will put your piece: ");
		scanf ("%d", &ans_server);

		while (!validmove(ans_server, SERVER)) {
			printf("Not valid. Pick another tile number: ");
			scanf ("%d", &ans_server);
		}

		p1Move(ans_server);

		//inform the opponent of server's move
		bzero(sbuffer, 256);
		sprintf (sbuffer, "%d", ans_server);
		n = send(client_sock, sbuffer, sizeof(sbuffer), 0);
		if (n < 0)
			die_with_error("Error: send() Failed.");

		//send size of server's pieces on board
		bzero(sbuffer, 256);
		int a = b.p1_pieces_size;
		sprintf (sbuffer, "%d", a);

		n = send(client_sock, sbuffer, sizeof(sbuffer), 0);
		if (n < 0)
			die_with_error("Error: send() Failed.");

		//send updated positions of server's tile pieces
		for (int i = 0; i < b.p1_pieces_size; i++) {
			bzero(sbuffer, 256);
			int a = p1.tileswpieces [i];
			sprintf (sbuffer, "%d", a);

			n = send(client_sock, sbuffer, sizeof(sbuffer), 0);
			if (n < 0)
				die_with_error("Error: send() Failed.");
		}

		printboard ();
		//-------------------------------------------------------

		//CLIENT'S TURN
		//-------------------------------------------------------
		printf("\nOpponent's turn\n");

		//receive info of client's move
		bzero(rbuffer, 256);
		n = recv(client_sock, rbuffer, sizeof(rbuffer), 0);
		if (n < 0) 
			die_with_error("Error: recv() Failed.");
		ans_client = atoi (rbuffer);
		p2Move(ans_client);

		//receive size of client's pieces on board
		bzero(rbuffer, 256);
		n = recv(client_sock, rbuffer, sizeof(rbuffer), 0);
		if (n < 0) 
			die_with_error("Error: recv() Failed.");
		b.p2_pieces_size = atoi (rbuffer);

		//receive updated positions of client's tile pieces
		for (int i = 0; i < b.p2_pieces_size; i++) {
			bzero(rbuffer, 256);
			n = recv(client_sock, rbuffer, sizeof(rbuffer), 0);
			if (n < 0)
				die_with_error("Error: recv() Failed.");
			int value = atoi (rbuffer);
			p2.tileswpieces [i] = value;
		}

		printboard ();
		//-------------------------------------------------------
	}

	//deallocate memory
	fin ();

	//closing connection
	close(client_sock);
	close(server_sock);
	return 0;
}