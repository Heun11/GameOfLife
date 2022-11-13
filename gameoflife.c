#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h> 

#define COLORTYPE 0

#if COLORTYPE==0
	#define O "\x1B[37m"
	#define X "\x1B[31m"
	#define R "\x1B[0m"
#elif COLORTYPE==1
	#define X "\x1b[48;2;255;255;255m\x1b[38;2;255;255;255m"
	#define O "\x1b[48;2;20;20;20m\x1b[38;2;20;20;20m"
	#define R "\x1B[0m"
#endif

void printBoard(void *void_board, int WIDTH, int HEIGHT)
{
	char (*board)[HEIGHT][WIDTH] = (char(*)[HEIGHT][WIDTH]) void_board;
	for(int i=0;i<HEIGHT;i++) {
		for(int j=0;j<WIDTH;j++) {
			if((*board)[i][j]=='X') {
				printf(X"X "R);
			}
			else {
				printf(O"O "R);
			}
		}
		printf("\n");
	}
}

int countN(void *void_board, int WIDTH, int HEIGHT, int i, int j)
{
	char (*board)[HEIGHT][WIDTH] = (char(*)[HEIGHT][WIDTH]) void_board;
	int c = 0;
	int ni, nj, neighbor;
	for(int xoff=-1;xoff<=1;xoff++) {
		for(int yoff=-1;yoff<=1;yoff++) {
			if(xoff==0&&yoff==0) {
				continue;
			}
			else {
				ni=i+xoff;
				nj=j+yoff;
				if(i>-1&&i<HEIGHT&&j>-1&&j<WIDTH) {
					neighbor = (*board)[ni][nj];
					if(neighbor=='X') {
						c++;
					}
				}
			}
		}
	}
	return c;
}

void updateBoard(void *void_board, int WIDTH, int HEIGHT)
{
	char (*board)[HEIGHT][WIDTH] = (char(*)[HEIGHT][WIDTH]) void_board;
	char* newBoard = (char*)malloc((HEIGHT*WIDTH)*sizeof(char));
	
	for(int i=0;i<HEIGHT;i++) {
		for(int j=0;j<WIDTH;j++) {
			int n=countN(board, WIDTH, HEIGHT, i, j);
			if((*board)[i][j]=='X') {
				if(n>3) {
					newBoard[i*WIDTH+j]='O';
				}
				else if(n==2||n==3) {
					newBoard[i*WIDTH+j]='X';
				}
				else {
					newBoard[i*WIDTH+j]='O';
				}
			}
			else {
				if(n==3) {
					newBoard[i*WIDTH+j]='X';
				}
				else {
					newBoard[i*WIDTH+j]='O';
				}
			}
		}
	}
	for(int i=0;i<HEIGHT;i++) {
		for(int j=0;j<WIDTH;j++) {
			(*board)[i][j]=newBoard[i*WIDTH+j];
		}
	}
	free(newBoard);
}

int main()
{
	srand(time(NULL));
	int type;
	
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	int WIDTH = w.ws_col/2;
	int HEIGHT = w.ws_row;

	char board[HEIGHT][WIDTH];
	for(int i=0;i<HEIGHT;i++) {
		for(int j=0;j<WIDTH;j++) {
			type = rand()%2;
			if(type==0) {
				board[i][j] = 'O';
			}
			else {
				board[i][j] = 'X';
			}
		}
	}

	while(1) {
		system("clear");
		updateBoard(board, WIDTH, HEIGHT);
		printBoard(board, WIDTH, HEIGHT);
		usleep(500000);
	}
	return 0;
}
