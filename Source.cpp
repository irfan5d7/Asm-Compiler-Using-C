#include"Header.h"

char *regis_name(int n);
void generate_opcode(char**opcodes);
void run(symbol_table** symbols, intermediate_code** code_gen, char**opcodes, int s, int ins_no, void* memory);
void search(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);
void mov(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);
void movr(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);
void read(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);
void print(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);
void add(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);
void sub(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);
void mul(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);
void jmp(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);
void if_run(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins);

char* csv(char *query, int* i)
{
	int j = 0;
	char* tmp = (char*)malloc(sizeof(char) * 32);
	while (query[*i] != '\n' && query[*i] != ' ' && query[*i] != '\0' && query[*i] != '[' && query[*i] != ']'  && query[*i] != ',')
	{
		tmp[j++] = query[(*i)++];
	}
	tmp[j] = '\0';
	return tmp;
}



char* read_code(char* file_name)
{
	char *buff = (char*)malloc(sizeof(char) * 10240);
	FILE* fp = fopen(file_name, "r");
	fscanf(fp, "%[^\0]s", buff);
	fclose(fp);
	return buff;
}

int main()
{
	char** opcodes = get_opcodes();
	generate_opcode(opcodes);
}
void generate_opcode(char**opcodes)
{
	int i, j, s, mem_indx, ins_no,val,k;
	//char * file_name = (char*)malloc(sizeof(char) * 32);
	void *memory = malloc(1024);
	int offset;
	//printf("Enter File Name:  ");
	//scanf("%s", file_name);
	char *code = (char*)malloc(sizeof(char) * 64);
	char *codeop = (char*)malloc(sizeof(char) * 16);
	symbol_table** symbols = (symbol_table**)malloc(sizeof(symbol_table*) * 10);
	lable_table* lable_tab = (lable_table*)malloc(sizeof(lable_table));
	lable_tab->blockname = (char**)malloc(sizeof(char*) * 10);
	lable_tab->address = (int*)malloc(sizeof(lable_tab) * 10);
	for (i = 0; i < 10; i++)
	{
		lable_tab->blockname[i] = (char*)malloc(sizeof(char) * 16);
	}

	if_else* if_ele = (if_else*)malloc(sizeof(if_else));
	if_ele->address = (int*)malloc(sizeof(lable_tab) * 10);
	if_ele->top = -1;
	lable_tab->top = -1;
	for (i = 0; i < 10; i++)
	{
		symbols[i] = (symbol_table*)malloc(sizeof(symbol_table*));
	}
	s = mem_indx = 0;
	val = offset = 0;
	char *file_name = (char*)malloc(sizeof(char) * 16);
	printf("Enter File name :  ");
	fflush(stdin);
	gets(file_name);
	FILE* fp = fopen(file_name, "r");
	while (!feof(fp))
	{

		i = j = 0;
		fgets(code, 100, fp);
		codeop = csv(code, &i);
		i++;

		if (str_cmp(codeop, "DATA"))
		{
			codeop = csv(code, &i);
			symbols[s]->isConst = 0;
			symbols[s]->start_addr = mem_indx;
			symbols[s]->size = 1;
			strcpy(symbols[s]->name, codeop);
			if (code[i] != '\n')
			{
				i += 1;
				codeop = csv(code, &i);
				symbols[s]->size = atoi(codeop);
			}
			mem_indx += symbols[s++]->size;
		}
		else if (str_cmp(codeop, "CONST"))
			{
				codeop = csv(code, &i);
				symbols[s]->isConst = 1;
				symbols[s]->start_addr = mem_indx;
				strcpy(symbols[s]->name, codeop);
				i += 3;
				codeop = csv(code, &i);
				val = atoi(codeop);
				symbols[s++]->size = val;
				//memcpy(*(&memory + mem_indx), &val, sizeof(int)*1);
				offset += sizeof(char);
				mem_indx++;
			}
		else if (str_cmp(codeop, "START:"))
			break;
	}
	FILE* fpp = fopen("symbol_table.txt", "w");
	for (i = 0; i < s; i++)
	{
		fprintf(fpp, "%s\t%d\t%d\n", symbols[i]->name, symbols[i]->start_addr, symbols[i]->size, memory);
	}
	fclose(fpp);
	ins_no = 0;
	intermediate_code** code_gen = (intermediate_code**)malloc(sizeof(intermediate_code) * 50);
	for (i = 0; i < 50; i++)
	{
		code_gen[i] = (intermediate_code*)malloc(sizeof(intermediate_code));
	}
	int loop, indx, if_flag = 0;;
	while (!feof(fp))
	{
		i = 0;
		j = 1;
		fgets(code, 100, fp);
		while (code[i] == '\t')
			i++;
		codeop = csv(code, &i);
		if (str_cmp(codeop, "ELSE"))
		{
			code_gen[if_ele->address[if_ele->top--]]->parameters[4] = ins_no + 2;
			code_gen[ins_no]->instruction_no = ins_no + 1;
			code_gen[ins_no]->opcode = indx = get_opcodeindx(opcodes, "JMP");
			if_ele->address[++if_ele->top] = ins_no + 1;
			codeop = csv(code, &i);
			j++;
			code_gen[ins_no]->parameters[0] = j;
			ins_no++;
			continue;
		}
		else if(str_cmp(codeop, "ENDIF"))
		{
			code_gen[if_ele->address[if_ele->top--]-1]->parameters[1] = ins_no;
			code_gen[ins_no]->instruction_no = ins_no + 1;
			code_gen[ins_no]->opcode = indx = get_opcodeindx(opcodes, "ENDIF");
			if_flag--;

		}
			
	/*	for (k = 0; k < if_flag; k++)
		{
			i++;

		}*/
		if (code[i] == ' ')
			i++;
		if (code[i] == ':' || code[i-1] == ':')
		{
			strcpy(lable_tab->blockname[++lable_tab->top], code);
			lable_tab->address[lable_tab->top] = ins_no;
		}
		else
		{
			code_gen[ins_no]->instruction_no = ins_no + 1;
			code_gen[ins_no]->opcode = indx = get_opcodeindx(opcodes, codeop);
			//i++;
			if (code_gen[ins_no]->opcode == 7)//IF
			{
				codeop = csv(code, &i);//regi
				i++;
				indx = get_regis_indx(codeop);
				code_gen[ins_no]->parameters[j++] = indx;
				codeop = csv(code, &i);//opr
				i++;
				code_gen[ins_no]->parameters[j++] = indx = get_opcodeindx(opcodes, codeop);
				codeop = csv(code, &i);//regi
				i++;
				indx = get_regis_indx(codeop);
				code_gen[ins_no]->parameters[j++] = indx;
				j++;
				if_flag++;
				if_ele->address[++if_ele->top] = ins_no;
				codeop = csv(code, &i);
			}
			else if (code_gen[ins_no]->opcode == 6)
			{
				
				code_gen[ins_no]->parameters[1] = lable_tab->address[lable_tab->top--];

			}
			while (code[i] != '\0' && code[i] != '\n')
			{
				while (code[i] == ' ')
					i++;
				codeop = csv(code, &i);
				indx = -1;
				indx = get_regis_indx(codeop);
				if (indx != -1)
				{
					code_gen[ins_no]->parameters[j] = indx;
				}
				else
				{
					for (int k = 0; k < s; k++)
					{
						if (str_cmp(symbols[k]->name, codeop))
						{
							int off = 0;
							if (code[i] == '[')
							{
								off += symbols[k]->start_addr;
								i++;
								codeop = csv(code, &i);
								off += atoi(codeop);
								code_gen[ins_no]->parameters[j] = off;
								i++;
							}
							else
							{
								code_gen[ins_no]->parameters[j] = symbols[k]->start_addr;
							}
							
							break;
						}
					}

				}
				j++;
				if (code[i] != '\0')
					i++;

			}
			code_gen[ins_no]->parameters[0] = j;
			ins_no++;
		}
	}



	fpp = fopen("int_code.txt", "w");
	for (i = 0; i < ins_no; i++)
	{
		fprintf(fpp, "%d\t%d",code_gen[i]->instruction_no, code_gen[i]->opcode);
		for (j = 1; j < code_gen[i]->parameters[0]; j++)
			fprintf(fpp, "\t%d", code_gen[i]->parameters[j]);
		fprintf(fpp, "\n");
	}
	fclose(fpp);
	
	run(symbols, code_gen, opcodes, s, ins_no, memory);
}

