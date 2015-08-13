/*
SUDOKU GAME USING OPENGL and MVC Design Pattern
To run this game, type make and then type ./Sudoku
A window will be launched. You can use the mouse or
the keyboard to select the box and enter the number.

AUTHOR - SHIVANG MENON
*/

#include "Sudoku.h";

time_t timer;
bool Success = false;

SudokuModel::SudokuModel(){
  number_of_filled_cells = 81;
  selected_box = -1;
  for(int i=0;i<81;i++){
   position_list.push_back(i);
  }
  random_shuffle(position_list.begin(),position_list.end());  //Shuffle the positions
	Create_valid_board();
	Create_sudoku_puzzle();
}

void SudokuModel::Create_valid_board(){
  srand(time(NULL));

  //Start with a valid board, and shuffle it
  //Swap 2 numbers, swap 2 rows in 0-2,3-5,6-8, swap 2 cols in 0-2,3-5,6-8
  int temp_sudoku[81]={3,2,9, 6,5,7, 8,4,1,
                      7,4,5, 8,3,1, 2,9,6,
                      6,1,8, 2,4,9, 3,7,5,
                      1,9,3, 4,6,8, 5,2,7,
                      2,7,6, 1,9,5, 4,8,3,
                      8,5,4, 3,7,2, 6,1,9,
                      4,3,2, 7,1,6, 9,5,8,
                      5,8,7, 9,2,3, 1,6,4,
                      9,6,1, 5,8,4, 7,3,2}; 
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
       board[i*9+j].number = temp_sudoku[i*9+j];
    }
  }

  int n1,n2;
  int p1,p2;Cell temp;
  int column_grid,row_grid,r1,r2,c1,c2;

  for(int i=0;i<10000;i++)
  {
    n1=rand()%9+1;
    do{
    n2=rand()%9+1;}while(n1==n2);

    for(int j=0;j<9;j++)
    {  for(int k=0;k<9;k++)
       {
          if(board[j*9+k].number==n1){p1=k;}
          if(board[j*9+k].number==n2){p2=k;}
       }
       temp=board[j*9+p1];
       board[j*9+p1]=board[j*9+p2];
       board[j*9+p2]=temp;
    }
  }

  for(int i=0;i<10000;i++){
    row_grid=rand()%3;
    r1 = row_grid*3 + rand()%3;
    do{r2=row_grid*3+rand()%3;}while(r1==r2);
    for(int j=0;j<9;j++){
       temp=board[r1*9+j];
       board[r1*9+j]=board[r2*9+j];
       board[r2*9+j]=temp;
    }
  }

  for(int i=0;i<10000;i++){
    column_grid=rand()%3;
    c1 = column_grid*3 + rand()%3;
    do{c2=column_grid*3+rand()%3;}while(c1==c2);
    for(int j=0;j<9;j++){
       temp=board[j*9+c1];
       board[j*9+c1]=board[j*9+c2];
       board[j*9+c2]=temp;
    }
  }
   //Print_board();
}

void SudokuModel::Create_sudoku_puzzle(){
  // Erase cells whose positions are there in position_list, and check if it results in a unique solution
  // If no unique solution, then undo erase and move onto next cell.
  int original_number; int nSols=0;

  for(int i=0;i<position_list.size();i++){
    original_number = board[position_list[i]].number;
    
    board[position_list[i]].number = 0;
    board[position_list[i]].given = false;
    number_of_filled_cells-=1;

    nSols=0;
    if(NoUniqueSolution(&nSols)){
      board[position_list[i]].number = original_number;
      board[position_list[i]].given = true;
      number_of_filled_cells+=1;
    }
  }   
}

bool SudokuModel::Find_unassigned_cell(int &pos)
{
  for(pos=0;pos<81;pos++){
    if(board[pos].number==0)
      return true;
  }       
}

