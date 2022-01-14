#include <iostream>
#include <time.h>
#include <conio.h>
#include<cstdlib>
#include<fstream>
#include<string>

#include<fcntl.h>
#include<io.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include<mmsystem.h>


void startGame();
void mainMenu();
void choice();
void setConsoleColour(unsigned short colour); 
void ClearScreen();
void hidecursor();
void choice();
void highScore();
void gotoXY(int x,int y);



using namespace std;



unsigned char col [] = {FOREGROUND_INTENSITY, FOREGROUND_GREEN, FOREGROUND_RED, FOREGROUND_BLUE, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE};
string name[2];
int color[2];
enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6};



int main()

{	
	startGame();
	return 0;
}


struct cBall
{

	int x, y;
	int originalX, originalY;
	eDir direction;


	cBall(int posX, int posY)
	{
		originalX = posX;
		originalY = posY;
		x = posX; y = posY;
		direction = STOP;
	}
	void Reset()
	{
		x = originalX; y = originalY;
		direction = STOP;
	}
	void changeDirection(eDir d)
	{
		direction = d;
	}
	void randomDirection()
	{
		direction = (eDir)((rand() % 6) + 1);
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline eDir getDirection() { return direction; }
	void Move()
	{
		switch (direction)
		{
		case STOP:
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		case UPRIGHT:
			x++; y--;  
			break;
		case DOWNRIGHT:
			x++; y++;
			break;
		default:
			break;
		}
	}
};
struct cPaddle
{
	int x, y;
	int originalX, originalY;

	cPaddle()
	{
		x = y = 0;
	}
	cPaddle(int posX, int posY) : cPaddle()
	{
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	}
	inline void Reset() { x = originalX; y = originalY; }
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUp() { y--; }
	inline void moveDown() { y++; }
	
};
struct cGameManger
{
	
	int width, height;
	int score1, score2;
	char up1, down1, up2, down2;
	bool quit;
	bool endofgame = false;
	cBall *ball;
	cPaddle *player1;
	cPaddle *player2;


	cGameManger(int w, int h)
	{
		srand(time(NULL));
		quit = false;
		up1 = 'w'; up2 = 'i';
		down1 = 's'; down2 = 'k';
		score1 = score2 = 0;
		width = w; height = h;
		ball = new cBall(w / 2, h / 2);
		player1 = new cPaddle(1, h / 2 - 3);
		player2 = new cPaddle(w - 2, h / 2 - 3);
	}
	~cGameManger()
	{
		delete ball, player1, player2;
	}

	void ScoreUp(cPaddle * player)
	{
		PlaySound((LPCSTR) "score.wav", NULL, SND_FILENAME | SND_ASYNC);
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;

		ball->Reset();
		player1->Reset();
		player2->Reset();
	}



	void Draw()
	{
		ClearScreen();

		string pong[9] = {"#######################################################"\
		                  ,"#######################################################"\
		                  ,"####         ####        ####    ####  ####        ####"\
		                  ,"####  #####  ####  ####  ####  #  ###  ####  ##########"\
		                  ,"####         ####  ####  ####  ##  ##  ####  ###   ####"\
		                  ,"####  ###########  ####  ####  ###  #  ####  ####  ####"\
		                  ,"####  ###########        ####  ####    ####        ####"\
		                  ,"#######################################################"\
		                  ,"#######################################################"};

		for (int x= 0; x< 9; x++)
		{
			gotoXY(25,x+1);
			for (int t= 0; t< 55; t++)
			if (pong[x][t] == '#')
			pong[x][t] = '\xCE';
			cout<<pong[x];
		}
		gotoXY(((55-width)/2) +25,12);

		for (int i = 0; i < width + 1; i++)
			cout << "\xB2";
		cout << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{gotoXY(((55-width)/2) +25 + j,13+i);
				int ballx = ball->getX();
				int bally = ball->getY();
				int player1x = player1->getX();
				int player2x = player2->getX();
				int player1y = player1->getY();
				int player2y = player2->getY();

				if (j == 0)
					cout << "\xB2";
				if (!endofgame)
				{
					if (ballx == j && bally == i)
					{
						setConsoleColour(col[1] | col[2]);
						_setmode(_fileno(stdout), 0x20000);
						wprintf(L"\x263b"); //ball
						_setmode(_fileno(stdout), 0X4000);
						setConsoleColour(col[4]);
					}

					else if (player1x == j && (player1y <=i && player1y +3 >= i))
					{
						setConsoleColour(col[color[0]]);
	            		cout<<"\xDB";//player1
	            		setConsoleColour(col[4]);
					}

					else if (player2x == j &&(player2y <=i && player2y +3 >= i))
					{
						setConsoleColour(col[color[1]]);
	            		cout<<"\xDB";//player1
	            		setConsoleColour(col[4]);
					} //player2
					else
					{
						setConsoleColour(col[0] | col[3]);
	            	    cout<<"\xDB";//player1
	            	    setConsoleColour(col[4]);
					}
				}
				else
				{
					unsigned char r;

					if(score1 > score2)
						{
							if ((j == 18 || j==17) && (i >3 && i <13))
								r = col[color[0]];
							else
								r = col[0] | col[3];
						}
						else
						{
							if((i == 3 || i == 10|| i == 6) && (j>13 && j <24))
								r = col[color[1]];
							else if ((i > 3 && i < 7) &&  (j == 22 ||j == 23))
								r = col[color[1]];
							else if ((i > 6 && i < 11) &&  (j == 14 ||j == 15))
								r = col[color[1]];
							else
								r = col[0] | col[3];
						}
						setConsoleColour(r);
            			cout<<"\xDB";
            			setConsoleColour(col[4]);


				}
				if (j == width - 1)
					cout << "\xB2";
			}
			cout << endl;
		}
		gotoXY(((55-width)/2) +25,13+height);
		for (int i = 0; i < width + 1; i++)
			cout << "\xB2";
			cout<<endl;
		
		


	

