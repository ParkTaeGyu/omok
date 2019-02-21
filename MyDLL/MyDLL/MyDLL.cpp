// MyDLL.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
using namespace std;

#define EXPORT extern "C" __declspec(dllexport)
#define BLACK_COLOR 1
#define WHITE_COLOR 0
#define MAXXY	16		//	가로 세로 너비
int board[MAXXY][MAXXY];
int w[MAXXY][MAXXY];

int dx[] = { 0, 0, -1, -1, -1, 1, 1, 1 };
int dy[] = { -1, 1, 0, -1, 1, 0, -1, 1 };

typedef struct coord
{
	int x;
	int y;
} tk_code;

// DLL을 로드한 곳에서 EXPORT한 함수명을 쓸수있게 함

EXPORT void OmokAI(int *NewX, int *NewY, int mc, int CurTurn, int **CurBoard);
EXPORT int sero_up(int board[16][16], int x, int y, int _color);
EXPORT int sero_down(int board[16][16], int x, int y, int _color);
EXPORT int garo_right(int board[16][16], int x, int y, int _color);
EXPORT int garo_left(int board[16][16], int x, int y, int _color);
EXPORT int degak_right_down(int board[16][16], int x, int y, int _color);
EXPORT void setJWeight(int c);
EXPORT tk_code aiturn(int start);

BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved){

return TRUE;

}
EXPORT bool exc(int x, int y) {
	return (x >= 0 && x < 16 && y >= 0 && y < 16) && (board[x][y] == -1);
}
//실제 사용
EXPORT void OmokAI(int *NewX, int *NewY, int mc, int CurTurn, int **CurBoard)
{
	int ec = mc == WHITE_COLOR ? BLACK_COLOR : WHITE_COLOR;
	int temp[16][16];
	memcpy(temp, CurBoard, sizeof(temp));

	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 16; j++)
			board[j][i] = temp[i][j];

	if (CurTurn == 0) { *NewX = 7, *NewY = 8; return; }

	tk_code result = aiturn(CurTurn);

	*NewX = result.x;
	*NewY = result.y;

	return;
}

EXPORT bool isBlocked(int x, int y) {

	if (board[x][y] != -1) return true;
	else return false;

}

EXPORT void setWeight(int x, int y, int weight) {
	w[x][y] = 0;
	for (int i = 0; i < 8; i++) {
		if (exc(x + dx[i], y + dy[i])) w[x + dx[i]][y + dy[i]] += weight;

	}

}
EXPORT void setHWeight(int c) {

	int cnt = 0;
	for (int x = 0; x < 15; x++) {

		for (int y = 0; y < 15; y++) {
			// 상하 가중치 계산
			if (board[y][x] == c) {
				int starty = y;
				int tempx = x;
				int tempy = y;

				while (board[tempy + 1][tempx] == c) {
					tempy++;
					cnt++;
				}

				if (cnt == 1) {

					if (!isBlocked(tempy + 1, tempx) && !isBlocked(starty - 1, tempx)) {

						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx)) w[tempy + 1][tempx] += 70;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, tempx)) w[starty - 1][tempx] += 70;
					}
					else {

						if (exc(tempy + 1, tempx)) w[tempy + 1][tempx] += 2;
						if (exc(starty - 1, tempx)) w[starty - 1][tempx] += 2;

					}

				}
				else if (cnt == 2) {



					if (board[tempy + 1][tempx] == -1 && board[starty - 1][tempx] == -1) {

						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx)) w[tempy + 1][tempx] += 5000;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, tempx)) w[starty - 1][tempx] += 5000;

					}
					else
					{
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx)) w[tempy + 1][tempx] += 70;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, tempx)) w[starty - 1][tempx] += 70;
					}

				}
				else if (cnt >= 3) {

					


						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx)) w[tempy + 1][tempx] += 99999;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, tempx)) w[starty - 1][tempx] += 99999;

					
				

					
				}

			}
			cnt = 0;
		}
	}


}

