#ifndef PIXELBYTE_MEMORY_H
#define PIXELBYTE_MEMORY_H

#include "pch.h"

typedef uint8_t* pxb_memory_t;

pxb_memory_t pxb_memory_create(const size_t sizeInBytes);

void* pxb_memory_allocate(pxb_memory_t memory, const size_t offset, const size_t sizeInBytes);

void pxb_memory_deallocate(pxb_memory_t memory, const size_t offset, const size_t sizeInBytes);

void pxb_memory_destroy(pxb_memory_t memory);

#endif // PIXELBYTE_MEMORY_H
