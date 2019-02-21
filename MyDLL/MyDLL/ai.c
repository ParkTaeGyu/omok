#include "head.h"

#define BOARDSIZE 18
#define BLACK_COLOR 1
#define WHITE_COLOR 2



MY_COORD aiturn(int board[18][18],int start)
{

	int ai_board[18][18] = { 0, };
	int eval = 0;
	int num_of_coord = 0;

	MY_COORD coord[324] = { 9999, };
	srand(time(NULL));
	int randnum = 0;
	
	//컴퓨터가 흑일때
	if (start == 1)
	{
		for (int i = 0; i < 18; i++)
		{
			for (int j = 0; j < 18; j++)
			{
				if (board[i][j] == 1)
				{	
					randnum = rand() % 8 + 1;

					if (randnum == 1)
					{
						ai_board[i][j + 1] = 999;
					}
					else if (randnum == 2)
					{
						ai_board[i][j - 1] = 999;
					}
					else if (randnum == 3)
					{
						ai_board[i + 1][j + 1] = 999;
					}
					else if (randnum == 4)
					{
						ai_board[i + 1][j] = 999;
					}
					else if (randnum == 5)
					{
						ai_board[i + 1][j - 1] = 999;
					}
					else if (randnum == 6)
					{
						ai_board[i - 1][j] = 999;
					}
					else if (randnum == 7)
					{
						ai_board[i - 1][j - 1] = 999;
					}
					else if (randnum == 8)
					{
						ai_board[i - 1][j + 1] = 999;
					}
				}
			}
		}
	}   
	//가중치 두기
	else
	{
		//각각 경우의 수
		for (int x = 0; x < 18; x++)
		{
			for (int y = 0; y < 18; y++)
			{
				if (board[x][y] == 0)
				{
					//////////////////수비//////////////
					#pragma region 세칸 막기(두개일때막음)
					// 3칸되는 것 막기
					//위아래
					if (sero_up(board, x, y, BLACK_COLOR) + sero_down(board, x, y, BLACK_COLOR) >= 2)
					{
						ai_board[x][y] += 60;
					}
					//좌우
					if (garo_right(board, x, y, BLACK_COLOR) + garo_left(board, x, y, BLACK_COLOR) >= 2)
					{
						ai_board[x][y] += 60;
					}
					//대각선 오른쪽 아래 왼쪽 위
					if (degak_right_down(board, x, y, BLACK_COLOR) + degak_left_up(board, x, y, BLACK_COLOR) >= 2)
					{
						ai_board[x][y] += 60;
					}
					//대각선 왼쪽아래 오른쪽 위
					if (degak_left_down(board, x, y, BLACK_COLOR) + degak_right_up(board, x, y, BLACK_COLOR) >= 2)
					{
						ai_board[x][y] += 60;
					}
					#pragma endregion

					#pragma region 네칸 막기(세개일때막음)
					// 4칸되는 것 막기
					//위아래
					if (sero_up(board, x, y, BLACK_COLOR) + sero_down(board, x, y, BLACK_COLOR) >= 3)
					{
						if (board[x - sero_up(board, x, y, WHITE_COLOR)][y] == 0 && board[x + sero_down(board, x, y, WHITE_COLOR)][y] == 0)
						{
							ai_board[x][y] += 15000;
						}
						ai_board[x][y] += 65;
					}
					//좌우
					if (garo_right(board, x, y, BLACK_COLOR) + garo_left(board, x, y, BLACK_COLOR) >= 3)
					{
						if (board[x][y - garo_left(board, x, y, WHITE_COLOR)] == 0 && board[x][y + garo_right(board, x, y, WHITE_COLOR)] == 0)
						{
							ai_board[x][y] += 15000;
						}
						ai_board[x][y] += 65;
					}
					//대각선 오른쪽 아래 왼쪽 위
					if (degak_right_down(board, x, y, BLACK_COLOR) + degak_left_up(board, x, y, BLACK_COLOR) >= 3)
					{
						if (board[x - degak_left_up(board, x, y, WHITE_COLOR)][y - degak_left_up(board, x, y, WHITE_COLOR)] == 0 && board[x + degak_right_down(board, x, y, WHITE_COLOR)][y + degak_right_down(board, x, y, WHITE_COLOR)] == 0)
						{
							ai_board[x][y] += 15000;
						}
						ai_board[x][y] += 65;
					}
					//대각선 왼쪽아래 오른쪽 위
					if (degak_left_down(board, x, y, BLACK_COLOR) + degak_right_up(board, x, y, BLACK_COLOR) >= 3)
					{
						if (board[x - degak_right_up(board, x, y, WHITE_COLOR)][y + degak_right_up(board, x, y, WHITE_COLOR)] == 0 && board[x + degak_left_down(board, x, y, WHITE_COLOR)][y - degak_left_down(board, x, y, WHITE_COLOR)] == 0)
						{
							ai_board[x][y] += 15000;
						}
						ai_board[x][y] += 65;
					}
#pragma endregion

					#pragma region 다섯칸 막기(네개일때막음)
					//위아래
					if (sero_up(board, x, y, BLACK_COLOR) + sero_down(board, x, y, BLACK_COLOR) >= 4)
					{
						ai_board[x][y] += 50000;
					}
					//좌우
					if (garo_right(board, x, y, BLACK_COLOR) + garo_left(board, x, y, BLACK_COLOR) >= 4)
					{
						ai_board[x][y] += 50000;
					}
					//대각선 오른쪽 아래 왼쪽 위
					if (degak_right_down(board, x, y, BLACK_COLOR) + degak_left_up(board, x, y, BLACK_COLOR) >= 4)
					{
						ai_board[x][y] += 50000;
					}
					//대각선 왼쪽아래 오른쪽 위
					if (degak_left_down(board, x, y, BLACK_COLOR) + degak_right_up(board, x, y, BLACK_COLOR) >= 4)
					{
						ai_board[x][y] += 50000;
					}
#pragma endregion

					///////////////////공격//////////////
					#pragma region 두칸 공격(두개만듬)
					//위아래
					if (sero_up(board, x, y, WHITE_COLOR) + sero_down(board, x, y, WHITE_COLOR) >= 1)
					{
						ai_board[x][y] += 10;
					}
					//좌우
					if (garo_right(board, x, y, WHITE_COLOR) + garo_left(board, x, y, WHITE_COLOR) >= 1)
					{
						ai_board[x][y] += 10;
					}
					//대각선 오른쪽 아래 왼쪽 위
					if (degak_right_down(board, x, y, WHITE_COLOR) + degak_left_up(board, x, y, WHITE_COLOR) >= 1)
					{
						ai_board[x][y] += 10;
					}
					//대각선 왼쪽아래 오른쪽 위
					if (degak_left_down(board, x, y, WHITE_COLOR) + degak_right_up(board, x, y, WHITE_COLOR) >= 1)
					{
						ai_board[x][y] += 10;
					}
					#pragma endregion

					#pragma region 세칸 공격(세개만듬)
					//위아래
					if (sero_up(board, x, y, WHITE_COLOR) + sero_down(board, x, y, WHITE_COLOR) >= 2)
					{
						//비어있는 세칸 만들기
						if (board[x - sero_up(board, x, y, WHITE_COLOR)][y] != BLACK_COLOR && board[x + sero_down(board, x, y, WHITE_COLOR)][y] != BLACK_COLOR)
						{
							ai_board[x][y] += 160;
						}
						ai_board[x][y] += 200;
					}
					//좌우
					if (garo_right(board, x, y, WHITE_COLOR) + garo_left(board, x, y, WHITE_COLOR) >= 2)
					{
						if (board[x][y - garo_left(board, x, y, WHITE_COLOR)] != BLACK_COLOR && board[x][y + garo_right(board, x, y, WHITE_COLOR)] != BLACK_COLOR)
						{
							ai_board[x][y] += 160;
						}
						ai_board[x][y] += 200;
					}
					//대각선 오른쪽 아래 왼쪽 위
					if (degak_right_down(board, x, y, WHITE_COLOR) + degak_left_up(board, x, y, WHITE_COLOR) >= 2)
					{
						if (board[x - degak_left_up(board, x, y, WHITE_COLOR)][y - degak_left_up(board, x, y, WHITE_COLOR)] != BLACK_COLOR && board[x + degak_right_down(board, x, y, WHITE_COLOR)][y + degak_right_down(board, x, y, WHITE_COLOR)] != BLACK_COLOR)
						{
							ai_board[x][y] += 160;
						}
						ai_board[x][y] += 200;
					}
					//대각선 왼쪽아래 오른쪽 위
					if (degak_left_down(board, x, y, WHITE_COLOR) + degak_right_up(board, x, y, WHITE_COLOR) >= 2)
					{
						if (board[x - degak_right_up(board, x, y, WHITE_COLOR)][y + degak_right_up(board, x, y, WHITE_COLOR)] != BLACK_COLOR && board[x + degak_left_down(board, x, y, WHITE_COLOR)][y - degak_left_down(board, x, y, WHITE_COLOR)] != BLACK_COLOR)
						{
							ai_board[x][y] += 160;
						}
						ai_board[x][y] += 200;
					}
#pragma endregion

					#pragma region 네칸 공격(네개만듬)
					//위아래
					if (sero_up(board, x, y, WHITE_COLOR) + sero_down(board, x, y, WHITE_COLOR) >= 3)
					{
						//비어있는 네칸 만들기
						if (board[x - sero_up(board, x, y, WHITE_COLOR)][y] != BLACK_COLOR && board[x + sero_down(board, x, y, WHITE_COLOR)][y] != BLACK_COLOR)
						{
							ai_board[x][y] += 30000;
						}
						ai_board[x][y] += 950;
					}
					//좌우
					if (garo_right(board, x, y, WHITE_COLOR) + garo_left(board, x, y, WHITE_COLOR) >= 3)
					{
						if (board[x][y - garo_left(board, x, y, WHITE_COLOR)] != BLACK_COLOR && board[x][y + garo_right(board, x, y, WHITE_COLOR)] != BLACK_COLOR)
						{
							ai_board[x][y] += 30000;
						}
						ai_board[x][y] += 950;
					}
					//대각선 오른쪽 아래 왼쪽 위
					if (degak_right_down(board, x, y, WHITE_COLOR) + degak_left_up(board, x, y, WHITE_COLOR) >= 3)
					{
						if (board[x - degak_left_up(board, x, y, WHITE_COLOR)][y - degak_left_up(board, x, y, WHITE_COLOR)] != BLACK_COLOR && board[x + degak_right_down(board, x, y, WHITE_COLOR)][y + degak_right_down(board, x, y, WHITE_COLOR)] != BLACK_COLOR)
						{
							ai_board[x][y] += 30000;
						}
						ai_board[x][y] += 950;
					}
					//대각선 왼쪽아래 오른쪽 위
					if (degak_left_down(board, x, y, WHITE_COLOR) + degak_right_up(board, x, y, WHITE_COLOR) >= 3)
					{
						if (board[x - degak_right_up(board, x, y, WHITE_COLOR)][y + degak_right_up(board, x, y, WHITE_COLOR)] != BLACK_COLOR && board[x + degak_left_down(board, x, y, WHITE_COLOR)][y - degak_left_down(board, x, y, WHITE_COLOR)] != BLACK_COLOR)
						{
							ai_board[x][y] += 30000;
						}

						ai_board[x][y] += 950;
					}
#pragma endregion

					#pragma region 다섯칸 공격(다섯개만듬)
										//위아래
										if (sero_up(board, x, y, WHITE_COLOR) + sero_down(board, x, y, WHITE_COLOR) >= 4)
										{
											ai_board[x][y] += 99999;
										}
										//좌우
										if (garo_right(board, x, y, WHITE_COLOR) + garo_left(board, x, y, WHITE_COLOR) >= 4)
										{
											ai_board[x][y] += 99999;
										}
										//대각선 오른쪽 아래 왼쪽 위
										if (degak_right_down(board, x, y, WHITE_COLOR) + degak_left_up(board, x, y, WHITE_COLOR) >= 4)
										{
											ai_board[x][y] += 99999;
										}
										//대각선 왼쪽아래 오른쪽 위
										if (degak_left_down(board, x, y, WHITE_COLOR) + degak_right_up(board, x, y, WHITE_COLOR) >= 4)
										{
											ai_board[x][y] += 99999;
										}
					#pragma endregion
				}
			}
		}
	}

	//가중치가 가장 큰 곳 찾기
	for (int x = 0; x < 18; x++)
	{
		for (int y = 0; y < 18; y++)
		{
			if (board[x][y] == 0 && eval < ai_board[x][y])
			{
				eval = ai_board[x][y];
			}
		}
	}

	// 같은 수를 놓지 않게 랜덤으로 바꿈 
	for (int x = 0; x < 18; x++)
	{
		for (int y = 0; y < 18; y++)
		{
			if (board[x][y] == 0 && ai_board[x][y] == eval)
			{
				coord[num_of_coord].x = x;
				coord[num_of_coord].y = y;
				num_of_coord++;
			}
		}
	}


	return coord[rand() % (num_of_coord)];
}


