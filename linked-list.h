struct list_node_s
{
    int data;
    struct list_node_s *next;
};

int Member(int value, struct list_node_s *head_p);

int Insert(int value, struct list_node_s **head_pp);

int Delete(int value, struct list_node_s **head_pp);

void LogLinkedList(struct list_node_s **head);
