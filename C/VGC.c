#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Games  {
    char title[39];
    char publisher[39];
    int id;
}Games;

int gameregister(Games *p);
int deletelementbyindex(Games *p, int *a);
int deletelementbyid(Games *p, int *a);
void findelementbyid(Games *p, int *a);
void sortbyid(Games *p, int *a);

int main()
{
    int operation = 0;
    int max, j = 0;

    Games games[100];

    printf("Register the games:\n\n");

    max = gameregister(games);

    while(1){
        printf("Please select the operation you wish to make:\n\n");
        printf("\t\t(1)Print the list of games\t (2)Find a game by it's ID\n");
        printf("\t\t(3)Add more games to list\t (4)Remove a game\n");
        printf("\t\t(5)Sort games \t");
        printf("\t\t (0)exit\n");
        scanf("%d", &operation);

        switch(operation)
        {
            case(1):
                for(j = 0; j < max; j++)
                {
                    printf("Title: %-40s", games[j].title);
                    printf("Publisher: %-40s", games[j].publisher);
                    printf("Id: %d\n", games[j].id);
                }
                printf("\n\n");
                break;
            case(2):
                findelementbyid(games, &max);
                break;
            case(3):
                max = max + gameregister(games + max);
                break;
            case(4):
                int deleter = 0;
                printf("\t\t(1)Remove game by id\t (2)Remove game by index\n");
                scanf("%d", &deleter);
                switch(deleter)
                {
                    case(1):
                        deletelementbyid(games, &max);
                    break;
                    case(2):
                        deletelementbyindex(games, &max);
                    break;
            default:
                printf("Invalid operation: %d\a\n", operation);
                break;
                }
                break;
            case(5):
                sortbyid(games, &max);
                for(j = 0; j < max; j++)
                {
                    printf("Title: %-40s", games[j].title);
                    printf("Publisher: %-40s", games[j].publisher);
                    printf("Id: %d\n", games[j].id);
                }
                printf("\n\n");
                break;
            case(0):
                exit(0);
            default:
                printf("Invalid operation: %d \a\n", operation);
                break;
        }
    }
    return 0;
}

int gameregister(Games *p)
{
    printf("How many games would you like to register? You may enter up to 100 games.\n");
    int m = 0;
    scanf("%d", &m);

    if (m > 100 | m < 0)
    {
        printf("Enter a number between 100 and 0\n");
        scanf("%d", &m);
    }
    else
    {
        int i = 0;
        while (i < m)

        {
            printf("Enter the name of the game %d (use _ for spacing)\n", i + 1);
            scanf("%s", (p+i)->title);
            printf("Enter the name of the publisher of game %d (use _ for spacing)\n", i + 1);
            scanf("%s", (p+i)->publisher);
            printf("Enter the id of game %d\n", i + 1);

            scanf("%d", &(p+i)->id);


            i = i + 1;
        }
    }
    return m;
}

int deletelementbyindex(Games *p, int *a)
{
    int position = 0;
    printf("enter the position of the element you wish to remove\n");
    scanf("%d", &position);

    int i = 0;
    if(position >= *a)
    {
        printf("There is no element in entered index.\n");
    }
    else
    {
        for(i=position; i< *a-1 ;i++)
        {
            strcpy((p+i)->title, (p+i+1)->title);
            strcpy((p+i)->publisher, (p+i+1)->publisher);
            (p+i)->id = (p+i+1)->id;
        }
        *a = *a-1;
    }
}

int deletelementbyid(Games *p, int *a)
{
    int enteredid = 0;
    printf("enter the id of the element you wish to remove\n");
    scanf("%d", &enteredid);
    int j = 0;
    int i = 0;
    int check = 0;

    for(j = 0;j< *a; j++)
    {
        if(enteredid == (p+j)->id)
        {
            for(i=j;i< *a;i++)
            {
                strcpy((p+i)->title, (p+i+1)->title);
                strcpy((p+i)->publisher, (p+i+1)->publisher);
                (p+i)->id = (p+i+1)->id;
            }
            *a = *a-1;
            check = check + 1;
        }
    }

    if (check == 0)
    {
            printf("A game with such ID is not in the catalogue\n");
    }
}

void findelementbyid(Games *p, int *a)
{
    int enteredid = 0;
    printf("enter the id of the element you wish to view\n");
    scanf("%d", &enteredid);
    int j = 0;
    int check = 0;

    for(j = 0;j< *a; j++)
    {
        if(enteredid == (p+j)->id)
        {
                printf("Title: %-40s",(p+j)->title);
                printf("Publisher: %-40s",(p+j)->publisher);
                printf("ID: %d\n",(p+j)->id);
                check = check + 1;
        }
    }
    if (check == 0)
    {
        printf("A game with such ID is not in the catalogue\n");
    }
}

void sortbyid(Games *p, int *a)
{
    int swap = 0;
    char swapt[45];
    char swapp[45];
    int i,j = 0;

    for (i = 0; i < *a; i++)
    {
        for (j = 0; j < *a - 1; j++)
        {
            if((p+j)->id > (p+j+1)->id)
            {
                swap = (p+j)->id;
                (p+j)->id = (p+j+1)->id;
                (p+j+1)->id = swap;
                strcpy(swapt, (p+j)->title);
                strcpy((p+j)->title, (p+j+1)->title);
                strcpy((p+j+1)->title, swapt);
                strcpy(swapp, (p+j)->publisher);
                strcpy((p+j)->publisher, (p+j+1)->publisher);
                strcpy((p+j+1)->publisher, swapp);
            }

        }
    }
}
