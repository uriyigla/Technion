//
// Created by chens on 29/06/2023.
//

#include <unistd.h>
#include <cstring>
#include <cstddef>
#include <cmath>
#include <sys/mman.h>
#include <cstdint>
#include <iostream>

#define MAX_ORDER  (10)
#define NMAP_SIZE_THRESHOLD (1024*128 - sizeof(malloc_meta_data))
#define LARGEST_BLOCK_SIZE (128*1024)
#define INITIAL_BLOCKS_NUM (32)
#define ALIGNMENT (128*1024*32)

int free_blocks = 0;
size_t total_bytes_with_meta = 0;
int total_allocated_blocks = 0;
size_t used_bytes_no_meta  = 0;

int mm_block = 0;
int mm_blocks_freed=0;
size_t bytes_mm = 0;
size_t bytes_mm_freed = 0;

int expected_cookie = rand();

typedef struct MallocMetaData{
    int cookie;
    size_t size;
    bool is_free;
    MallocMetaData * next;
    MallocMetaData * prev;
    bool did_someone_eat_my_cookie() {
        if(!(this->cookie == expected_cookie)) {
            exit(0xdeadbeef);
            return true;
        }
        return false;
    }
    int get_order() {
        return (int)log2((size + sizeof(struct MallocMetaData))/128);
    }
} malloc_meta_data, *pMalloc_meta_data;

std::ostream& operator<<(std::ostream& os, const malloc_meta_data& meta_data) {
    os << "cookie: " << meta_data.cookie << "\n";
    os << "size: " << meta_data.size << "\n";
    os << "is_free: " << (meta_data.is_free ? "true" : "false") << "\n";
    os << "next: " << meta_data.next << "\n";
    os << "prev: " << meta_data.prev << "\n";
    return os;
}

class SubList {
private:
    pMalloc_meta_data head;
    int size;
public:
    SubList() : head(nullptr), size(0) {};
    ~SubList() = default;
    SubList(const SubList &list) = delete;
    pMalloc_meta_data findFreeBlock();
    void removeBlock(pMalloc_meta_data pData);
    pMalloc_meta_data insertBlock(pMalloc_meta_data pData);

};

class MemoryList{
public:
    bool is_initiealized;
    SubList list[MAX_ORDER+1];
    SubList mmList;

    pMalloc_meta_data merge_blocks(pMalloc_meta_data block);
    bool are_they_buddies(pMalloc_meta_data block1, pMalloc_meta_data block2);
    pMalloc_meta_data split_block(size_t size, pMalloc_meta_data block);
    size_t  get_smallest_size_pow_of_2(size_t size);
    int get_order_of_block(size_t size);
    bool size_in_location_avalible( pMalloc_meta_data oldp,size_t size);
    void expand_location_to_size( pMalloc_meta_data oldp,size_t requiard_size);
    friend SubList;
public:
    MemoryList() :  is_initiealized(false),list(), mmList() {};
    ~MemoryList() = default;
    MemoryList(const MemoryList &list) = delete;
    pMalloc_meta_data getBlock(size_t size);
    void free_block(pMalloc_meta_data block);
    void initialize();
    bool is_this_initileaised();
    pMalloc_meta_data create_mm_meta_data(void* data,size_t size);
    void free_mm(pMalloc_meta_data block);
    pMalloc_meta_data srealloc_aux(size_t i, pMalloc_meta_data pData);
};

bool MemoryList::is_this_initileaised() {
    return this->is_initiealized;
}