bool SudokuModel::Check_validity(int pos, int num){

  int row = pos/9;
  int col = pos%9;
  for(int c=0;c<9;c++){
  if(board[row*9+c].number == num){
    return false;
  }
  }

  for(int r=0;r<9;r++){
    if(board[r*9+col].number == num){
       return false;
    }
  }

  int startrow = row - row%3;
  int startcol = col - col%3;

  for(int r=startrow;r<startrow+3;r++){
    for(int c=startcol;c<startcol+3;c++){
       if(board[r*9+c].number == num){
          return false;
       }
    }
  }
  return true;

}

void SudokuModel::Print_board(){
	for(int i=0;i<9;i++)
   {  for(int j=0;j<9;j++)
         {cout<<board[i*9+j].number<<" ";}
         cout<<endl; 
   }
  cout<<endl;
}

bool SudokuModel::NoUniqueSolution(int *number_of_solutions){
  int pos;

  if (!Find_unassigned_cell(pos)){
   *number_of_solutions=*number_of_solutions+ 1;
   return true;}

  for (int num = 1; num <= 9; num++)

  {

    if (Check_validity(pos, num))

    {

        board[pos].number = num;

        if (NoUniqueSolution(number_of_solutions)){
            if((*number_of_solutions)>1){
              board[pos].number=0;
                return true;}
            }

        board[pos].number = 0;

    }

  }

  return false;

}

SudokuController::SudokuController(SudokuModel* m){
  model = m;
}

void SudokuController::CheckExistingErrorCells(int input_position){
  //Checks if the existing errors are cleared or not
  for(std::vector<int>::iterator it=ErrorList.begin();it!=ErrorList.end();++it){

    int position = *it;
    int row_number = position/9;
    int col_number = position%9;
    if(!SingleCellErrors(row_number,col_number)){
      model->board[*it].mistake = false;
      ErrorList.erase(it);
      --it;
    }
  }
}

bool SudokuController::SingleCellErrors(int row_number,int col_number){

  int box_start_row = (row_number/3)*3;
  int box_start_col = (col_number/3)*3;

  for(int j=0;j<9;j++){
    if(j==col_number){continue;}

    if(model->board[row_number*9+j].number == model->board[row_number*9+col_number].number){
      return true;
    }
  }

  for(int j=0;j<9;j++){
    if(j==row_number){continue;}

    if(model->board[j*9+col_number].number == model->board[row_number*9+col_number].number){
      return true;
    }
  }

  for(int i=box_start_row;i<box_start_row+3;i++){
    for(int j=box_start_col;j<box_start_col+3;j++){

      if(i==row_number || j==col_number){continue;}

      if(model->board[i*9+j].number == model->board[row_number*9+col_number].number){
        return true;
      }

    }
  }

  return false;
}

void SudokuController::CheckRowColBoxErrors(int position){

  int row_number = position/9;
  int col_number = position%9;
  int box_start_row = (row_number/3)*3;
  int box_start_col = (col_number/3)*3;

  model->board[position].mistake = false;

  for(int j=0;j<9;j++){
    if(j==col_number){continue;}

    if(model->board[row_number*9+j].number == model->board[row_number*9+col_number].number){
      if(!model->board[row_number*9+j].given && !model->board[row_number*9+j].mistake){
        model->board[row_number*9+j].mistake = true;
        ErrorList.push_back(row_number*9+j);
      }

      if(!model->board[row_number*9+col_number].mistake){
        model->board[row_number*9+col_number].mistake = true;
        ErrorList.push_back(row_number*9+col_number);
      }
    }
  }

  for(int j=0;j<9;j++){
    if(j==row_number){continue;}

    if(model->board[j*9+col_number].number == model->board[row_number*9+col_number].number){
      if(!model->board[j*9+col_number].given && !model->board[j*9+col_number].mistake){
        model->board[j*9+col_number].mistake = true;
        ErrorList.push_back(j*9+col_number);
      }

      if(!model->board[row_number*9+col_number].mistake){
        model->board[row_number*9+col_number].mistake = true;
        ErrorList.push_back(row_number*9+col_number);
      }
    }
  }

  for(int i=box_start_row;i<box_start_row+3;i++){
    for(int j=box_start_col;j<box_start_col+3;j++){

      if(i==row_number || j==col_number){continue;}

      if(model->board[i*9+j].number == model->board[row_number*9+col_number].number){
        if(!model->board[i*9+j].given && !model->board[i*9+j].mistake){
          model->board[i*9+j].mistake = true;
          ErrorList.push_back(i*9+j);
        }

        if(!model->board[row_number*9+col_number].mistake){
          model->board[row_number*9+col_number].mistake = true;
          ErrorList.push_back(row_number*9+col_number);
        }
      }

    }
  }
}

