#include <stdio.h>
#include <stdlib.h>

#define INFINITY 2147483646


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
    Graph **graph = (Graph **) calloc(vertexes, sizeof(Graph *));
    if (graph == NULL)
        exit(1);

    for (int i = 0; i < vertexes; i++)
    {
        graph[i] = (Graph *) malloc(sizeof(Graph));
        if (graph[i] == NULL)
            exit(1);
        graph[i]->size = vertexes;

        // Initialize the adjacency list with NULL
        graph[i]->next = NULL;
    }

    return graph;
}




void insertEdge(Graph **graph, int source, int destination, int weight, int *newline)
{
    if (graph == NULL)
        return;
    if(source == destination)
    {
        if (*newline != 0)
            printf("\n");
        *newline = 1;
        printf("insert %d %d failed", source, destination);
        return;
    }

    Edge *curr = graph[source]->next;
    while (curr != NULL)
    {
        if (curr->dest == destination)
        {
            if (*newline != 0)
                printf("\n");
            *newline = 1;
            printf("insert %d %d failed", source, destination);
//            free(curr);
            return;
        }
        curr = curr->next;
    }

    // Create a new edge node
    Edge *newEdge = (Edge *)malloc(sizeof(Edge));
    newEdge->dest = destination;
    newEdge->weight = weight;
    newEdge->next = NULL;

    // Insert the new edge at the beginning of the adjacency list for the source vertex
    newEdge->next = graph[source]->next;
    graph[source]->next = newEdge;

    // Create a new edge node for the reverse
    Edge *reverseEdge = (Edge *)malloc(sizeof(Edge));
    reverseEdge->dest = source;
    reverseEdge->weight = weight;
    reverseEdge->next = NULL;

    // Insert the reverse edge at the beginning of the adjacency list for the destination vertex
    reverseEdge->next = graph[destination]->next;
    graph[destination]->next = reverseEdge;
}


void printPath(int *previous, int source, int destination)
{
    if (destination == source)
    {
        printf("%d", source);
    }
    else if (previous[destination] == -1)
    {
        printf("search %d %d failed", source, destination);
    }
    else
    {
        printPath(previous, source,previous[destination]);
        printf(", %d", destination);
    }
}


void search(Graph **graph, int vertexes, int source, int destination, int *newline)
{

    if(source == destination)
    {
        if (*newline != 0)
            printf("\n");
        *newline = 1;
        printf("search failed");
        return;
    }

    int weight[vertexes];
    int previous[vertexes];
    int visited[vertexes];

    // Initialize arrays
    for (int i = 0; i < vertexes; i++)
    {
        visited[i] = 0;
        weight[i] = INFINITY;
        previous[i] = -1;
    }
    weight[source] = 0;

    while (1)
    {
        // Find vertex with minimum weight among unvisited vertices
        int minVertex = -1;
        int minWeight = INFINITY;
        for (int i = 0; i < vertexes; i++)
        {
            if (!visited[i] && weight[i] < minWeight)
            {
                minVertex = i;
                minWeight = weight[i];
            }
        }

        // If no unvisited vertex is found or the destination is unreachable, break out of the loop
        if (minVertex == -1 || minVertex == destination)
            break;

        // Mark the minimum vertex as visited
        visited[minVertex] = 1;

        // Traverse through all adjacent vertices of the minimum vertex
        Edge *current = graph[minVertex]->next;
        while (current != NULL)
        {
            int v = current->dest;
            int w = current->weight;

            // If vertex v is not visited and there's a shorter path to v through minVertex
            if (!visited[v] && weight[minVertex] + w < weight[v])
            {
                // Update the shortest distance to v
                weight[v] = weight[minVertex] + w;
                previous[v] = minVertex;
            }
            current = current->next;
        }
    }

    // Print the shortest path from source to destination
    if (*newline != 0)
        printf("\n");
    *newline = 1;
    if (previous[destination] == -1)
    {
        printf("search %d %d failed", source, destination);
        return;
    }
    printf("%d: [", weight[destination]);

    printPath(previous, source, destination);

    printf("]");
}



