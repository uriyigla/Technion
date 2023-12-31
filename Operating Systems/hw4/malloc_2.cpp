//
// Created by chens on 29/06/2023.
//

#include <unistd.h>
#include <cstring>
#include <cstddef>
#include <cmath>
#include <iostream>

size_t free_blocks = 0;
size_t free_bytes = 0;
size_t total_allocated_blocks = 0;
size_t total_allocated_bytes = 0;

typedef struct MallocMetaData{
    size_t size;
    bool is_free;
    MallocMetaData *next;
    MallocMetaData *prev;
} malloc_meta_data, *pMalloc_meta_data;

pMalloc_meta_data tail = nullptr;
malloc_meta_data head_dummy = {0, false ,tail, nullptr};

void* find_block_in_list(size_t size){
    pMalloc_meta_data curr = head_dummy.next;
    while(curr){
        if(curr->is_free && curr->size >= size){
            free_bytes -= curr->size;
            free_blocks -= 1;
            curr->is_free = false;
            void* address = curr + 1;
            return address;
        }
        curr = curr->next;
    }
    return nullptr;
}

void* add_new_meta_data(size_t size){
    void *data = sbrk(sizeof(malloc_meta_data) + size);
    if(data == (void*)-1) {
        return nullptr;
    }
    pMalloc_meta_data meta_data = (pMalloc_meta_data)data;
    meta_data->next = nullptr;
    meta_data->size = size;
    meta_data->is_free = false;
    if (tail != nullptr) {
        meta_data->prev = tail;
        tail->next = meta_data;
        tail = meta_data;
    } else {
        head_dummy.next = meta_data;
        tail = meta_data;
        meta_data->prev = &head_dummy;
    }

    total_allocated_bytes += size;
    total_allocated_blocks += 1;
    void* addres = meta_data + 1;
    return addres;
}

void* smalloc(size_t size){
    if((size == 0) || (size > pow(10, 8))) {
        return nullptr;
    }
    void* block = find_block_in_list(size);
    if(block){
        return block;
    }
    return add_new_meta_data(size);
}


void* scalloc(size_t num, size_t size){
    void* block = smalloc(num * size);
    if(block) {
        return memset(block, 0, (num * size));
    }
    return nullptr;
}


void sfree(void* p){
    if(p == nullptr){
        return;
    }
    pMalloc_meta_data meta_data = ((pMalloc_meta_data)(p)) - 1;
    if(meta_data->is_free){
        return;
    }
    free_bytes += meta_data->size;
    free_blocks += 1;
    meta_data->is_free = true;
}


void* srealloc(void* oldp, size_t size){
    if(size > pow(10, 8)){
        return nullptr;
    } else if(oldp == nullptr){
        return smalloc(size);
    }
    pMalloc_meta_data meta_data = (pMalloc_meta_data) oldp - 1;
    if(meta_data->size >= size) {
        return oldp;
    }
    void* new_block = smalloc(size);
    if(new_block != nullptr){
        memmove(new_block, oldp, meta_data->size);
        sfree(oldp);
        return new_block;
    }
    return nullptr;
}

size_t _num_free_blocks(){
    return free_blocks;
}

size_t _num_free_bytes(){
    return free_bytes;
}

size_t _num_allocated_blocks(){
    return total_allocated_blocks;
}

size_t _num_allocated_bytes(){
    return total_allocated_bytes;
}

size_t _size_meta_data(){
    return sizeof(malloc_meta_data);
}

size_t _num_meta_data_bytes(){
    return (sizeof(malloc_meta_data) * total_allocated_blocks);
}