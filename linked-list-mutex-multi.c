#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include "linked-list.h"

int n, m, thread_count, rand_upper, m_member, m_insert, m_delete;

float m_member_fraction, m_insert_fraction, m_delete_fraction;

pthread_mutex_t mutex;
pthread_mutex_t mutex_count_member;
pthread_mutex_t mutex_count_insert;
pthread_mutex_t mutex_count_delete;

struct list_node_s *head = NULL;

int count_member = 0;
int count_insert = 0;
int count_delete = 0;

void *StartRoutine()
{
    int finished_member = 0;
    int finished_insert = 0;
    int finished_delete = 0;

    while (finished_member == 0 || finished_insert == 0 || finished_delete == 0)
    {
        int random_value = rand() % rand_upper;
        int random_select = rand() % 3;

        if (random_select == 0 && finished_member == 0)
        {
            pthread_mutex_lock(&mutex_count_member);
            if (count_member < m_member)
            {
                count_member++;
                pthread_mutex_unlock(&mutex_count_member);
                pthread_mutex_lock(&mutex);
                Member(random_value, head);
                pthread_mutex_unlock(&mutex);
            }
            else
            {
                pthread_mutex_unlock(&mutex_count_member);
                finished_member = 1;
            }
        }

        else if (random_select == 1 && finished_insert == 0)
        {
            pthread_mutex_lock(&mutex_count_insert);
            if (count_insert < m_insert)
            {
                count_insert++;
                pthread_mutex_unlock(&mutex_count_insert);
                pthread_mutex_lock(&mutex);
                Insert(random_value, &head);
                pthread_mutex_unlock(&mutex);
            }
            else
            {
                pthread_mutex_unlock(&mutex_count_insert);
                finished_insert = 1;
            }
        }

        else if (random_select == 2 && finished_delete == 0)
        {
            pthread_mutex_lock(&mutex_count_delete);
            if (count_delete < m_delete)
            {
                count_delete++;
                pthread_mutex_unlock(&mutex_count_delete);
                pthread_mutex_lock(&mutex);
                Delete(random_value, &head);
                pthread_mutex_unlock(&mutex);
            }
            else
            {
                pthread_mutex_unlock(&mutex_count_delete);
                finished_delete = 1;
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    printf("Enter values for n, m, thread_count, m_member_fraction, m_insert_fraction, m_delete_fraction \n(Use `Enter` or `Space` to seperate values\n:");
    scanf("%d %d %d %f %f %f", &n, &m, &thread_count, &m_member_fraction, &m_insert_fraction, &m_delete_fraction);

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

    if (m_member_fraction + m_insert_fraction + m_delete_fraction != 1.0)
    {
        printf("Invalid fraction values\n");
        exit(0);
    }

    printf("\nn= %d\nm= %d\nthread_count= %d\nm_member= %f\nm_insert= %f\nm_delete= %f\n", n, m, thread_count, m_member_fraction, m_insert_fraction, m_delete_fraction);

    m_member = round(m_member_fraction * m);
    m_insert = round(m_insert_fraction * m);
    m_delete = round(m_delete_fraction * m);

    if (m_member + m_insert + m_delete != m)
    {
        printf("m, m_member, m_insert, m_delete values are mismatch\n");
        exit(0);
    }

    printf("\nMember() function will execute: %d times\n", m_member);
    printf("Insert() function will execute: %d times\n", m_insert);
    printf("Delete() function will execute: %d times\n", m_delete);

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

    printf("\ncount_member_total= %d\n", count_member);
    printf("count_insert_total= %d\n", count_insert);
    printf("count_delete_total= %d\n", count_delete);
    LogLinkedList(&head);

    return 0;
}
