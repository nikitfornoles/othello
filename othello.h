#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct {
	//total number of tiles owned by each player
	int p1_pieces_size;
	int p2_pieces_size;
	bool p1_tiles [65];
	bool p2_tiles [65];
} board;

board b;

typedef struct {
	int *tileswpieces; //location of each player's pieces identified by numbers 1-64
} player;

player p1, p2;

void initialize_game ();
void get_p1_pieces_size ();
void get_p2_pieces_size ();
void generateInitialPieces ();
void printboard ();

void printmsg1 () {
	printf ("The player who has the most pieces on the board wins.\n");
}

void initialize_game () {
	srand(time(NULL));
	b.p1_pieces_size = 0;
	b.p2_pieces_size = 0;
	p1.tileswpieces = calloc(b.p1_pieces_size, sizeof(int));
	p2.tileswpieces = calloc(b.p2_pieces_size, sizeof(int));

	for (int i = 0; i < 65; i++) {
		b.p1_tiles [i] = false;
		b.p2_tiles [i] = false;
	}
}

/*
void get_p1_pieces_size () {
	b.p1_pieces_size = sizeof(p1.tileswpieces)/sizeof(int);
}

void get_p2_pieces_size () {
	b.p2_pieces_size = sizeof(p2.tileswpieces)/sizeof(int);
}
*/

void generateInitialPieces () {
	int initialPiecesPosition [] = {28, 29, 36, 37};
	int size = sizeof(initialPiecesPosition)/sizeof(int);
	int random;

	for (int i = 0; i < size; i++) {
		random = rand() % 2 + 1;

		if (random == 1) {
			if (b.p1_pieces_size < 2) {
				b.p1_pieces_size = b.p1_pieces_size + 1;
				p1.tileswpieces = realloc(p1.tileswpieces, sizeof(int)*b.p1_pieces_size);
				p1.tileswpieces [b.p1_pieces_size - 1] = initialPiecesPosition [i];
				b.p1_tiles [initialPiecesPosition[i]] = true;
			}
			else {
				i = i - 1;
			}
		}
		else if (random == 2) {
			if (b.p2_pieces_size < 2) {
				b.p2_pieces_size = b.p2_pieces_size + 1;
				p2.tileswpieces = realloc(p2.tileswpieces, sizeof(int)*b.p2_pieces_size);
				p2.tileswpieces [b.p2_pieces_size - 1] = initialPiecesPosition [i];
				b.p2_tiles [initialPiecesPosition[i]] = true;
			}
			else {
				i = i - 1;
			}
		}
	}
}

void printboard () {
	int length = 8;
	
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			int num = (i*length) + (j+1);
			if (b.p1_tiles[num] == true) {
				printf ("\033[33m\033[1m"); //yellow bold
			}
			else if (b.p2_tiles[num] == true) {
				printf ("\033[32m\033[1m"); //green bold
			}
			else {
				printf ("\033[0m"); //reset color				
			}
			printf ("%d", num);
			printf (" ");
			if ((num / 10) < 1) {
				printf(" ");
			}
		}
		printf("\n");
	}
}

void belongsToP1 () {
	for (int i = 0; i < b.p1_pieces_size; ++i) {

	}
}

void fin () {
}
