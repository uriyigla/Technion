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



int free_blocks = 0; // free placses in list
size_t total_bytes_with_meta = 0;

int total_allocated_blocks = 0; //len of list
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
        //std::cout << " order of size  " << size << "  is  " <<(int)log2((size + sizeof(struct MallocMetaData))/128) << std::endl;
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

    pMalloc_meta_data merge_blocks(pMalloc_meta_data block); //merges 2 buddies
    bool are_they_buddies(pMalloc_meta_data block1, pMalloc_meta_data block2);//checks if they are buddies
    pMalloc_meta_data split_block(size_t size, pMalloc_meta_data block);//obvious i think
    //idx helpers
    size_t  get_smallest_size_pow_of_2(size_t size);
    int get_order_of_block(size_t size);
    //realoc helpers
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
    //pMalloc_meta_data srealloc_aux(size_t size, pMalloc_meta_data old_block);

    //mm Funcs
    pMalloc_meta_data create_mm_meta_data(void* data,size_t size);
    void free_mm(pMalloc_meta_data block);

    pMalloc_meta_data srealloc_aux(size_t i, pMalloc_meta_data pData);
};

/// functions for small allocations

////   INITIALIZER FUNCTIONS
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
    //initiealize the blocks
   // //std::cout << "HEAP TOP  : " << curr << std::endl;
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


