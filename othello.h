#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

//FUNCTION PROTOTYPES
//------------------------------------
void printmsg1 ();
void setTile2DValues ();
void initialize_game ();
void generateInitialPieces ();
void printboard ();
void update_filled_tiles ();
bool game_over ();
bool validmove (int ans, int playerID);
void p1Move (int ans);
void p2Move (int ans);
void fin ();
//------------------------------------

//STRUCTURES
//------------------------------------------------------------------------------------
//board
typedef struct {
	//total number of tiles owned by each player
	int p1_pieces_size;
	int p2_pieces_size;
	bool p1_tiles [65]; //0-64
	bool p2_tiles [65]; //0-64
	bool filled_tile [65]; //0-64
} board;

board b;
//************************************************************************************
//player
typedef struct {
	int *tileswpieces; //location of each player's pieces. Identified by numbers 1-64
} player;

player p1, p2;
//************************************************************************************
//tile2D
typedef struct {
	int row;
	int col;
} tile2D;

tile2D boardTiles [65]; //0-64
//------------------------------------------------------------------------------------

void printmsg1 () {
	printf ("\033[34m\033[1m"); //blue bold
	printf ("The player who has the most pieces on the board wins.\n");
	printf ("\033[0m"); //reset color
	printf("\n");
}

void setTile2DValues () {
	int length = 8;
	
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			int num = (i*length) + (j+1);
			boardTiles [num].row = i;
			boardTiles [num].col = j;
		}
	}
}

void initialize_game () {
	printmsg1 ();
	setTile2DValues ();
	srand(time(NULL));
	b.p1_pieces_size = 0;
	b.p2_pieces_size = 0;
	p1.tileswpieces = calloc(b.p1_pieces_size, sizeof(int));
	p2.tileswpieces = calloc(b.p2_pieces_size, sizeof(int));

	for (int i = 0; i < 65; i++) {
		b.p1_tiles [i] = false;
		b.p2_tiles [i] = false;
		b.filled_tile [i] = false;
	}
}

void generateInitialPieces () {
	//pieces of matching color should be placed diagonally at the start of the game
	int vertical1 [] = {28, 37};
	int vertical2 [] = {29, 36};

	//dictates the location of server and client's initial pieces
	int random = rand() % 2 + 1; //1 to 2

	p1.tileswpieces = realloc(p1.tileswpieces, sizeof(int)*2);
	p2.tileswpieces = realloc(p2.tileswpieces, sizeof(int)*2);

	if (random == 1) {
		for (int i = 0; i < 2; ++i) {
			p1.tileswpieces [i] = vertical1 [i];
			b.p1_tiles [vertical1[i]] = true;
			p2.tileswpieces [i] = vertical2 [i];
			b.p2_tiles [vertical2[i]] = true;
		}
	}
	else {
		for (int i = 0; i < 2; ++i) {
			p1.tileswpieces [i] = vertical2 [i];
			b.p1_tiles [vertical2[i]] = true;
			p2.tileswpieces [i] = vertical1 [i];
			b.p2_tiles [vertical1[i]] = true;
		}
	}
	b.p1_pieces_size = 2;
	b.p2_pieces_size = 2;
}

void printboard () {
	int length = 8;
	
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			int num = (i*length) + (j+1);
			if (b.p1_tiles[num] == true) {
				printf ("\033[33m\033[1mB"); //yellow bold
			}
			else if (b.p2_tiles[num] == true) {
				printf ("\033[32m\033[1mW"); //green bold
			}
			else {
				printf ("\033[0m"); //reset color
				printf ("%d", num);		
			}
			printf (" ");
			if (((num / 10) < 1) && !b.p1_tiles[num] && !b.p2_tiles[num]) {
				printf(" ");
			}

			if (b.p1_tiles[num] || b.p2_tiles[num]) {
				printf(" ");
			}
		}
		printf("\n");
	}
}

void update_filled_tiles () {
	for (int i = 0; i < 64; ++i) {
		if (b.p1_tiles[i] || b.p2_tiles[i]) {
			b.filled_tile [i] = true;
		}
	}
}

bool game_over () {
	update_filled_tiles ();
	bool over = true;

	for (int i = 1; i < 64; ++i) {
		if (b.filled_tile[i] == false) {
			over = false;
			break;
		}
	}

	return over;
}

bool horizontal (int ans, int num, int playerID) {
	bool h = false;
	if (boardTiles[ans].row == boardTiles[num].row) {
		h = true;
	}
	return h;
}

bool vertical (int ans, int num, int playerID) {
	bool v = false;
	if (boardTiles[ans].col == boardTiles[num].col) {
		v = true;
	}
	return v;
}

bool diagonal (int ans, int num, int playerID) {
	bool dLeft = false;

	int rowDiff = abs(boardTiles[ans].row - boardTiles[num].row);
	int colDiff = abs(boardTiles[num].col - boardTiles[ans].col);

	if (rowDiff == colDiff) {
		dLeft = true;
	}
	return dLeft;
}

bool validmove (int ans, int playerID) {
	update_filled_tiles ();
	bool valid = false;

	//check if the tile is empty
	if (b.filled_tile[ans] == false) {
		//check if the move created a straight line
		if (playerID == 1) {
			int size = b.p1_pieces_size;
			for (int i = 0; i < size; ++i) {
				if (horizontal(ans, p1.tileswpieces[i], playerID) || vertical(ans, p1.tileswpieces[i], playerID)
					|| diagonal(ans, p1.tileswpieces[i], playerID)) {
					valid = true;
				}
			}
		}
		else if (playerID == 2) {
			int size = b.p2_pieces_size;
			for (int i = 0; i < size; ++i) {
				if (horizontal(ans, p2.tileswpieces[i], playerID) || vertical(ans, p2.tileswpieces[i], playerID)
					|| diagonal(ans, p2.tileswpieces[i], playerID)) {
					valid = true;
				}
			}
		}
	}
	return valid;	
}

void p1Move (int ans) {
	b.p1_pieces_size = b.p1_pieces_size + 1;
	p1.tileswpieces = realloc(p1.tileswpieces, sizeof(int)*b.p1_pieces_size);
	p1.tileswpieces [b.p1_pieces_size - 1] = ans;
	b.p1_tiles [ans] = true;
	b.filled_tile [ans] = true;
}

void p2Move (int ans) {
	b.p2_pieces_size = b.p2_pieces_size + 1;
	p2.tileswpieces = realloc(p2.tileswpieces, sizeof(int)*b.p2_pieces_size);
	p2.tileswpieces [b.p2_pieces_size - 1] = ans;
	b.p2_tiles [ans] = true;
	b.filled_tile [ans] = true;
}

void fin () {
	free(p1.tileswpieces);
	free(p2.tileswpieces);
}