#
#  Minesweeper game
#
#  Author(s): Jaxon Wright and Chad Teitsma
#
from array import *
import random

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

        self.is_m
    def __init__(self,nbr_mines,is_mine,visible):
        self.nbr_mines = nbr_mines
        self.is_mine = is_mine
        self.visible = visible
    
    
class Minesweeper:
    
    # initialize a minesweeper object
    def __init__(self,board_size, percent_mines):
        self.board_size = board_size
        self.nbr_mines = (board_size*board_size*(percent_mines/100.0))
         # setup a 2-dimensional array of Cell objects
        self.board = a
        
    # places mines randomly on the board   
    def place_mines_on_board(self):
        #TODO
        return
        
    # for each non-mine cell on the board, set @nbr_mines of each Cell on the board
    # to the number of mines in the immediate neighborhood.
    def fill_in_minecount_for_non_mine_cells(self):
        #TODO
        return
    
    # processes cell selection by user during the game
    # returns WON, LOST, or INPROGRESS
    def select_cell(self,row,col):
        #TODO
        return
    
    # returns the number of mines in the immediate neighborhood of a cell
    # at location (row,col) on the board.
    def get_nbr_neighbor_mines(self,row,col):
        #TODO
        return
    
    # returns the number of cells that are currently visible on the board
    def nbr_visible_cells(self):
        #TODO
        return
        
    # if the mine count of a cell at location (row,col) is zero, then make 
    # the cells in the immediate neighborhood visible and repeat this
    # process for each of the cells in this set of cells that have a mine
    # count of zero, and so on.
    def set_all_neighbor_cells_visible(self,row,col):
        #TODO
        return
    
    # returns a string representation of the board
    def to_string(self,display_mines):
        #TODO
        return


def get_board_size():
    #TODO
    return

def get_percent_mines():
    #TODO
    return

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
    
    board_size = get_board_size()
    percent_mines = get_percent_mines()
    board = Minesweeper(board_size,percent_mines)
    

# run the game
main();
    
        