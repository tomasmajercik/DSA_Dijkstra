#include <stdio.h>
#include <stdlib.h>

#define INFINITY 2147483646

typedef struct GRAPH
{
    int size;
    int** weight;
}Graph;

Graph * createGraph(int size)
{

    Graph *graph = (Graph*)malloc(sizeof (Graph));
    graph->size = size;

    graph->weight = (int**)malloc(sizeof (int*) * size);
    for (int i = 0; i < size; i++)
    {
        graph->weight[i] = (int*) malloc(sizeof (int) * size);
        for (int j = 0; j < size; j++)
        {
            graph->weight[i][j] = INFINITY;
        }
    }
    return graph;
}

void search(Graph *graph, int source, int destination)
{
    int *distance = (int*) malloc(sizeof (int) * graph->size);
    int *predecessors = (int*) malloc(sizeof (int) * graph->size);
    int *visited = (int*) malloc(sizeof (int) * graph->size);



}


int main()
{
    int vertexes, edges;
    scanf("%d %d", &vertexes, &edges);
    Graph *graph = createGraph(vertexes);

    for(int i = 1; i < edges; i++)
    {
        int source, destination, weight;
        scanf(" (%d , %d , %d) ", &source, &destination, &weight);
        graph->weight[source][destination] = weight;
    }
    printf("fertig");
    char DO;

    while((scanf(" %c", &DO) == 1))
    {
        switch(DO)
        {
            case 's':
                int source, destination;
                scanf(" %d %d", &source, &destination);
                search(graph, source, destination);
                break;
        }
    }

    /////// Freeing stuff ///////
    for (int i = 0; i < vertexes; i++)
    {
        free(graph->weight[i]);
    }
    free(graph->weight);
    free(graph);
    return 0;
}
