#include "workqueue.h"
#include "../antiviRus/antiviRus.h"

Workqueue *mkqueue() {
    Workqueue *wq;

    wq = (Workqueue *)malloc($i sizeof(Workqueue));
    assert(wq);
    wq->head = NULL;
    wq->tail = NULL;
    InitializeCriticalSection(&wq->lock);
    InitializeConditionVariable(&wq->has_work);
    wq->finished = false;

    return wq;
}

void pushqueue(Workqueue *wq, Entry *entry) {
    Node *new_node;

    new_node = (Node *)malloc($i sizeof(Node));
    assert(new_node);
    memset($1 new_node, 0, sizeof(Node));
    new_node->path = (int8 *)malloc($i (strlen($c entry->path) + 1) * sizeof(int8));
    assert(new_node->path);
    strcpy($c new_node->path, $c entry->path);
    new_node->next = NULL;

    EnterCriticalSection(&wq->lock);
    if (wq->tail) {
        wq->tail->next = new_node;
        wq->tail = new_node;
    } else {
        wq->head = new_node;
        wq->tail = new_node;
    }
    WakeConditionVariable(&wq->has_work);
    LeaveCriticalSection(&wq->lock);
}