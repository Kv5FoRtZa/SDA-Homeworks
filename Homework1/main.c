#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int e_litera_buna(char v[], int i)
{
	if (v[i] == 'a' || v[i] == 'b' || v[i] == 'c' || v[i] == 'd')
		return 1;
	return 0;
}

void ultimul_param(char v[], int poz)
{
	poz = poz + 2;
	if (e_litera_buna(v, poz))
	{
		char registru[4] = {"ewx"};
		registru[1] = v[poz];
		printf("%s\n", registru);
	} else if (v[poz] >= '0' && v[poz] <= '9')
	{
		if (v[poz + 1] >= '0' && v[poz + 1] <= '9')
		{
			int nr = v[poz + 1] - 48 + 10 * v[poz] - 480;
			if (v[poz + 2] == ';' || v[poz + 2] == ')')
			{
				printf("%d\n", nr);
			}
		} else if (v[poz + 1] == ';' || v[poz + 1] == ')')
		{
			int nr = v[poz] - 48;
			printf("%d\n", nr);
		}
	}
}

void linie_simpla(char v[])
{
	char registru_1[4] = {"ewx"};
	char registru_2[4] = {"ewx"};
	if (e_litera_buna(v, 0))
	{
		registru_1[1] = v[0];
		if (v[2] == '=')
		{
			if (e_litera_buna(v, 4))
			{
				registru_2[1] = v[4];
				if (v[5] == ';')
				{
					printf("MOV %s, %s\n", registru_1, registru_2);
				} else if (v[6] == '&')
				{
					printf("AND %s, ", registru_1);
					ultimul_param(v, 6);
				} else if (v[6] == '|')
				{
					printf("OR %s, ", registru_1);
					ultimul_param(v, 6);
				} else if (v[6] == '^')
				{
					printf("XOR %s, ", registru_1);
					ultimul_param(v, 6);
				} else if (v[6] == '+')
				{
					printf("ADD %s, ", registru_1);
					ultimul_param(v, 6);
				} else if (v[6] == '-')
				{
					printf("SUB %s, ", registru_1);
					ultimul_param(v, 6);
				} else if (v[6] == '>')
				{
					printf("SHR %s, ", registru_1);
					ultimul_param(v, 7);
				} else if (v[6] == '<')
				{
					printf("SHL %s, ", registru_1);
					ultimul_param(v, 7);
				} else if (v[6] == '*')
				{
					if (v[0] != 'a')
					{
						printf("MOV eax, %s\n", registru_1);
					}
					printf("MUL ");
					ultimul_param(v, 6);
					if (v[0] != 'a')
					{
						printf("MOV %s, eax\n", registru_1);
					}
				} else if (v[6] == '/')
				{
					if (v[8] == '0')
					{
						printf("Error\n");
					} else
					{
						if (v[0] != 'a')
						{
							printf("MOV eax, %s\n", registru_1);
						}
						printf("DIV ");
						ultimul_param(v, 6);
						if (v[0] != 'a')
						{
							printf("MOV %s, eax\n", registru_1);
						}
					}
				}
			} else if (v[4] >= '0' && v[4] <= '9')
			{
				printf("MOV %s, ", registru_1);
				ultimul_param(v, 2);
			}
		}
	}
}

void instructiuni(char v[], int poz)
{
	char registru[4] = {"ewx"};
	registru[1] = v[poz];
	printf("CMP %s, ", registru);
	if (v[poz + 3] == ' ')
	{
		ultimul_param(v, poz + 2);
	} else
	{
		ultimul_param(v, poz + 3);
	}
	if (v[poz + 2] == '=')
	{
		printf("JNE end_label\n");
	} else if  (v[poz + 2] == '>')
	{
		if (v[poz + 3] == '=')
		{
			printf("JL end_label\n");
		} else
		{
			printf("JLE end_label\n");
		}
	} else if  (v[poz + 2] == '<')
	{
		if (v[poz + 3] == '=')
		{
			printf("JG end_label\n");
		} else
		{
			printf("JGE end_label\n");
		}
	}
}

void citire(void)
{
	char v[100];
	char registru[4] = {"ewx"};
	int vf = 0;
	while (fgets(v, 100, stdin))
	{
		if (v[0] == '\n')
		{
			break;
		}
		if (v[0] == 'i')
		{
			instructiuni(v, 4);
		} else if (v[0] == 'w')
		{
			printf("start_loop:\n");
			instructiuni(v, 7);
			vf = 1;
		} else if (v[0] == 'f')
		{
			vf = 2;
			registru[1] = v[5];
			printf("MOV %s, 0\n", registru);
			printf("start_loop:\n");
			instructiuni(v, 12);
		} else if (v[0] != '}')
		{
			if (v[0] == '\t')
			{
				char aux[101];
				strcpy(aux, v + 1);
				strcpy(v, aux);
			}
			linie_simpla(v);
		} else
		{
			if (vf == 1)
			{
				printf("JMP start_loop\n");
				printf("end_label:\n");
			} else if (vf == 0)
			{
				printf("end_label:\n");
			} else
			{
				printf("ADD %s, 1\n", registru);
				printf("JMP start_loop\n");
				printf("end_loop:\n");
			}
		}
	}
}

int main(void)
{
	citire();
	return 0;
}
