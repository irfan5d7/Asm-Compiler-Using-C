#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int str_cmp(char *s1, char *s2);
char** get_opcodes();
int *get_regisindx(char* regis);

typedef struct symbol_table symbol_table;
typedef struct block_addr block_addr;
typedef struct intermediate_code intermediate_code;
struct symbol_table
{
	char name[16];
	int start_addr;
	int size;
	int isConst;
};

char* mem = (char*)malloc(sizeof(char) * 100);

struct block_addr
{
	char name[16];
	int address;
};

struct intermediate_code
{
	int instruction_no;
	int opcode;
	int parameters[6];
};


typedef struct if_else
{
	int top;
	int *address;
};

typedef struct lable_table
{
	int top;
	char** blockname;
	int *address;
};


char** get_opcodes()
{
	int i, j, k;
	FILE *fp = fopen("opcodes.txt", "r");
	char* buff = (char*)malloc(sizeof(char) * 512);
	fscanf(fp, "%[^\0]s", buff);
	fclose(fp);
	char** opcodes = (char**)malloc(sizeof(char*) * 15);
	for (i = 0; i < 15; i++)
	{
		opcodes[i] = (char*)malloc(sizeof(char) * 16);
	}
	i = j = k = 0;
	while (i < 15)
	{
		j = 0;
		while (buff[k] != '\n' && buff[k] != '\0')
		{
			opcodes[i][j++] = buff[k++];
		}
		opcodes[i][j] = '\0';
		if (buff[k] != '\0')
			k++;

		i++;
	}
	return opcodes;
}



int str_cmp(char *s1, char *s2)
{
	int i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return 0;
		i++;
	}
	if ((s1[i] == '\0' && s2[i] == '\0') || ((s1[i] == ' ' && s2[i] == ' ')))
		return 1;
	else
		return 0;
}


int get_opcodeindx(char** opcode, char* operation)
{
	for (int i = 0; i < 15; i++)
	{
		if (str_cmp(opcode[i], operation))
		{
			return i + 1;
		}
	}
}

int get_regis_indx(char* regis)
{
	if (str_cmp("AX", regis))
		return 0;
	else if (str_cmp("BX", regis))
		return 1;
	else if (str_cmp("CX", regis))
		return 2;
	else if (str_cmp("DX", regis))
		return 3;
	else if (str_cmp("EX", regis))
		return 4;
	else if (str_cmp("FX", regis))
		return 5;
	else if (str_cmp("GX", regis))
		return 6;
	else if (str_cmp("HX", regis))
		return 7;
	else
		return -1;
}