///         SUB LIST FUNCTIONS
pMalloc_meta_data SubList::findFreeBlock() {
    pMalloc_meta_data curr = head;
    //std::cout << "serching free in order :  " <<std::endl;
    if(head) {
        //std::cout << head->get_order()<<std::endl;
    } else {
        //std::cout << "head is null " << std::endl;
    }
    while (curr) {
        //std::cout << "curr address  " << curr << std::endl;
        //std ::cout << *curr << std::endl;
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
    //std::cout << "Inserting block idx  " << block->get_order() << std::endl;
    if (this->head == nullptr) {
        // If the list is empty, make the new node the head
        this->head = block;
    } else if (block < this->head) {
        // If the new node is smaller than the head, insert it at the beginning
        block->next = this->head;
        this->head->prev = block;
        this->head = block;
    } else {
        pMalloc_meta_data current = this->head;

        // Find the appropriate position to insert the new node
        while (current->next != nullptr && current->next < block) {
            current = current->next;
        }

        // Make the appropriate links
        block->next = current->next;
        block->prev = current;
        current->next = block;
        // If the new node is not inserted at the end of the list
        if (block->next != nullptr) {
            //std::cout << "insert not end but middle" << std::endl;
            block->next->prev = block;
        }
    }
    return block;
}

void SubList::removeBlock(pMalloc_meta_data block) {
    //std::cout << "remove start" << std::endl;
    //std::cout << "list is ok " << this->size << std::endl;
    //std::cout << "list IDX   " << block->get_order() << std::endl;
    //std::cout << "block address  " << block << " " << *block << std::endl;

    this->size--;
    //std::cout << this->head << block << "shold be same.." <<std::endl;
    if (this->head == block) {
        //std::cout << "head is block" << std::endl;

        this->head = block->next;
        if (block->next != nullptr) {
            //std::cout << "list is not block " << std::endl;
            (block->next)->prev = nullptr;
        }

        //std::cout << "remove end" << std::endl;
        block->prev= nullptr;
        block->next= nullptr;

        return;
    }

    //std::cout << "there must be prev..." << std::endl;
    (block->prev)->next = block->next;
    //std::cout << "There is" << std::endl;

    if (block->next != nullptr) {
        //std::cout << "yes next" << std::endl;
        (block->next)->prev = block->prev;
        //std::cout << "next succeeded" << std::endl;
    }
    block->prev= nullptr;
    block->next= nullptr;

    //std::cout << "remove end" << std::endl;
}



///     SIZING HELPER FUNCTIONS
//finds what size of a block should we use for give data
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
//given a size, returns idx of sub list
int MemoryList::get_order_of_block(size_t size) {
    if (size <= 128) {
        return 0;
    }
    return log2(size/128);
}




////                  MAMAGER FUNCTIONS ---- HELPERS
pMalloc_meta_data MemoryList::split_block(size_t size, pMalloc_meta_data block) {
    int order = block->get_order();
    //std::cout << "split begin " << std::endl;

    if(block->size + sizeof(malloc_meta_data) == size) {
        return block;
    }
   //std::cout << "split block  : splitin  " << *block <<  "  adres   " << block<< std::endl;

    if (order == 0) {
        return nullptr;
    }
    list[block->get_order()].removeBlock(block);
    size_t new_size = 128 * pow(2, (order-1));
    pMalloc_meta_data new_block = reinterpret_cast<pMalloc_meta_data>(reinterpret_cast<uintptr_t>(block) + new_size);
   // //std::cout<< "firs add  :" <<reinterpret_cast<uintptr_t>(block) <<"  2nd add   " << reinterpret_cast<uintptr_t>(new_block) << "  diff  " << new_size << std::endl;
    new_block->size = new_size - sizeof(malloc_meta_data);
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

    //std::cout << block1 << "  block 1 address" << std::endl;
    //std::cout << *block1 << "  block 1" << std::endl;

    //std::cout << block2 << "  block 2 address" << std::endl;
    //std::cout << *block2 << "  block 2" << std::endl;

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

    //std::cout << "the XOR result is: " << (address1 ^ address2) << std::endl;
    //std::cout << "the block size is: " << blockSize << std::endl;

    return (address1 ^ blockSize) == address2;
}

//assuming block1<block2
pMalloc_meta_data MemoryList::merge_blocks(pMalloc_meta_data block) {
    /*if(!are_they_buddies(block1,block2)) {
        //std::cout << "NOT BUDDIES" << std::endl;
        return block1;
    }
    if (!(block1->is_free && block2->is_free)) {
        //std::cout << "BUDDIES NOT FREE" << std::endl;
        return block1;
    }
    if(block1->size != block2->size) {
        //std::cout << "BUDDIES different size" << std::endl;
        return block1;
    }
    if(block1->size + sizeof(malloc_meta_data) >= LARGEST_BLOCK_SIZE) {
        return block1;
    }
    //std::cout << "merge begin num  " << _num_allocated_blocks() << std::endl;

    int order = get_order_of_block(block1->size + sizeof(malloc_meta_data));
    //std::cout << "the order of the block we remove  " << order << std::endl;
    list[order].removeBlock(block1);
    list[order].removeBlock(block2);
    block1->size += sizeof(malloc_meta_data) + block2->size;
    list[order+1].insertBlock(block1);
    total_allocated_blocks-=1;
    free_blocks-=1;
    if(are_they_buddies(block1,block1->next)) {
        merge_blocks(block1, block1->next);
    }
    if(are_they_buddies(block1->prev,block1)) {
        merge_blocks(block1->prev,block1);
    }
    return block1;*/

    int order = get_order_of_block(block->size);
    while (order <  MAX_ORDER - 1) {

        pMalloc_meta_data buddy = (pMalloc_meta_data) (((intptr_t) block) ^ (block->size + sizeof(malloc_meta_data)));
        //std::cout << "BUDDY IS  " <<*buddy << "      he lives in addres  " << buddy << std::endl;
        if (!(buddy->is_free && buddy->size == block->size)) {
            //buddy not free or has a different size
            break;
        }
        //std::cout << "good" << std::endl;
        list[block->get_order()].removeBlock(block);
        //std::cout << "1 remove" << std::endl;

        list[buddy->get_order()].removeBlock(buddy);
        //std::cout << "2remove!" << std::endl;
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

//// realoc helpers
bool MemoryList::size_in_location_avalible( pMalloc_meta_data oldp,size_t requiard_size) {
    pMalloc_meta_data block = oldp;
    int order = block->get_order();
    size_t curr_size = block->size + sizeof(malloc_meta_data);
    while (order <  MAX_ORDER - 1 && curr_size < requiard_size) {
        pMalloc_meta_data buddy = (pMalloc_meta_data) (((intptr_t) block) ^ (curr_size));
        //std::cout << "BUDDY IS  " <<*buddy << "      he lives in addres  " << buddy << std::endl;
        if (!(buddy->is_free && buddy->size == block->size)) {
            //buddy not free or has a different size
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
        //std::cout << "BUDDY IS  " <<*buddy << "      he lives in addres  " << buddy << std::endl;
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


////                  MAMAGER FUNCTIONS ---- public

void MemoryList::free_block(pMalloc_meta_data block) {
    block->did_someone_eat_my_cookie();
    //std::cout << " free start" <<std::endl;
    free_blocks +=1;
    used_bytes_no_meta -= block->size;
    //std::cout << "free before merger num  " << _num_allocated_blocks() << std::endl;

    block->is_free = true;
    merge_blocks(block);
    /*if(are_they_buddies(block,block->next)) {
        //std::cout << "374"  << std::endl;

        merge_blocks(block, block->next);
    }else if(are_they_buddies(block->prev,block)) {
        //std::cout << "378"<< std::endl;

        merge_blocks(block->prev,block);
    }*/
    //std::cout << "free post merger num  " << _num_allocated_blocks() << std::endl;

    //std::cout << " FREE DONE" <<std::endl;
}

pMalloc_meta_data MemoryList::getBlock(size_t size) {
    size_t size_new = this->get_smallest_size_pow_of_2(size);
    //std::cout << "get block begin num  " << _num_allocated_blocks() << std::endl;

    int curr_order = this->get_order_of_block(size_new);
    //std::cout << "432 "   << std::endl;

    int n = curr_order;

    pMalloc_meta_data block = nullptr;

    while(n <= MAX_ORDER) {
        //std::cout << "439 "   << std::endl;

        block = list[n].findFreeBlock();
        //std::cout << "442 "   << std::endl;

        if (block != nullptr) {
            if(n > curr_order) {
                //std::cout << "446 "   << std::endl;
                block = split_block(size_new, block);
                //std::cout << "fin block 197" << std::endl;
            }

            used_bytes_no_meta += block->size;
            free_blocks -= 1;
            block->is_free = false;
            //std::cout << "find block  : returing curr" << *block << std::endl;
            //std::cout << "get block end  num  " << _num_allocated_blocks() << std::endl;

            return block;
        }
        n++;
    }
    //std::cout << "return null" <<std::endl;
    return nullptr;
}



/// helper for memeap allocations
pMalloc_meta_data MemoryList::create_mm_meta_data(void *data, size_t size) {
    pMalloc_meta_data metaData = (pMalloc_meta_data) data;
    metaData->size = size;
    metaData->is_free = false;
    metaData->cookie = expected_cookie;
    this->mmList.insertBlock(metaData);

    //std::cout << "total_bytes_with_meta :    " << _num_allocated_bytes()<< std::endl;
    bytes_mm += size;
    mm_block += 1;
    //std::cout << "total_bytes_with_meta AFTER UPDATE:    " << _num_allocated_bytes()<< std::endl;

    return metaData;
}

void MemoryList::free_mm(pMalloc_meta_data block) {
    //std::cout << "break free!!" << std::endl;
    block->did_someone_eat_my_cookie();
    this->mmList.removeBlock(block);
    //std::cout << "break free!! removed from list" << std::endl;

    total_bytes_with_meta -= (block->size + sizeof(malloc_meta_data));
    mm_block -=1;
    mm_blocks_freed +=1;
    bytes_mm_freed += block->size;
    //bytes_mm -= block->size;
    //std::cout << "break free!! changed allocations.  allocated bytes" << _num_allocated_bytes() << std::endl;

    munmap(block, block->size + sizeof(malloc_meta_data));
}



///        Assigment Functions

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

    //std::cout<< "line 405 allocated blocks" << total_allocated_blocks<< std::endl;
    //std::cout<< "line 40 free block " << total_allocated_blocks<< std::endl;

    if(size == 0) {
        return nullptr;
    }
    if (size <= NMAP_SIZE_THRESHOLD) {

        pMalloc_meta_data block = manager.getBlock(size);
        if ( block != nullptr) {
            //std::cout << *block << std::endl;

            return block + 1;
        } else {
            return nullptr;
        }
    }
    //use mmap
    //std::cout << "449 mmap :: size asked  "<< size << "  size meta  "<< sizeof(malloc_meta_data)  << "  last size "  << _num_allocated_bytes() <<  std::endl;
    //std::cout << size + sizeof(malloc_meta_data) + _num_allocated_bytes() << std::endl;
    void * data = mmap(NULL, size + sizeof(malloc_meta_data), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (data != MAP_FAILED){
        //std::cout << " mmap went WELL 484" << std::endl;
        pMalloc_meta_data metaData = manager.create_mm_meta_data(data, size);
        //std::cout << " mmap adding to list went well" << std::endl;
        //std::cout << " END malloc total_bytes_with_meta :    " << _num_allocated_bytes()<< std::endl;

        return metaData + 1;
    }
    //std::cout << "456 return null" << std::endl;

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
    //else mmap
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
    //mmap
    void* new_data = smalloc(size);
    if(new_data) {
        memcpy(new_data, oldp, meta_data->size);
        sfree(oldp);
    }
    return nullptr;
}


///        GETTERS
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