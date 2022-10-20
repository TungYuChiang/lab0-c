#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *h = malloc(1 * sizeof(struct list_head));
    if (!h) {
        return NULL;
    }
    h->next = h;
    h->prev = h;
    return h;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l) {
        return;
    }
    element_t *node, *next;
    list_for_each_entry_safe (node, next, l, list) {
        list_del(&node->list);
        q_release_element(node);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }

    element_t *q = malloc(1 * sizeof(element_t));
    if (!q) {
        return false;
    }

    int str_size = strlen(s);
    q->value = malloc((str_size + 1) * sizeof(char));
    if (!q->value) {
        free(q);
        return false;
    }
    strncpy(q->value, s, str_size);
    *(q->value + str_size) = '\0';
    list_add(&q->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *q = malloc(1 * sizeof(element_t));
    if (!q) {
        return false;
    }
    int str_size = strlen(s);
    q->value = malloc((str_size + 1) * sizeof(char));
    if (!q->value) {
        free(q);
        return false;
    }
    strncpy(q->value, s, str_size);
    *(q->value + str_size) = '\0';
    list_add_tail(&q->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head) {
        return NULL;
    }

    element_t *element;
    element = list_first_entry(head, element_t, list);
    list_del(&(element->list));

    if (bufsize) {
        strncpy(sp, element->value, bufsize);
        *(sp + bufsize - 1) = '\0';
    }
    return element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head) {
        return NULL;
    }

    element_t *element;
    element = list_last_entry(head, element_t, list);
    list_del(&(element->list));

    if (bufsize) {
        strncpy(sp, element->value, bufsize);
        *(sp + bufsize - 1) = '\0';
    }
    return element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head) {
        return 0;
    }

    struct list_head *node;
    int size = 0;
    list_for_each (node, head)
        size++;
    return size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head) {
        return false;
    }

    struct list_head *rabbit, *turtle;
    rabbit = head->next;
    turtle = head->next;
    while (rabbit != head && rabbit->next != head) {
        rabbit = rabbit->next->next;
        turtle = turtle->next;
    }

    list_del(turtle);
    q_release_element(list_entry(turtle, element_t, list));
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head) {
        return false;
    }

    struct list_head *curr, *next, *tmp;
    curr = head->next;
    next = curr->next;
    int flag = 0;

    while (curr != head && next != head) {
        element_t *curr_entry = list_entry(curr, element_t, list);
        element_t *next_entry = list_entry(next, element_t, list);

        while (next != head) {
            if (!strcmp(curr_entry->value, next_entry->value)) {
                flag = 1;
                tmp = next;
                next = next->next;
                list_del(tmp);
                q_release_element(list_entry(tmp, element_t, list));
                next_entry = list_entry(next, element_t, list);
            } else {
                ;
                next = next->next;
                next_entry = list_entry(next, element_t, list);
            }
        }

        next = curr->next;
        if (flag == 1) {
            list_del(curr);
            q_release_element(curr_entry);
            flag = 0;
        }
        curr = next;
        next = next->next;
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    struct list_head *first;
    first = head->next;
    while (first != head && first->next != head) {
        struct list_head *second = first->next;
        list_del_init(first);
        list_add(first, second);
        first = first->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head) {
        return;
    }
    struct list_head *first, *second;
    first = head->next;
    second = head;
    while (first != second && first->next != second) {
        second = second->prev;

        struct list_head *tmp;
        tmp = first;
        list_del_init(first);
        list_add(first, second->prev);
        list_del_init(second);
        list_add(second, tmp->prev);
        second = first;
        first = tmp;

        first = first->next;
    }
}

/* Sort elements of queue in ascending order */
struct list_head *mergeTwoLists(struct list_head *L1, struct list_head *L2)
{
    if (!L2)
        return L1;
    if (!L1)
        return L2;
    struct list_head *head, *tmp;
    struct list_head **indir_ptr = &head;

    //將串列終點改為NULL,方便設定停止值
    L1->prev->next = NULL;
    L2->prev->next = NULL;
    head = L2;
    while (L1 && L2) {
        element_t *L1_entry = list_entry(L1, element_t, list);
        element_t *L2_entry = list_entry(L2, element_t, list);

        if (L1_entry->value < L2_entry->value) {
            *indir_ptr = L1;
            L1 = L1->next;
        } else {
            *indir_ptr = L2;
            L2 = L2->next;
        }

        indir_ptr = &(*indir_ptr)->next;
    }

    if (L1) {
        *indir_ptr = L1;
    } else {
        *indir_ptr = L2;
    }
    tmp = head;
    //因為是雙向，上面我們只改了next下面while檢查沒有改到的prev
    while (tmp->next != NULL) {
        tmp->next->prev = tmp;
        tmp = tmp->next;
    }
    tmp->next = head;
    head->prev = tmp;
    return head;
}
struct list_head *my_mergesort(struct list_head *head)
{
    struct list_head *middle, *fast;
    middle = head->next;
    fast = head->next;
    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        middle = middle->next;
    }
    struct list_head *left = my_mergesort(head->next);
    struct list_head *right = my_mergesort(middle);

    return mergeTwoLists(left, right);
}

void q_sort(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    head->next = my_mergesort(head);
}