void MemoryList::initialize() {
    intptr_t address = (intptr_t)sbrk(0);
    if((void*)address == (void*)-1)
        return;
    size_t tmp1 = ((int)ceil((double)address/ALIGNMENT));
    size_t tmp2 = tmp1*ALIGNMENT;
    size_t size = tmp2-address;
    if((void*)sbrk(size) == (void*)-1)
        return;

    void* heapbottom = sbrk(LARGEST_BLOCK_SIZE*INITIAL_BLOCKS_NUM);
    if(heapbottom == (void*)-1) {
        return;
    }
    char* curr = (char*)heapbottom;
    for(int i = 0; i < 32; i++){
        pMalloc_meta_data meta_data = (pMalloc_meta_data) curr;
        meta_data->is_free = true;
        meta_data->size = LARGEST_BLOCK_SIZE - sizeof(malloc_meta_data);
        meta_data->cookie = expected_cookie;
        list[MAX_ORDER].insertBlock(meta_data);
        curr += LARGEST_BLOCK_SIZE;
    }
    total_allocated_blocks = INITIAL_BLOCKS_NUM;
    free_blocks = INITIAL_BLOCKS_NUM;
    total_bytes_with_meta = INITIAL_BLOCKS_NUM * LARGEST_BLOCK_SIZE;
    this->is_initiealized = true;
}

