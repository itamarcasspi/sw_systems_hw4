#ifndef GRAPH_
#define GRAPH_
#include <stdlib.h>
#define ARRAY_SIZE 16

typedef struct GRAPH_NODE_ *pnode;;

typedef struct edge_ {
    int weight;
    pnode endpoint;
    struct edge_ *next;
} edge, *pedge;


typedef struct GRAPH_NODE_ {
    int node_num;
    pedge edges;
    struct GRAPH_NODE_ *next;
} node, *pnode;


pnode find_node(pnode head,int id);

int biggest_nodes(pnode head);

void add_edge(pnode *head,int src, int dst,int weight);

int delete_edge(pnode *head,int src, int dest);

void delete_node(pnode *head,int id);

void create_node(pnode *head,int id);

void printGraph_cmd(pnode head); //for self debug

void deleteGraph_cmd(pnode* head);

int shortsPath_cmd(pnode head,int node_a,int node_b);

int TSP_cmd(pnode head,int input[],int size);


#endif
