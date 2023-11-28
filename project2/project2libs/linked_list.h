typedef struct list { //simple linked list
    unsigned short course_nr;
    struct list *next;
} list;

void list_init(list **head);

int list_find(list *head, unsigned short COURSE_NR);
 
list *newNode(unsigned short COURSE_NR);

void list_add(list **head, list *new_node);

void list_rmv(list **head, unsigned short COURSE_NR);

void list_print(list *head);

void list_clear(list **head);
