#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "linked-list.h"

int Member(int value, struct list_node_s *head_p)
{
    struct list_node_s *curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
    {
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int Insert(int value, struct list_node_s **head_pp)
{
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL)
        {
            *head_pp = temp_p;
        }
        else
        {
            pred_p->next = temp_p;
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

int Delete(int value, struct list_node_s **head_pp)
{
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value)
    {
        if (pred_p == NULL)
        {
            *head_pp = curr_p->next;
        }
        else
        {
            pred_p->next = curr_p->next;
        }
        free(curr_p);

        return 1;
    }
    else
    {
        return 0;
    }
}

void LogLinkedList(struct list_node_s **head)
{
    char *filePath[30];
    sprintf(filePath, "logs/linked_list-%ld.log", time(NULL));
    FILE *file = fopen(filePath, "w+");
    if (file != NULL)
    {
        struct list_node_s *curr_p = *head;
        long i = 1;
        while (curr_p != NULL)
        {
            char *str_i = malloc(sizeof(long));
            sprintf(str_i, "%ld", i);

            int data_length = snprintf(NULL, 0, "%d", curr_p->data);
            char *str_data = malloc(data_length);
            sprintf(str_data, "%d", curr_p->data);

            fputs(str_i, file);
            fputs(",", file);
            fputs(str_data, file);
            fputs("\n", file);
            fflush(file);
            free(str_i);
            free(str_data);
            curr_p = curr_p->next;
            i++;
        }
        fclose(file);
        printf("Log file saved at `%s`\n", filePath);
    }
    else
    {
        printf("Logging failed\n");
    }
}
