#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "othello.h"

void die_with_error(char *error_msg) {
	printf("%s", error_msg);
	exit(-1);
}

int main(int argc, char *argv[]) {
	int client_sock, port_no, n;
	struct sockaddr_in server_addr;

	struct hostent *server;
	struct  hostent {
		char    *h_name;        /* official name of host */
		char    **h_aliases;    /* alias list */
		int     h_addrtype;     /* host address type */
		int     h_length;       /* length of address */
		char    **h_addr_list;  /* list of addresses from name server */
		#define h_addr  h_addr_list[0]  /* address, for backward compatiblity */
	};

	if (argc < 3) {
		printf("Usage: %s hostname port_no", argv[0]);
		exit(1);
	}

	// Create a socket using TCP
	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock < 0)
		die_with_error("Error: socket() Failed.");

	server = gethostbyname(argv[1]);
	if (server == NULL) {
		die_with_error("Error: No such host.");
	}

	// Establish a connection to server
	port_no = atoi(argv[2]);
	bzero((char *) &server_addr,  sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&server_addr.sin_addr.s_addr, server->h_length);

	server_addr.sin_port = htons(port_no);
	if (connect(client_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		die_with_error("Error: connect() Failed.");

	char sbuffer[256];
	char rbuffer[256];

	//connection successful
	printf("\n");
	printf("################################\n");
	printf("###### Welcome to OTHELLO ######\n");
	printf("################################\n");
	printf("\n");

	initialize_game ();

	//receives positions of player 1's initial tile pieces
	b.p1_pieces_size = 0;
	p1.tileswpieces = realloc(p1.tileswpieces, sizeof(int)*2);
	for (int i = 0; i < 2; i++) {
		bzero(rbuffer, 256);
		n = recv(client_sock, rbuffer, sizeof(rbuffer), 0);
		if (n < 0)
			die_with_error("Error: recv() Failed.");
		p1.tileswpieces[i] = atoi(rbuffer);
		b.p1_tiles[p1.tileswpieces[i]] = true;
		b.p1_pieces_size = b.p1_pieces_size + 1;
	}

	//receives positions of player 2's initial tile pieces
	b.p1_pieces_size = 0;
	p2.tileswpieces = realloc(p2.tileswpieces, sizeof(int)*2);
	for (int i = 0; i < 2; i++) {
		bzero(rbuffer, 256);
		n = recv(client_sock, rbuffer, sizeof(rbuffer), 0);
		if (n < 0)
			die_with_error("Error: recv() Failed.");
		p2.tileswpieces[i] = atoi(rbuffer);
		b.p2_tiles[p2.tileswpieces[i]] = true;
		b.p1_pieces_size = b.p1_pieces_size + 1;
	}

	printboard ();
	sleep (1);

	while (!game_over()) {
		int ans_client;
		int ans_server;

		//SERVER'S TURN
		//-------------------------------------------------------
		printf("\nOpponent's turn\n");

		//receive info of server's move
		bzero(rbuffer, 256);
		n = recv(client_sock, rbuffer, sizeof(rbuffer), 0);
		if (n < 0) 
			die_with_error("Error: recv() Failed.");
		ans_server = atoi (rbuffer);
		p1Move(ans_server);

		//receive size of server's pieces on board
		bzero(rbuffer, 256);
		n = recv(client_sock, rbuffer, sizeof(rbuffer), 0);
		if (n < 0) 
			die_with_error("Error: recv() Failed.");
		b.p1_pieces_size = atoi (rbuffer);

		//receive updated positions of server's tile pieces
		for (int i = 0; i < b.p1_pieces_size; i++) {
			bzero(rbuffer, 256);
			n = recv(client_sock, rbuffer, sizeof(rbuffer), 0);
			if (n < 0)
				die_with_error("Error: recv() Failed.");
			int value = atoi (rbuffer);
			p1.tileswpieces [i] = value;
		}

		printboard ();
		//-------------------------------------------------------

		//CLIENT'S TURN
		//-------------------------------------------------------
		printf("\nEnter the tile number where you will put your piece: ");
		scanf ("%d", &ans_client);

		while (!validmove(ans_client)) {
			printf("Not valid. Pick another tile number: ");
			scanf ("%d", &ans_client);
		}

		p2Move(ans_client);

		//inform the opponent of client's move
		bzero(sbuffer, 256);
		sprintf (sbuffer, "%d", ans_client);
		n = send(client_sock, sbuffer, sizeof(sbuffer), 0);
		if (n < 0)
			die_with_error("Error: send() Failed.");

		//send size of client's pieces on board
		int a = b.p2_pieces_size;

		bzero(sbuffer, 256);
		sprintf (sbuffer, "%d", a);
		n = send(client_sock, sbuffer, sizeof(sbuffer), 0);
		if (n < 0)
			die_with_error("Error: send() Failed.");

		//send updated positions of client's tile pieces
		for (int i = 0; i < b.p2_pieces_size; i++) {
			int a = p2.tileswpieces [i];

			bzero(sbuffer, 256);
			sprintf (sbuffer, "%d", a);
			n = send(client_sock, sbuffer, sizeof(sbuffer), 0);
			if (n < 0)
				die_with_error("Error: send() Failed.");
		}

		printboard ();
		//-------------------------------------------------------
	}

	//deallocate memory
	fin ();

	//closing connection
	close(client_sock);
	return 0;
}