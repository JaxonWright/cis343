/************************************************************************
 * minesweeper.c														*
 *																		*
 * Author(s): Jaxon Wright, Chad Teitsma  								*
 ***********************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE_MIN 5
#define BOARD_SIZE_MAX 15
#define PCT_MINES_MIN 10
#define PCT_MINES_MAX 70

#define RCN_COLOR	"\x1b[38;2;180;180;180m"
#define MIN_COLOR	"\x1b[31m"
#define ZER_COLOR   "\x1b[36m"
#define ONE_COLOR	"\x1b[38;2;156;122;255m"
#define TWO_COLOR 	"\x1b[38;2;95;171;97m"
#define THR_COLOR	"\x1b[38;2;255;118;109m"
#define FOU_COLOR 	"\x1b[38;2;115;105;189m"
#define FIV_COLOR	"\x1b[38;2;184;93;90m"
#define SIX_COLOR	"\x1b[38;2;67;144;146m"
#define SEV_COLOR	"\x1b[38;2;191;238;124m"
#define EIG_COLOR	"\x1b[38;2;255;171;251m"
#define CLE_COLOR	"\x1b[39;49m"

typedef struct {
	bool is_mine;
	int mines;
	bool visible;
} Cell;

typedef enum {WON, LOST, INPROGRESS} Status;

/************************************************************************
 * YOU MUST NOT DEFINE ANY GLOBAL VARIABLES (i.e., OUTSIDE FUNCTIONS).  *
 * COMMUNICATION BETWEEN FUNCTIONS MUST HAPPEN ONLY VIA PARAMETERS.     *
 ************************************************************************/

/************************************************************************
 * Function declarations/prototypes										*
 ************************************************************************/
void printColored(Cell);

void displayMenu();

int getBoardSize();

int getPercentMines();

void initBoard(int size, Cell board[][size]);

void placeMinesOnBoard(int size, Cell board[][size], int nbrMines);

void fillInMineCountForNonMineCells(int size, Cell board[][size]);

int nbrOfMines(int size, Cell board[][size]);

int getNbrNeighborMines(int row, int col, int size, Cell board[][size]);

void displayBoard(int size, Cell board[][size], bool displayMines);

Status selectCell(int row, int col, int size, Cell board[][size]);

int nbrVisibleCells(int size, Cell board[][size]);

//void setImmediateNeighborCellsVisible(int row, int col, int size, Cell board[][size]);

void setAllNeighborCellsVisible(int row, int col, int size, Cell board[][size]);


/************************************************************************
 * Main driver of the program. Uses the functions defined above.		*
 ************************************************************************/
int main()
{
	int row, col, size, nbrMines;
	char command;
	bool displayMines = false;
	Status gameState = INPROGRESS;

	printf("************************************************************************************************************************\n");
	printf("* _|      _|  _|_|_|  _|      _|  _|_|_|_|    _|_|_|  _|          _|  _|_|_|_|  _|_|_|_|  _|_|_|    _|_|_|_|  _|_|_|   *\n");
	printf("* _|_|  _|_|    _|    _|_|    _|  _|        _|        _|          _|  _|        _|        _|    _|  _|        _|    _| *\n");
	printf("* _|  _|  _|    _|    _|  _|  _|  _|_|_|      _|_|    _|    _|    _|  _|_|_|    _|_|_|    _|_|_|    _|_|_|    _|_|_|   *\n");
	printf("* _|      _|    _|    _|    _|_|  _|              _|    _|  _|  _|    _|        _|        _|        _|        _|    _| *\n");
	printf("* _|      _|  _|_|_|  _|      _|  _|_|_|_|  _|_|_|        _|  _|      _|_|_|_|  _|_|_|_|  _|        _|_|_|_|  _|    _| *\n");
	printf("* a game by jaxon wright and chad teitsma                                                                              *\n");
	printf("************************************************************************************************************************\n\n");
	size = getBoardSize();

	// declare 2D array of cells
	Cell board[size][size];

	initBoard(size,board);

	// determine number of mine to place on the board
	nbrMines = (int) (size * size * (getPercentMines()/100.0));

	// place mines randomly on the board
	placeMinesOnBoard(size,board,nbrMines);

	// For each non-mine cell, sets the neighboring mine count
	fillInMineCountForNonMineCells(size,board);

	displayBoard(size,board,displayMines);

	while (true) {
		printf("Enter command (m/M for command menu): ");
		scanf("%c",&command);
		getchar();

		switch (command) {
			case 'm': case 'M':
				displayMenu();
				break;

			case 'c': case 'C':
				do {
					printf("Enter row and column of cell: ");
					scanf("%d%d",&row,&col);
					getchar();
					if (row < 1 || row > size || col < 1 || col > size) {
						printf("Invalid row or column values. Try again.\n");
					}
				} while (row < 1 || row > size || col < 1 || col > size);
				row--;
				col--;
				gameState = selectCell(row,col,size,board);
				displayBoard(size,board,displayMines);
				break;

			case 's': case 'S':
				displayMines = true;
				displayBoard(size,board,displayMines);
				break;

			case 'h': case 'H':
				displayMines = false;
				displayBoard(size,board,displayMines);
				break;

			case 'b': case 'B':
				displayBoard(size,board,displayMines);
				break;

			case 'q': case 'Q':
				printf("Bye.\n");
				return 0;

			default:
				printf("Invalid command. Try again.\n");
		}

		// display appropriate message if the game is over
		if (gameState == WON) {
			printf("You found all the mines. Congratulations. Bye.");
			return 0;
		} else if (gameState == LOST) {
			printf("Oops. Sorry, you landed on a mine. Bye");
			return 0;
		}
	}

	return 0;
}

