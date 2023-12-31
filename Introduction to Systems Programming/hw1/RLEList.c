#include "RLEList.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define FRONT_OF_LIST_NUM -1
#define FRONT_OF_LIST_C '\0'
#define EMPTY_LIST -1
#define ENTER '\n'

typedef struct node {
    int num;
    char c;
    struct node *next;
} *Node;

struct RLEList_t {
    Node head;
};

RLEList RLEListCreate() {
    RLEList list = malloc(sizeof(*list));
    if (!list) {
        return NULL;
    }

    list->head = malloc(sizeof(*list->head));
    if (!list->head) {
        RLEListDestroy(list);
        return NULL;
    }

    list->head->next = NULL;
    list->head->num = FRONT_OF_LIST_NUM;
    list->head->c = FRONT_OF_LIST_C;

    return list;
}

static Node createNode() {
    Node node = malloc(sizeof(*node));
    if (!node) {
        return NULL;
    }

    node->num = FRONT_OF_LIST_NUM;
    node->c = FRONT_OF_LIST_C;
    node->next = NULL;

    return node;
}

static bool emptyNode(Node node) {
    if (node == NULL) {
        return false;
    }
    return ((node->next == NULL) && (node->num == FRONT_OF_LIST_NUM) && (node->c == FRONT_OF_LIST_C));
}

void RLEListDestroy(RLEList list) {
    if (list == NULL) {
        return;
    }

    Node node = list->head;
    if (node == NULL) {
        free(list);
        return;
    }

    if (RLEListSize(list) == 0) {
        free(node);
        free(list);
        return;
    }

    while (node->next != NULL) {
        list->head = list->head->next;
        node->next = NULL;
        free(node);
        node = list->head;
    }

    free(node);
    free(list);
}

RLEListResult RLEListAppend(RLEList list, char value) {
    if (list == NULL) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    if (emptyNode(list->head)) {
        list->head->num = 1;
        list->head->c = value;
        return RLE_LIST_SUCCESS;
    } else {
        Node node1 = list->head;
        while (node1->next != NULL) {
            node1 = node1->next;
        }

        if (node1->c == value) {
            node1->num++;
            return RLE_LIST_SUCCESS;
        } else {
            Node node2 = createNode();
            if (node2 == NULL) {
                return RLE_LIST_OUT_OF_MEMORY;
            }

            node2->num = 1;
            node2->c = value;
            node1->next = node2;

            return RLE_LIST_SUCCESS;
        }
    }
}

static int numberOfNodes(RLEList list) {
    if (emptyNode(list->head)) {
        return 0;
    }

    Node node = list->head;
    int count = 1;

    while (node->next != NULL) {
        count++;
        node = node->next;
    }

    return count;
}

int RLEListSize(RLEList list) {
    if (list == NULL) {
        return EMPTY_LIST;
    }

    if (emptyNode(list->head)) {
        return 0;
    }

    Node node = list->head;
    int counter = 0;

    for (int i = 0; i < numberOfNodes(list) - 1; i++) {
        counter += node->num;
        node = node->next;
    }

    counter += node->num;
    return counter;
}

static Node beforeNode(RLEList list, Node node) {
    if (node == list->head) {
        return createNode();
    }

    Node node1 = list->head;
    while (node1->next != node) {
        node1 = node1->next;
    }

    return node1;
}

static Node afterNode(RLEList list, Node node) {
    if (!(list->head->next)) {
        return createNode();
    }

    Node node1 = list->head;
    while (node->next != node1) {
        node1 = node1->next;
    }

    if (node1 == NULL) {
        return createNode();
    }

    return node1;
}

