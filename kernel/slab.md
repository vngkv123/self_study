# slab allocator
**kmalloc -> allocate mamory for objects smaller than page size in the kernel.**
- kernel uses 3 layers for allocating and managing memory.  
- `static __always_inline void *kmalloc(size_t size, gfp_t flags)`
- `kmalloc() -> __kmalloc() -> __do_kmalloc() -> slab_alloc() -> kmalloc_slab() or slab_alloc()`
- below code is little bit different to other kernel routine.
```
p1 = kmalloc(32);
...
slab_get_obj();

objp = index_to_obj(cachep, slabp, slabp->free);
slab->s_mem + cache->size * idx;
next = slab_bufctl(slabp)[slabp->free];
      (kmem_bufctl_t *) (slabp + 1);
slabp->free = next;
```
# struct kmem_cache
**defined in mm/slab.h**
```
struct kmem_cache{
      unsigned int object_size;
      unsigned int size;
      unsigned int align;
      unsigned long flags;
      const char *name;
      int refcount;
      void (*ctor)(void *);
      struct list_head list;
};
```
