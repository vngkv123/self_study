# slab allocator
**kmalloc -> allocate mamory for objects smaller than page size in the kernel.**
- kernel uses 3 layers for allocating and managing memory.  
```
static __always_inline void *kmalloc(size_t size, gfp_t flags)`
```