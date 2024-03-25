#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "addop.h"
#include "list.h"
#include "queue.h"
/* Shuffle based on Fisher-Yates shuffle */
void q_shuffle(struct list_head *head)
{
    if (head == NULL || head->next == head || head->next->next == head)
        return;
    int len = q_size(head);
    while (len > 0) {
        int random = rand() % len;
        struct list_head *old = head->next;
        int count = 0;
        while (count < random) {
            old = old->next;
            count++;
        }
        struct list_head *new = head->prev;
        if (new != old) {
            list_move(new, old);
            list_move_tail(old, head);
        }
        len--;
    }
}