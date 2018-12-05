#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Games {
    int id;
    char title[30];
    char publisher[30];
    struct Games * next;
} games_t;

void print_list(games_t * head, int m) {
    games_t * current = head;
    if(m == 0)
    {
     printf("No game in the list\n\n\a");
    }
    for(int k = 0; k < m; k++)
    {
        printf("%-40s", current->title);
        printf("%-40s", current->publisher);
        printf("%d \n", current->id);
        current = current->next;
    }
}

int add_to_list(games_t * head)
{
    //games_t * current = head;

    games_t * iter = head;

    int max = 0;
    printf("How many games would you like to enter? \n");
    scanf("%d", &max);

    for(int i = 0; i < max; i++)
    {
        iter->next = malloc(sizeof(games_t));
        //iter = iter->next;
        printf("Enter the title of the game\n");
        scanf("%s", iter->title);
        printf("Enter the publisher of the game\n");
        scanf("%s", iter->publisher);
        printf("Enter the id of the game\n");
        scanf("%d", &iter->id);
        iter = iter->next;
        //current = current->next;
        iter -> next = NULL;
    }
    printf("\n\n");

    return max;
}

int add_to_list_more(games_t * head, int m)
{
   // games_t * current; I came up with this idea to move the current pointer in the other function but
   // I need to initialize it before using it so gg :(
    games_t * current = head;
    for(int c = 0; c < m; c++)
    {
        current = current->next;
    }
    int maxx = 0;
    printf("How many games would you like to enter? \n");
    scanf("%d", &maxx);

    for(int i = 0; i < maxx; i++)
    {
        current->next = malloc(sizeof(games_t));
        //iter = iter->next;
        printf("Enter the title of the game\n");
        scanf("%s", current->title);
        printf("Enter the publisher of the game\n");
        scanf("%s", current->publisher);
        printf("Enter the id of the game\n");
        scanf("%d", &current->id);
        current = current->next;
        current -> next = NULL;
    }
    printf("\n\n");

    return maxx + m;
}

void removebyindex(games_t *head, int m)
{
    //games_t * current = head;

    games_t * iter = head;

    int location = 0;
    printf("Enter the index of the element you wish to remove \n");
    scanf("%d", &location);

    for(int i = 0; i < location; i++)
    {
        iter = iter->next;
    }
    printf("removed the game: \n %-40s", iter ->title);
    printf("%-40s", iter ->publisher);
    printf("%d \n", iter ->id);
    for(int j = location; j < m; j++)
    {
        strcpy(iter ->title, iter -> next -> title);
        strcpy(iter ->publisher, iter -> next -> publisher);
        iter -> id = iter -> next -> id;
    }
}

void findbyid(games_t * head) {
    games_t * current = head;

    int enteredid = 0;
    printf("Enter the ID you wish to view\n");
    scanf("%d", &enteredid);

    while (current -> next!= NULL) {

        if(enteredid == current -> id)
        {
        printf("%-40s", current->title);
        printf("%-40s", current->publisher);
        printf("%d \n", current->id);
        }
        current = current->next;
    }
}

void removebyid(games_t * head, int m)
{
    //games_t * current = head;

    games_t * iter = head;

    int deleteid = 0;
    printf("Enter the id you wish to remove \n");
    scanf("%d", &deleteid);
    int loc_count = 0;
    for(int i = 0; i < m; i++)
    {
        if(deleteid == iter -> id)
        {
            printf("removed the game: \n %-40s", iter ->title);
            printf("%-40s", iter ->publisher);
            printf("%d \n", iter ->id);

            for(int j = loc_count; j < m; j++)
            {
                strcpy(iter ->title, iter -> next -> title);
                strcpy(iter ->publisher, iter -> next -> publisher);
                iter -> id = iter -> next -> id;
            }

        }
        iter = iter->next;
        loc_count = loc_count+1;
    }
}

int main() {
    games_t * game_list = malloc(sizeof(games_t));
    int m, operator = 0;
    printf("Enter the games\n");
    m = add_to_list(game_list);

    while(1)
    {
        printf("Please select the operation you wish to make:\n\n");
        printf("\t\t(1)Print the list of games\t (2)Find a game by it's ID\n");
        printf("\t\t(3)Add more games to list\t (4)Remove a game\n");
        printf("\t\t\t (0)exit\n\n");
        scanf("%d", &operator);
    switch(operator)
    {
        case(1):
            print_list(game_list, m);
            break;
        case(2):
            findbyid(game_list);
            break;
        case(3):
            m = add_to_list_more(game_list, m);
            break;
        case(4):
            int deleter = 0;
            printf("\t\t(1)Remove game by id\t (2)Remove game by index\n");
            scanf("%d", &deleter);
            switch(deleter)
            {
                case (1):
                    removebyid(game_list, m);
                    m = m - 1;
                    break;
                case (2):
                    removebyindex(game_list, m);
                    m = m - 1;
                    break;
                default:
                    printf("Invalid operation: %d\n\a", deleter);
            }
            break;
        case(0):
            printf("\a\n");
            exit(0);
        default:
            printf("Invalid operation: %d\n\a", operator);
    }
    }
//game_list->next = malloc(sizeof(games_t));
//game_list->next->next = malloc(sizeof(games_t));
//game_list->next->next->next = malloc(sizeof(games_t));
//game_list->next->next->next->next = NULL;
}