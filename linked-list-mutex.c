#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

struct list_node_s
{
    int data;
    struct list_node_s *next;
};

int n, m, thread_count;

float m_member, m_insert, m_delete, mMember, mInsert, mDelete;

pthread_mutex_t mutex;

struct list_node_s *head = NULL;

int count_member = 0;
int count_insert = 0;
int count_delete = 0;

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

void *StartRoutine()
{
    int rand_upper = pow(2, 16) - 1;

    int finished_member = 0;
    int finished_insert = 0;
    int finished_delete = 0;

    while (count_insert + count_member + count_delete < m)
    {
        int random_value = rand() % rand_upper;
        int random_select = rand() % 3;

        if (random_select == 0 && finished_member == 0)
        {
            pthread_mutex_lock(&mutex);
            if (count_member < mMember)
            {
                Member(random_value, head);
                count_member++;
            }
            else
            {
                finished_member = 1;
            }
            pthread_mutex_unlock(&mutex);
        }

        else if (random_select == 1 && finished_insert == 0)
        {

            pthread_mutex_lock(&mutex);
            if (count_insert < mInsert)
            {
                Insert(random_value, &head);
                count_insert++;
            }
            else
            {
                finished_insert = 1;
            }
            pthread_mutex_unlock(&mutex);
        }

        else if (random_select == 2 && finished_delete == 0)
        {

            pthread_mutex_lock(&mutex);
            if (count_delete < mDelete)
            {
                Delete(random_value, &head);
                count_delete++;
            }
            else
            {
                finished_delete = 1;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

void LogLinkedList()
{
    char *filePath[30];
    sprintf(filePath, "logs/linked_list-%ld.log", time(NULL));
    FILE *file = fopen(filePath, "w+");
    if (file != NULL)
    {
        struct list_node_s *curr_p = head;
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
        printf("Log saved at `%s`\n", filePath);
    }
    else
    {
        printf("Log failed\n");
    }
}

int main(int argc, char *argv[])
{
    printf("Enter values for n, m, thread_count, m_member, m_insert, m_delete \n(Use `Enter` or `Space` to seperate values\n:");
    scanf("%d %d %d %f %f %f", &n, &m, &thread_count, &m_member, &m_insert, &m_delete);

    if (n <= 0)
    {
        printf("Invalid `n` value\n");
        exit(0);
    }

    if (m <= 0)
    {
        printf("Invalid `m` value\n");
        exit(0);
    }

    if (thread_count <= 0 && thread_count > 8)
    {
        printf("Invalid thread count value. (0<thread_count<=8)\n");
        exit(0);
    }

    if (m_member + m_insert + m_delete != 1.0)
    {
        printf("Invalid fraction values\n");
        exit(0);
    }

    printf("\nn= %d\nm= %d\nthread_count= %d\nm_member= %f\nm_insert= %f\nm_delete= %f\n", n, m, thread_count, m_member, m_insert, m_delete);

    struct timeval time_begin, time_end;

    pthread_t *thread_handlers;
    thread_handlers = malloc(sizeof(pthread_t) * thread_count);

    int rand_upper = pow(2, 16) - 1;
    int i = 0;
    srand(time(0));
    while (i < n)
    {
        if (Insert(rand() % rand_upper, &head) == 1)
        {
            i++;
        }
    }

    mMember = m_member * m;
    mInsert = m_insert * m;
    mDelete = m_delete * m;

    int thread;
    gettimeofday(&time_begin, NULL);
    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handlers[thread], NULL, &StartRoutine, NULL);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handlers[thread], NULL);
    }
    gettimeofday(&time_end, NULL);

    double time_diff = (double)(time_end.tv_usec - time_begin.tv_usec) / 1000000 + (double)(time_end.tv_sec - time_begin.tv_sec);
    printf("\nTime Spent : %.6f secs\n", time_diff);
    LogLinkedList();

    return 0;
}