/************************************************************************
 * Displays list of commands allowed at the prompt						*
 ************************************************************************/
void displayMenu()
{
	printf("List of available commands:\n");
	printf("   Show Mines: s/S\n");
	printf("   Hide Mines: h/H\n");
	printf("   Select Cell: c/C\n");
	printf("   Display Board: b/B\n");
	printf("   Display Menu: m/M\n");
	printf("   Quit: q/Q\n\n");
}

/************************************************************************
 * Initializes the fields of each cell on the board as follows:			*
 * 		is_mine field to false											*
 * 		mines field to 0												*
 * 		visible field to false											*
 ************************************************************************/
void initBoard(int size, Cell board[][size])
{
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			Cell cell;
			
			cell.is_mine = false;
			cell.mines = 0;
			cell.visible = false;
			
			board[i][j] = cell;
		}
	}
}

/************************************************************************
 * Places the specified number of mines randomly on the board			*
 ************************************************************************/
void placeMinesOnBoard(int size, Cell board[][size], int nbrMines){
	srand(time(NULL));
	int x, y;
	int count = 0;
	
	while (count < nbrMines){
		x = rand()%(size);
		y = rand()%(size);
		
		//if there is not a mine already at the randomly generated spot then 
		//put one there
		if(!board[x][y].is_mine){
			board[x][y].is_mine = true;
			count++;
		}
	}
}

/************************************************************************
 * For each non-mine cell on the board, set the "mines" field to the	*
 * number of mines in the immediate neighborhood.						*
 ************************************************************************/
void fillInMineCountForNonMineCells(int size, Cell board[][size]) {
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			if(!board[i][j].is_mine)
				board[i][j].mines = getNbrNeighborMines(i,j,size,board);
		}
	}
}

/************************************************************************
 * Counts and returns the number of mines on the board					*
 ************************************************************************/
int nbrOfMines(int size, Cell board[][size]) {
	int count = 0;
	
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			if (board[i][j].is_mine)
				count++;
		}
	}

	return count;
}

/************************************************************************
 * Returns the number of mines in the immediate neighborhood of a cell	*
 * at location (row,col) on the board.									*
 ************************************************************************/
int getNbrNeighborMines(int row, int col, int size, Cell board[][size]) {
	int count = 0;
	
	//check left
	if (col > 0 && board[row][col-1].is_mine){
		count++;
	}
	//check diag up left
	if ((col > 0 && row > 0) && board[row-1][col-1].is_mine){
		count++;
	}
	//check above
	if (row > 0 && board[row-1][col].is_mine){
		count++;
	}
	//check diag up right
	if ((row > 0 && col < size-1) && board[row-1][col+1].is_mine){
		count++;
	}
	//check right
	if (col < size-1 && board[row][col+1].is_mine){
		count++;
	}
	//check diag down right
	if ((row < size-1 && col < size-1) && board[row+1][col+1].is_mine){
		count++;
	}
	//check below
	if (row < size-1 && board[row+1][col].is_mine){
		count++;
	}
	//check diag down left
	if ((row < size-1 && col > 0) && board[row+1][col-1].is_mine){
		count++;
	}
	
	return count;
}

/************************************************************************
 * Displays the board. If a cell is not currently visible and has a		*
 * mine, show the mine if the displayMines is true. Used for debugging	*
 * and testing purposes.												*
 ************************************************************************/
void displayBoard(int size, Cell board[][size], bool displayMines) {	
	//Print top row, displaying column numbers
	printf("\t");
	for (int i=1; i <= size; i++)
		printf(RCN_COLOR "%d\t" CLE_COLOR,i);
	
	printf("\n");
	//print row numbers and rest of board
	for (int i=0; i < size; i++){
		//Stuff is getting colored lulz
		printf(RCN_COLOR "%d\t" CLE_COLOR,i+1);
		for (int j=0; j < size; j++){
			if(board[i][j].is_mine && displayMines)
				printf(MIN_COLOR "*\t" CLE_COLOR);
			else if (board[i][j].visible)
				printColored(board[i][j]);
			else
				printf("?\t");
		}	
		printf("\n");
	}
}

