#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_sort.h"
#include "timsort.h"

typedef struct {
    struct list_head list;
    int val;
    int seq;
} element_t;

#define SAMPLES 10000

// void q_create_runs(struct list_head *head)
// {
//     if (head == NULL || head->next == head)
//         return;
//     struct list_head *first;
//     struct list_head *safe;
//     bool flag = false;
//     list_for_each_safe (first, safe, head) {
//         int count = 0;
//         while (count < rand() % 3 + 3 - 1 && safe != head) {
//             safe = safe->next;
//             if (flag)
//                 list_move(safe->prev, first->prev);
//             first = first->prev;
//             count++;
//         }
//         flag = !flag;
//     }
// }

// void q_create_runs(struct list_head *head)
// {
//     if (head == NULL || head->next == head)
//         return;
//     bool flag = false;
//     int len = SAMPLES/100;
//     while(len > 0) {
//         int count = 0;
//         int k = rand() % SAMPLES;
//         struct list_head *old = head->next;
//         while (count < k ) {
//             old = old->next;
//             count++;
//         }
//         k = rand() % (SAMPLES/100);
//         // k = rand() % 10;
//         count = 0;
//         struct list_head *safe = old->next;
//         struct list_head *tmp = head->prev;
//         while (count < k && safe != head && safe->prev != tmp) {
//             if(flag)
//                 list_move_tail(safe->prev, head);
//             else
//                 list_move(safe->prev, tmp);
//             count++;
//             safe = safe->next;
//         }

//         flag = !flag;
//         len--;
//     }
// }

static void create_sample(struct list_head *head, element_t *space, int samples)
{
    printf("Creating sample\n");
    for (int i = 0; i < samples; i++) {
        element_t *elem = space + i;
        // elem->val = rand() % samples;
        elem->val = samples - i;
        list_add_tail(&elem->list, head);
    }
}

// static void copy_list(struct list_head *from,
//                       struct list_head *to,
//                       element_t *space)
// {
//     if (list_empty(from))
//         return;

//     element_t *entry;
//     list_for_each_entry (entry, from, list) {
//         element_t *copy = space++;
//         copy->val = entry->val;
//         copy->seq = entry->seq;
//         list_add_tail(&copy->list, to);
//     }
// }

int compare(void *priv, const struct list_head *a, const struct list_head *b)
{
    // if (a == b)
    //     return 0;

    // int res = list_entry(a, element_t, list)->val -
    //           list_entry(b, element_t, list)->val;

    // if (priv)
    //     *((int *) priv) += 1;

    // return res;
    return 0;
}

bool check_list(struct list_head *head, int count)
{
    if (list_empty(head))
        return 0 == count;

    element_t *entry, *safe;
    size_t ctr = 0;
    list_for_each_entry_safe (entry, safe, head, list) {
        ctr++;
    }
    int unstable = 0;
    list_for_each_entry_safe (entry, safe, head, list) {
        if (entry->list.next != head) {
            if (entry->val > safe->val) {
                fprintf(stderr, "\nERROR: Wrong order\n");
                return false;
            }
            if (entry->val == safe->val && entry->seq > safe->seq)
                unstable++;
        }
    }
    if (unstable) {
        fprintf(stderr, "\nERROR: unstable %d\n", unstable);
        return false;
    }

    if (ctr != SAMPLES) {
        fprintf(stderr, "\nERROR: Inconsistent number of elements: %ld\n",
                (long) ctr);
        return false;
    }
    return true;
}

typedef void (*test_func_t)(void *priv,
                            struct list_head *head,
                            list_cmp_func_t cmp);

typedef struct {
    char *name;
    test_func_t impl;
} test_t;

int main(void)
{
    struct list_head sample_head; /*, warmdata_head, testdata_head;*/
    int count;
    int nums = SAMPLES;

    /* Assume ASLR */
    srand((uintptr_t) &main);

    test_t tests[] = {// {.name = "tim_sort", .impl = timsort},
                      {.name = "list_sort", .impl = list_sort},
                      {NULL, NULL}};
    test_t *test = tests;

    INIT_LIST_HEAD(&sample_head);

    element_t *samples = malloc(sizeof(*samples) * SAMPLES);
    // element_t *warmdata = malloc(sizeof(*warmdata) * SAMPLES);
    // element_t *testdata = malloc(sizeof(*testdata) * SAMPLES);

    create_sample(&sample_head, samples, nums);
    // q_create_runs(&sample_head);

    /* check if dataset is sorted correctly */
    // struct list_head *node, *safe;
    // element_t *node_e;
    // list_for_each_safe (node, safe, &sample_head) {
    //     node_e = list_entry(node, element_t, list);
    //     printf(" %d ", node_e->val);
    // }


    while (test->impl) {
        printf("==== Testing %s ====\n", test->name);
        /* Warm up */
        // INIT_LIST_HEAD(&warmdata_head);
        // INIT_LIST_HEAD(&testdata_head);
        // copy_list(&sample_head, &testdata_head, testdata);
        // copy_list(&sample_head, &warmdata_head, warmdata);
        // test->impl(&count, &warmdata_head, compare);

        /* Test */
        count = 0;
        test->impl(&count, &sample_head, compare);
        printf("  Comparisons:    %d\n", count);
        printf("  List is %s\n",
               check_list(&sample_head, nums) ? "sorted" : "not sorted");
        test++;
    }


    return 0;
}