		cout << name[0]<<": " << score1 << endl <<name[1]<< " : " << score2 << endl;

if(endofgame){

 fstream highScore;
	int high;

    highScore.open("highScore.txt",ios::in);//read high score

 	
	if(highScore.is_open()){
		string line;
    	getline(highScore,line);
		high=stoi(line);
        highScore.close();
    }
if(high<score1 || high<score2){

	 if(score1>score2){
    fstream highScore;
	fstream highScoreN;
	int high;

    highScore.open("highScore.txt",ios::out);//write high score
	highScoreN.open("highScoreN.txt",ios::out);//write the name
    if(highScoreN.is_open()){
        
		highScoreN<<name[0];
        highScoreN.close();
    }
	if(highScore.is_open()){
        highScore<<score1;
        highScore.close();
    }
	
		 }

	  if(score2>score1){
    fstream highScore;
	fstream highScoreN;
	int high;

    highScore.open("highScore.txt",ios::out);//write high score
	highScoreN.open("highScoreN.txt",ios::out);//write the name
    if(highScoreN.is_open()){
        
		highScoreN<<name[1];
        highScoreN.close();
    }
	if(highScore.is_open()){
        highScore<<score2;
        highScore.close();
    }
	
		 }
		 
	
			
		
	
			Sleep(30000);
			cout << "press any key to exit";
			getch();
			quit = true;
		}}
	}
	void Input()
	{
		ball->Move();

		int player1y = player1->getY();
		int player2y = player2->getY();

		bool bKey [5];
		for (int k = 0; k < 5; k++)
		    bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("WSIKQ"[k]))) != 0;

		if (bKey[0] && player1y > 0)
			player1->moveUp();
		if (bKey[2] && player2y > 0)
			player2->moveUp();
		if (bKey[1] && (player1y + 4 < height))
			player1->moveDown();
		if (bKey[3] && player2y + 4 < height)
			player2->moveDown();
		if (ball->getDirection() == STOP)
				ball->randomDirection();
		if (bKey[4])
			quit = true;

	}
	void Logic()
	{
		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();

		//left paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player1x + 1)
				if (bally == player1y + i){
					ball->changeDirection((eDir)((rand() % 3) + 4));
				PlaySound((LPCSTR) "paddle.wav", NULL, SND_FILENAME | SND_ASYNC);
				}

		//right paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player2x - 1)
				if (bally == player2y + i){
					ball->changeDirection((eDir)((rand() % 3) + 1));
				PlaySound((LPCSTR) "paddle.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
		//bottom wall
		if (bally == height - 1){
			ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
		PlaySound((LPCSTR) "wall.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		//top wall
		if (bally == 0){
			ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
		PlaySound((LPCSTR) "wall.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		//right wall
		if (ballx == width - 1)
			ScoreUp(player1);
		//left wall
		if (ballx == 0)
			ScoreUp(player2);
		if((score1 + score2) > 4)
		{
			endofgame = true;
		}
	

	}
	void Run()
	{
		while (!quit)
		{
			Draw();
			hidecursor();
			Input();
			Logic();
			

		}
		
	}
};

void startGame()
{
    char Continue;
    cout<<"\n\n\n\t\t\t\t      -->PONG GAME PROJECT!!<--  "<<endl;
    cout<<"\n\t\t\t\t\tMADE BY: GROUP ONE  "<<endl;
	cout<<"\n\t\t\t\t\t SECTION: ONE "<<endl;
    cout<<"\n\n\n\t\t\t\tGROUP MEMBERS         "<<"ID No."<<endl;
	cout<<"\t\t\t\t-------------------------------\n";
    cout<<"\t\t\t\tABEL HAILEMICAHEL   "<<"UGR/2301/12"<<endl;
    cout<<"\t\t\t\tBLEN ASSEFA         "<<"UGR/9640/12"<<endl;
    cout<<"\t\t\t\tELENA GIRMA         "<<"UGR/6368/12"<<endl;
    cout<<"\t\t\t\tELENI ZEWDU         "<<"UGR/7764/12"<<endl;
    cout<<"\nplease enter any key to continue: ";
    cin>>Continue;
	system("cls");
	mainMenu();
	


}
void mainMenu(){

   int mainChoice;
    cout<<"\n\n\n\t\t\t\t-------------------------------------------------------  "<<endl;
	cout<<"\t\t\t\t-------------------------------------------------------  "<<endl;
	cout<<"\t\t\t\t----------------------CONSOLE PONG GAME----------------  "<<endl;
	cout<<"\t\t\t\t-------------------------------------------------------  "<<endl;
	cout<<"\t\t\t\t-------------------------------------------------------  "<<endl;
 
    cout<<"\n\n\n\t\t\t\tMAIN MENU"<<endl;
    cout<<"\t\t\t\t1.START NEW GAME"<<endl;
    cout<<"\t\t\t\t2.HIGH SCORE"<<endl;
    cout<<"\t\t\t\t3.EXIT THE PROGRAM"<<endl;
    cout<<"\n\n\nplease enter any key to continue: ";
    cin>>mainChoice;
	switch(mainChoice){
       case 1:{
	   	choice();
		cGameManger c(35, 17);
		c.Run();
		break;}
	   case 2:
	     highScore();
		 mainMenu();
		 break;
		case 3: 
		break;
		default:
		cout<< "you entered incorrect choice closing..."<<endl;
		break;}
	
	
}
void highScore(){
	system("cls");
    fstream highScore;
	fstream highScoreN;
	int high;

    highScore.open("highScore.txt",ios::in);//open to read high score
	highScoreN.open("highScoreN.txt",ios::in);//open to read the name
 		
	 cout<<"\n\n\n\t\t\t\t-------------------------------------------------------  "<<endl;
	cout<<"\t\t\t\t-------------------------------------------------------  "<<endl;
	cout<<"\t\t\t\t--------------HIGH SCORE OF THE GAME----------------  "<<endl;
	cout<<"\t\t\t\t-------------------------------------------------------  "<<endl;
	cout<<"\t\t\t\t-------------------------------------------------------  "<<endl;
		    cout<<"\n\n\n NAME  SCORE "<<endl;
    if(highScoreN.is_open()){
        string line;
       
        while(getline(highScoreN,line)){ 
            cout<<" "<<line<<"  ";
        }
        highScoreN.close();
    }
	if(highScore.is_open()){
        string line;
        while(getline(highScore,line)){ 
            cout<<line<<endl;
        }
        highScore.close();
    }
	cout<<"\n\n\n\t\t\t please enter any key to exit the program"<<endl;
	cin>>high;
	system("cls");
	
	}
void choice()
{   
	string s[] = {"first", "second"};
	for(int i = 0; i<2; i++)
	{
		cout<<"Enter "<< s[i] <<" player name: ";
		cin>>name[i];
		cout<< "choose color        1 for ";
		setConsoleColour(FOREGROUND_GREEN);
		cout<<"\xDB\xDB";
		setConsoleColour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		cout<<"        2 for ";
		setConsoleColour(FOREGROUND_RED);
		cout<<"\xDB\xDB";
		setConsoleColour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		cout<<"        3 for ";
		setConsoleColour(FOREGROUND_BLUE);
		cout<<"\xDB\xDB\n";
		setConsoleColour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		cin>>color[i];
	
	}
	system("cls");

}
void setConsoleColour(unsigned short colour)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    SetConsoleTextAttribute(hOut, colour);
}
void ClearScreen()
{
COORD cursorPosition;	cursorPosition.X = 0;	cursorPosition.Y = 0;	
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}
void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoXY(int x, int y)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD CursorPosition;

	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console,CursorPosition);
}

