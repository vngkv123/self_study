# DWARF INFORMATION
**what is dwarf?**
- `Debugging With Attributed Record Formats`
- Used to stack unwinding in c/c++

**what is stack unwind?**
- Let's take an example.
```
#include <iostream>
#include <stdio.h>

using namespace std;

void f3(void){
  throw 10;
}

void f2(void){
  f3();
}

void f1(void){
  f2();
}

int main(void){
  try{
    f1();
  }
  catch(int e){
    printf("got exception : %d\n", e);
  }
}
```

- If i throw some value, it backtrace stack frame, find last catch statement.
- In above example code, throw statement is in the f3().
- Function flow is main() -> f1() -> f2() -> f3().
- So, catch statement is not defined in f3(), f2() and f1(), it has to backtrace from f3() to main().
- To successfully backtrace, DWARF information is needed, which placed in .eh_frame section or .debug_frame section.

**CFI? CIE? FDE?**
- CFI is Call Frame Information, which is information to stack backtrace( stack unwind ).
- CFI record information about stack frame on special section( .eh_frame ) in each function.
- In CFI, CFA ( Canocical Frame Address ) is used to represent current function start frame pointer.
```
.cfi_def_cfa           <reg, ofs>
.cfi_def_cfa_register  <reg>
.cfi_def_cfa_offset    <ofs>
.cfi_offset    <reg, ofs>
```
- CIE set CFA's initial rule.
- FDE record some change in CFI in each function.
- Each CFI record contains a Common Information Entry (CIE) record followed by 1 or more Frame Description Entry (FDE) records
- DWARF uses a data structure called a Debugging Information Entry (DIE) to represent each variable, type, procedure, etc.

```
main:
.LFB1024:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA1024
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
.LEHB0:
	call	_Z2f1v
.LEHE0:
.L9:
	movl	$0, %eax
	jmp	.L13
.L11:
	cmpq	$1, %rdx
	je	.L8
	movq	%rax, %rdi
.LEHB1:
	call	_Unwind_Resume
.LEHE1:
.L8:
	movq	%rax, %rdi
	call	__cxa_begin_catch
	movl	(%rax), %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
.LEHB2:
	call	printf
.LEHE2:
	call	__cxa_end_catch
	jmp	.L9
.L12:
	movq	%rax, %rbx
	call	__cxa_end_catch
	movq	%rbx, %rax
	movq	%rax, %rdi
.LEHB3:
	call	_Unwind_Resume
.LEHE3:
.L13:
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
```
- Every function start with .cfi_startproc and end with .cfi_endproc
