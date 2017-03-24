#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <Python.h>

#include "sgx_tkey_exchange.h"
#include "sgx_ukey_exchange.h"
#include "global_data.h"

static sgx_ec256_public_t g_sp_pub_key = {
    {
        0x72, 0x12, 0x8a, 0x7a, 0x17, 0x52, 0x6e, 0xbf,
        0x85, 0xd0, 0x3a, 0x62, 0x37, 0x30, 0xae, 0xad,
        0x3e, 0x3d, 0xaa, 0xee, 0x9c, 0x60, 0x73, 0x1d,
        0xb0, 0x5b, 0xe8, 0x62, 0x1c, 0x4b, 0xeb, 0x38
    },
    {
        0xd4, 0x81, 0x40, 0xd9, 0x50, 0xe2, 0x57, 0x7b,
        0x26, 0xee, 0xb7, 0x41, 0xe7, 0xc6, 0x14, 0xe2,
        0x24, 0xb7, 0xbd, 0xc9, 0x03, 0xf2, 0x9a, 0x28,
        0xa8, 0x3c, 0xc8, 0x10, 0x11, 0x14, 0x5e, 0x06
    }

};

void initialize_remote_attestation(sgx_ec256_public_t* p_public_key, int use_pse, sgx_ra_context_t* p_context)
{
    sgx_status_t ret = sgx_ra_init(p_public_key, use_pse, p_context);
    if(ret != SGX_SUCCESS)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Failed to call sgx_ra_init. Error code: 0x%x\n", ret);
        return;
    }
    fprintf(stderr, "Successfully initialized remote attestation\n");
}


//sgx_ra_msg1_t* get_msg1(sgx_ra_context_t context){
//    sgx_ra_msg1_t* p_msg1;
//    p_msg1 = (sgx_ra_msg1_t*) malloc(sizeof(sgx_ra_msg1_t));
//    if(NULL == p_msg1)
//    {
//        fprintf(stderr, "Failed to allocate memory for p_msg1\n");
//        return NULL;
//    }
//
//    sgx_status_t ret = sgx_ra_get_msg1(context, p_msg1);
//    if(ret != SGX_SUCCESS)
//    {
//        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
//        fprintf(stderr, "Failed to call sgx_ra_get_msg1. Error code: 0x%x\n", ret);
//        return NULL;
//    }
//
//    fprintf(stderr, "Successfully created msg1\n");
//    return p_msg1;
//}

void initialize_quote()
{
    sgx_epid_group_id_t gid = {0};
    sgx_target_info_t qe_target_info;

    memset(&qe_target_info, 0, sizeof(qe_target_info));
    sgx_status_t ret = sgx_init_quote(&qe_target_info, &gid);
    if(ret != SGX_SUCCESS)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Failed to call sgx_init_quote. Error code: 0x%x\n", ret);
        return;
    }
    fprintf(stderr, "Successfully initialized quote\n");
}


int main()
{
    fprintf(stderr, "Testing allocation... ");
    char* s = malloc(1024);
    if (s == NULL)
    {
        fprintf(stderr, "allocation failed\n");
        return 1;
    }
    fprintf(stderr, "allocation succeeded\n");

    fprintf(stderr, "&g_sp_pub_key: %p\n", &g_sp_pub_key);
    fprintf(stderr, "&g_global_data: %p\n", &g_global_data);
    fprintf(stderr, "g_global_data.enclave_size: %lx\n", g_global_data.enclave_size);
    fprintf(stderr, "g_global_data.heap_size: %lx\n", g_global_data.heap_size);
    fprintf(stderr, "enclave_base: %x\n", get_enclave_base());
    fprintf(stderr, "__ImageBase: %p\n", &__ImageBase);

    sgx_ra_context_t context = INT_MAX;
    initialize_remote_attestation(&g_sp_pub_key, 0, &context);

//    sgx_ra_msg1_t* msg1 = get_msg1(context);
//    initialize_quote();

    return 0;
}
