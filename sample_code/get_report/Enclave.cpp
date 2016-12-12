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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>      /* vsnprintf */
//#include "sgx_utils.h"

#include "Enclave.h"
//#include "Enclave_t.h"  /* print_string */

#include <string.h>
#include "sgx_error.h"
#include "util.h"
#include "se_memcpy.h"
#include "trts_inst.h"


/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
//void printf(const char *fmt, ...)
//{
//    char buf[BUFSIZ] = {'\0'};
//    va_list ap;
//    va_start(ap, fmt);
//    vsnprintf(buf, BUFSIZ, fmt, ap);
//    va_end(ap);
//    ocall_print_string(buf);
//}

extern "C"
{

void my_do_ereport(const sgx_target_info_t *target_info, const sgx_report_data_t *report_data, sgx_report_t *report)
{
  // Call ENCLU[EREPORT]
  // Took this from linux-sgx/sdk/trts/linux/trts_pic.S and
  // linux-sgx/common/inc/internal/linux/linux-regs.h

  asm("/* SE_PROLOG */     \
       push    %rbx;       \
       push    %rcx;       \
       push    %rdx;       \
       movq    %rdi, %rbx; \
       movq    %rsi, %rcx; \
                           \
       movq    $0, %rax;    \
       ENCLU;              \
			   \
       /* SE_EPILOG */     \
       pop     %rdx; 	   \
       pop     %rcx;	   \
       pop     %rbx;	   \
       ret");
}


sgx_status_t my_sgx_create_report(sgx_report_t *report)
{
  // Create a report with REPORT_DATA = TARGET_INFO = NULL
  // Took this from linux-sgx/sdk/selib/sgx_create_report.cpp

  // allocate memory
  // 
  // To minimize the effort of memory management, the three elements allocation 
  // are combined in a single malloc. The calculation for the required size has
  // an assumption, that
  // the elements should be allocated in descending order of the alignment size. 
  //
  // If the alignment requirements are changed, the allocation order needs to
  // change accordingly.
  //
  // Current allocation order is:
  //     report -> target_info -> report_data
  //
  // target_info: 512-byte aligned, 512-byte length
  // report_data: 128-byte aligned,  64-byte length
  // report:      512-byte aligned, 432-byte length
  //
  size_t size = ROUND_TO(sizeof(sgx_target_info_t), TARGET_INFO_ALIGN_SIZE) +
                ROUND_TO(sizeof(sgx_report_data_t), REPORT_DATA_ALIGN_SIZE) +
                ROUND_TO(sizeof(sgx_report_t), REPORT_ALIGN_SIZE);
  size += MAX(MAX(TARGET_INFO_ALIGN_SIZE, REPORT_DATA_ALIGN_SIZE), REPORT_ALIGN_SIZE) - 1;

  void *buffer = malloc(size);
  if(buffer == NULL)
  {
      return SGX_ERROR_OUT_OF_MEMORY;
  }
  memset(buffer, 0, size);
  size_t buf_ptr = (size_t) buffer;

  buf_ptr = ROUND_TO(buf_ptr, REPORT_ALIGN_SIZE);
  sgx_report_t *tmp_report = (sgx_report_t*) buf_ptr;
  buf_ptr += sizeof(*tmp_report);

  buf_ptr = ROUND_TO(buf_ptr, TARGET_INFO_ALIGN_SIZE);
  sgx_target_info_t *tmp_target_info = (sgx_target_info_t*) buf_ptr;
  buf_ptr += sizeof(*tmp_target_info);

  buf_ptr = ROUND_TO(buf_ptr, REPORT_DATA_ALIGN_SIZE);
  sgx_report_data_t *tmp_report_data = (sgx_report_data_t*) buf_ptr;

  // Do EREPORT
  fprintf(stderr, "Calling do_ereport... ");
  do_ereport(tmp_target_info, tmp_report_data, tmp_report);
  fprintf(stderr, "do_ereport called\n");

  // Copy data to the user buffer
  memcpy_s(report, sizeof(*report), tmp_report, sizeof(*tmp_report));

  // cleanup
  // memset_s(buffer, size, 0, size);
  free(buffer);

  return SGX_SUCCESS;
}

}

void create_report(sgx_report_t* report)
{
  sgx_status_t sgx_ret;

//  memset(&report, 0, sizeof(report)); 

//  sgx_ret = sgx_create_report(NULL, NULL, report);
  sgx_ret = my_sgx_create_report(report);
  if(SGX_SUCCESS != sgx_ret)
    {
        fprintf(stderr, "sgx_create_report failed");
    }
}
