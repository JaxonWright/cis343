#
#  Minesweeper game
#
#  Author(s): Jaxon Wright and Chad Teitsma
#


# Constants
WON = 0
LOST = 1
INPROGRESS = 2
BOARD_SIZE_MIN = 5
BOARD_SIZE_MAX = 15
PCT_MINES_MIN = 10
PCT_MINES_MAX = 70
RCN_COLOR = "\033[38;5;245m"
MIN_COLOR = "\033[38;5;160m"
ZER_COLOR = "\033[38;5;248m"
ONE_COLOR = "\033[38;5;98m"
TWO_COLOR = "\033[38;5;70m"
THR_COLOR = "\033[38;5;209m"
FOU_COLOR = "\033[38;5;111m"
FIV_COLOR = "\033[38;5;209m"
SIX_COLOR = "\033[38;5;153m"
SEV_COLOR = "\033[38;5;115m"
EIG_COLOR = "\033[38;5;134m"
CLE_COLOR = "\x1b[39;49m"

class Cell:
    def __init__(self,nbr_mines,is_mine,visible):
        self.nbr_mines = nbr_mines
        self.is_mine = is_mine
        self.visible = visible
    
class Minesweeper:
    
    # initialize a minesweeper object
    def __init__(self,board_size, percent_mines):
        self.board_size = board_size
        self.nbr_mines = int(board_size*board_size*(percent_mines/100.0))
        
         # setup a 2-dimensional array of Cell objects
        self.board = [[0 for x in range(self.board_size)] for x in range(self.board_size)]
        for i in range(self.board_size):
            for j in range(self.board_size):
                self.board[i][j]=Cell(0,False,False)
                
        self.place_mines_on_board()
        self.fill_in_minecount_for_non_mine_cells()
        
    # places mines randomly on the board   
    def place_mines_on_board(self):
        count = 0
        while count < self.nbr_mines:
            x = random.randrange(self.board_size)
            y = random.randrange(self.board_size)
            
            if not self.board[x][y].is_mine:
                self.board[x][y].is_mine = True
                count += 1
        
    # for each non-mine cell on the board, set @nbr_mines of each Cell on the board
    # to the number of mines in the immediate neighborhood.
    def fill_in_minecount_for_non_mine_cells(self):
        for i in range(self.board_size):
            for j in range(self.board_size):
                if not self.board[i][j].is_mine:
                    self.board[i][j].nbr_mines = self.get_nbr_neighbor_mines(i,j)
    
    # processes cell selection by user during the game
    # returns WON, LOST, or INPROGRESS
    def select_cell(self,row,col):
        self.board[row][col].visible = True
        
        if self.board[row][col].is_mine:
            return LOST
            
        if self.board[row][col].nbr_mines == 0:
            self.set_all_neighbor_cells_visible(row,col)
            
        if self.nbr_mines + self.nbr_visible_cells() == self.board_size*self.board_size:
            return WON
            
        return INPROGRESS
    
    # returns the number of mines in the immediate neighborhood of a cell
    # at location (row,col) on the board.
    def get_nbr_neighbor_mines(self,row,col):
        count = 0
    
        #check left
      	if col > 0 and self.board[row][col-1].is_mine:
      		count += 1
      	#check diag up left
      	if (col > 0 and row > 0) and self.board[row-1][col-1].is_mine:
      		count += 1
      	#check above
      	if (row > 0 and self.board[row-1][col].is_mine):
      		count += 1
      	#check diag up right
      	if (row > 0 and col < self.board_size-1) and self.board[row-1][col+1].is_mine:
      		count += 1
      	#check right
      	if col < self.board_size-1 and self.board[row][col+1].is_mine:
      		count += 1
      	#check diag down right
      	if (row < self.board_size-1 and col < self.board_size-1) and self.board[row+1][col+1].is_mine:
      		count += 1
      	#check below
      	if row < self.board_size-1 and self.board[row+1][col].is_mine:
      		count += 1
      	#check diag down left
      	if (row < self.board_size-1 and col > 0) and self.board[row+1][col-1].is_mine:
      		count += 1
     
      	return count
    
    # returns the number of cells that are currently visible on the board
    def nbr_visible_cells(self):
        count = 0
        
        for i in range(self.board_size):
            for j in range(self.board_size):
                if self.board[i][j].visible:
                    count += 1
        
        return count
        
    # if the mine count of a cell at location (row,col) is zero, then make 
    # the cells in the immediate neighborhood visible and repeat this
    # process for each of the cells in this set of cells that have a mine
    # count of zero, and so on.
    def set_all_neighbor_cells_visible(self,row,col):
        #check left
        if col > 0 and not self.board[row][col-1].visible:
        	self.select_cell(row, col-1)
        #check diag up left
        if (col > 0 and row > 0) and self.board[row-1][col-1].visible:
        	self.select_cell(row-1, col-1)
        #check above
        if (row > 0 and not self.board[row-1][col].visible):
        	self.select_cell(row-1, col)
        #check diag up right
        if ((row > 0 and col < self.board_size-1) and not self.board[row-1][col+1].visible):
        	self.select_cell(row-1, col+1)
        #check right
        if (col < self.board_size-1 and not self.board[row][col+1].visible):
        	self.select_cell(row, col+1)
        #check diag down right
        if ((row < self.board_size-1 and col < self.board_size-1) and not self.board[row+1][col+1].visible):
        	self.select_cell(row+1, col+1)
        #check below
        if (row < self.board_size-1 and not self.board[row+1][col].visible):
        	self.select_cell(row+1, col)
        #check diag down left
        if ((row < self.board_size-1 and col > 0) and not self.board[row+1][col-1].visible):
        	self.select_cell(row+1, col-1)
    
    # returns a string representation of the board
    def to_string(self,display_mines):
        result = ""
        
        #top row
        for i in range(self.board_size):
            if i == 0:
                result += RCN_COLOR
                result += "%6d" % (i+1)
            else:
                result += "%3d" % (i+1)
        result += "\n"
            
        #the rest    
        for i in range(self.board_size):
            #row label
            result += RCN_COLOR
            result += "%3d" % (i+1)
            result += CLE_COLOR
            for j in range(self.board_size):
                if self.board[i][j].visible:
                    if self.board[i][j].is_mine:
                        result += MIN_COLOR + "  *" + CLE_COLOR
                    else:
                        result += self.print_colored(self.board[i][j].nbr_mines)
                else:
                    if display_mines and self.board[i][j].is_mine:
                        result += MIN_COLOR + "  *" + CLE_COLOR
                    else:
                        result += "  ?"
            result += "\n"
            
        return result
    
    def print_colored(self,nbr_mines):
        if nbr_mines == 0:
            return ZER_COLOR + "  0" + CLE_COLOR
        if nbr_mines == 1:
            return ONE_COLOR + "  1" + CLE_COLOR
        if nbr_mines == 2:
            return TWO_COLOR + "  2" + CLE_COLOR
        if nbr_mines == 3:
            return THR_COLOR + "  3" + CLE_COLOR
        if nbr_mines == 4:
            return FOU_COLOR + "  4" + CLE_COLOR
        if nbr_mines == 5:
            return FIV_COLOR + "  5" + CLE_COLOR
        if nbr_mines == 6:
            return SIX_COLOR + "  6" + CLE_COLOR
        if nbr_mines == 7:
            return SEV_COLOR + "  7" + CLE_COLOR
        if nbr_mines == 8:
            return EIG_COLOR + "  8" + CLE_COLOR

