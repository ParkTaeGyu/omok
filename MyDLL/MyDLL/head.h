
#define BUFSIZE 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <Windows.h>


typedef struct coord
{
	int x;
	int y;
} MY_COORD;

typedef struct MY_NODE
{
	MY_COORD coord;
	int index;
	struct MY_NODE *prev_node;
	struct MY_NODE *next_node;
} MY_NODE;



