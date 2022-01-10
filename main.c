#include <stdio.h>
#include "graph.h"

int main()
{
    pnode hd = NULL;
    char menu;
    int src = 0;
    int dst, weight;

    scanf(" %c", &menu);
    while (menu != 'E')
    {
        if (menu == 'A')
        {
            deleteGraph_cmd(&hd); //delete existing graph
            hd = NULL;
            scanf(" %d", &src);
            scanf(" %c", &menu); //expects an 'n'

            while (menu == 'n')
            {
                scanf(" %d", &src);    //node id
                create_node(&hd, src); //create node and add edges until encouters an n;
                while (1)              //each iteration adds a new edge;
                {

                    if (scanf(" %d", &dst) == 0) //meaning we get either an n or a new menu charcter, then stop adding edges;
                    {
                        scanf(" %c", &menu); //if we get n we will continue to add another node, else we will exit the outer loop
                        break;
                    }
                    scanf(" %d", &weight);
                    add_edge(&hd, src, dst, weight);
                }
            }
        }
        else if (menu == 'B')
        {
            scanf(" %d", &src);    //node id
            create_node(&hd, src); //create node and add edges until encouters a char;
            while (1)              //each iteration adds a new edge;
            {
                if (scanf(" %d", &dst) == 0) //meaning we get a new menu charcter, then stop adding edges;
                {
                    scanf(" %c", &menu); //we will exit the outer loop
                    break;
                }
                scanf(" %d", &weight);
                add_edge(&hd, src, dst, weight);
            }
        }
        else if (menu == 'D')
        {
            scanf(" %d", &src);
            delete_node(&hd, src);
            scanf(" %c", &menu);
        }
        else if (menu == 'S')
        {
            scanf(" %d", &src);
            scanf(" %d", &dst);
            printf("Dijsktra shortest path: %d\n", shortsPath_cmd(hd, src, dst));
            scanf(" %c", &menu);
        }
        else if (menu == 'T')
        {   
            int arr[ARRAY_SIZE];
            for (int i = 0; i < ARRAY_SIZE; i++)
            {
                arr[i] = -1;
            }
            int *temp = arr;
            int counter = 0;
            while (counter < ARRAY_SIZE && menu == 'T') //each iteration adds a new edge;
            {
                int scanned = scanf(" %d", &dst);
                if (scanned == 0) //meaning we get a new menu charcter, then stop adding edges;
                {
                    scanf(" %c", &menu); //we will exit the outer loop
                    break;
                }
                if (scanned ==1 )
                {
                    arr[counter] = dst;
                    temp++;
                    counter++;
                }
                
            }
            int temp_arr[ARRAY_SIZE];
            for (int i = 0; i < ARRAY_SIZE; i++)
            {
                temp_arr[i] = -1;
            }

            for (int i = 0; i < ARRAY_SIZE; i++)
            {
                for (int j = 0; j < ARRAY_SIZE; j++)
                {
                    if(arr[i] == arr[j] && i!=j && arr[i]!= -1 && arr[j] != -1)
                    {
                        arr[i] = -1;
                        counter--;
                    }
                }
            }
            for (int i = 0,j = 0; i < ARRAY_SIZE; i++)
            {
                if(arr[i]!=-1)
                {
                    temp[j++] = arr[i];
                }
            }
            
            TSP_cmd(hd,temp_arr,counter);
        }
        else if (menu == 'Z')
        {
            printGraph_cmd(hd);
            scanf(" %c",&menu);
            break;
        }
    }
    // printGraph_cmd(hd);
    deleteGraph_cmd(&hd);

    return 0;
}