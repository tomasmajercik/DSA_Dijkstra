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

    //######## initialization #####//
    for (int i = 0; i < graph->size; i++)
    {
        distance[i] = INFINITY;
        predecessors[i] = -1;
        visited[i] = 0;
    }
    distance[source] = 0; // the initial vertex is 0
    //#############################//

    for (int loop = 1; loop < graph->size; loop++)
    {
        int minDistance = INFINITY;
        int minIndex = 0;

        for (int i = 0; i < graph->size; i++)
        {
            if (visited[i] == 0 && distance[i] <= minDistance)
            {
                minDistance = distance[i];
                minIndex = i;
            }
        }

        int MINimalIndex = minIndex;
        visited[MINimalIndex] = 1;

        for (int i = 0; i < graph->size; i++)
        {
            if(!visited[i] && graph->weight[MINimalIndex][i] && distance[MINimalIndex]!= INFINITY &&
               distance[MINimalIndex] + graph->weight[MINimalIndex][i] < distance[i])
            {
                distance[i] = distance[MINimalIndex] + graph->weight[MINimalIndex][i];
                predecessors[i] = MINimalIndex;
            }
        }


    }

    int current = destination;
    while (current != -1)
    {
        printf("%d ", current);
        current = predecessors[current]; // Move to predecessor in the shortest path
    }
    printf("\n");

    free(distance);
    free(visited);
    free(predecessors);
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