void SudokuController::CheckforErrors(int position){
  model->NoMistake = true;
  CheckExistingErrorCells(position);
  CheckRowColBoxErrors(position);

  if(!ErrorList.empty()){
    model->NoMistake = false;
  }

}


SudokuModel* SudokuView::model=0;
SudokuController* SudokuView::controller=0;

SudokuView::SudokuView(int argc, char** argv, SudokuModel* m, SudokuController* c){
  model=m;
  controller = c;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(700, 700);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Sudoku Game");
  glClearColor(1.0, 1.0, 1.0, 0.0);   // white background
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialkeys);
  glutMouseFunc(mouseclick);

  glutReshapeFunc(reshape);

  glutMainLoop();
}

void SudokuView::reshape(int w,int h){
  glViewport(0,0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, (GLdouble)-w, (GLdouble)w);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void DrawText(const char* text,int length,double x,double y){
  glMatrixMode(GL_PROJECTION);
  double *matrix = new double[16];
  glGetDoublev(GL_PROJECTION_MATRIX,matrix);
  glLoadIdentity();
  glOrtho(0,700,0,700,-5,5);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
  glLoadIdentity();
  glRasterPos2f(x,y);
  for(int i=0;i<length;i++){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)text[i]);//GLUT_BITMAP_9_BY_15,text);
  }
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(matrix);
  glMatrixMode(GL_MODELVIEW);

}

void SudokuView::display(){
   // clear all
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_BLEND); //Enable blending.
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
  // Clear the matrix
  glLoadIdentity();
  
  //Track time
  double Seconds_in_game;
  time_t current_time;

  if(!Success){
    current_time = time(NULL);
    
    Seconds_in_game = difftime(current_time,timer);
    model->Seconds_in_game = fmod(Seconds_in_game,60.0);
    
    model->Minutes_in_game = int(Seconds_in_game)/60;
    if(model->Minutes_in_game >=60){model->Minutes_in_game %= 60;}

    model->Hours_in_game = int(Seconds_in_game)/3600;
    if(model->Hours_in_game >= 24){model->Hours_in_game %=24;}
  }

  std::ostringstream strm;
  strm <<model->Hours_in_game;std::string numStr = "TIME = " + strm.str(); strm.str(std::string());
  strm <<model->Minutes_in_game; numStr = numStr + ":" + strm.str();strm.str(std::string());
  strm <<model->Seconds_in_game; numStr = numStr + ":" + strm.str();strm.str(std::string());

  glColor3f(0.0,0.0,1.0);
  DrawText(numStr.data(),numStr.length(),300.0,650.0); //Display Time


  int width_board=400;int height_board=400;

  //Draw the outer box of the sudoku
  glColor3f(0.0,0.0,0.0);
  glLineWidth(4.0);
  glBegin(GL_LINE_LOOP);
  glVertex2f(150.0,150.0);
  glVertex2f(550.0,150.0);
  glVertex2f(550.0,550.0);
  glVertex2f(150.0,550.0);
  glEnd();

  double width_cell = 400.0/double(9);
  double height_cell = 400.0/double(9);

  //draw the lines in between to create rows and cols and cells
  for(int i=1;i<9;i++){
   glLineWidth(1.0);
   if(i%3==0){glLineWidth(4.0);}
   glBegin(GL_LINES);
   glColor3f(0.0,0.0,0.0);
   glVertex2f(150.0+i*width_cell,150.0);
   glVertex2f(150.0+i*width_cell,550.0);
   glEnd();

   glBegin(GL_LINES);
   glColor3f(0.0,0.0,0.0);
   glVertex2f(150.0,150.0+i*height_cell);
   glVertex2f(550.0,150.0+i*height_cell);
   glEnd();
  }

  int row_number,col_number;

  //draw the selected box (in green)
  if(model->selected_box!=-1){
    
    row_number = model->selected_box/9;
    col_number = model->selected_box%9;

    double x_coord = 150.0 + (width_cell*col_number);
    double y_coord = 550.0 - (height_cell*(1+row_number));


    glColor4f(0.0,0.0,1.0,1.0);
    glLineWidth(5.5);
    

    glBegin(GL_LINE_LOOP);
    glVertex2f(x_coord,y_coord);
    glVertex2f(x_coord+(400.0/9.0),y_coord);
    glVertex2f(x_coord+(400.0/9.0),y_coord+(400.0/9.0));
    glVertex2f(x_coord,y_coord+(400.0/9.0));
    glEnd();

  }
  

  //display the numbers
  for(int i=0;i<81;i++){
    if(model->board[i].number!=0){
      row_number = i/9;
      col_number = i%9;
      
      if(model->board[i].given){
        glColor4f(0.0,0.0,0.0,1.0);
      }
      else{
        if(model->board[i].mistake){
          glColor3f(1.0,0.0,0.0);
        }
        else{
          glColor4f(0.196078,0.8,0.196078,1.0);
        }
        
      }

      std::ostringstream strm;
      strm << model->board[i].number;
      std::string numStr = strm.str();


      
      DrawText(numStr.c_str(),1,(150.0+(col_number*width_cell)+(width_cell/2.0)-5.0),(550.0-(row_number*height_cell)-(height_cell/2.0)-5.0));
    }
  }

  if(model->number_of_filled_cells==81 && model->NoMistake){
    glColor3f(1.0,0.0,0.0);
    string text = "Congratulations!! You have solved the sudoku";
    Success = true;
    DrawText(text.data(),text.length(),175.0,625.0);
  }


  glutSwapBuffers();
}