EXPORT void setRDWeight(int c) {


	int cnt = 0;
	for (int x = 0; x < 15; x++) {

		for (int y = 0; y < 15; y++) {
		
			//우대각 가중치 계산
			if (board[y][x] == c) {
				int startx = x;
				int starty = y;
				int tempx = x;
				int tempy = y;

				while (board[tempy + 1][tempx + 1] == c) {
					tempy++;
					tempx++;
					cnt++;
				}

				if (cnt == 1) {
					
					if (!isBlocked(tempy + 1, tempx + 1) && !isBlocked(starty - 1, startx - 1)) {

						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx + 1)) w[tempy + 1][tempx + 1] += 70;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx - 1)) w[starty - 1][startx - 1] += 70;

					}
					else {

						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx + 1)) w[tempy + 1][tempx + 1] += 2;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx - 1)) w[starty - 1][startx - 1] += 2;


					}

					
				}
				else if (cnt == 2) {


					if (board[tempy + 1][tempx+1] == -1 && board[starty - 1][startx-1] == -1) {

						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx + 1)) w[tempy + 1][tempx + 1] += 5000;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx - 1)) w[starty - 1][startx - 1] += 5000;

					}
					else
					{
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx + 1)) w[tempy + 1][tempx + 1] += 70;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx - 1)) w[starty - 1][startx - 1] += 70;
					}


					
				}
				else if (cnt >= 3) {

					

						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx + 1)) w[tempy + 1][tempx + 1] += 99999;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx - 1)) w[starty - 1][startx - 1] += 99999;

						


				
				}

			}

			cnt = 0;
		}
	}
}

