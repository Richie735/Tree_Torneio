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

void Print(BTREE);
void PrintGame(BTREE);
void PrintAllGames(BTREE);
void PrintLeafs(BTREE);
void PrintWinnerGames(BTREE);
void PrintPlayerGames(BTREE, char*);
void PrintTree(BTREE, int);

int CountPlayers(BTREE);
int CountWinnerSets(BTREE, void*);
int CountPlayerSets(BTREE, char*);
int CountTotalSets(BTREE);
int CountLeafs(BTREE);
int CountPlayerGames(BTREE, char*);
int CountDisputSets(BTREE, char*);

void Position(BTREE, char);

STATUS ReadPlayersFromFile(void**, const char*);

/********** Main ***********************************************/
int main()
{
	BTREE Btree;
	void* players[STAGES];

	if (ReadPlayersFromFile(players, "torneio.txt"))
	{
		Btree = CreateBtree(players, 0, STAGES);
		
		printf("TORNEIO:\n");
		PrintTree(Btree, 0);

		printf("\n-Lista de participantes:\n");
		PrintLeafs(Btree);

		printf("\n-Lista de Jogos:\n");
		PrintAllGames(Btree);
		
		printf("\n-Numero de eliminatorias: %d", BtreeDeep(Btree) - 1);
		printf("\n-Numero de Jogos: %d", BtreeSize(Btree) / 2);
		printf("\n-Numero de Jogadores: %d", CountPlayers(Btree));

		printf("\n\n-Vencedor: %s", ((PLAYER*)DATA(Btree))->name);
		printf("\n  Sets ganhos: %d", CountWinnerSets(Btree, DATA(Btree)));
		printf("\n  Jogos disputados:\n");
		PrintWinnerGames(Btree);


		BtreeFree(Btree);
	}
	else
		printf("ERRO na leitura do ficheiro\n");


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
* Funcao: Printa a arvore InOrder
*
* Parametros:	btree - apontador arvore
*
* Saida: void
***************************************************************/
void Print(BTREE btree) {
	if (btree != NULL) {
		Print(btree->left);
		printf("%s\n", ((PLAYER*)(btree->data))->name);
		Print(btree->right);
	}
}


/****************************************************************
* Funcao: Printa um jogo
*
* Parametros:	btree - arvore
*
* Saida: void
***************************************************************/
void PrintGame(BTREE btree) {
	if (btree != NULL)	printf("   %s -VS- %s\n", ((PLAYER*)(btree->left->data))->name, ((PLAYER*)(btree->right->data))->name);
}


/****************************************************************
* Funcao: Printa todos os jogo
*
* Parametros:	btree - arvore
*
* Saida: void
***************************************************************/
void PrintAllGames(BTREE btree)
{
	if (btree != NULL && !BtreeLeaf(btree)) {
		PrintAllGames(btree->left);
		PrintAllGames(btree->right);
		PrintGame(btree);
	}
}

/****************************************************************
* Funcao: Printa os nos que sao folha
*
* Parametros:	btree - arvore
*
* Saida: void
***************************************************************/
void PrintLeafs(BTREE btree)
{
	if (btree != NULL)
	{
		PrintLeafs(btree->left);
		if (BtreeLeaf(btree))
		{
			printf("   %s\n", ((PLAYER*)(btree->data))->name);
		}
		PrintLeafs(btree->right);
	}

}


/****************************************************************
* Funcao: Printa os vencedores dos jogos
*
* Parametros:	btree - arvore
*
* Saida: void
***************************************************************/
void PrintWinnerGames(BTREE btree)
{
	if (btree != NULL && !BtreeLeaf(btree)) {
		PrintGame(btree);

		if (!strcmp(((PLAYER*)DATA(btree))->name, ((PLAYER*)DATA(LEFT(btree)))->name))	PrintWinnerGames(LEFT(btree));
		else PrintWinnerGames(RIGHT(btree));
	}
}


/****************************************************************
* Funcao: Printa todos os jogos de um jogador
*
* Parametros:	btree - arvore
*				name - nome do jogador desejado
*
* Saida: void
***************************************************************/
void PrintPlayerGames(BTREE btree, char* name)
{
	if (btree != NULL && !BtreeLeaf(btree)) {
		if (!strcmp(((PLAYER*)DATA(btree))->name, name))	PrintGame(btree);

		PrintPlayerGames(btree->left, name);
		PrintPlayerGames(btree->right, name);
	}
}


/****************************************************************
* Funcao: Printa o nome dos jogadores em formato de arvore
*
* Parametros:	btree - arvore
*				space - numero de espacos a printar
*
* Saida: void
***************************************************************/
void PrintTree(BTREE btree, int space)
{
	if (btree != NULL) {
		space += 10;
		PrintTree(btree->right,space);
		
		printf("\n");
		for (int i = 10; i < space; i++)		printf(" ");
		printf("%s\n", ((PLAYER*)DATA(btree))->name);
		
		PrintTree(btree->left, space);
	}
}


/****************************************************************
* Funcao: Conta os jogadores
*
* Parametros:	btree - arvore
*
* Saida: numero de jogadores
***************************************************************/
int CountPlayers(BTREE btree)
{
	int count = 0;
	if (btree != NULL)
	{
		count += CountPlayers(btree->left);

		if (BtreeLeaf(btree))	count++;

		count += CountPlayers(btree->right);
	}
	return count;
}


/****************************************************************
* Funcao:
*
* Parametros:	btree -
*		winner -
*
* Saida: numero de set do vencedor
***************************************************************/
int CountWinnerSets(BTREE btree, void* winner)
{
	int count = 0;
	if (btree != NULL)
	{
		if (!strcmp(((PLAYER*)winner)->name, ((PLAYER*)DATA(btree))->name))
		{
			count += (((PLAYER*)DATA(btree))->sets);
		}
		count += (((PLAYER*)DATA(btree))->sets) + CountWinnerSets(btree->left, winner) + CountWinnerSets(btree->right, winner);
	}

	return count;
}


/****************************************************************
* Funcao: Conta os sets de um jogador
*
* Parametros:	btree - arvore
*		name - nome do jogador
*
* Saida: numero de sets
***************************************************************/
int CountPlayerSets(BTREE btree, char* name)
{
	int count = 0;

	if (btree != NULL && !BtreeLeaf(btree))
	{
		if (!strcmp(name, ((PLAYER*)btree->data)->name))	count += (((PLAYER*)btree->data)->sets) + CountPlayerSets(btree->left, name) + CountPlayerSets(btree->right, name);
		else	count = CountPlayerSets(btree->left, name) + CountPlayerSets(btree->right, name);
	}

	return count;
}


/****************************************************************
* Funcao: Conta os sets da arvore
*
* Parametros:	btree - arvore
*
* Saida: numero de sets
***************************************************************/
int CountTotalSets(BTREE btree)
{
	int count = 0;
	if (btree != NULL)
	{
		count = (((PLAYER*)(btree)->data)->sets) + CountTotalSets(btree->left) + CountTotalSets(btree->right);
	}
	return count;
}

/****************************************************************
* Funcao: Conta as folhas da arvore
*
* Parametros:	btree - arvore
*
* Saida: numero de folhas
***************************************************************/
int CountLeafs(BTREE btree)
{
	int count = 0;
	if (btree != NULL)
	{
		count += CountLeafs(btree->left);
		if (BtreeLeaf(btree))
			count++;
		count += CountLeafs(btree->right);
	}
	return count;
}


/****************************************************************
* Funcao: Conta os jogos de um jogador
*
* Parametros:	btree - arvore
*		name - nomde do jogador
*
* Saida: numero de jogos do jogador
***************************************************************/
int CountPlayerGames(BTREE btree, char* name)
{
	int count = 0;

	if (btree != NULL)
	{
		if (!strcmp(name, ((PLAYER*)btree->data)->name))
			count += 1 + CountPlayerGames(btree->left, name) + CountPlayerGames(btree->right, name);
		else
			count = CountPlayerGames(btree->left, name) + CountPlayerGames(btree->right, name);
	}
	return count;
}


/****************************************************************
* Funcao: Conta os sets de um jogador
*
* Parametros:	btree - arvore
*		name - nomde do jogador
*
* Saida: numero de sets do jogador
***************************************************************/
int CountDisputSets(BTREE btree, char* name)
{
	int count = 0;

	if (btree != NULL && !BtreeLeaf(btree))
	{
		if (!strcmp(name, ((PLAYER*)btree->left->data)->name) || !strcmp(name, ((PLAYER*)btree->right->data)->name))
			count += ((PLAYER*)(btree->left->data))->sets + ((PLAYER*)(btree->right->data))->sets + CountDisputSets(btree->left, name) + CountDisputSets(btree->right, name);
		else
			count = CountDisputSets(btree->left, name) + CountDisputSets(btree->right, name);
	}
	return count;
}


/****************************************************************
* Funcao: Printa o jogo onde o jogador foi eliminado
*
* Parametros:	btree - arvore
*		name - nome do jogador
*
* Saida: void
***************************************************************/
void Position(BTREE btree, char* name)
{
	int deep = 0;

	if (btree != NULL)
	{
		if (!strcmp(name, ((PLAYER*)btree->data)->name))
		{
			deep = BtreeDeep(btree);
			switch (deep)
			{
			case 4:
				printf("\nVencedor");
				break;
			case 3:
				printf("\nEliminado na final");
				break;
			case 2:
				printf("\nEliminado nas meias finais");
				break;
			case 1:
				printf("\nEliminado nos quartos de final");
				break;
			}
			return;
		}
		else
		{
			Position(btree->left, name);
			Position(btree->right, name);
		}

	}
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
