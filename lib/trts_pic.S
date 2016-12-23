/*
 * Copyright (C) 2011-2016 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


// XXX: Removed everything related to entering / exiting the enclave, because we don't enter / exit the enclave when run inside the library OS

    .file "trts_pic.S"

#include "trts_pic.h"

    .text

#ifdef LINUX32
    mov     SE_WORDSIZE(%esp), %edi
#endif
    movl    %edi, (%xax)
    ret

/* 
 * ---------------------------------------------------------------------
 * Function: thread_data_t* get_thread_data(void);
 *
 *     Get the address of thread_data
 * ---------------------------------------------------------------------
 */
DECLARE_LOCAL_FUNC get_thread_data
    READ_TD_DATA self_addr 
    ret

/* 
 * ------------------------------------------------------------------------
 * extern "C" int do_egetkey(key_request_t *key_request, key_128bit_t *key)
 * return value:
 *	0 - success
 *	none-zeor - EGETKEY error code
 * EGETKEY: rbx - the address of KEYREQUEST structure
 *	   rcx - the address where the key is outputted
 * ------------------------------------------------------------------------
 */
DECLARE_LOCAL_FUNC do_egetkey
    SE_PROLOG
    mov  $SE_EGETKEY, %xax      /* EGETKEY leaf */
    ENCLU
#ifdef SE_SIM
    cmp  $SGX_SUCCESS, %xax     /* In simulation mode, ZF flag will not be set */
    jnz	 .Legetkey_done         /* because the stack clean operation will always clean ZF flag */
#else
    jz   .Legetkey_done         /* if EGETKEY error, ZF flag is set and error code is set to xax */
#endif
    xor  %xax, %xax
.Legetkey_done:
    SE_EPILOG
    ret


/* 
 * -------------------------------------------------------------------------
 * extern "C" void do_ereport(sgx_target_info_t *target_info, sgx_report_data_t *report_data, sgx_report_t *report);
 * EREPORT: rbx - the address of TARGETINFO;
 *          rcx - the address of REPORTDATA;
 *          rdx - the address where REPORT is outputted
 * -------------------------------------------------------------------------
 */
DECLARE_LOCAL_FUNC do_ereport
    SE_PROLOG
    mov       $SE_EREPORT, %xax  /* EREPORT leaf */
    ENCLU
    SE_EPILOG
    ret
    
#define _RDRAND_RETRY_TIMES 10	
/* 
 * -------------------------------------
 * extern "C" uint32_t do_rdrand(uint32_t *rand);
 * return value:
 *	non-zero: rdrand succeeded
 *	zero: rdrand failed
 * -------------------------------------
 */
DECLARE_LOCAL_FUNC do_rdrand
    mov $_RDRAND_RETRY_TIMES, %ecx
.Lrdrand_retry:
    .byte 0x0F, 0xC7, 0xF0	    /* rdrand %eax */
    jc	.Lrdrand_return
    dec	%ecx
    jnz 	.Lrdrand_retry
    xor 	%xax, %xax
    ret
.Lrdrand_return:
#ifdef LINUX32
    mov     SE_WORDSIZE(%esp), %ecx
#else
    mov     %rdi, %rcx
#endif
    movl    %eax, (%xcx)
    mov     $1, %xax
    ret

/* 
 * -------------------------------------------------------------------------
 * extern "C" __attribute__((regparm(1))) void continue_execution(sgx_exception_info_t *info);
 * -------------------------------------------------------------------------
 */
DECLARE_LOCAL_FUNC continue_execution
#ifdef LINUX32
    mov     %xax, %xcx
#else
    mov     %xdi, %xcx
#endif
    mov     SE_WORDSIZE*4(%xcx), %xdx
    mov     %xdx, %xsp
    sub     $(SE_WORDSIZE), %xsp
#ifdef LINUX64
    mov     SE_WORDSIZE*17(%xcx), %xax
#else
    mov     SE_WORDSIZE*9(%xcx), %xax
#endif
    mov     %xax, (%xsp)
    mov     SE_WORDSIZE*0(%xcx), %xax
    mov     SE_WORDSIZE*2(%xcx), %xdx
    mov     SE_WORDSIZE*3(%xcx), %xbx
    mov     SE_WORDSIZE*5(%xcx), %xbp
    mov     SE_WORDSIZE*6(%xcx), %xsi
    mov     SE_WORDSIZE*7(%xcx), %xdi
#ifdef LINUX64
    mov     SE_WORDSIZE*8(%xcx), %r8
    mov     SE_WORDSIZE*9(%xcx), %r9
    mov     SE_WORDSIZE*10(%xcx), %r10
    mov     SE_WORDSIZE*11(%xcx), %r11
    mov     SE_WORDSIZE*12(%xcx), %r12
    mov     SE_WORDSIZE*13(%xcx), %r13
    mov     SE_WORDSIZE*14(%xcx), %r14
    mov     SE_WORDSIZE*15(%xcx), %r15
    push    SE_WORDSIZE*16(%xcx)
    popf
#else
    push    SE_WORDSIZE*8(%xcx)
    popf
#endif
    mov     SE_WORDSIZE*1(%xcx), %xcx
    ret
