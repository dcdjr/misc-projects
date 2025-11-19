#include <stdio.h>
#include <string.h>

#define HEAP_SIZE (1024 * 1024)
static unsigned char heap[HEAP_SIZE];

typedef struct block_header {
    size_t size;
    int free;
    struct block_header *next;
} block_header;

block_header *free_list = (block_header *)heap;

void init_allocator() {
    free_list->size = HEAP_SIZE - sizeof(block_header);
    free_list->free = 1;
    free_list->next = NULL;
}

// --- Utility: get the real physical next block ---
block_header *phys_next(block_header *block) {
    unsigned char *addr = (unsigned char *)(block + 1);
    addr += block->size;
    if (addr >= heap + HEAP_SIZE) return NULL;
    return (block_header *)addr;
}

// --- Find a free block (first fit) ---
block_header *find_free_block(size_t size) {
    block_header *cur = free_list;
    while (cur) {
        if (cur->free && cur->size >= size)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

// --- Split a block if leftover is big enough ---
void split_block(block_header *block, size_t size) {
    if (block->size < size + sizeof(block_header)) return;

    unsigned char *addr = (unsigned char *)(block + 1);
    addr += size;

    block_header *new_block = (block_header *)addr;
    new_block->size = block->size - size - sizeof(block_header);
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
}

// --- Coalesce block with its physical neighbor ---
void coalesce(block_header *block) {
    block_header *pn = phys_next(block);
    if (!pn) return;
    if (!pn->free) return;

    // merge
    block->size += pn->size + sizeof(block_header);
    block->next = pn->next;
}

// --- Insert a freed block back into free list (sorted by physical memory) ---
void my_free(void *ptr) {
    if (!ptr) return;

    block_header *block = (block_header *)ptr - 1;
    block->free = 1;

    block_header *cur = free_list;
    block_header *prev = NULL;

    // find where to insert the block physically
    while (cur && cur < block) {
        prev = cur;
        cur = cur->next;
    }

    block->next = cur;
    if (prev == NULL)
        free_list = block;
    else
        prev->next = block;

    // try merging forward
    coalesce(block);

    // try merging backward
    if (prev && prev->free)
        coalesce(prev);
}

// --- Allocate memory ---
void *my_malloc(size_t size) {
    block_header *block = find_free_block(size);
    if (!block) return NULL;

    split_block(block, size);
    block->free = 0;

    return (void *)(block + 1);
}

// --- Reallocate memory ---
void *my_realloc(void *ptr, size_t new_size) {
    if (!ptr) return my_malloc(new_size);

    block_header *block = (block_header *)ptr - 1;

    // Case 1: shrinking
    if (new_size <= block->size) {
        split_block(block, new_size);
        return ptr;
    }

    // Check physical neighbor
    block_header *pn = phys_next(block);

    // Case 2: expand in place
    if (pn && pn->free &&
        block->size + pn->size + sizeof(block_header) >= new_size) {

        coalesce(block);
        split_block(block, new_size);
        return ptr;
    }

    // Case 3: fallback allocate/copy
    void *new_ptr = my_malloc(new_size);
    if (!new_ptr) return NULL;

    size_t copy_size = (block->size < new_size) ? block->size : new_size;
    memcpy(new_ptr, ptr, copy_size);

    my_free(ptr);
    return new_ptr;
}

int main() {
    init_allocator();

    int *digits = my_malloc(4 * sizeof(int));
    

    return 0;
}
