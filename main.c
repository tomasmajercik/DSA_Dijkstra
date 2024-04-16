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

typedef struct NodeInBinHeap
{
    int vertex;
    int weight;
}NodeInBinHeap;

typedef struct BinHeap
{
    int capacity;
    int actualSize;
    NodeInBinHeap *nodeInBinHeap;
}BinHeap;


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

////// Binary heap functions/////
BinHeap *createBinHeap(int capacity)
{
    BinHeap *heap = (BinHeap *)malloc(sizeof(BinHeap));
    if (heap == NULL)
        exit(1);

    heap->capacity = capacity*2;
    heap->actualSize = 0;
    heap->nodeInBinHeap = (NodeInBinHeap *)malloc(2*capacity * sizeof(NodeInBinHeap));

    return heap;
}
void insertInBinHeap(BinHeap *heap, int vertex, int weight)
{
    int index = heap->actualSize;
    heap->nodeInBinHeap[index].vertex = vertex;
    heap->nodeInBinHeap[index].weight = weight;
    heap->actualSize++;

    // Restore heap
    while (index > 0 && heap->nodeInBinHeap[index].weight < heap->nodeInBinHeap[(index - 1) / 2].weight)
    {   //swap
        NodeInBinHeap temp = heap->nodeInBinHeap[index];
        heap->nodeInBinHeap[index] = heap->nodeInBinHeap[(index - 1) / 2];
        heap->nodeInBinHeap[(index - 1) / 2] = temp;
        index = (index - 1) / 2;
    }
}
void heapify(BinHeap *heap, int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Find the smallest among the current node, its left child, and its right child
    if (left < heap->actualSize && heap->nodeInBinHeap[left].weight < heap->nodeInBinHeap[smallest].weight)
        smallest = left;
    if (right < heap->actualSize && heap->nodeInBinHeap[right].weight < heap->nodeInBinHeap[smallest].weight)
        smallest = right;

    // If the smallest node is not the current node, swap them and continue heapifying
    if (smallest != index)
    {
        NodeInBinHeap temp = heap->nodeInBinHeap[index];
        heap->nodeInBinHeap[index] = heap->nodeInBinHeap[smallest];
        heap->nodeInBinHeap[smallest] = temp;
        heapify(heap, smallest);
    }
}
NodeInBinHeap extractMin(BinHeap *heap)
{
    // Extract the minimum node
    NodeInBinHeap minNode = heap->nodeInBinHeap[0];
    heap->nodeInBinHeap[0] = heap->nodeInBinHeap[heap->actualSize - 1];
    heap->actualSize--;

    // Restore heap property
    heapify(heap, 0);

    return minNode;
}
void deleteFromBinHeap(BinHeap *heap, int vertex)
{
    // Replace the node to be deleted with the last node
    heap->nodeInBinHeap[vertex] = heap->nodeInBinHeap[heap->actualSize - 1];
    heap->actualSize--;

    // Restore heap property
    heapify(heap, vertex);
}
void destroyBinaryHeap(BinHeap* heap)
{
    free(heap->nodeInBinHeap);
    free(heap);
}



//// graph functions
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
    if (source == destination)
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

    // Create a binary heap and insert the source vertex with weight 0
    BinHeap *heap = createBinHeap(vertexes);
    insertInBinHeap(heap, source, 0);

    while (heap->actualSize > 0)
    {
        // Extract the vertex with the minimum weight from the binary heap
        NodeInBinHeap minNode = extractMin(heap);
        int minVertex = minNode.vertex;
        visited[minVertex] = 1;

        // Break out of the loop if the destination vertex is reached
        if (minVertex == destination)
            break;

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

                // Update the weight of vertex v in the binary heap
                insertInBinHeap(heap, v, weight[v]);
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

    // Destroy the binary heap
    destroyBinaryHeap(heap);
}


void ssearch(Graph **graph, int vertexes, int source, int destination, int *newline)
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
    BinHeap *heap = createBinHeap(vertexes);

    // Insert edges
    for (int i = 0; i < edges; i++)
    {
        scanf(" (%d , %d , %d)", &source, &destination, &weight);
        insertEdge(graph, source, destination, weight, &newLine);
        // visualizeGraph(graph, vertexes);
    }
    // Process queries
    char DO;
    while (scanf(" %c", &DO) == 1)
    {
        switch (DO)
        {
            case 's':
                scanf(" %d %d", &source, &destination);
//                visualizeGraph(graph, vertexes);
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

    //// destroy elements ////
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
    free(graph);
    free(heap);
    return 0;
}