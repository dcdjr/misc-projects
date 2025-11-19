# Custom Memory Allocator (`my_malloc.c`)

A simple C implementation of `malloc`, `free`, and `realloc` built to understand how low-level memory management works.  
Uses a **simulated 1MB heap**, block headers, a free list, splitting, coalescing, and physical-order insertion.

---

## Features
- 1 MB static simulated heap  
- `my_malloc()` — first-fit allocation  
- `my_free()` — frees a block and reinserts it in physical order  
- `my_realloc()` — supports shrinking, in-place expansion, or allocate-and-copy  
- Block splitting  
- Forward and backward coalescing  
- Physical neighbor discovery  
- Simple free list linked structure  

---

## Block Structure

```c
typedef struct block_header {
    size_t size;
    int free;
    struct block_header *next;
} block_header;