void SudokuView::keyboard (unsigned char key, int x, int y) 
{
  if(!Success){
    // Keystroke processing here
    switch (key-48){
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
        if(model->selected_box!=-1)
        {
          
          if(!model->board[model->selected_box].given){
            
            if(model->board[model->selected_box].number==0){
              model->number_of_filled_cells+=1;
            }
            model->board[model->selected_box].number = key-48;  
            //When user enters a number check if it eliminates any of the existing errors or creates a new error        
            controller->CheckforErrors(model->selected_box); 
          }
        
        }
        break;
    }
  }
}

void SudokuView::specialkeys(int key,int x,int y){
  // If user presses any of the arrow keys move the box
  switch(key){
    case GLUT_KEY_RIGHT:
      if(model->selected_box==-1){model->selected_box+=1;}
      else if(model->selected_box%9==8){model->selected_box-=8;}
      else {model->selected_box+=1;}
      break;
    case GLUT_KEY_LEFT:
      if(model->selected_box==-1){model->selected_box+=1;}
      else if(model->selected_box%9==0){model->selected_box+=8;}
      else {model->selected_box-=1;}
      break;
    case GLUT_KEY_UP:
      if(model->selected_box==-1){model->selected_box+=1;}
      else if(model->selected_box>=0&&model->selected_box<=8){model->selected_box+=72;}
      else {model->selected_box-=9;}
      break;
    case GLUT_KEY_DOWN:
      if(model->selected_box==-1){model->selected_box+=1;}
      else if(model->selected_box>=72&&model->selected_box<=80){model->selected_box-=72;}
      else {model->selected_box+=9;}
      break;  
  }
}



void SudokuView::mouseclick(int button,int state, int x, int y)
{
  if(x<150.0 || y<150.0 || x>550.0 || y>550.0){
    model->selected_box = -1;
  }
  else{
    int row_number = ((y-150.0)*9)/400;
    int col_number = ((x-150.0)*9)/400;
    model->selected_box = (row_number*9)+col_number;
  }
}

int main(int argc, char** argv){
  timer = time(NULL);
  SudokuModel* m = new SudokuModel();
  SudokuController* c = new SudokuController(m);
  SudokuView* v = new SudokuView(argc,argv,m,c);
  
  glutMainLoop();
  return 0;
}