def get_board_size():
    board_size = 0
    while board_size < BOARD_SIZE_MIN or board_size > BOARD_SIZE_MAX:
        board_size = raw_input("Enter board size ("+str(BOARD_SIZE_MIN)+" .. "+str(BOARD_SIZE_MAX)+"): ")
        try:
            board_size = int(board_size)
            if  board_size < BOARD_SIZE_MIN or board_size > BOARD_SIZE_MAX:
                print "Invalid board size. Number out of range. Try again."
        except ValueError:
            print("Invalid board size. Must be a number. Try again.")

    return board_size

def get_percent_mines():
    percent_mines = 0
    while percent_mines < PCT_MINES_MIN or percent_mines >PCT_MINES_MAX:
        percent_mines = raw_input("Enter percentage of mines on the board ("+str(PCT_MINES_MIN)+" .. "+str(PCT_MINES_MAX)+"): ")
        try:
            percent_mines = int(percent_mines)
            if  percent_mines < PCT_MINES_MIN or percent_mines > PCT_MINES_MAX:
                print "Invalid value for percentage. Number out of range. Try again."
        except ValueError:
            print("Invalid value for percentage. Must be a number. Try again.")

    return percent_mines
    
def display_menu():
    print "List of available commands:"
    print "   Show Mines: s/S"
    print "   Hide Mines: h/H"
    print "   Select Cell: c/C"
    print "   Display Board: b/B"
    print "   Display Menu: m/M"
    print "   Quit: q/Q"


