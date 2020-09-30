#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* For q_sort() */
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2);
list_ele_t *merge_sort(list_ele_t *head);

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        fprintf(stderr, "malloc() failed in q_new().\n");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        if (tmp->value)
            free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q || !s)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        fprintf(stderr, "malloc() failed: newh in q_insert_head().\n");
        return false;
    }
    newh->value = strdup(s);
    if (!newh->value) {
        fprintf(stderr, "strdup() failed: new->value in q_insert_head().\n");
        free(newh);
        return false;
    }
    newh->next = q->head;
    q->head = newh;
    // If newh is the first element.
    if (!q->tail)
        q->tail = newh;
    // Increase amount of elements.
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q || !s)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        fprintf(stderr, "malloc() failed: newh in q_insert_tail().\n");
        return false;
    }
    newh->value = strdup(s);
    if (!newh->value) {
        fprintf(stderr, "strdup() failed: newh->value q_insert_tail().\n");
        free(newh);
        return false;
    }
    newh->next = NULL;
    // If newh is the first element.
    if (!q->tail) {
        q->tail = q->head = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    // Increase amount of elements.
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    list_ele_t *rm = q->head;
    if (sp && rm->value) {
        strncpy(sp, rm->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    q->head = q->head->next;
    free(rm->value);
    free(rm);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head || !q->head->next)
        return;
    list_ele_t *pre = NULL;
    list_ele_t *cur = q->head;
    list_ele_t *next;
    q->head = q->tail;
    q->tail = cur;
    while (cur) {
        next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
    q->head = merge_sort(q->head);
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}
/*recursive merge*/
/*list_ele_t *merge(list_ele_t *l1,list_ele_t *l2 ) {*/

/*if (!l2) return l1;*/
/*if (!l1) return l2;*/
/*if(strcasecmp(l1->value, l2->value) < 0) {*/
/*l1->next = merge(l1->next, l2);*/
/*return l1;*/
/*} else {*/
/*l2->next = merge(l1, l2->next);*/
/*return l2;*/
/*}*/
/*} */
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if (!l1)
        return l2;
    if (!l2)
        return l1;

    list_ele_t *head = NULL;
    list_ele_t **tmp = &head;
    /* sort list elements based on strnatcmp */
    while (l1 && l2) { /* DON'T strcmp a NULL ptr */
        if (strcasecmp(l1->value, l2->value) < 0) {
            *tmp = l1;
            l1 = l1->next;

        } else {
            *tmp = l2;
            l2 = l2->next;
        }
        tmp = &((*tmp)->next);
    }
    if (l1)
        *tmp = l1;
    if (l2)
        *tmp = l2;
    return head;
}
list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    // split list
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *l1 = merge_sort(head);
    list_ele_t *l2 = merge_sort(fast);
    return merge(l1, l2);
}
