#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include "linked-list.h"

int n, m, thread_count, rand_upper;

float m_member, m_insert, m_delete, mMember, mInsert, mDelete;

pthread_mutex_t mutex;

struct list_node_s *head = NULL;

int count_member = 0;
int count_insert = 0;
int count_delete = 0;

void *StartRoutine()
{
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

    rand_upper = pow(2, 16) - 1;
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
    LogLinkedList(&head);

    return 0;
}
