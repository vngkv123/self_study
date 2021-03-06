# Challenge 2 Write-up
**GDB**
```
gef➤  r
Starting program: /shared/aegis/reversing/challenge2/challenge2
Debugging Detect
[Inferior 1 (process 4876) exited with code 0377]
```
- 일반적으로 gdb를 열어서 디버깅을 할려고하면, 위와 같이 Debugging Detect를 보내면서 죽게된다.
- Linux에서는 디버깅탐지를 일반적으로 ptrace()라는 함수를 이용해서 진행한다.
- ptrace()함수에는 TRACEME라는 옵션을 줄 수가 있고, 각 프로세스에는 하나의 디버거만 붙을 수 있다. ( ptrace()의 내부 구현이 그렇게 되어 잇다. )
```
void __attribute__((constructor)) detection(void){
	if( ptrace(PTRACE_TRACEME, 0, 1, 0) == -1 ){
		printf("Debugging Detect\n");
		exit(-1);
	}
}
```
- ptrace가 정상적으로 실행되었으면, return 값으로는 0이 반환되는데, 디버거가 붙어있게되면 -1이 리턴되어서 exit을 통해 탈출하게 된다.
- 소스코드를 보고 분석을 하였을 시에는, 이렇게 쉽게 찾아낼 수 있지만, gcc의 __attribute__((constructor)) 확장 속성을 이용하게되면, 위의 함수는 main()함수 이전에 실행되게 된다.
- .init_array라는 영역에 들어가게되어, main이전에 __libc_start_main()이라는 함수에서 실행해주게 된다.
```
  [19] .init_array       INIT_ARRAY       0000000000600e08  00000e08
       0000000000000010  0000000000000000  WA       0     0     8
       
```
- gdb에서 분석을 하게되면 이와 같다.
```
gef➤  x/gx 0x0000000000600e08
0x600e08:	0x0000000000400660
gef➤
0x600e10:	0x0000000000400686
gef➤
0x600e18:	0x0000000000400640
```
- 하나하나 주소를 x/i라는 명령을 통해서 instruction을 분석해보게되면,
```
gef➤  x/40i 0x400686
   0x400686 <detection>:	push   rbp
   0x400687 <detection+1>:	mov    rbp,rsp
   0x40068a <detection+4>:	mov    ecx,0x0
   0x40068f <detection+9>:	mov    edx,0x1
   0x400694 <detection+14>:	mov    esi,0x0
   0x400699 <detection+19>:	mov    edi,0x0
   0x40069e <detection+24>:	mov    eax,0x0
   0x4006a3 <detection+29>:	call   0x400560 <ptrace@plt>
   0x4006a8 <detection+34>:	cmp    rax,0xffffffffffffffff
   0x4006ac <detection+38>:	jne    0x4006c2 <detection+60>
   0x4006ae <detection+40>:	mov    edi,0x400888
   0x4006b3 <detection+45>:	call   0x400520 <puts@plt>
   0x4006b8 <detection+50>:	mov    edi,0xffffffff
   0x4006bd <detection+55>:	call   0x400570 <exit@plt>
   0x4006c2 <detection+60>:	nop
   0x4006c3 <detection+61>:	pop    rbp
   0x4006c4 <detection+62>:	ret
```
- 0x400686을 분석하게되면 위와같은 결과를 얻을 수 있다.
I can't type korean anymore T_T... Some github errors exist...
- It call ptrace() function in this address, and check whether return value is -1 or not.
- So, to solve this challenge with `GDB`, i set first breakpoint in 0x4006a3 ( ptrace() ) : `b *0x4006a3`
```
 →   0x4006a3 <detection+29>   call   0x400560 <ptrace@plt>
   ↳    0x400560 <ptrace@plt+0>   jmp    QWORD PTR [rip+0x200ada]        # 0x601040
        0x400566 <ptrace@plt+6>   push   0x5
        0x40056b <ptrace@plt+11>  jmp    0x400500
        0x400570 <exit@plt+0>     jmp    QWORD PTR [rip+0x200ad2]        # 0x601048
        0x400576 <exit@plt+6>     push   0x6
        0x40057b <exit@plt+11>    jmp    0x400500
─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────[ threads ]────
[#0] Id 1, Name: "challenge2", stopped, reason: BREAKPOINT
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────[ trace ]────
[#0] 0x4006a3 → Name: detection()
[#1] 0x40084d → Name: __libc_csu_init()
[#2] 0x7ffff7a2d7bf → Name: __libc_start_main(main=0x4007b2 <main>, argc=0x1, argv=0x7fffffffe5a8, init=0x400800 <__libc_csu_init>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fffffffe598)
[#3] 0x4005b9 → Name: _start()
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
gef➤
```
- `n` is next-step command in `gdb`, so enter the `n`, i get return value in `rax` register.
```
$rax   : 0xffffffffffffffff
```
- And change the return value to 0 to bypass next `cmp` instruction : `set $rax=0`
- ! Now we bypass the `anti-debugging` routine.
- Next step is bypass `strncmp` function in `verify`
```
exploit-peda$ pd verify
Dump of assembler code for function verify:
   0x00000000004006c5 <+0>:	push   rbp
   0x00000000004006c6 <+1>:	mov    rbp,rsp
   0x00000000004006c9 <+4>:	sub    rsp,0x20
   0x00000000004006cd <+8>:	mov    QWORD PTR [rbp-0x18],rdi
   0x00000000004006d1 <+12>:	mov    edi,0x4008a0
   0x00000000004006d6 <+17>:	call   0x400520 <puts@plt>
   0x00000000004006db <+22>:	mov    rax,QWORD PTR [rbp-0x18]
   0x00000000004006df <+26>:	add    rax,0x8
   0x00000000004006e3 <+30>:	mov    rax,QWORD PTR [rax]
   0x00000000004006e6 <+33>:	mov    edx,0x4
   0x00000000004006eb <+38>:	mov    rsi,rax
   0x00000000004006ee <+41>:	mov    edi,0x4008cd
   0x00000000004006f3 <+46>:	call   0x400510 <strncmp@plt>
```
- set `b *0x4006f3`
- and `c` -> continue
- `n` to do `next-step`, change the `rax` register ( return value ) to 0 which means two strings are equal.
```
0x00000000004006f8 in verify ()
exploit-peda$ set $rax=0
exploit-peda$ c
Continuing.
A3gis_ch4llenge2_c0mplete!
```