pMalloc_meta_data SubList::findFreeBlock() {
    pMalloc_meta_data curr = head;
    while (curr) {
        curr->did_someone_eat_my_cookie();
        if (curr->is_free) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

pMalloc_meta_data SubList::insertBlock(pMalloc_meta_data block) {
    this->size++;
    if (this->head == nullptr) {
        this->head = block;
    } else if (block < this->head) {
        block->next = this->head;
        this->head->prev = block;
        this->head = block;
    } else {
        pMalloc_meta_data current = this->head;
        while (current->next != nullptr && current->next < block) {
            current = current->next;
        }
        block->next = current->next;
        block->prev = current;
        current->next = block;
        if (block->next != nullptr) {
            block->next->prev = block;
        }
    }
    return block;
}
void SubList::removeBlock(pMalloc_meta_data block) {
    this->size--;
    if (this->head == block) {
        this->head = block->next;
        if (block->next != nullptr) {
            (block->next)->prev = nullptr;
        }
        block->prev= nullptr;
        block->next= nullptr;
        return;
    }
    (block->prev)->next = block->next;
    if (block->next != nullptr) {
        (block->next)->prev = block->prev;
    }
    block->prev= nullptr;
    block->next= nullptr;
}

size_t  MemoryList::get_smallest_size_pow_of_2(size_t size) {
    size_t metadata_size = sizeof(malloc_meta_data);
    size_t total_data_size = size + metadata_size;
    int curr = 0;
    size_t order_size = 128;
    while (curr < MAX_ORDER && order_size < total_data_size) {
        curr++;
        order_size *= 2;
    }
    return order_size;
}
int MemoryList::get_order_of_block(size_t size) {
    if (size <= 128) {
        return 0;
    }
    return log2(size/128);
}
pMalloc_meta_data MemoryList::split_block(size_t size, pMalloc_meta_data block) {
    int order = block->get_order();
    if(block->size + sizeof(malloc_meta_data) == size) {
        return block;
    }
    if (order == 0) {
        return nullptr;
    }
    list[block->get_order()].removeBlock(block);
    size_t new_size = 128 * pow(2, (order-1));
    pMalloc_meta_data new_block = reinterpret_cast<pMalloc_meta_data>(reinterpret_cast<uintptr_t>(block) + new_size);
    new_block->prev = nullptr;
    new_block->next = nullptr;
    new_block->is_free = true;
    new_block->cookie = expected_cookie;
    list[new_block->get_order()].insertBlock(new_block);
    total_allocated_blocks++;
    free_blocks++;
    block->size = new_size - sizeof(malloc_meta_data);
    list[block->get_order()].insertBlock(block);
    return split_block(size, block);
}

bool MemoryList::are_they_buddies(pMalloc_meta_data block1, pMalloc_meta_data block2) {
    if (block1 == nullptr || block2 == nullptr) {
        return false;
    }
    block1->did_someone_eat_my_cookie();
    block2->did_someone_eat_my_cookie();
    if (block1->size != block2->size) {
        return false;
    }
    size_t blockSize = block1->size + sizeof(malloc_meta_data);
    if (blockSize >= LARGEST_BLOCK_SIZE) {
        return false;
    }
    uintptr_t address1 = reinterpret_cast<uintptr_t>(block1);
    uintptr_t address2 = reinterpret_cast<uintptr_t>(block2);
    return (address1 ^ blockSize) == address2;
}

pMalloc_meta_data MemoryList::merge_blocks(pMalloc_meta_data block) {

    int order = get_order_of_block(block->size);
    while (order <  MAX_ORDER - 1) {
        pMalloc_meta_data buddy = (pMalloc_meta_data) (((intptr_t) block) ^ (block->size + sizeof(malloc_meta_data)));
        if (!(buddy->is_free && buddy->size == block->size)) {
            break;
        }
        list[block->get_order()].removeBlock(block);
        list[buddy->get_order()].removeBlock(buddy);
        if((intptr_t)block > (intptr_t)buddy){
            block = buddy;
        }
        block->size += buddy->size + sizeof(malloc_meta_data);
        total_allocated_blocks-=1;
        free_blocks-=1;
        list[block->get_order()].insertBlock(block);
        order = get_order_of_block(block->size);
    }
    return block;
}

bool MemoryList::size_in_location_avalible( pMalloc_meta_data oldp,size_t requiard_size) {
    pMalloc_meta_data block = oldp;
    int order = block->get_order();
    size_t curr_size = block->size + sizeof(malloc_meta_data);
    while (order <  MAX_ORDER - 1 && curr_size < requiard_size) {
        pMalloc_meta_data buddy = (pMalloc_meta_data) (((intptr_t) block) ^ (curr_size));
        if (!(buddy->is_free && buddy->size == block->size)) {
            return false;
        }
        if((intptr_t)block > (intptr_t)buddy){
            return false;
        }
        curr_size *=2 ;
        order = get_order_of_block(block->size);
    }
    return true;
}

void MemoryList::expand_location_to_size( pMalloc_meta_data oldp,size_t requiard_size) {
    pMalloc_meta_data block = oldp;
    int order = block->get_order();
    size_t curr_size = block->size + sizeof(malloc_meta_data);
    list[order].removeBlock(block);
    while (order <  MAX_ORDER - 1 && curr_size < requiard_size) {
        pMalloc_meta_data buddy = (pMalloc_meta_data) (((intptr_t) block) ^ (curr_size));
        list[order].removeBlock(buddy);
        if((intptr_t)block > (intptr_t)buddy){
            block = buddy;
        }
        curr_size *=2 ;
        order = get_order_of_block(block->size);
        block->size = curr_size - sizeof(malloc_meta_data);
        free_blocks-=1;
        total_allocated_blocks-=1;
    }
    list[block->get_order()].insertBlock(block);

}

pMalloc_meta_data MemoryList::srealloc_aux(size_t i, pMalloc_meta_data oldp) {
    size_t size = get_smallest_size_pow_of_2(i);
    if (size_in_location_avalible(oldp, size)) {
        expand_location_to_size(oldp, size);
        return oldp;
    }
    else return nullptr;
}

void MemoryList::free_block(pMalloc_meta_data block) {
    block->did_someone_eat_my_cookie();
    free_blocks +=1;
    used_bytes_no_meta -= block->size;
    block->is_free = true;
    merge_blocks(block);
}

pMalloc_meta_data MemoryList::getBlock(size_t size) {
    size_t size_new = this->get_smallest_size_pow_of_2(size);
    int curr_order = this->get_order_of_block(size_new);
    int n = curr_order;
    pMalloc_meta_data block = nullptr;
    while(n <= MAX_ORDER) {
        block = list[n].findFreeBlock();
        if (block != nullptr) {
            if(n > curr_order) {
                block = split_block(size_new, block);
            }
            used_bytes_no_meta += block->size;
            free_blocks -= 1;
            block->is_free = false;
            return block;
        }
        n++;
    }
    return nullptr;
}

pMalloc_meta_data MemoryList::create_mm_meta_data(void *data, size_t size) {
    pMalloc_meta_data metaData = (pMalloc_meta_data) data;
    metaData->size = size;
    metaData->is_free = false;
    metaData->cookie = expected_cookie;
    this->mmList.insertBlock(metaData);
    bytes_mm += size;
    mm_block += 1;
    return metaData;
}

void MemoryList::free_mm(pMalloc_meta_data block) {
    block->did_someone_eat_my_cookie();
    this->mmList.removeBlock(block);
    total_bytes_with_meta -= (block->size + sizeof(malloc_meta_data));
    mm_block -=1;
    mm_blocks_freed +=1;
    bytes_mm_freed += block->size;
    munmap(block, block->size + sizeof(malloc_meta_data));
}

MemoryList manager;
 int count = 0;
void* smalloc(size_t size){
    if (!manager.is_this_initileaised()) {
        manager.initialize();
    }
    if((size == 0) || (size > pow(10, 8))) {
        return nullptr;
    }
    count++;
    std::cout << " begining malloc for the time :    " << count << std::endl;
    if(size == 0) {
        return nullptr;
    }
    if (size <= NMAP_SIZE_THRESHOLD) {
        pMalloc_meta_data block = manager.getBlock(size);
        if ( block != nullptr) {
            return block + 1;
        } else {
            return nullptr;
        }
    }
    void * data = mmap(NULL, size + sizeof(malloc_meta_data), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (data != MAP_FAILED){
        pMalloc_meta_data metaData = manager.create_mm_meta_data(data, size);
        return metaData + 1;
    }
    return nullptr;
}

void* scalloc(size_t num, size_t size){
    void* block = smalloc(num * size);
    if(block) {
        return memset(block, 0, (num * size));
    }
    return nullptr;
}

void sfree(void* p){
    if(!p){
        return;
    }
    pMalloc_meta_data meta_data = ((pMalloc_meta_data)(p)) - 1;
    if(meta_data->is_free){
        return;
    }
    if(meta_data->size <= NMAP_SIZE_THRESHOLD) {
        manager.free_block(meta_data);
        return;
    }
    manager.free_mm(meta_data);
}

void* srealloc(void* oldp, size_t size) {
    if (oldp == nullptr) {
        return smalloc(size);
    }
    pMalloc_meta_data meta_data = (pMalloc_meta_data) oldp - 1;
    if (meta_data->size >= size) {
        return oldp;
    }
    if(meta_data->is_free) {
        return nullptr;
    }
    if (size <= NMAP_SIZE_THRESHOLD) {
        pMalloc_meta_data new_block = manager.srealloc_aux(size, meta_data);
        if (new_block == nullptr) {
            new_block = (pMalloc_meta_data)(smalloc(size)) - 1;
            memmove(new_block + 1, oldp, meta_data->size);
            sfree(oldp);
        }
        if (new_block == nullptr) {
            return nullptr;
        }
        return new_block + 1;
    }
    void* new_data = smalloc(size);
    if(new_data) {
        memcpy(new_data, oldp, meta_data->size);
        sfree(oldp);
    }
    return nullptr;
}

size_t _num_free_blocks(){
    return free_blocks;
}

size_t _num_free_bytes(){
    return total_bytes_with_meta + (bytes_mm_freed + mm_blocks_freed * sizeof(malloc_meta_data)) - used_bytes_no_meta - (total_allocated_blocks * sizeof(malloc_meta_data));
}

size_t _num_allocated_bytes(){
    return total_bytes_with_meta - ((total_allocated_blocks-mm_blocks_freed) * sizeof(malloc_meta_data) ) + bytes_mm;
}

size_t _num_allocated_blocks(){
    return (total_allocated_blocks + mm_block);
}

size_t _size_meta_data(){
    return sizeof(malloc_meta_data);
}

size_t _num_meta_data_bytes(){
    return  (sizeof(malloc_meta_data) * (mm_block + total_allocated_blocks));
}