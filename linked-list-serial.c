#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "linked-list.h"

int main(int argc, char *argv[])
{
    int n, m, m_member, m_insert, m_delete;
    float m_member_fraction, m_insert_fraction, m_delete_fraction;

    printf("Enter values for n, m, m_member_fraction, m_insert_fraction, m_delete_fraction \n(Use `Enter` or `Space` to seperate values\n:");
    scanf("%d %d %f %f %f", &n, &m, &m_member_fraction, &m_insert_fraction, &m_delete_fraction);

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

    if (m_member_fraction + m_insert_fraction + m_delete_fraction != 1.0)
    {
        printf("Invalid fraction values\n");
        exit(0);
    }

    printf("\nn= %d\nm= %d\nm_member= %f\nm_insert= %f\nm_delete= %f\n", n, m, m_member_fraction, m_insert_fraction, m_delete_fraction);

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

    int rand_upper = pow(2, 16) - 1;
    struct list_node_s *head = NULL;
    struct timeval time_begin, time_end;

    int i = 0;
    srand(time(0));
    while (i < n)
    {
        if (Insert(rand() % rand_upper, &head) == 1)
        {
            i++;
        }
    }

    int count_tot = 0;
    int count_member = 0;
    int count_insert = 0;
    int count_delete = 0;

    gettimeofday(&time_begin, NULL);
    while (count_tot < m)
    {
        int rand_value = rand() % rand_upper;
        int rand_select = rand() % 3;

        if (rand_select == 0 && count_member < m_member)
        {
            Member(rand_value, head);
            count_member++;
        }

        else if (rand_select == 1 && count_insert < m_insert)
        {
            Insert(rand_value, &head);
            count_insert++;
        }

        else if (rand_select == 2 && count_delete < m_delete)
        {
            Delete(rand_value, &head);
            count_delete++;
        }

        count_tot = count_insert + count_member + count_delete;
    }
    gettimeofday(&time_end, NULL);

    double time_diff = (double)(time_end.tv_usec - time_begin.tv_usec) / 1000000 + (double)(time_end.tv_sec - time_begin.tv_sec);
    printf("\nTime Spent : %.6f secs\n", time_diff);
    LogLinkedList(&head);

    return 0;
}
