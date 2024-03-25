#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CACHE_SIZE 1000 

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int count;
    int hits;
    int misses;
} LRUCache;

LRUCache* createCache() {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->head = NULL;
    cache->tail = NULL;
    cache->count = 0;
    cache->hits = 0;
    cache->misses = 0;
    return cache;
}

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void addToFront(LRUCache* cache, Node* node) {
    if (cache->count == 0) {
        cache->head = node;
        cache->tail = node;
    }
    else {
        node->next = cache->head;
        cache->head->prev = node;
        cache->head = node;
    }
    cache->count++;
}

void removeFromEnd(LRUCache* cache) {
    if (cache->count == 0)
        return;

    Node* temp = cache->tail;
    cache->tail = cache->tail->prev;
    if (cache->tail != NULL)
        cache->tail->next = NULL;
    free(temp);
    cache->count--;
}

void moveToFront(LRUCache* cache, Node* node) {
    if (node == cache->head)
        return;

    if (node == cache->tail) {
        cache->tail = node->prev;
        cache->tail->next = NULL;
    }
    else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    node->next = cache->head;
    node->prev = NULL;
    cache->head->prev = node;
    cache->head = node;
}

void displayCache(LRUCache* cache) {
    Node* current = cache->head;
    printf("LRU Cache: ");
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void accessData(LRUCache* cache, int data) {
    Node* current = cache->head;
    bool found = false;


    while (current != NULL) {
        if (current->data == data) {
            found = true;
            break;
        }
        current = current->next;
    }

    if (found) {
        moveToFront(cache, current);
        cache->hits++;
    }
    else {
        cache->misses++;
        Node* newNode = createNode(data);
        if (cache->count >= CACHE_SIZE) {
            removeFromEnd(cache);
        }
        addToFront(cache, newNode);
    }
}

float cacheHitRatio(LRUCache* cache) {
    if (cache->hits + cache->misses == 0)
        return 0;
    return (float)cache->hits / (cache->hits + cache->misses);
}

int main() {
    int cache_slot = 1000; 
    int cache_hit = 28110; 

    LRUCache* cache = createCache();




    for (int i = 0; i < cache_hit; i++) {
        accessData(cache, i % cache_slot);
    }


    printf("Cache Hit Ratio: %.8f\n", cacheHitRatio(cache));

    return 0;
}
