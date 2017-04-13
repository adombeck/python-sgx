#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <Python.h>

#include "sgx_tkey_exchange.h"
#include "sgx_tkey_exchange_t.h"
#include "sgx_ukey_exchange.h"
#include "sgx_uae_service.h"
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


void print_public_key(sgx_ec256_public_t public_key)
{
    int i;

    fprintf(stderr, "&(public_key.gx): %p\n", &(public_key.gx));
    fprintf(stderr, "public_key.gx: ");
    for(i = 0; i < 32; i++)
        fprintf(stderr, "%02hhx", public_key.gx[i]);
    fprintf(stderr, "\n");

    fprintf(stderr, "public_key.gy: ");
    for(i = 0; i < 32; i++)
        fprintf(stderr, "%02hhx", public_key.gy[i]);
    fprintf(stderr, "\n");
}


void initialize_remote_attestation(sgx_ec256_public_t public_key, int use_pse, sgx_ra_context_t* p_context)
{
    // sgx_ec256_public_t* p_public_key = &g_sp_pub_key;
    sgx_status_t ret = sgx_ra_init(&public_key, use_pse, p_context);
    if(ret != SGX_SUCCESS)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Failed to call sgx_ra_init. Error code: 0x%x\n", ret);
        return;
    }
    fprintf(stderr, "Successfully initialized remote attestation\n");
}


void get_new_public_key(sgx_ra_context_t context, sgx_ec256_public_t** pp_enclave_public_key)
{
    sgx_ec256_public_t* tmp1 = malloc(sizeof(sgx_ec256_public_t));
    fprintf(stderr, "tmp1 before sgx_ra_get_ga: %p\n", tmp1);
    sgx_ec256_public_t* tmp2 = malloc(sizeof(sgx_ec256_public_t));
    // XXX: Test if tmp2 is Null

    sgx_status_t ret = sgx_ra_get_ga(context, tmp1);
    if(ret != SGX_SUCCESS)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Failed to call sgx_ra_get_ga. Error code: 0x%x\n", ret);
        return;
    }

    fprintf(stderr, "tmp1 after sgx_ra_get_ga: %p\n", tmp1);

    memcpy(tmp2->gx, tmp1->gx, sizeof(tmp2->gx));
    memcpy(tmp2->gy, tmp1->gy, sizeof(tmp2->gy));
    *pp_enclave_public_key = tmp2;
    free(tmp1);

    fprintf(stderr, "Successfully generated session key pair\n");
    print_public_key(**pp_enclave_public_key);
}

int main()
{
    fprintf(stderr, "&g_sp_pub_key: %p\n", &g_sp_pub_key);
    fprintf(stderr, "&g_global_data: %p\n", &g_global_data);
    fprintf(stderr, "g_global_data.enclave_size: %lx\n", g_global_data.enclave_size);
    fprintf(stderr, "g_global_data.heap_size: %lx\n", g_global_data.heap_size);
    //fprintf(stderr, "enclave_base: %x\n", get_enclave_base());
    fprintf(stderr, "__ImageBase: %p\n", &__ImageBase);

    sgx_ra_context_t context = INT_MAX;
    //initialize_remote_attestation(&g_sp_pub_key, 0, &context);
    initialize_remote_attestation(g_sp_pub_key, 0, &context);

    sgx_ec256_public_t* p_enclave_public_key;
    get_new_public_key(context, &p_enclave_public_key);
    print_public_key(*p_enclave_public_key);

    return 0;
}