EXPORT void setLDWeight(int c) {
	int cnt = 0;
	for (int x = 0; x < 15; x++) {

		for (int y = 0; y < 15; y++) {

			//좌대각 가중치 계산
			if (board[y][x] == c) {
				int startx = x;
				int starty = y;
				int tempx = x;
				int tempy = y;

				while (board[tempy + 1][tempx - 1] == c) {
					tempx--;
					tempy++;
					cnt++;
				}

				if (cnt == 1) {

					if (!isBlocked(tempy + 1, tempx - 1) && !isBlocked(starty - 1, startx + 1)) {


						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx - 1)) w[tempy + 1][tempx - 1] += 70;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx + 1)) w[starty - 1][startx + 1] += 70;
					}
					else {


						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempy + 1, tempx - 1)) w[tempy + 1][tempx - 1] += 2;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx + 1)) w[starty - 1][startx + 1] += 2;

					}
					
					
					
				}
				else if (cnt == 2) {

					if (board[tempy + 1][tempx - 1] == -1 && board[starty - 1][startx + 1] == -1) {

						if (exc(tempy + 1, tempx - 1)) w[tempy + 1][tempx - 1] += 5000;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx + 1)) w[starty - 1][startx + 1] += 5000;

					}
					else
					{

						if (exc(tempy + 1, tempx - 1)) w[tempy + 1][tempx - 1] += 70;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx + 1)) w[starty - 1][startx + 1] += 70;
					}



				}
				else if (cnt >= 3) {

						if (exc(tempy + 1, tempx - 1)) w[tempy + 1][tempx - 1] += 99999;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(starty - 1, startx + 1)) w[starty - 1][startx + 1] += 99999;

									
				}

			}

			cnt = 0;
		}
	}

}
EXPORT void setJWeight(int c) {

	int cnt = 0;
	for (int x = 0; x < 15; x++) {

		for (int y = 0; y < 15;y++) {


			//좌우 가중치 계산
			if (board[x][y] == c) {
				int starty = y;
				int tempx = x;
				int tempy = y;

				while (board[tempx][tempy+1] == c) {
					tempy++;
					cnt++;
				}

				if (cnt == 1) {

					if (!isBlocked(tempx, starty - 1) && !isBlocked(tempx, starty - 1)) {

						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempx, tempy + 1)) w[tempx][tempy + 1] += 70;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempx, starty - 1)) w[tempx][starty - 1] = 70;

					}
					else {

						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempx, tempy + 1)) w[tempx][tempy + 1] += 2;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempx, starty - 1)) w[tempx][starty - 1] = 2;


					}
				}
				else if (cnt == 2) {


					if (board[tempx][tempy+1] == -1 && board[tempx][starty - 1] == -1) {

						if (exc(tempx, tempy + 1)) w[tempx][tempy + 1] += 5000;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempx, starty - 1)) w[tempx][starty - 1] = 5000;

					}
					else
					{

						if (exc(tempx, tempy + 1)) w[tempx][tempy + 1] += 70;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempx, starty - 1)) w[tempx][starty - 1] = 70;
					}

				}
				else if (cnt >= 3) {

					

						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempx, tempy + 1)) w[tempx][tempy + 1] += 99999;
						//경계를 넘지 않았을 경우+돌이 안놓여져있는경우
						if (exc(tempx, starty - 1)) w[tempx][starty - 1] = 99999;
					

				}
				
				cnt = 0;

				
				//막혀져있다면 /2로 나누기

			}

		}


	}

}
EXPORT bool SamSamChk(int x, int y,int c) {
	
	int dedakCnt = 0;
	int jikCnt = 0;
	int chkx = x;
	int chky = y;

	int cnt = 0;
	int d1 = 0;
	int d2 = 0;
	int j1 = 0;
	int j2 = 0;

	
	//우대각 체크
	
	while (exc(chkx - 1, chky - 1) && board[chkx-1][chky-1]==c) {
		chkx--;
		chky--;		
		d1++;
	}
	if (board[chkx - 1][chky - 1] == c) d1++;

	chkx = x;
	chky = y;

	while (exc(chkx + 1, chky + 1) && board[chkx+1][chky+1] == c) {
		chkx++;
		chky++;
		d1++;
	}
	if (board[chkx + 1][chky + 1] == c) d1++;
	chkx = x;
	chky = y;

	//좌대각 체크
	
	while (exc(chkx - 1, chky + 1) && board[chkx-1][chky+1] == c) {
		chkx--;
		chky++;
		d2++;
	}
	if (board[chkx - 1][chky + 1] == c) d2++;
	chkx = x;
	chky = y;

	
	while (exc(chkx + 1, chky - 1) && board[chkx+1][chky-1] == c) {
		chkx++;
		chky--;
		d2++;
	}
	if (board[chkx = 1][chky - 1] == c) d2++;
	chkx = x;
	chky = y;

	//좌우 체크
	while (exc(chkx,chky-1) && board[chkx][chky-1] == c) {
		chky--;
		j1++;
	}
	if (board[chkx][chky - 1] == c) j1++;
	chky = y;

	while (exc(chkx,chky+1) && board[chkx][chky+1] == c) {
		chky++;
		j1++;
	}
	if (board[chkx][chky + 1] == c) j1++;
	chky = y;

	//위아래 체크
	while (exc(chkx - 1, chky) && board[chkx - 1][chky] == c) {

		chkx--;
		j2++;
	}
	if (board[chkx - 1][chky] == c) j2++;
	chkx = x;

	while (exc(chkx + 1, chky) && board[chkx + 1][chky] == c) {

		chkx++;
		j2++;
	}
	if (board[chkx + 1][chky] == c) j2++;


	/*
	if (j2 == 2) cnt += 3;
	if (j1 == 2) cnt += 3;
	if (d1 == 2) cnt += 3;
	if (d2 == 2) cnt += 3;
	*/
	if (cnt >= 6) { return true; }
	else return false;
}
EXPORT tk_code aiturn(int turn)
{
	memset(w, -1, sizeof(w));
	int myColor;
	int eColor = BLACK_COLOR;
	myColor = ((turn % 2 == 0) ? BLACK_COLOR : WHITE_COLOR);
	if (myColor == BLACK_COLOR) { eColor = WHITE_COLOR; }

	tk_code coorde[324] = { 0, };
	

	for(int i=0;i<16;i++)
		for (int j = 0; j < 16; j++) {
			if (board[i][j] == eColor) {
				
				setWeight(i, j, 2);
			}
			else if (board[i][j] == myColor) {
				setWeight(i, j, 3);
			}
		}
	setJWeight(eColor);
	setHWeight(eColor);
	setRDWeight(eColor);
	setLDWeight(eColor);

	setJWeight(myColor);
	setHWeight(myColor);
	setRDWeight(myColor);
	setLDWeight(myColor);

	if (turn % 2 == 0) {}
	else if (turn % 2 == 1) {}
	int eval = 0;
	//가중치가 가장 큰 곳 찾기

	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			if (board[x][y] == -1 && eval < w[x][y])
			{
				eval = w[x][y];
			}
		}
	}


	int idx = 0;
	// 같은 수를 놓지 않게 랜덤으로 바꿈 
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			if (board[x][y] == -1 && w[x][y] == eval && !SamSamChk(x,y,myColor))
			{
				coorde[idx].x = y;
				coorde[idx].y = x;
				idx++;
			}
		}
	}
	int tmp = idx;
	int tmp2 = 0;
	int p;
	int distance = 999;

	//센터랑 제일 가깝거나 오목을 만들 수 있는 경우에 우선순위를 둠
	for (p = 0; p < idx; p++) {
		tmp2 = abs(7 - coorde[p].x) + abs(8 - coorde[p].y);
		if (distance < tmp2) { 
			distance = tmp2;
		}
	}
	
	

	return coorde[rand()%idx];
}

