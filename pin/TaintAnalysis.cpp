#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "pin.H"

/*
 *
 *	stack variables layer

   0x0000000000400626 <+0>:	push   rbp
   0x0000000000400627 <+1>:	mov    rbp,rsp
   0x000000000040062a <+4>:	sub    rsp,0x130
   0x0000000000400631 <+11>:	mov    DWORD PTR [rbp-0x124],edi
   0x0000000000400637 <+17>:	mov    QWORD PTR [rbp-0x130],rsi
   0x000000000040063e <+24>:	mov    rax,QWORD PTR fs:0x28
   0x0000000000400647 <+33>:	mov    QWORD PTR [rbp-0x8],rax
   0x000000000040064b <+37>:	xor    eax,eax
   0x000000000040064d <+39>:	mov    DWORD PTR [rbp-0x11c],0xdeadbeef
   0x0000000000400657 <+49>:	mov    DWORD PTR [rbp-0x118],0xcafebabe
   0x0000000000400661 <+59>:	mov    DWORD PTR [rbp-0x114],0xc0dec0de
   0x000000000040066b <+69>:	lea    rax,[rbp-0x110]
   0x0000000000400672 <+76>:	mov    edx,0x100
   0x0000000000400677 <+81>:	mov    esi,0x41
   0x000000000040067c <+86>:	mov    rdi,rax
   0x000000000040067f <+89>:	call   0x400500 <memset@plt>

	Create Parent stack frame

*/

/* For Follow Taint Object -> Untrusted input */

typedef struct TaintObject
{
    unsigned int id;
    void *start;
    void *end;
    TaintObject *next;
} TaintObject;

typedef struct Stack
{
	/* rbp base address */
	void *start;
	/* current top stack */
	void *end;
	Stack *next
} Stack;

typedef struct StackLocalVariable
{
    bool isTainted;
    unsigned int size;
    void *start;
    void *end;
    StackLocalVariable *next;
} StackLocalVariable;

TaintObject *gHeadTaintObject;
Stack *gHeadStack;
StackLocalVariable *gHeadStackLocalVariable;

/*
Retrieves the value of registers with the current context.
*/
void getContext(CONTEXT *ctxt)
{
	fprintf(stdout, "rax: 0x%lx\nrbx: 0x%lx\nrcx: 0x%lx\nrdx: 0x%lx\nrsp: 0x%lx\nrbp: 0x%lx\nrsi: 0x%lx\nrdi: 0x%lx\nr8: 0x%lx\nr9: 0x%lx\n",
	PIN_GetContextReg(ctxt, REG_RAX),
	PIN_GetContextReg(ctxt, REG_RBX),
	PIN_GetContextReg(ctxt, REG_RCX),
	PIN_GetContextReg(ctxt, REG_RDX),
	PIN_GetContextReg(ctxt, REG_RSP),
	PIN_GetContextReg(ctxt, REG_RBP),
	PIN_GetContextReg(ctxt, REG_RSI),
	PIN_GetContextReg(ctxt, REG_RDI),
	PIN_GetContextReg(ctxt, REG_R8),
	PIN_GetContextReg(ctxt, REG_R9));
}

/*
Retrieves the arguments of a system call.
*/
void getSyscallArgs(CONTEXT *ctxt, SYSCALL_STANDARD std)
{
	for (int i = 0; i < 5; i++) {
		ADDRINT scargs = PIN_GetSyscallArgument(ctxt, std, i);
		fprintf(stdout, "arg%d: 0x%lx\n", i, scargs);
	}
}

/*
Retrieves the arguments of the sendto and recvfrom system calls. Dereferences then increments
the bufptr pointer to grab the value at each byte in the buffer.
*/
void getSyscallArgsVal(CONTEXT *ctxt, SYSCALL_STANDARD std)
{
	ADDRINT buf = PIN_GetSyscallArgument(ctxt, std, 1);
	ADDRINT len = PIN_GetSyscallArgument(ctxt, std, 2);
	int buflen = (int)len;
	char *bufptr = (char *)buf;
	fprintf(stdout, "buffer start: 0x%lx\n", buf);
	fprintf(stdout, "length: %d\n", buflen);

	for (int i = 0; i < buflen; i++, bufptr++) {
		fprintf(stdout, "%c", *bufptr);
	}
	fprintf(stdout, "\n");
}

/*
Entry function before system call execution. Checks all system call numbers but hooks
sendto and recvfrom.
*/
//void syscallEntryCallback(THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std, void *v)
void _SYSCALL_ENTRY_CALLBACK(THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std, VOID *v)
{
	ADDRINT scnum = PIN_GetSyscallNumber(ctxt, std);
	if (scnum == __NR_read)
	{
		fprintf(stdout, "systemcall read: %lu\n", scnum);
		getSyscallArgsVal(ctxt, std);

	}
	if (scnum == __NR_write)
	{
		fprintf(stdout, "systemcall write: %lu\n", scnum);
		getSyscallArgsVal(ctxt, std);
	}
}

/*
Exit function after system call execution. Grabs the system call return value.
*/
//void syscallExitCallback(THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std, void *v)
void _SYSCALL_EXIT_CALLBACK(THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std, VOID *v)
{
	//ADDRINT retval = PIN_GetSyscallReturn(ctxt, std);
	//fprintf(stdout, "retval: %lu\n", retval);
}

int Usage()
{
	fprintf(stdout, "../../../pin -t obj-intel64/syscalltest.so -- sample program");
	return -1;
}

int32_t main(int32_t argc, char *argv[])
{
    if (PIN_Init(argc, argv))
    {
        return Usage();
    }

    fprintf(stdout, "call PIN_AddSyscallEntryFunction\n");
    PIN_AddSyscallEntryFunction(&_SYSCALL_ENTRY_CALLBACK, NULL);

    fprintf(stdout, "call PIN_AddSyscallExitFunction\n");
    PIN_AddSyscallExitFunction(&_SYSCALL_EXIT_CALLBACK, NULL);

    fprintf(stdout, "call PIN_StartProgram()\n");
    PIN_StartProgram();

    return(0);
}
}
