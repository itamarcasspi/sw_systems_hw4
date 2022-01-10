#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_VAL_LENGTH 10
#define INF 200000000

int count_nodes(pnode head)
{
    int counter = 0;
    while (head != NULL)
    {
        counter++;
        head = head->next;
    }
    return counter;
}

pnode find_node(pnode head, int id)
{
    while (head != NULL)
    {
        if (head->node_num == id)
        {
            return head;
        }
        head = head->next;
    }

    return NULL;
}

void delete_related_edge(pnode *head,int dest)
{
    pnode n = head;
    while (n != NULL)
    {
        pedge e = n->edges;
        pedge p = e;
        while(e != NULL)
        {
            if(e->endpoint->node_num == dest)
            {
                p->next = e->next;
                delete_edge(head,n->node_num,dest);
                e = p;
            }
            else
            {
                p = e;
                e = e->next;
            }
        }
        n = n->next;
    }
}

int biggest_node(pnode head)
{
    int max = 0;
    while (head != NULL)
    {
        if (max < head->node_num)
        {
            max = head->node_num;
        }
        head = head->next;
    }
    return max;
}

void create_node(pnode *head, int id)
{
    //if node exists, do nothing
    if (find_node(*head, id) != NULL)
    {
        return;
    }
    pnode new_node = malloc(sizeof(node));
    // new_node->node_num = (int)malloc(sizeof(int));
    new_node->node_num = id;
    new_node->edges = NULL;
    new_node->next = NULL;
    pnode curr_node = *head;

    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        while (curr_node->next != NULL)
        {
            curr_node = curr_node->next;
        }
        curr_node->next = new_node;
    }
}

void add_edge(pnode *head, int src, int dst, int weight)
{
    //create new edge
    pnode source = find_node(*head, src);
    if (source == NULL)
    {
        perror("Could not find source to create edge");
        exit(1);
    }
    pedge new_edge = malloc(sizeof(edge));
    new_edge->weight = weight;
    new_edge->next = NULL;

    pnode destination = find_node(*head, dst);
    //if no destination node found, create one
    if (destination == NULL)
    {
        create_node(head, dst);
        destination = find_node(*head, dst);
    }
    new_edge->endpoint = destination;

    //if the source node has no existing edge, just point to the new edge
    if (source->edges == NULL)
    {
        source->edges = new_edge;
        return;
    }

    pedge curr_edge = source->edges;
    //find the last edge in the list
    while (curr_edge->next != NULL)
    {
        //if we found an existing edge with the same dest just change the weight and free the new edge
        if (curr_edge->endpoint->node_num == dst)
        {
            curr_edge->weight = weight;
            free(new_edge);
            return;
        }
        curr_edge = curr_edge->next;
    }
    //if the last edge in the list has the same dst, just change the weight and free the new edge we created
    if (curr_edge->endpoint->node_num == dst)
    {
        curr_edge->weight = weight;
        free(new_edge);
        return;
    }
    curr_edge->next = new_edge;
}

