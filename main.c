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

void search(Graph* graph, int source, int destination)
{
    if(graph == NULL)
        return;

    int* distance = (int*)malloc(sizeof(int) * graph->size);
    int* predecessors = (int*)malloc(sizeof(int) * graph->size);
    int* visited = (int*)malloc(sizeof(int) * graph->size);

    // Initialization
    for (int i = 0; i < graph->size; i++)
    {
        distance[i] = INFINITY;
        predecessors[i] = -1;
        visited[i] = 0;
    }
    distance[source] = 0;

    for (int loop = 0; loop < graph->size; loop++)
    {
        int minDistance = INFINITY;
        int minIndex = -1;

        // Find the vertex with the minimum distance among unvisited vertices
        for (int i = 0; i < graph->size; i++)
        {
            if (visited[i] == 0 && distance[i] <= minDistance)
            {
                minDistance = distance[i];
                minIndex = i;
            }
        }


        int minVertex = minIndex;
        visited[minVertex] = 1;

        // Relaxation
        for (int i = 0; i < graph->size; i++)
        {
            if (!visited[i] && graph->weight[minVertex][i] &&
                distance[minVertex] + graph->weight[minVertex][i] < distance[i])
            {
                distance[i] = distance[minVertex] + graph->weight[minVertex][i];
                predecessors[i] = minVertex;
            }
        }
    }

    printf("%d: [", distance[destination]);

    // Store the vertices in the path
    int current = destination;
    while (current != -1)
    {
        printf("%d", current);
        current = predecessors[current];
        if (current != -1)
            printf(", ");
    }
    printf("]\n");



    free(distance);
    free(predecessors);
    free(visited);
}




int main()
{
    int vertexes, edges;
    int source, destination, weight;
    scanf("%d %d", &vertexes, &edges);
    Graph* graph = createGraph(vertexes);

    for (int i = 0; i < edges; i++)
    {
        scanf(" (%d, %d, %d)", &source, &destination, &weight);
        graph->weight[source][destination] = weight;
    }
    char DO;

    while ((scanf(" %c", &DO) == 1))
    {
        switch (DO)
        {
            case 's':
                scanf(" %d %d", &source, &destination);
                search(graph, source, destination);
                break;
        }
    }

    // Freeing memory
    for (int i = 0; i < vertexes; i++)
    {
        free(graph->weight[i]);
    }
    free(graph->weight);
    free(graph);
    return 0;
}

