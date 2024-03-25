#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "queue.h"

#define SAMPLES 10000

// void q_create_runs(struct list_head *head)
// {
//     if (head == NULL || head->next == head)
//         return;
//     struct list_head *first;
//     struct list_head *safe;
//     list_for_each_safe (first, safe, head) {
//         int count = 0;
//         while (count < rand() % 3 + 3 - 1 && safe != head) {
//             safe = safe->next;
//             list_move(safe->prev, first->prev);
//             first = first->prev;
//             count++;
//         }
//     }
// }

void q_create_runs(struct list_head *head)
{
    if (head == NULL || head->next == head)
        return;
    bool flag = false;
    int len = SAMPLES / 100;
    while (len > 0) {
        int count = 0;
        int k = rand() % SAMPLES;
        struct list_head *old = head->next;
        while (count < k) {
            old = old->next;
            count++;
        }
        k = rand() % (SAMPLES / 100);
        // k = rand() % 10;
        count = 0;
        struct list_head *safe = old->next;
        struct list_head *tmp = head->prev;
        while (count < k && safe != head && safe->prev != tmp) {
            if (flag)
                list_move_tail(safe->prev, head);
            else
                list_move(safe->prev, tmp);
            count++;
            safe = safe->next;
        }

        flag = !flag;
        len--;
    }
}

static void create_sample(struct list_head *head, element_t *space, int samples)
{
    printf("Creating sample\n");
    for (int i = 0; i < samples; i++) {
        element_t *elem = space + i;
        // char v = (char) (rand() % 10000);
        char v = (char) i;
        elem->value = &v;
        list_add_tail(&elem->list, head);
    }
}

int main(void)
{
    struct list_head sample_head;
    int nums = SAMPLES;

    /* Assume ASLR */
    srand((uintptr_t) &main);

    INIT_LIST_HEAD(&sample_head);

    element_t *samples = malloc(sizeof(*samples) * SAMPLES);

    create_sample(&sample_head, samples, nums);
    q_create_runs(&sample_head);

    /* check if dataset is sorted correctly */
    // struct list_head *node, *safe;
    // element_t *node_e;
    // list_for_each_safe (node, safe, &sample_head) {
    //     node_e = list_entry(node, element_t, list);
    //     printf(" %s ", node_e->value);
    // }

    q_sort(&sample_head, false);

    return 0;
}