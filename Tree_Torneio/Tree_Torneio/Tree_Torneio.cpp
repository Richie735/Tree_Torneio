#define _CRT_SECURE_NO_WARNINGS

#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME 20
#define STAGES 15
#define DATA(node) ((node)->data)
#define LEFT(node) ((node)->left)
#define RIGHT(node) ((node)->right)
#define EMPTY NULL
#define NO_LINK NULL

typedef struct _PLAYER
{
	char name[MAX_NAME];
	int sets;
}PLAYER;

typedef struct _BTREE_NODE
{
	void* data;
	struct _BTREE_NODE* left;
	struct _BTREE_NODE* right;
} BTREE_NODE;

typedef BTREE_NODE* BTREE;
typedef enum _BTREE_LOCATION { BTREE_LEFT, BTREE_RIGHT } BTREE_LOCATION;
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS;


/********** Funcoes ********************************************/

BTREE_NODE* NewBtreeNode(void*);
BTREE_NODE* AddBtreeNode(BTREE_NODE*, BTREE_NODE*, BTREE_LOCATION);
BTREE_NODE* InitNode(void*, BTREE_NODE*, BTREE_NODE*);
BTREE_NODE* CreateBtree(void**, int, int);

void BtreeFree(BTREE);

int BtreeSize(BTREE);
int BtreeDeep(BTREE);
BOOLEAN BtreeLeaf(BTREE_NODE*);

STATUS ReadPlayersFromFile(void**, const char*);

/********** Main ***********************************************/
int main()
{
	return 0;
}



/********** Funcoes ********************************************/

/****************************************************************
* Funcao: Cria um no
*
* Parametros:	data - apontador generico para os dados a inserir no no' criado
* 
* Saida: apontador para o no' criado ou NULL em caso de erro
***************************************************************/
BTREE_NODE* NewBtreeNode(void* data)
{
	BTREE_NODE* tmp;

	if ((tmp = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) != NULL)
	{
		DATA(tmp) = data;
		LEFT(tmp) = RIGHT(tmp) = NULL;
	}

	return tmp;
}


/****************************************************************
* Funcao: Adiciona um no a arvore
*
* Parametros:	upnode - no anterior ao que se pretende adicionar
*				node - no que se pretende adicionar
*				where - localizacao onde o no deve ser adicionado
* 
* Saida: apontador para o no' criado ou NULL em caso de erro
***************************************************************/
BTREE_NODE* AddBtreeNode(BTREE_NODE* upnode, BTREE_NODE* node, BTREE_LOCATION where)
{
	BTREE_NODE* tmp = upnode;
	
	if (where == BTREE_LEFT)
	{
		if (LEFT(upnode) == NULL)	LEFT(upnode) = node;
		else	tmp = NULL;
	}
	else
	{
		if (RIGHT(upnode) == NULL)	RIGHT(upnode) = node;
		else	tmp = NULL;
	}

	return tmp;
}


/****************************************************************
* Funcao: Inicializa um no
*
* Parametros:	data - apontador generico para os dados a inserir no no' criado
*				node1 - no esquerdo
*				node1 - no direito
*
* Saida: apontador para o no' criado ou NULL em caso de erro
***************************************************************/
BTREE_NODE* InitNode(void* data, BTREE_NODE* node1, BTREE_NODE* node2)
{
	BTREE_NODE* tmp = NULL;

	tmp = NewBtreeNode(data);
	LEFT(tmp) = node1;
	RIGHT(tmp) = node2;

	return(tmp);
}


/****************************************************************
* Funcao: Cria uma arvore
*
* Parametros:	v - 
*				i - contador
*				size - tamanho da arvore
*
* Saida: apontador para o no' criado ou NULL em caso de erro
***************************************************************/
BTREE_NODE* CreateBtree(void** v, int i, int size)
{
	if (i >= size)	return(NULL);
	else	return(InitNode(*(v + i), CreateBtree(v, 2 * i + 1, size), CreateBtree(v, 2 * i + 2, size)));
}


/****************************************************************
* Funcao: Liberta a arvore
*
* Parametros:	btree - arvore
*
* Saida: void
***************************************************************/
void BtreeFree(BTREE btree)
{
	if (btree != NULL)
	{
		BtreeFree(LEFT(btree));
		BtreeFree(RIGHT(btree));
		free(btree);
	}
}


/****************************************************************
* Funcao: Retorna o tamanho da arvore
*
* Parametros:	btree - arvore
*
* Saida: tamanho da arvore
***************************************************************/
int BtreeSize(BTREE btree)
{
	int count = 0;

	if (btree != NULL)	count = 1 + BtreeSize(LEFT(btree)) + BtreeSize(RIGHT(btree));

	return(count);
}


/****************************************************************
* Funcao: Retorna a profundidade da arvore
*
* Parametros:	btree - arvore
*
* Saida: profundidade da arvore
***************************************************************/
int BtreeDeep(BTREE btree)
{
	int deep = 0, left, right;

	if (btree != NULL)
	{
		left = BtreeDeep(LEFT(btree));
		right = BtreeDeep(RIGHT(btree));
		deep = 1 + ((left > right) ? left : right);
	}

	return(deep);
}


/****************************************************************
* Funcao: Verifica se e uma folha
*
* Parametros:	btree - apontador arvore
*
* Saida: TRUE se for folha e FALSE se nao for
***************************************************************/
BOOLEAN BtreeLeaf(BTREE_NODE* btree)
{
	if ((LEFT(btree) == NULL) && (RIGHT(btree) == NULL))	return(TRUE);
	else	return(FALSE);
}


/****************************************************************
* Funcao: Le a arvore de um ficheiro
*
* Parametros:	players - vetor onde se vai guardar os jogadores
*				file_name - nome do ficheiro
* 
* Saida: OK ou ERROR
***************************************************************/
STATUS ReadPlayersFromFile(void** players, const char* file_name)
{
	FILE* fp;
	int j, i = 0;
	void* data;

	if ((fp = fopen(file_name, "r")) != NULL)
	{
		while (!feof(fp))
		{
			if ((data = malloc(sizeof(PLAYER))) != NULL)
			{
				fscanf(fp, "%[^;];", ((PLAYER*)data)->name);
				fscanf(fp, "%d\n", &(((PLAYER*)data)->sets));
				players[i] = data;
				i++;
			}
			else
			{
				for (j = i; j >= 0; j--)
					free(players[j]);
				return(ERROR);
			}
		}
		fclose(fp);
		return(OK);
	}
	else	return(ERROR);
}