#include <stdio.h>
#include <stdlib.h>

//#define INFINITY 2147483646
#define INFINITY 2000


typedef struct Edge
{
    int dest;
    int weight;
    struct Edge *next;
} Edge;

typedef struct Graph
{
    int size;
    Edge *next;
} Graph;

Graph **createGraph(int vertexes)
{
    Graph **graph = (Graph **)malloc(sizeof(Graph *) * vertexes);
    if (graph == NULL)
        exit(1);

    for (int i = 0; i < vertexes; i++)
    {
        graph[i] = (Graph *)malloc(sizeof(Graph));
        if (graph[i] == NULL)
            exit(1);
        graph[i]->size = vertexes;
        graph[i]->next = NULL;
    }

    return graph;
}

void insertEdge(Graph **graph, int source, int destination, int weight)
{
    if (graph == NULL)
        return;

    // Create a new edge node
    Edge *newEdge = (Edge *)malloc(sizeof(Edge));
    newEdge->dest = destination;
    newEdge->weight = weight;
    newEdge->next = NULL;

}

void search(Graph *graph, int source, int destination)
{
    int N = graph->size;
    int visited[N];
    int weight[N];

    for (int i = 0; i < N; i++)
    {
        visited[i] = 0;
        weight[i] = INFINITY;
    }

    int PriorityQueue[N];
    int front = 0, rear = 0;

    PriorityQueue[rear] = source;

    int u;
    while (front <= rear)
    {

    }


}

int main()
{
    int vertexes, edges;
    int source, destination, weight;
    scanf("%d %d", &vertexes, &edges);

    // Create the graph
    Graph **graph = createGraph(vertexes);

    // Insert edges
    for (int i = 0; i < edges; i++)
    {
        scanf(" (%d , %d , %d)", &source, &destination, &weight);
        insertEdge(graph, source, destination, weight);
    }

    // Process queries
    char DO;
    while (scanf(" %c", &DO) == 1)
    {
        switch (DO)
        {
            case 's':
                scanf(" %d %d", &source, &destination);
                // Search function logic goes here
                break;
        }
    }

    // Free memory for each graph
    for (int i = 0; i < vertexes; i++)
    {
        Edge *current = graph[i]->next;
        while (current != NULL)
        {
            Edge *temp = current;
            current = current->next;
            free(temp);
        }
        free(graph[i]);
    }

    // Free memory for the array of graphs
    free(graph);
    return 0;
}