void printColored(Cell board){
	switch(board.mines){
		case 0:
			printf(ZER_COLOR "%d\t" CLE_COLOR, 0);
			break;
		case 1:
			printf(ONE_COLOR "%d\t" CLE_COLOR, 1);
			break;
		case 2:
			printf(TWO_COLOR "%d\t" CLE_COLOR, 2);
			break;
		case 3:
			printf(THR_COLOR "%d\t" CLE_COLOR, 3);
			break;
		case 4:
			printf(FOU_COLOR "%d\t" CLE_COLOR, 4);
			break;
		case 5:
			printf(FIV_COLOR "%d\t" CLE_COLOR, 5);
			break;
		case 6:
			printf(SIX_COLOR "%d\t" CLE_COLOR, 6);
			break;
		case 7:
			printf(SEV_COLOR "%d\t" CLE_COLOR, 7);
			break;
		case 8:
			printf(EIG_COLOR "%d\t" CLE_COLOR, 8);
			break;
	}
}

/************************************************************************
 * Prompts the user for board size, reads and validates the input		*
 * entered, and returns the integer if it is within valid range.		*
 * repeats this in a loop until the user enters a valid value.			*
 ************************************************************************/
int getBoardSize() {
	int size = 0;

	printf("Enter the board size ( %d to %d ): ", BOARD_SIZE_MIN, BOARD_SIZE_MAX); 
	
	//scans from user input to character array
	char board_size[50];
  	fgets(board_size,50,stdin);
  	//convert from char array to integer. If it is not a number returns a negative int
  	size = atoi(board_size);

	if (size < BOARD_SIZE_MIN || size > BOARD_SIZE_MAX){
		printf("Invalid input.\n");
		size = getBoardSize();
	}
	return size;
}

/************************************************************************
 * Prompts the user for percentage of mines to place on the board,		*
 * reads and validates the input entered, and returns the integer if it	*
 * is within valid range. repeats this in a loop until the user enters	*
 * a valid value for board size.										*
 ************************************************************************/
int getPercentMines() {
	int percent = 0;

	printf("Enter the percentage of mines on the board ( %d to %d ): ", PCT_MINES_MIN, PCT_MINES_MAX); 
	
	//scans from user input to character array
	char board_size[50];
  	fgets(board_size,50,stdin);
  	//convert from char array to integer. If it is not a number returns a negative int
  	percent = atoi(board_size);

	if (percent < PCT_MINES_MIN|| percent > PCT_MINES_MAX){
		printf("Invalid input.\n");
		percent = getPercentMines();
	}
	return percent;
}

/************************************************************************
 * Process cell selection by user during the game						*
 ************************************************************************/
Status selectCell(int row, int col, int size, Cell board[][size]){
	
	board[row][col].visible = true;
	
	if (board[row][col].is_mine){
		return LOST;
	}
	if (board[row][col].mines == 0) {
		setAllNeighborCellsVisible(row, col, size, board);
	}
	if(nbrOfMines(size, board) + nbrVisibleCells(size, board) == size*size) {
		return WON;
	}

	return INPROGRESS;
}

/************************************************************************
 * Returns the number of cells that are currently visible.				*
 ************************************************************************/
int nbrVisibleCells(int size, Cell board[][size]){
	int count = 0;

	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			if (board[i][j].visible)
				count++;
		}
	}
	
	return count;
}

/************************************************************************
 * If the mine count of a cell at location (row,col) is zero, then make	*
 * the cells in the immediate neighborhood visible and repeat this		*
 * process for each of the cells in this set of cells that have a mine	*
 * count of zero, and so on.											*
 ************************************************************************/
void setAllNeighborCellsVisible(int row, int col, int size, Cell board[][size]){
	
	//check left
	if (col > 0 && !board[row][col-1].visible)
		selectCell(row, col-1, size, board);
	//check diag up left
	if ((col > 0 && row > 0) && !board[row-1][col-1].visible)
		selectCell(row-1, col-1, size, board);
	//check above
	if (row > 0 && !board[row-1][col].visible)
		selectCell(row-1, col, size, board);
	//check diag up right
	if ((row > 0 && col < size-1) && !board[row-1][col+1].visible)
		selectCell(row-1, col+1, size, board);
	//check right
	if (col < size-1 && !board[row][col+1].visible)
		selectCell(row, col+1, size, board);
	//check diag down right
	if ((row < size-1 && col < size-1) && !board[row+1][col+1].visible)
		selectCell(row+1, col+1, size, board);
	//check below
	if (row < size-1 && !board[row+1][col].visible)
		selectCell(row+1, col, size, board);
	//check diag down left
	if ((row < size-1 && col > 0) && !board[row+1][col-1].visible)
		selectCell(row+1, col-1, size, board);
	
}
