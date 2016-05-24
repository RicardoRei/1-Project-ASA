#include <stdio.h>
#include <stdlib.h>


typedef struct node* link;
typedef struct Vertex* item; /* para facilitar a leitura de ponteiros para a estrutura vertex e node*/

/*
*@brief Vertex
* 		estrutura que vai guardar informacao relatica a um vertice.
*/
struct Vertex
{
	int id;
	int color;
	item pi;
	int d;
};

struct node
{
	item _vertex;
	link next;
};

typedef struct
{
	link head;
	link tail;
} Queue;

/*
*@brief struct Graph:
*		estrutura que contem uma lista de adjacencias, um inteiro com o numero de vertices, o vertice que representa
*		o Paul, e o vector com todos os vertices.
*/
typedef struct
{
	link* adjacenciesList;
	int verticesNumber;
	item _paul;
	item* _vertices;

}Graph;

item createVertex(int i);
Graph builtGraph();
Graph breadthFirstSearch(Graph graph);
void outPut(Graph graph);

link createNode(item i);
link insertLinkedList(item i, link head);
link dequeue(Queue* queue);
void enqueue(Queue *queue, item i);

int main()
{
	Graph graph = builtGraph();
	graph = breadthFirstSearch(graph);
	outPut(graph);

	return EXIT_SUCCESS;
}

/*
*@brief createVertex;
*		Funcao que cria um vertice inicialmente com o valor pi (previous) a NULL, d (distancia a paul) a -1, e a color a 0 (branco)
*/
item createVertex(int i)
{
	item new = (item)malloc(sizeof(struct Vertex));
	new->id = i;
	new->color = 0;
	new->pi = NULL;
	new->d = -1;
	return new;
}

/*
*@brief builtGraph;
*		Le do standard input o numero de vertices e arestas, o numero do vertice que representa o paul e as transicoes/arestas.
*		Constroi o grafo com essa informacao.
*		
*/
Graph builtGraph()
{
	int vertices, edges, paul, vertexOne, vertexTwo, i;

	Graph graph;

	scanf("%d %d", &vertices, &edges);
	graph.adjacenciesList = (link*) malloc(sizeof(link)*vertices);
	graph.verticesNumber = vertices;
	graph._vertices = (item*) malloc(sizeof(item)*vertices);

	for ( i = 0; i < vertices; i++)
	{
		graph.adjacenciesList[i] = NULL;
		graph._vertices[i] = createVertex(i);
	}

	scanf("%d", &paul);
	graph._paul = graph._vertices[paul-1];

	for (i = 0; i < edges; i++)
	{
		scanf("%d %d", &vertexOne, &vertexTwo);
		graph.adjacenciesList[vertexOne-1] = insertLinkedList(graph._vertices[vertexTwo-1], graph.adjacenciesList[vertexOne-1]);
		graph.adjacenciesList[vertexTwo-1] = insertLinkedList(graph._vertices[vertexOne-1], graph.adjacenciesList[vertexTwo-1]);
		
	}

	return graph;
}

/*
*@brief breadthFirstSearch;
*		Recebe o um grafo e executa o algoritmo breadth-first-search estudado nas aulas, marcando os vertices com cores cinzento(1) 
*		e preto(2), associando uma distancia ao vertice do Paul e marcando o vertice anterior.
*/
Graph breadthFirstSearch(Graph graph)
{
	link current;
	link adjancent;									/*ponteiro auxiliar que ira servir para percorrer essa lista (iterador)*/
	Queue* queue = (Queue*) malloc(sizeof(Queue)); /*inicializacao da fila*/
	queue->head = NULL;
	queue->tail = NULL;

	enqueue(queue, graph._paul); 						    /*guardamos na fila o vertice inicial*/

	current = graph.adjacenciesList[graph._paul->id+1];/*current vai apontar para o primeiro nó da lista de adjacencia do Paul*/
	graph._paul->color = 1;        							/*marcamos o vertice do inicial a cinzento*/
	graph._paul->d = 0;										/*a sua distancia a zero*/

	while (queue->head != NULL)  							/*enquanto a fila nao esta vazia = ha vertices com caminhos para o vertice 
															  inicial por visitar*/
	{
		current = dequeue(queue);							/*tiramos o primeiro*/
		adjancent = graph.adjacenciesList[current->_vertex->id];/*vamos buscar o primeiro nó da sua lista de adjacencias*/
		while (adjancent != NULL)								/*vamos iterando sobre ela*/
		{
			if (adjancent->_vertex->color == 0) 				/*sempre que virmos um no a branco significa que nao foi visitado*/
			{
				adjancent->_vertex->color = 1;
				adjancent->_vertex->d = current->_vertex->d +1;
				adjancent->_vertex->pi = current->_vertex;
				enqueue(queue, adjancent->_vertex); 			/*marcamos e metemos na queue para ver os seus adjacentes mais tarde*/
			}
			adjancent = adjancent->next;
		}
		current->_vertex->color = 2;							/*depois de ver todos os adjacentes do current marcamos a preto*/
		free(current);
	}
	
	return graph;
}
/*
*@brief outPut
*		funcao que nos da com base em toda a estrutura o outpu esperado.
*/
void outPut(Graph graph)
{
	int i, M = 0, *counting;

	/*ciclo para encontrar a maior distancia ao Paul*/
	for(i = 0; i < graph.verticesNumber; i++)
	{
		if(graph._vertices[i]->d > M)
			M = graph._vertices[i]->d;
	}

	printf("%d\n", M);

	counting = (int*) malloc(sizeof(int)*M);
	/*inicializacao de um vector em que cada indice vai corresponder a uma distancia relativa ao Paul*/
	for (i = 0; i < M; i++) 
	{
		counting[i] = 0;
	}

	/*ciclo em que vamos marcar todas as ocorrencias de uma distancia no vector contador.*/
	for (i = 0; i < graph.verticesNumber; i++)
	{
		if(graph._vertices[i]->d > -1)
		{
			counting[graph._vertices[i]->d-1]++;
		}
	}

	/*imprimimos o conteudo do vector.*/
	for (i = 0; i < M; i++)
	{
		printf("%d\n", counting[i]);
	}
}

/*
*@brief createNode:
*		cria um no atraves de um vertice.
*/
link createNode(item i)
{
	link new = (link) malloc(sizeof(struct node));
	new->_vertex = i;
	new->next = NULL;
	return new;
}

/*
*@brief insertLinkedList:
*		insere um elemento no inicio da lista.
*/
link insertLinkedList(item i, link head)
{
	link new = createNode(i);
	if (head == NULL)
		head = new;
	else
	{
		new->next = head;
		head = new;
	}
	return head;
}

/*
*@brief dequeue:
*		retira da fila o primeiro elemento.
*/
link dequeue(Queue* queue)
{
	link aux = queue->head;

	if (queue->head == NULL && queue->tail == NULL)
	{
		return NULL;
	}
	else if (queue->head->next == NULL && queue->tail->next == NULL)
	{
		queue->head = queue->head->next;
		queue->tail = queue->head;
	}
	else
		queue->head = queue->head->next;
	return aux;
}

/*
*@brief enqueue:
*		insere na final da fila um elemento.
*/
void enqueue(Queue *queue, item i)
{
	link new = createNode(i);
	if( queue->head == NULL && queue->tail == NULL)
	{
		queue->head = new;
		queue->tail = new;
	}
	else
	{
		queue->tail->next = new;
		queue->tail = new;
	}
}

