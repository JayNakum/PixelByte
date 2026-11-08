#include "memory.h"

pxb_memory_t pxb_memory_create(const size_t sizeInBytes)
{
    pxb_memory_t allocation = (uint8_t*)malloc(sizeInBytes);

#ifdef PXB_DEBUG
    memset(allocation, 0xA5, sizeInBytes);
#endif

    PXB_ASSERT(allocation != NULL);

    return allocation;
}

void* pxb_memory_allocate(pxb_memory_t memory, const size_t offset, const size_t sizeInBytes)
{
#ifdef PXB_DEBUG
    memset(memory + offset, 0xA5, sizeInBytes);
#endif

    (void)(sizeInBytes);
    return (void*)(memory + offset);
}

void pxb_memory_deallocate(pxb_memory_t memory, const size_t offset, const size_t sizeInBytes)
{
#ifdef PXB_DEBUG
    memset(memory + offset, 0xA5, sizeInBytes);
#endif

    (void)(memory);
    (void)(offset);
    (void)(sizeInBytes);
}

void pxb_memory_destroy(pxb_memory_t memory)
{
    free(memory);
}