def main():
    
    print ONE_COLOR
    print "************************************************************************************************************************"
    print "* _|      _|  _|_|_|  _|      _|  _|_|_|_|    _|_|_|  _|          _|  _|_|_|_|  _|_|_|_|  _|_|_|    _|_|_|_|  _|_|_|   *"
    print "* _|_|  _|_|    _|    _|_|    _|  _|        _|        _|          _|  _|        _|        _|    _|  _|        _|    _| *"
    print "* _|  _|  _|    _|    _|  _|  _|  _|_|_|      _|_|    _|    _|    _|  _|_|_|    _|_|_|    _|_|_|    _|_|_|    _|_|_|   *"
    print "* _|      _|    _|    _|    _|_|  _|              _|    _|  _|  _|    _|        _|        _|        _|        _|    _| *"
    print "* _|      _|  _|_|_|  _|      _|  _|_|_|_|  _|_|_|        _|  _|      _|_|_|_|  _|_|_|_|  _|        _|_|_|_|  _|    _| *"
    print "* a game by jaxon wright and chad teitsma....this time in python!                                                      *"
    print "************************************************************************************************************************\n"
    print CLE_COLOR
    
    display_mines = False
    game_state = INPROGRESS
    
    
    board_size = get_board_size()
    percent_mines = get_percent_mines()
    board = Minesweeper(board_size,percent_mines)
    
    print board.to_string(display_mines)
    
    while True:
        command = raw_input("Enter command (m/M for menu): ")
        str(command)
        
        if command == 'm' or command == 'M':
            display_menu()
            
        elif command == 'c' or command == 'C':
            row = 0
            col = 0
            while row < 1 or row > board_size or col < 1 or col > board_size:
                line = raw_input("Enter row and column of cell: ")
                coordinate = line.split()
                input_error = False
                try:
                    row = coordinate[0]
                    col = coordinate[1]
                    row = int(row)
                    col = int(col)
                except ValueError:
                    print "Invalid row or column values. Must be a number. Try again."
                    input_error = True
                except IndexError:
                    print "Invalid row or column values. Please enter 2 values. Try again."
                    input_error = True
                
                if  (row < 1 or row > board_size or col < 1 or col > board_size) and not input_error:
                    print "Invalid row or column values. Out of bounds. Try again."
                    
            game_state = board.select_cell(row-1,col-1)
            print board.to_string(display_mines)
            
        elif command == 's' or command == 'S':
             display_mines = True
             print board.to_string(display_mines)
             
        elif command == 'h' or command == 'H':
            display_mines = False
            print board.to_string(display_mines)
        
        elif command == 'b' or command == 'B':
            print board.to_string(display_mines)
            
        elif command == 'q' or command == 'Q':
            print "Bye."
            return
        
        else:
            print "Invalid command. Try again."
        #end of elif's    
            
        if game_state == WON:
            print  "You found all the mines. Congratulations. Bye."
            return
        elif game_state == LOST:
            print "Oops. Sorry, you landed on a mine. Bye"
            return
    #end of while
    
    #end of main
                    
# run the game
main()
    
        