#include<stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#define NONE 0
#define BLACK 1
#define WHITE -1
#define PLAYER1 0
#define PLAYER2 1
#define boolean int
#define true 1
#define false 0
typedef int(*FUNC)(int[8][8],int);
void wait(int);
void drawboard(int [8][8]);
void setboard(int[8][8],int,int,int);
void boardreset(int [8][8]);
void printplayercolor(int);
int getNumCode(char[2]);
boolean isputed(int[8][8],int,int);
void setAndUpdate(int[8][8],int,int,int);
void setReviece(int[8][8],int,int);
int getPuted(int[8][8],int,int);
boolean canPut(int[8][8],int,int,int);
boolean canPutAny(int[8][8],int);
void endGamePrint(int[8][8]);
int getColorByPlayer(int);
int main(int argc,char *argv[]){

	int bord[8][8];
	int tmpbord[8][8];
	int turn = PLAYER1;
	int selectpos;
	boolean skiped = false;
	boolean isComputer1 = false;
	boolean isComputer2 = false;
	int select_x,select_y;
	char inputstr[2];
	boardreset(bord);
	FUNC lpFunc1 = NULL;
	FUNC lpFunc2 = NULL;
	if(argc!=1){
		if(strcmp(argv[1],"-p")!=0){
			isComputer1 = true;
			HMODULE hModule1 = LoadLibrary(argv[1]);
			if(hModule1 == NULL){
				printf("dllファイルの読み込みに失敗しました。");
				return -1;
			}
			lpFunc1 = (FUNC)GetProcAddress(hModule1,"GetPutPoint");
			if(lpFunc1 == NULL){
				printf("関数のアドレス取得に失敗しました。");
				FreeLibrary(hModule1);
				return -1;
			}
		}
		if(argc>2){
			if(strcmp(argv[2],"-p")!=0){
				isComputer2 = true;
				HMODULE hModule2 = LoadLibrary(argv[2]);
				if(hModule2 == NULL){
					printf("dllファイルの読み込みに失敗しました。");
					return -1;
				}
				lpFunc2 = (FUNC)GetProcAddress(hModule2,"GetPutPoint");
				if(lpFunc2 == NULL){
					printf("関数のアドレス取得に失敗しました。");
					FreeLibrary(hModule2);
					return -1;
				}
			}
		}
	}
	system("cls");
	while(1){
		drawboard(bord);

		if(isAllPlaced(bord)){
			endGamePrint(bord);
			return 0;
		}
		printplayercolor(getColorByPlayer(turn));
		printf("のターンです。\n");
		if(canPutAny(bord,turn)==false){
			if(skiped==true){
				endGamePrint(bord);
				return 0;
			}
			printf("手詰まりです。パスします。\n");
			wait(4000000);
			turn = 1- turn;
			skiped = true;system("cls");
			continue;
		}
		skiped = false;
		if(turn!=PLAYER1&&isComputer1){
			for(int i=0;i<8;i++){
				memcpy(tmpbord[i],bord[i],sizeof(int)*8);
			}
			selectpos = (lpFunc1)(tmpbord,turn);
			printf("コンピュータの手(%d)\n",selectpos);
			wait(10000000);
		//	scanf("%d",&tmp);
		}else if(turn!=PLAYER2&&isComputer2){
			for(int i=0;i<8;i++){
				memcpy(tmpbord[i],bord[i],sizeof(int)*8);
			}
			selectpos = (lpFunc2)(tmpbord,turn);
			printf("コンピュータの手(%d)\n",selectpos);
			wait(10000000);
		}else{
			printf("手を入力してください(A1)-->");
			scanf("%s",inputstr);
			selectpos = getNumCode(inputstr);
		}
		select_x = selectpos/10;
		select_y = selectpos%10;
		system("cls");
		if(isputed(bord,select_x,select_y)==true){
			printf("そこはすでにおかれています\n");
				//printf("(%d,%d)\n",select_x,select_y);
			continue;
		}else{
			if(canPut(bord,select_x,select_y,turn)==false){
				printf("そこにおくことはできません。\n");
				continue;
			}
			setAndUpdate(bord,select_x,select_y,turn);
			turn = 1 - turn;
		}
	}
}
void wait(int waittime){
	for(int i=0;i<waittime;i++){}
}
int getColorByPlayer(int turn){
	if(turn==PLAYER1){
		return BLACK;
	}else{
		return WHITE;
	}daewad
	int i;
}
void endGamePrint(int board[8][8]){
	int p1p = 0;
	int p2p = 0;
	for(int y=0;y<8;y++){
		for(int x=0;x<8;x++){
			if(board[x][y]==BLACK){
				p1p++;
			}else if(board[x][y]==WHITE){
				p2p++;
			}
		}
	}
	if(p1p>p2p){
		printf("プレイヤー１の勝ちです　おめでとう\n");
	}else if(p1p<p2p){
		printf("プレイヤー２の勝ちです　おめでとう\n");
	}else{
		printf("引き分けです　そういうこともあるよ\n");
	}
	return;
}
boolean isAllPlaced(int board[8][8]){
	boolean ret = true;
	for(int y=1;y<=8;y++){
		for(int x=1;x<=8;x++){
			ret *=getPuted(board,x,y);
		}
	}
	return ret*ret;
}
boolean canPutAny(int board[8][8],int turn){
	boolean  canPutb = false;
	for(int y=1;y<=8;y++){
		for(int x=1;x<=8;x++){
			if(canPut(board,x,y,turn)==true){
				//printf("(%d,%d)に置くことが可能\n",x,y);
				return true;
			}
		}
	}
	return canPutb;
}
boolean canPut(int board[8][8],int x,int y,int turn){
	int mycolor = (1-turn)*2-1;
	boolean canplace = false;
	if(isputed(board,x,y)==true){
		return false;
	}
	for(int yp = -1;yp<=1;yp++){
		for(int xp = -1;xp<=1;xp++){
			if(xp==0&&yp==0){
				continue;
			}
			int cuposx = x+xp;
			int cuposy = y+yp;
			if(cuposx<=0||cuposy<=0||cuposx>8||cuposy>8){
				continue;
			}
			//printf("(%d,%d,%d)\n",xp,yp,getPuted(board,cuposx,cuposy));
			if(getPuted(board,cuposx,cuposy)==-mycolor){
				while(true){
					cuposx += xp;
					cuposy += yp;
					if(cuposx<=0||cuposy<=0||cuposx>8||cuposy>8){
						break;
					}
					if(getPuted(board,cuposx,cuposy)==mycolor){
						return true;
					}else if(getPuted(board,cuposx,cuposy)==-mycolor){
						continue;
					}else{
						break;
					}
				}
			}
		}
	}
	return canplace;
}
void setAndUpdate(int board[8][8],int x,int y,int turn){
	int mycolor = (1-turn)*2-1;
	setboard(board,x,y,mycolor);
	for(int yp = -1;yp<=1;yp++){
		for(int xp = -1;xp<=1;xp++){
			if(xp==0&&yp==0){
				continue;
			}
			int cuposx = x+xp;
			int cuposy = y+yp;
			int  pc = 1;

			if(cuposx<=0||cuposy<=0||cuposx>8||cuposy>8){
				continue;
			}
			if(getPuted(board,cuposx,cuposy)==-mycolor){
				while(true){
					cuposx += xp;
					cuposy += yp;
					//printf("(%d,%d)\n", cuposx,cuposy);
					if(cuposx<=0||cuposy<=0||cuposx>8||cuposy>8){
						break;
					}
					if(getPuted(board,cuposx,cuposy)==mycolor){
						for(int i=1;i<pc+1;i++){
							setReviece(board,cuposx-xp*i,cuposy-yp*i);
						}
						break;
					}else if(getPuted(board,cuposx,cuposy)==-mycolor){
						pc++;
						continue;
					}else{
						break;
					}
				}
			}
		}
	}
}
int getPuted(int board[8][8],int x,int y){
	return board[x-1][y-1];
}
void setReviece(int board[8][8],int x,int y){
	board[x-1][y-1]*=-1;
}
int getNumCode(char inc[2]){
	return (inc[0]-'A'+1)*10+(inc[1]-'1'+1);
}
boolean isputed(int board[8][8],int x,int y){
	return board[x-1][y-1]!=NONE ? true : false;
}
void setboard(int board[8][8],int x,int y,int color){
	board[x-1][y-1] = color;
}

void drawboard(int bord[8][8]){
	printf(" | ");
	for(int x=0;x<8;x++)
		printf("%c| ",65+x);
	printf("\n");
	for(int y=0;y<8;y++){
		printf("%d|",y+1);
		for(int x=0;x<8;x++){
			printplayercolor(bord[x][y]);
			printf("|");
		}
		printf("\n");
	}
}
void boardreset(int bord[8][8]){
	memset(bord,0,sizeof(int[8][8]));
	for(int i=0;i<8*8;i++){
		bord[i/8][i%8] = NONE;
	}
	setboard(bord,4,4,WHITE);
	setboard(bord,5,4,BLACK);
	setboard(bord,4,5,BLACK);
	setboard(bord,5,5,WHITE);
}
void printplayercolor(int p){
	switch(p){
		case WHITE:
		printf("●");
		break;
		case BLACK:
		printf("◯");
		break;
		case NONE:
		printf("・");
		break;
		default:
		printf("エラーが発生しますた");
	}
}