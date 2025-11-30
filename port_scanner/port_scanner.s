	.file	"port_scanner.c"
	.intel_syntax noprefix
	.text
	.section .rdata,"dr"
.LC0:
	.ascii "127.0.0.1\0"
.LC1:
	.ascii "Port %i is OPEN.\12\0"
	.text
	.globl	scan_port
	.def	scan_port;	.scl	2;	.type	32;	.endef
	.seh_proc	scan_port
scan_port:
	# save old base pointer
	push	rbp
	.seh_pushreg	rbp
	# set new base pointer
	mov	rbp, rsp
	.seh_setframe	rbp, 0
	# allocate 112 bytes of stack space for locals
	sub	rsp, 112
	.seh_stackalloc	112
	.seh_endprologue
	mov	QWORD PTR 16[rbp], rcx
	mov	rax, QWORD PTR 16[rbp]
	mov	eax, DWORD PTR [rax]
	mov	DWORD PTR -4[rbp], eax
	mov	WORD PTR -48[rbp], 2
	lea	rax, -48[rbp]
	lea	rdx, 4[rax]
	lea	rax, .LC0[rip]
	mov	r8, rdx
	mov	rdx, rax
	mov	ecx, 2
	mov	rax, QWORD PTR __imp_inet_pton[rip]
	call	rax
	mov	eax, DWORD PTR -4[rbp]
	movzx	eax, ax
	mov	ecx, eax
	mov	rax, QWORD PTR __imp_htons[rip]
	call	rax
	mov	WORD PTR -46[rbp], ax
	mov	r8d, 0
	mov	edx, 1
	mov	ecx, 2
	mov	rax, QWORD PTR __imp_socket[rip]
	call	rax
	mov	QWORD PTR -16[rbp], rax
	mov	DWORD PTR -52[rbp], 200
	lea	rdx, -52[rbp]
	mov	rax, QWORD PTR -16[rbp]
	mov	DWORD PTR 32[rsp], 4
	mov	r9, rdx
	mov	r8d, 4102
	mov	edx, 65535
	mov	rcx, rax
	mov	rax, QWORD PTR __imp_setsockopt[rip]
	call	rax
	lea	rdx, -52[rbp]
	mov	rax, QWORD PTR -16[rbp]
	mov	DWORD PTR 32[rsp], 4
	mov	r9, rdx
	mov	r8d, 4101
	mov	edx, 65535
	mov	rcx, rax
	mov	rax, QWORD PTR __imp_setsockopt[rip]
	call	rax
	lea	rdx, -48[rbp]
	mov	rax, QWORD PTR -16[rbp]
	mov	r8d, 16
	mov	rcx, rax
	mov	rax, QWORD PTR __imp_connect[rip]
	call	rax
	mov	DWORD PTR -20[rbp], eax
	cmp	DWORD PTR -20[rbp], 0
	jne	.L2
	mov	eax, DWORD PTR -4[rbp]
	lea	rcx, .LC1[rip]
	mov	edx, eax
	call	__mingw_printf
.L2:
	mov	rax, QWORD PTR -16[rbp]
	mov	rcx, rax
	mov	rax, QWORD PTR __imp_closesocket[rip]
	call	rax
	mov	eax, 0
	add	rsp, 112
	pop	rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC2:
	.ascii "WSAStartup failed: %d\12\0"
	.align 8
.LC3:
	.ascii "Winsock initialized successfully.\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	push	rbp
	.seh_pushreg	rbp
	sub	rsp, 480
	.seh_stackalloc	480
	lea	rbp, 128[rsp]
	.seh_setframe	rbp, 128
	.seh_endprologue
	call	__main
	lea	rax, -80[rbp]
	mov	rdx, rax
	mov	ecx, 514
	mov	rax, QWORD PTR __imp_WSAStartup[rip]
	call	rax
	mov	DWORD PTR 344[rbp], eax
	cmp	DWORD PTR 344[rbp], 0
	je	.L5
	mov	eax, DWORD PTR 344[rbp]
	lea	rcx, .LC2[rip]
	mov	edx, eax
	call	__mingw_printf
	mov	eax, 1
	jmp	.L9
.L5:
	lea	rax, .LC3[rip]
	mov	rcx, rax
	call	puts
	mov	DWORD PTR 348[rbp], 1
	jmp	.L7
.L8:
	mov	ecx, 4
	call	malloc
	mov	QWORD PTR 336[rbp], rax
	mov	rax, QWORD PTR 336[rbp]
	mov	edx, DWORD PTR 348[rbp]
	mov	DWORD PTR [rax], edx
	mov	rdx, QWORD PTR 336[rbp]
	lea	rcx, scan_port[rip]
	lea	rax, -88[rbp]
	mov	r9, rdx
	mov	r8, rcx
	mov	edx, 0
	mov	rcx, rax
	call	pthread_create
	mov	rax, QWORD PTR -88[rbp]
	mov	rcx, rax
	call	pthread_detach
	add	DWORD PTR 348[rbp], 1
.L7:
	cmp	DWORD PTR 348[rbp], 2047
	jle	.L8
	mov	rax, QWORD PTR __imp_WSACleanup[rip]
	call	rax
	mov	eax, 0
.L9:
	add	rsp, 480
	pop	rbp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev8, Built by MSYS2 project) 15.2.0"
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	malloc;	.scl	2;	.type	32;	.endef
	.def	pthread_create;	.scl	2;	.type	32;	.endef
	.def	pthread_detach;	.scl	2;	.type	32;	.endef
