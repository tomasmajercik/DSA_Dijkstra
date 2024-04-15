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
    Graph **graph = (Graph **) malloc(sizeof(Graph *) * vertexes);
    if (graph == NULL)
        exit(1);

    for (int i = 0; i < vertexes; i++)
    {
        graph[i] = (Graph *) malloc(sizeof(Graph));
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
    Edge *newEdge = (Edge *) malloc(sizeof(Edge));
    newEdge->dest = destination;
    newEdge->weight = weight;
    newEdge->next = NULL;

    // Find the last edge in the adjacency list of the source vertex
    Edge *current = graph[source]->next;
    if (current == NULL)
    {
        // If the adjacency list is empty, make the new edge the first edge
        graph[source]->next = newEdge;
    }
    else
    {
        // Traverse the adjacency list until the last edge
        while (current->next != NULL)
        {
            current = current->next;
        }
        // Append the new edge to the end of the list
        current->next = newEdge;
    }

    Edge *edge = (Edge*) malloc(sizeof (edge));
    edge->dest = source;
    edge->weight = weight;
    edge->next = NULL;

    current = graph[destination]->next;
    if(current == NULL)
        graph[destination]->next = edge;
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = edge;
    }
}


void printPath(int *previous, int source, int destination) {
    if (destination == source) {
        printf("%d", source);
    } else if (previous[destination] == -1) {
        printf("No path from %d to %d\n", source, destination);
    } else {
        printPath(previous, source, previous[destination]);
        printf(", %d", destination);
    }
}

void search(Graph **graph, int vertexes, int source, int destination)
{
    int visited[vertexes];
    int weight[vertexes];
    int previous[vertexes];

    // Initialize arrays
    for (int i = 0; i < vertexes; i++)
    {
        visited[i] = 0;
        weight[i] = INFINITY;
        previous[i] = -1;
    }

    // Initialize priority queue
    int PriorityQueue[vertexes];
    int front = 0, rear = 0;

    // Start from the source node
    PriorityQueue[rear++] = source;
    weight[source] = 0;

    while (front < rear)
    {
        int u = PriorityQueue[front++];

        // Mark the current node as visited
        visited[u] = 1;

        // Traverse through all adjacent vertices of u
        Edge *current = graph[u]->next;
        while (current != NULL)
        {
            int v = current->dest;
            int w = current->weight;

            // If vertex v is not visited and there's a shorter path to v through u
            if (!visited[v] && (weight[u] + w) < weight[v])
            {
                // Update the shortest distance to v
                weight[v] = weight[u] + w;
                previous[v] = u;

                // Enqueue v to the priority queue
                PriorityQueue[rear++] = v;
            }
            current = current->next;
        }
    }

    // Print the shortest path from source to destination
    printf("Weight of route [");
    printPath(previous, source, destination);
    printf("]: %d\n", weight[destination]);
}
void visualizeGraph(Graph **graph, int vertexes)
{
    printf("Graph Visualization:\n");
    for (int i = 0; i < vertexes; i++)
    {
        Edge *current = graph[i]->next;
        printf("%d -> ", i);
        while (current != NULL)
        {
            printf("(%d, %d) ", current->dest, current->weight);
            current = current->next;
        }
        printf("\n");
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
    printf("\n");
    visualizeGraph(graph, vertexes);
    // Process queries
    char DO;
    while (scanf(" %c", &DO) == 1)
    {
        switch (DO)
        {
            case 's':
                scanf(" %d %d", &source, &destination);
                search(graph, vertexes, source, destination);
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