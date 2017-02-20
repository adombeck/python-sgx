#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#define BUF_NUM 2

#define MOD2(x) ((x) % BUF_NUM)

struct sealed_buf_t
{
  unsigned int index;
  void * sealed_buf_ptr[BUF_NUM];
};


#ifdef __cplusplus
extern "C" {
#endif


int initialize_enclave(struct sealed_buf_t* sealed_buf);
int increase_and_seal_data(size_t tid, struct sealed_buf_t* sealed_buf);

sgx_status_t SGX_CDECL print(const char* string);
sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf);
sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter);
sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
