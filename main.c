#include <stdio.h>
#include <stdlib.h>

//#define INFINITY 2147483646
#define INFINITY 2000


typedef struct Edge
{
    int parent;
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

    Edge *curr = graph[source]->next;
    while (curr != NULL)
    {
        if (curr->dest == destination)
        {
            // Edge already exists, update its weight
            curr->weight = weight;
            return;
        }
        curr = curr->next;
    }

    // Create a new edge node
    Edge *newEdge = (Edge *) malloc(sizeof(Edge));
    newEdge->dest = destination;
    newEdge->weight = weight;
    newEdge->next = NULL;
    newEdge->parent = -1;

    // Find the last edge in the adjacency list of the source vertex
    Edge *current = graph[source]->next;
    if (current == NULL)
    {
        // If the adjacency list is empty, make the new edge the first edge
        graph[source]->next = newEdge;
    } else
    {
        // Traverse the adjacency list until the last edge
        while (current->next != NULL)
        {
            current = current->next;
        }
        // Append the new edge to the end of the list
        current->next = newEdge;
    }

    Edge *edge = (Edge *) malloc(sizeof(edge));
    edge->dest = source;
    edge->weight = weight;
    edge->next = NULL;
    edge->parent = -1;

    current = graph[destination]->next;
    if (current == NULL)
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


void printPath(int *previous, int source, int destination)
{
    if (destination == source)
    {
        printf("%d", source);
    }
    else if (previous[destination] == -1)
    {
        printf("search failed");
    }
    else
    {
        printPath(previous, source, previous[destination]);
        printf(", %d", destination);
    }
}
void search(Graph **graph, int vertexes, int source, int destination)
{
    int visited[vertexes];
    int weight[vertexes];

    // Initialize arrays
    for (int i = 0; i < vertexes; i++)
    {
        visited[i] = 0;
        weight[i] = INFINITY;
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

        if (graph[u]->next == NULL)
        {
            continue; // Skip to the next iteration if there are no edges for u
        }

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
                graph[v]->next->parent = u;  // Update parent of v
                PriorityQueue[rear++] = v;
            }
            current = current->next;
        }
    }

    // Print the shortest path from source to destination (no need for printPath)
    int current = destination;
    if (destination == source) {
        printf("%d", source);
    } else if (graph[destination]->next->parent == -1) {
        printf("search failed");
    } else {
        printf("%d: [", weight[destination]);
        while (current != source) {
            printf("%d, ", current);
            current = graph[current]->next->parent;
        }
        printf("%d]\n", source);
    }
}

void ssearch(Graph **graph, int vertexes, int source, int destination)
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
    printf("%d: [", weight[destination]);
    if (destination == source)
    {
        printf("%d", source);
    }
    else if (previous[destination] == -1)
    {
        printf("search failed");
    }
    else
    {
        printPath(previous, source, previous[destination]);
        printf(", %d", destination);
    }
    printf("]\n");
}

void update(Graph **graph, int source, int destination, int newWeight)
{
    if (graph == NULL)
        return;

    Edge *edge = graph[source]->next;
    while (edge != NULL)
    {
        if (edge->dest == destination)
        {
            if ((edge->weight + newWeight) >= 0)
            {
                edge->weight = newWeight;
                return;
            }
            else
            {
                printf("update %d %d failed", source, destination);
            }
        }
        edge = edge->next;
    }
}


void delete(Graph **graph, int source, int destination)
{
    if (graph == NULL)
        return;

    Edge *prev = NULL;
    Edge *current = graph[source]->next;

    while (current != NULL)
    {
        if (current->dest == destination)
        {
            if (prev == NULL) // If the edge is the first in the list
                graph[source]->next = current->next;
            else
            {
                prev->next = current->next;
            }
            free(current);
            break; // Break after deleting the edge from source to destination
        }
        prev = current;
        current = current->next;
    }

    // Now, delete the corresponding edge from destination to source
    prev = NULL;
    current = graph[destination]->next;

    while (current != NULL)
    {
        if (current->dest == source)
        {
            if (prev == NULL) // If the edge is the first in the list
                graph[destination]->next = current->next;
            else
            {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("delete failed");
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
//    printf("\n");
//    visualizeGraph(graph, vertexes);
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
            case 'i':
                scanf(" %d %d %d", &source, &destination, &weight);
                insertEdge(graph, source, destination, weight);
                break;
            case 'u':
                scanf(" %d %d %d", &source, &destination, &weight);
                update(graph, source, destination, weight);
                break;
            case 'd':
                scanf(" %d %d", &source, &destination);
                delete(graph, source, destination);
                break;
            default:
                printf("huh?");
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