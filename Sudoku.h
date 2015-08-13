/*
SUDOKU GAME USING OPENGL and MVC Design Pattern
To run this game, type make and then type ./Sudoku
A window will be launched. You can use the mouse or
the keyboard to select the box and enter the number.

AUTHOR - SHIVANG MENON
*/


#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

#ifdef LINUX
//Linux Headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef OSX
// MAC Headers
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#ifdef WINDOWS
//Windows Headers
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#endif

using namespace std;

struct Cell{
   int number;
   bool given=true;
   bool mistake=false;
};

bool operator<(Cell c1,Cell c2){
   return c1.number<c2.number;
}

class SudokuModel{
public:
  Cell board[81];
  vector<int> position_list;      //list of positions (0-80) - to be shuffled later
  int number_of_filled_cells;     //used to keep a tab of how many cells user has filled (including filled cells given at the beginning)
  int selected_box;               //box that is selected by mouse click or keyboard keys (basically the cell in which the user can enter the number)
  
  //used to display and track time
  double Seconds_in_game;   
  int Minutes_in_game;
  int Hours_in_game;
  
  bool NoMistake;                 //flag to check if all numbers entered by user is right
	
  SudokuModel();
	void Create_valid_board();      //used to create a valid board from which numbers are "hidden" later
	void Create_sudoku_puzzle();    //empty cells from valid board (for user to fill)
  
  bool Check_validity(int pos,int num);   //checks if num is there in corresponding row,col or box
	bool NoUniqueSolution(int *number_of_solutions);   //recursive function to find if current puzzle has a unique solution or not
	
	void Print_board();             //Prints board
  bool Find_unassigned_cell(int &pos); //checks if any cell in board is still unassigned

};

class SudokuController{
public:
  SudokuModel* model;             //Pointer to model object
  vector<int> ErrorList;          //List of cell positions that are blatantly incorrect (if there are same numbers in rows,cols or boxes)
  SudokuController(SudokuModel*);
  
  void CheckforErrors(int);       //Checks board for errors (Same numbers in rows,cols or boxes)  
  void CheckRowColBoxErrors(int);
  void CheckExistingErrorCells(int);
  bool SingleCellErrors(int,int);
};

class SudokuView{ 
public:
  SudokuView(int argc, char** argv, SudokuModel* m, SudokuController* c);
  static void reshape(int, int); // Called by GLUT
  static void display(); // Called by GLUT
  static void keyboard(unsigned char key, int x, int y);
  static void specialkeys(int key, int x, int y);
  static void mouseclick(int button,int state, int x, int y);
  static SudokuModel* model;
  static SudokuController* controller;
};