int delete_edge(pnode *head, int src, int dest)
{
    pnode source = find_node(*head, src);
    if (source == NULL)
    {
        printf("Cant delete none existing edge of src = %d and dest = %d\n", src, dest);
        exit(1);
    }
    pedge prev, current;
    current = source->edges;
    prev = current;
    //if the first edge of the node is the desired one
    if (source->edges->endpoint->node_num == dest)
    {
        pedge temp = source->edges;
        source->edges = source->edges->next;
        free(temp);
        // source->edges = NULL;
        return 1;
    }

    while (current->next != NULL)
    {
        if (current->endpoint->node_num == dest)
        {
            prev->next = current->next;
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    if (current->endpoint->node_num == dest)
    {
        prev->next = NULL;
        free(current);
        return 1;
    }
    return 0;
}

void delete_node(pnode *head, int id)
{
    pnode prev;
    pnode current = *head;
    // delete_related_edge(head,id);
    if (current->node_num == id)
    {
        if ((*head)->edges != NULL)
        {
            while ((*head)->edges->next != NULL)
            {
                pedge temp = (*head)->edges;
                (*head)->edges = (*head)->edges->next;
                free(temp);
            }
            free((*head)->edges);
        }
        *head = current->next;
        free(current);
        return;
    }
    while (current != NULL)
    {
        if (current->node_num == id)
        {

            if (current->edges != NULL)
            {
                while (current->edges->next != NULL)
                {
                    pedge temp = current->edges;
                    current->edges = current->edges->next;
                    free(temp);
                }
                free(current->edges);
            }
            prev->next = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Error in node[%d] deletion as it does not exist", id);
    exit(1);
}

void printGraph_cmd(pnode head)
{
    printf("Graph representation [src]---(weight)--->[dest]\n");
    while (head != NULL)
    {
        pedge current_edge = head->edges;
        if (current_edge == NULL)
        {
            printf("[%d]\n", head->node_num);
        }
        while (current_edge != NULL)
        {
            printf("[%d]---(%d)--->[%d]\n", head->node_num, current_edge->weight, current_edge->endpoint->node_num);
            current_edge = current_edge->next;
        }
        head = head->next;
    }
}

void deleteGraph_cmd(pnode *head)
{
    while (*head != NULL)
    {
        delete_node(head, (*head)->node_num);
    }
}

int shortsPath_cmd(pnode head, int node_a, int node_b)
{

    int num_of_nodes = biggest_node(head) + 1;
    int dist[num_of_nodes][num_of_nodes];

    for (int i = 0; i < num_of_nodes; i++)
    {
        for (int j = 0; j < num_of_nodes; j++)
        {
            dist[i][j] = INF;
        }
        dist[i][i] = 0;
    }

    pnode current_node = head;
    while (current_node != NULL)
    {
        pedge current_edge = current_node->edges;
        while (current_edge != NULL)
        {
            dist[current_node->node_num][current_edge->endpoint->node_num] = current_edge->weight;
            current_edge = current_edge->next;
        }
        current_node = current_node->next;
    }

    for (int k = 0; k < num_of_nodes; k++)
    {
        for (int i = 0; i < num_of_nodes; i++)
        {
            for (int j = 0; j < num_of_nodes; j++)
            {
                if (dist[i][j] > dist[i][k] + dist[k][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    if (dist[node_a][node_b] >= INF || node_a == node_b)
    {
        return -1;
    }
    return dist[node_a][node_b];
}

// void swap (char *x, char *y)

// {
//     char temp;
//     temp = *x;
//     *x = *y;
//     *y = temp;
// }

// void permute(char *a, int i, int n, int *permutation_matrix, int size)
// {
//     int j;
//     if (i == n)
//     {
//         // while (permutation_matrix!=0)
//         // {
//         //     permutation_matrix+=size;
//         // }
//         printf("%s\n",a);
//     }
//     else {
//         for (j = i; j <= n; j++)
//         {
//             swap((a + i), (a + j));
//             permute(a, i + 1, n,permutation_matrix,size);
//             swap((a + i), (a + j)); 
//         }
//     }
// }
// int factorial(int in)
// {
//     if(in==1)
//     {
//         return 1;
//     }
//     return in*factorial(in-1);
// }

// int TSP_cmd(pnode head,char *input)
// {
//     int num_of_nodes = biggest_node(head) + 1;
//     int dist[num_of_nodes][num_of_nodes];
//     for (int i = 0; i < num_of_nodes; i++)
//     {
//         for (int j = 0; j < num_of_nodes; j++)
//         {
//             dist[i][j] = INF;
//         }
//         dist[i][i] = 0;
//     }
//     pnode current_node = head;
//     while (current_node != NULL)
//     {
//         pedge current_edge = current_node->edges;
//         while (current_edge != NULL)
//         {
//             dist[current_node->node_num][current_edge->endpoint->node_num] = current_edge->weight;
//             current_edge = current_edge->next;
//         }
//         current_node = current_node->next;
//     }

//     for (int k = 0; k < num_of_nodes; k++)
//     {
//         for (int i = 0; i < num_of_nodes; i++)
//         {
//             for (int j = 0; j < num_of_nodes; j++)
//             {
//                 if (dist[i][j] > dist[i][k] + dist[k][j])
//                 {
//                     dist[i][j] = dist[i][k] + dist[k][j];
//                 }
//             }
//         }
//     }
//     int str_size = strlen(input);
//     int num_of_permutations = factorial(str_size);
//     char permutation_matrix[num_of_permutations][str_size];
//     memset(permutation_matrix,0,num_of_permutations*str_size);
//     permute(input,0,str_size-1,permutation_matrix,str_size);
//     // for (int i = 0; i < num_of_permutations; i+=str_size)
//     // {
//     //     for (int j = 0; j < str_size; j++)
//     //     {
//     //         printf("%d",**(permutation_matrix+j));
//     //     }
//     //     printf("\n");

//     // }
    
    
// }

void swap (int *x, int *y)

{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void permute(int *a, int i, int n, int *permutation_matrix, int size)
{
    int j;
    if (i == n)
    {
        while (*permutation_matrix!=-1)
        {
            permutation_matrix+=size;
        }
        for (int i = 0; i < size; i++)
        {
            *permutation_matrix = *(a+i);
            permutation_matrix++;
        }        
    }
    else {
        for (j = i; j <= n; j++)
        {
            swap((a + i), (a + j));
            permute(a, i + 1, n,permutation_matrix,size);
            swap((a + i), (a + j)); 
        }
    }
}
int factorial(int in)
{
    if(in==1)
    {
        return 1;
    }
    return in*factorial(in-1);
}

int TSP_cmd(pnode head,int input[],int size)
{
    int num_of_nodes = biggest_node(head) + 1;
    int dist[num_of_nodes][num_of_nodes];
    for (int i = 0; i < num_of_nodes; i++)
    {
        for (int j = 0; j < num_of_nodes; j++)
        {
            dist[i][j] = INF;
        }
        dist[i][i] = 0;
    }
    pnode current_node = head;
    while (current_node != NULL)
    {
        pedge current_edge = current_node->edges;
        while (current_edge != NULL)
        {
            dist[current_node->node_num][current_edge->endpoint->node_num] = current_edge->weight;
            current_edge = current_edge->next;
        }
        current_node = current_node->next;
    }

    for (int k = 0; k < num_of_nodes; k++)
    {
        for (int i = 0; i < num_of_nodes; i++)
        {
            for (int j = 0; j < num_of_nodes; j++)
            {
                if (dist[i][j] > dist[i][k] + dist[k][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    int str_size = size;
    int num_of_permutations = factorial(str_size);
    int permutation_matrix[num_of_permutations][str_size];    
    
    for (int i = 0; i < num_of_permutations; i++)
    {
        for (int j = 0; j < str_size; j++)
        {
            permutation_matrix[i][j] = -1;
        }
    }
    permute(input,0,str_size-1,permutation_matrix,str_size);
    int best_route = INF;
    for (int i = 0; i < num_of_permutations; i++)
    {
        int current_route_cost = 0;
        for (int j = 0; j < size-1; j++)
        {
            if(dist[permutation_matrix[i][j]][permutation_matrix[i][j+1]] >0)
            {
                current_route_cost += dist[permutation_matrix[i][j]][permutation_matrix[i][j+1]];
            }
        }
        if(current_route_cost<best_route && current_route_cost != 0)
        {
            best_route = current_route_cost;
        }
    }
    printf("TSP shortest path: %d\n",best_route);
    return 0;
}