// 세로 위쪽 확인
int sero_up(int board[18][18], int x, int y, int _color) 
{
	int i, count;
	count = 0;

	for (i = x - 1; i >= 0; i--)
	{
		if (board[i][y] != _color) 
		{
			break;
		}
		else
		{
			count++;
		}
	}

	return count;
}

// 세로 아래쪽 확인
int sero_down(int board[18][18], int x, int y, int _color)
{
	int i, count;

	count = 0;

	
	for (i = x + 1; i < 18; i++) 
	{
		if (board[i][y] != _color) 
		{
			break;
		}
		else 
		{
			count++;
		}
	}

	return count;
}

//가로 오른쪽 확인
int garo_right(int board[18][18], int x, int y, int _color)
{
	int i, count;
	count = 0;

	for (i = y + 1; i <18 ; i++)
	{
		if (board[x][i] != _color)
		{
			break;
		}
		else
		{
			count++;
		}
	}

	return count;
}

//가로 왼쪽 확인
int garo_left(int board[18][18], int x, int y, int _color)
{
	int i, count;
	count = 0;

	for (i = y - 1; i >= 0; i--)
	{
		if (board[x][i] != _color)
		{
			break;
		}
		else
		{
			count++;
		}
	}

	return count;
}

//대각선 오른쪽 아래
int degak_right_down(int board[18][18], int x, int y, int _color)
{
	int i, j, count;
	count = 0;

	for ((i = x + 1, j = y + 1); (i < 18 && j < 18); (i++, j++)) 
	{
		if (board[i][j] != _color)
		{
			break;
		}
		else {
			count++;
		}
	}

	return count;
}

//대각선 왼쪽 위
int degak_left_up(int board[18][18], int x, int y, int _color)
{
	int i, j, count;
	count = 0;

	for ((i = x - 1, j = y - 1); (i >= 0 && j >= 0); (i--, j--)) 
	{
		if (board[i][j] != _color)
		{
			break;
		}
		else
		{
			count++;
		}
	}

	return count;
}

//대각선 오른쪽 위
int degak_right_up(int board[18][18], int x, int y, int _color)
{
	int i, j, count;
	count = 0;

	for ((i = x - 1, j = y + 1); (i >= 0 && j < 18); (i--, j++))
	{
		if (board[i][j] != _color)
		{
			break;
		}
		else
		{
			count++;
		}
	}

	return count;
}

//대각선 왼쪽 아래 
int degak_left_down(int board[18][18], int x, int y, int _color)
{
	int i, j, count;
	count = 0;

	for ((i = x + 1, j = y - 1); (i < 18 && j >= 0); (i++, j--))
	{
		if (board[i][j] != _color)
		{
			break;
		}
		else
		{
			count++;
		}
	}

	return count;
}


