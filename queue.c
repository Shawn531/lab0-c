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
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list)
        q_release_element(entry);
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *node = malloc(sizeof(element_t));
    if (!node)
        return false;
    node->value = strdup(s);
    if (!node->value) {
        free(node);
        return false;
    }
    list_add(&node->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *node = malloc(sizeof(element_t));
    if (!node)
        return false;
    node->value = strdup(s);
    if (!node->value) {
        free(node);
        return false;
    }
    list_add_tail(&node->list, head);
    return true;
}
/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    // check
    if (!head)
        return NULL;
    // list_first_entry
    element_t *node = list_first_entry(head, element_t, list);
    if (!node)
        return NULL;
    // list_del
    list_del(&node->list);
    // move to sp
    if (!sp || !bufsize)
        return NULL;
    // sp = node->value;
    strncpy(sp, node->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    // check
    if (!head)
        return NULL;
    // list_first_entry
    element_t *node = list_last_entry(head, element_t, list);
    if (!node)
        return NULL;
    // list_del
    list_del(&node->list);
    // move to sp
    if (!sp || !bufsize)
        return NULL;
    // sp = node->value;
    strncpy(sp, node->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int count = 0;
    if (!head)
        return 0;
    element_t *entry;
    list_for_each_entry (entry, head, list) {
        count++;
    }
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head)
        return false;
    int len = q_size(head);
    struct list_head *prt = head->next;
    int count = 0;
    while (count <= len / 2 - 1) {
        prt = prt->next;
        count++;
    }
    element_t *node_to_delete = list_entry(prt, element_t, list);
    list_del(prt);
    q_release_element(node_to_delete);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (head == NULL || head->next == head)
        return false;
    struct list_head *safe;
    element_t *node_e, *safe_e;
    bool dup = 0;
    list_for_each_entry_safe (node_e, safe_e, head, list) {
        while (&safe_e->list != head) {
            safe = (&safe_e->list)->next;
            if (strcmp(node_e->value, safe_e->value) == 0) {
                list_del(safe->prev);
                q_release_element(safe_e);
                dup = 1;
            } else
                break;
        }
        safe_e = list_entry((&node_e->list)->next, element_t, list);
        if (dup) {
            list_del(&node_e->list);
            q_release_element(node_e);
            dup = 0;
        }
    }
    return 1;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head) {}


/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    // put the first one to the first
    if (head == NULL || head->next == head)
        return;
    struct list_head *node = head->next;
    struct list_head *next = node->next;
    while (next != head) {
        next = node->next;
        list_move(node, head);
        node = next;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
