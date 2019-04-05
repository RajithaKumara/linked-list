#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

struct list_node_s
{
    int data;
    struct list_node_s *next;
};

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

int n, m;

float m_member, m_insert, m_delete;

int main(int argc, char *argv[])
{
    printf("Enter values for n, m, m_member, m_insert, m_delete \n(Use `Enter` or `Space` to seperate values\n:");
    scanf("%d %d %f %f %f", &n, &m, &m_member, &m_insert, &m_delete);

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

    if (m_member + m_insert + m_delete != 1.0)
    {
        printf("Invalid fraction values\n");
        exit(0);
    }

    printf("\nn= %d\nm= %d\nm_member= %f\nm_insert= %f\nm_delete= %f\n", n, m, m_member, m_insert, m_delete);

    int rand_upper = pow(2, 16) - 1;
    struct list_node_s *head = NULL;
    struct timeval time_begin, time_end;

    int i = 0;
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

    float mMember = m_member * m;
    float mInsert = m_insert * m;
    float mDelete = m_delete * m;

    gettimeofday(&time_begin, NULL);
    while (count_tot < m)
    {
        int rand_value = rand() % rand_upper;
        int rand_select = rand() % 3;

        if (rand_select == 0 && count_member < mMember)
        {
            Member(rand_value, head);
            count_member++;
        }

        else if (rand_select == 1 && count_insert < mInsert)
        {
            Insert(rand_value, &head);
            count_insert++;
        }

        else if (rand_select == 2 && count_delete < mDelete)
        {
            Delete(rand_value, &head);
            count_delete++;
        }

        count_tot = count_insert + count_member + count_delete;
    }
    gettimeofday(&time_end, NULL);

    double time_diff = (double)(time_end.tv_usec - time_begin.tv_usec) / 1000000 + (double)(time_end.tv_sec - time_begin.tv_sec);
    printf("\nTime Spent : %.6f secs\n", time_diff);

    return 0;
}
