#include "../antiviRus/antiviRus.h"
#include "../database/database.h"

struct s_node {
    int8 * path;
    struct s_node *next;    
};

typedef struct s_node Node;

struct s_queue {
    Node *head, *tail;
    CRITICAL_SECTION lock;
    CONDITION_VARIABLE has_work; 
    bool finished;
};

typedef struct s_queue Workqueue;

Workqueue *mkqueue();

void pushqueue(Workqueue *, Entry *);

void popqueue(Workqueue *, Entry *);

void destroyqueue(Workqueue *);

void showqueue(Workqueue *);