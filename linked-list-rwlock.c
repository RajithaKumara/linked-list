#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include "linked-list.h"

int n, m, thread_count, rand_upper, m_member, m_insert, m_delete;
float m_member_fraction, m_insert_fraction, m_delete_fraction;

pthread_rwlock_t rwlock_linked_list;
pthread_rwlock_t rwlock_count_member;
pthread_rwlock_t rwlock_count_insert;
pthread_rwlock_t rwlock_count_delete;

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
            pthread_rwlock_wrlock(&rwlock_count_member);
            if (count_member < m_member)
            {
                count_member++;
                pthread_rwlock_unlock(&rwlock_count_member);
                pthread_rwlock_rdlock(&rwlock_linked_list);
                Member(random_value, head);
                pthread_rwlock_unlock(&rwlock_linked_list);
            }
            else
            {
                pthread_rwlock_unlock(&rwlock_count_member);
                finished_member = 1;
            }
        }

        else if (random_select == 1 && finished_insert == 0)
        {
            pthread_rwlock_wrlock(&rwlock_count_insert);
            if (count_insert < m_insert)
            {
                count_insert++;
                pthread_rwlock_unlock(&rwlock_count_insert);
                pthread_rwlock_wrlock(&rwlock_linked_list);
                Insert(random_value, &head);
                pthread_rwlock_unlock(&rwlock_linked_list);
            }
            else
            {
                pthread_rwlock_unlock(&rwlock_count_insert);
                finished_insert = 1;
            }
        }

        else if (random_select == 2 && finished_delete == 0)
        {
            pthread_rwlock_wrlock(&rwlock_count_delete);
            if (count_delete < m_delete)
            {
                count_delete++;
                pthread_rwlock_unlock(&rwlock_count_delete);
                pthread_rwlock_wrlock(&rwlock_linked_list);
                Delete(random_value, &head);
                pthread_rwlock_unlock(&rwlock_linked_list);
            }
            else
            {
                pthread_rwlock_unlock(&rwlock_count_delete);
                finished_delete = 1;
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc == 7)
    {
        n = (int)strtol(argv[1], (char **)NULL, 10);
        m = (int)strtol(argv[2], (char **)NULL, 10);
        thread_count = (int)strtol(argv[3], (char **)NULL, 10);

        m_member_fraction = (float)atof(argv[4]);
        m_insert_fraction = (float)atof(argv[5]);
        m_delete_fraction = (float)atof(argv[6]);
    }
    else
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

        if (thread_count <= 0)
        {
            printf("Invalid thread count value\n");
            exit(0);
        }

        if (m_member_fraction + m_insert_fraction + m_delete_fraction != 1.0)
        {
            printf("Invalid fraction values\n");
            exit(0);
        }
    }

    // printf("\nn= %d\nm= %d\nthread_count= %d\nm_member= %f\nm_insert= %f\nm_delete= %f\n", n, m, thread_count, m_member_fraction, m_insert_fraction, m_delete_fraction);

    m_member = round(m_member_fraction * m);
    m_insert = round(m_insert_fraction * m);
    m_delete = round(m_delete_fraction * m);

    if (m_member + m_insert + m_delete != m)
    {
        printf("m, m_member, m_insert, m_delete values are mismatch\n");
        exit(0);
    }

    // printf("\nMember() function will execute: %d times\n", m_member);
    // printf("Insert() function will execute: %d times\n", m_insert);
    // printf("Delete() function will execute: %d times\n", m_delete);

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

    pthread_rwlock_init(&rwlock_linked_list, NULL);
    pthread_rwlock_init(&rwlock_count_member, NULL);
    pthread_rwlock_init(&rwlock_count_insert, NULL);
    pthread_rwlock_init(&rwlock_count_delete, NULL);
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
    pthread_rwlock_destroy(&rwlock_linked_list);
    pthread_rwlock_destroy(&rwlock_count_member);
    pthread_rwlock_destroy(&rwlock_count_insert);
    pthread_rwlock_destroy(&rwlock_count_delete);

    double time_diff = (double)(time_end.tv_usec - time_begin.tv_usec) / 1000000 + (double)(time_end.tv_sec - time_begin.tv_sec);
    printf("%.6f\n", time_diff);
    // LogLinkedList(&head);

    return 0;
}