void run(symbol_table** symbols, intermediate_code** code_gen, char**opcodes, int s, int ins_no, void* memory)
{

	int i, j, k, ins;
	int* regis = (int*)malloc(sizeof(int) * 8);
	i = 0;
	while (i<ins_no)
	{
		search(symbols, code_gen[i], opcodes, s, ins_no, memory, regis, &i);
	}
	system("pause");
}

void search(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{

	int c = code_gen->opcode;
	switch (c)
	{
	case 1: mov(symbols, code_gen, opcodes, s, ins_no, memory, regis, cur_ins);
		break;
	case 2: movr(symbols, code_gen, opcodes, s, ins_no, memory, regis, cur_ins);
		break;
	case 3: add(symbols, code_gen, opcodes, s, ins_no, memory, regis, cur_ins);
		break;
	case 4: sub(symbols, code_gen, opcodes, s, ins_no, memory, regis, cur_ins);
		break;
	case 5: mul(symbols, code_gen, opcodes, s, ins_no, memory, regis, cur_ins);
		break;
	case 6: jmp(symbols, code_gen, opcodes, s, ins_no, memory, regis, cur_ins);
		return;
		break;
	case 7: if_run(symbols, code_gen, opcodes, s, ins_no, memory, regis, cur_ins);
		return;
		break;
	case 8: //eq();
		break;
	case 9: //lt();
		break;
	case 10: //gt();
		break;
	case 11: //lteq();
		break;
	case 12: //gteq();
		break;
	case 13: print(symbols, code_gen, opcodes, s, ins_no, memory, regis, cur_ins);
		break;
	case 14: read(symbols, code_gen, opcodes, s, ins_no, memory, regis, cur_ins);
		break;
	case 15: 	break;
	}

	(*cur_ins)++;
}

void mov(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{
	int val = regis[code_gen->parameters[2]];
	int strt = symbols[code_gen->parameters[1]]->start_addr;
	int size = symbols[code_gen->parameters[1]]->size;
	(*cur_ins)--;
	(*cur_ins)++;
	memcpy(*(&memory + strt), &val, sizeof(int)*size);
}

void movr(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{
	int val;
	int strt = symbols[code_gen->parameters[2]]->start_addr;
	int size = 1;
	if (!symbols[code_gen->parameters[2]]->isConst)
	{
		symbols[code_gen->parameters[2]]->size;
		memcpy(&val, *(&memory + strt), sizeof(int)*size);
		regis[code_gen->parameters[1]] = (int)val;

	}
	else
	{
		regis[code_gen->parameters[1]] = symbols[code_gen->parameters[2]]->size;
	}

}

void read(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{
	int a;
	char* re = regis_name(code_gen->parameters[1]);
	printf("Enter %s Value :  ", re);
	scanf("%d", &a);
	regis[code_gen->parameters[1]] = a;

}




char* regis_name(int n)
{
	n += 65;
	char* reg = (char*)malloc(sizeof(char) * 3);
	reg[2] = '\0';
	reg[1] = 'X';
	reg[0] = n;
	return reg;
}
void print(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{
	int a = regis[code_gen->parameters[1]];
	char* re = regis_name(code_gen->parameters[1]);
	printf("Value of %s is: %d \n", re, a);
}
void add(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{
	regis[code_gen->parameters[1]] = regis[code_gen->parameters[2]] + regis[code_gen->parameters[3]];
}

void sub(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{
	regis[code_gen->parameters[1]] = regis[code_gen->parameters[2]] - regis[code_gen->parameters[3]];
}

void mul(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{
	regis[code_gen->parameters[1]] = regis[code_gen->parameters[2]] * regis[code_gen->parameters[3]];
}

void jmp(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{
	*cur_ins = code_gen->parameters[1];
}



void if_run(symbol_table** symbols, intermediate_code* code_gen, char**opcodes, int s, int ins_no, void* memory, int* regis, int *cur_ins)
{
	int a = regis[code_gen->parameters[1]];
	int b = regis[code_gen->parameters[3]];
	switch (code_gen->parameters[2])
	{
	case 8: if (a != b)
	{
		*cur_ins = code_gen->parameters[4] - 1;
		return;
	}
			else
				break;
			
	case 9: if (!(a < b))
	{
		*cur_ins = code_gen->parameters[4] - 1;
		return;
	}
			else
				break;
	case 10: if (!(a > b))
	{
		*cur_ins = code_gen->parameters[4] - 1;
		return;
	}
			 else
				 break;
	case 11: if (!(a <= b))
	{
		*cur_ins = code_gen->parameters[4] - 1;
		return;
	}
			 else
				 break;
	case 12: if (!(a >= b))
	{
		*cur_ins = code_gen->parameters[4] - 1;
		return;
	}
			 else
				 break;
	}
	(*cur_ins)++;
}


