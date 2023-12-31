//
// Created by chens on 29/06/2023.
//

#include <unistd.h>
#include <cstddef>
#include <cmath>

void* smalloc(size_t size){
    if((size == 0) || (size > pow(10, 8)))
        return NULL;
    void* result = sbrk(size);
    if (result == (void*)(-1)) {
        return NULL;
    }
    return result;

}