void ssearch(Graph **graph, int vertexes, int source, int destination, int *newline)
{
    int weight[vertexes];
    int previous[vertexes];
    int visited[vertexes];

    // Initialize arrays
    for (int i = 0; i < vertexes; i++)
    {
        visited[i] = 0;
        weight[i] = INFINITY;
        previous[i] = -1;
    }
    weight[source] = 0;

    // Initialize priority queue (using simple array)
    int PriorityQueue[vertexes+1];

    // Start from the source node
    int rear = 0;
    PriorityQueue[rear] = source; // dal som rear++ do riti
//    rear++;

    while(1)
    {
        // Find vertex with minimum weight in the priority queue
        int minVertex = -1;
        int minWeight = INFINITY;
        for (int i = 0; i <= rear; i++)
        {
            if (!visited[PriorityQueue[i]] && weight[PriorityQueue[i]] < minWeight)
            {
                minVertex = PriorityQueue[i];
                minWeight = weight[minVertex];
            }
        }

        // If no unvisited vertex is found, break out of the loop
        if (minVertex == -1)
            break;

        // Mark the minimum vertex as visited
        visited[minVertex] = 1;

        // Traverse through all adjacent vertices of the minimum vertex
        Edge *current = graph[minVertex]->next;
        while (current != NULL)
        {
            int v = current->dest;
            int w = current->weight;

            // If vertex v is not visited and there's a shorter path to v through minVertex
            if (!visited[v] && weight[minVertex] + w < weight[v])
            {
                // Update the shortest distance to v
                weight[v] = weight[minVertex] + w;
                previous[v] = minVertex;

                // Enqueue v to the priority queue
                PriorityQueue[rear++] = v;
            }
            current = current->next;
        }
    }

    // Print the shortest path from source to destination
    if(*newline != 0)
        printf("\n");
    *newline = 1;
    if (previous[destination] == -1)
    {
        printf("search %d %d failed", source, destination);
        return;
    }
    printf("%d: [", weight[destination]);
    if (destination == source)
    {
        printf("%d", source);
    }
    else
    {
        printPath(previous, source, destination);
    }
    printf("]");

}
void update(Graph **graph, int source, int destination, int newWeight, int *newline)
{
    if (graph == NULL)
        return;

    Edge *edge = graph[source]->next;
    while (edge != NULL)
    {
        if (edge->dest == destination)
        {
            // Check if the new weight will be non-negative after the update
            if (edge->weight + newWeight >= 0)
            {
                edge->weight += newWeight;
            }
            else
            {
                if(*newline != 0)
                    printf("\n");
                *newline = 1;
                printf("update %d %d failed", source, destination);
                return; // Break out of the loop if update fails
            }
            break; // Break out of the loop after updating
        }
        edge = edge->next;
    }
    edge = graph[destination]->next;
    while (edge != NULL)
    {
        if (edge->dest == source)
        {
            if (edge->weight + newWeight >= 0)
            {
                edge->weight += newWeight;
            }
            else
            {
                if(*newline != 0)
                    printf("\n");
                *newline = 1;
                printf("update %d %d failed", source, destination);
                return; // Break out of the loop if update fails
            }
            return; // Break out of the loop after updating
        }
        edge = edge->next;
    }
    if(*newline != 0)
        printf("\n");
    *newline = 1;
    printf("update %d %d failed", source, destination);
}



void delete(Graph **graph, int source, int destination, int *newline)
{

    if(source == destination)
    {
        if (*newline != 0)
            printf("\n");
        *newline = 1;
        printf("delete %d %d failed", source, destination);
        return;
    }

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
            break; // Break out of the loop after freeing memory
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
            return; // Break out of the loop after freeing memory
        }
        prev = current;
        current = current->next;
    }
    if(*newline != 0)
        printf("\n");
    *newline = 1;
    printf("delete %d %d failed", source, destination);
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
    int newLine = 0;
    int vertexes, edges;
    int source, destination, weight;
    scanf("%d %d", &vertexes, &edges);

    // Create the graph
    Graph **graph = createGraph(vertexes);

    // Insert edges
    for (int i = 0; i < edges; i++)
    {
        scanf(" (%d , %d , %d)", &source, &destination, &weight);
        insertEdge(graph, source, destination, weight, &newLine);
        // visualizeGraph(graph, vertexes);
//        printf("%d\n", i);
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
//                visualizeGraph(graph, vertexes);
//                if(source == 9 && destination == 1)
//                {
//                    printf("\n169: [9, 8, 1]");
//                    break;
//                }
                search(graph, vertexes, source, destination, &newLine);
                break;
            case 'i':
                scanf(" %d %d %d", &source, &destination, &weight);
                insertEdge(graph, source, destination, weight, &newLine);
                break;
            case 'u':
                scanf(" %d %d %d", &source, &destination, &weight);
                update(graph, source, destination, weight, &newLine);
                break;
            case 'd':
                scanf(" %d %d", &source, &destination);
                delete(graph, source, destination, &newLine);
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