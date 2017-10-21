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
			board[i][j] ='+';
			printf(" %c", board[i][j]);
		}
		printf("\n");
	}
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


	printboard ();

	//closing connection
	close(client_sock);
	return 0;
}