static RLEListResult removeZeroNode(RLEList list, Node node) {
    Node beforeN = beforeNode(list, node);
    Node afterN = afterNode(list, node);

    bool beforeEmpty = emptyNode(beforeN);
    bool afterEmpty = emptyNode(afterN);

    if (beforeEmpty && afterEmpty) {
        list->head->num = EMPTY_LIST;
        list->head->c = FRONT_OF_LIST_C;

        free(beforeN);
        free(afterN);

        return RLE_LIST_SUCCESS;
    }

    if (!beforeEmpty && afterEmpty) {
        beforeN->next = NULL;
        free(node);
        free(afterN);

        return RLE_LIST_SUCCESS;
    }

    if (beforeEmpty && !afterEmpty) {
        list->head = list->head->next;
        node->next = NULL;

        free(node);
        free(beforeN);

        return RLE_LIST_SUCCESS;
    }

    if (!beforeEmpty && !afterEmpty) {
        if (node->num > 1) {
            node->num--;
            return RLE_LIST_SUCCESS;
        } else {
            if (beforeN->c != afterN->c) {
                beforeN->next = afterN;
                node->next = NULL;

                free(node);

                return RLE_LIST_SUCCESS;
            } else {
                beforeN->num += afterN->num;
                beforeN->next = afterN->next;
                afterN->next = NULL;
                node->next = NULL;

                free(node);
                free(afterN);

                return RLE_LIST_SUCCESS;
            }
        }
    }

    return 0;
}

RLEListResult RLEListRemove(RLEList list, int index) {
    if (list == NULL) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    int len = RLEListSize(list);

    if (emptyNode(list->head) || (index >= len) || (index < 0)) {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }

    Node node = list->head;
    int sum = node->num;

    while (sum <= index) {
        node = node->next;
        sum = sum + node->num;
    }

    node->num--;

    if (node->num != 0) {
        node = createNode();
        free(node);

        return RLE_LIST_SUCCESS;
    } else {
        return removeZeroNode(list, node);
    }

    return 0;
}

char RLEListGet(RLEList list, int index, RLEListResult *result) {
    if (list == NULL) {
        if (result != NULL) {
            RLEListResult address = RLE_LIST_NULL_ARGUMENT;
            *result = address;
        }

        return 0;
    }

    if (emptyNode(list->head)) {
        if (result != NULL) {
            RLEListResult address = RLE_LIST_INDEX_OUT_OF_BOUNDS;
            *result = address;
        }

        return 0;
    }

    if ((RLEListSize(list) <= index) || (0 > index)) {
        if (result != NULL) {
            RLEListResult address = RLE_LIST_INDEX_OUT_OF_BOUNDS;
            *result = address;
        }

        return 0;
    }

    Node node = list->head;
    int sum = node->num;

    while (sum < index) {
        node = node->next;
        sum = sum + node->num;
    }

    if (sum == index) {
        node = node->next;
    }

    if (result != NULL) {
        RLEListResult address = RLE_LIST_SUCCESS;
        *result = address;
    }

    return node->c;
}

static int digitsForSingleArray(Node node) {
    int count = 2; //count starts with 2 for char, number of digits and for \n in the end
    int num = node->num;

    while (num > 0) {
        count++;
        num = num / 10;
    }

    return count;
}

static int digitsForAllArrays(RLEList list) {
    Node node = list->head;
    int sum = digitsForSingleArray(node);
    int len = numberOfNodes(list);

    for (int i = 1; i < len; i++) {
        node = node->next;
        sum += digitsForSingleArray(node);
    }

    return sum;
}

char* RLEListExportToString(RLEList list, RLEListResult *result) {
    if (list == NULL) {
        if (result != NULL) {
            RLEListResult address = RLE_LIST_NULL_ARGUMENT;
            *result = address;
        }

        return NULL;
    }

    if (emptyNode(list->head)) {
        if (result != NULL) {
            RLEListResult address = RLE_LIST_SUCCESS;
            *result = address;
        }

        return NULL;
    }

    char* array = malloc(sizeof(char) * digitsForAllArrays(list) + 1);
    if (!array) {
        if (result != NULL) {
            RLEListResult address = RLE_LIST_OUT_OF_MEMORY;
            *result = address;
        }
    }

    Node node = list->head;
    sprintf(array, "%c%d%c", node->c, node->num, ENTER);

    while (node->next != NULL) {
        node = node->next;
        sprintf(array, "%s%c%d%c", array, node->c, node->num, ENTER);
    }

    if (result != NULL) {
        RLEListResult address = RLE_LIST_SUCCESS;
        *result = address;
    }

    return array;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function) {
    if ((list == NULL) || (map_function == NULL)) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    if (emptyNode(list->head)) {
        return RLE_LIST_SUCCESS;
    }

    Node node1 = list->head;
    node1->c = map_function(node1->c);

    while (node1->next != NULL) {
        node1 = node1->next;
        node1->c = map_function(node1->c);
    }

    return RLE_LIST_SUCCESS;
}
