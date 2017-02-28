#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sgx_tseal.h"

#include <Python.h>

#define SECRET_LEN 256
#define PLAIN_TEXT_LEN 256

// Global data
sgx_sealed_data_t* g_sealed_buf;
uint8_t* g_secret;
uint8_t* g_plain_text;
uint8_t* g_unsealed_secret;
uint8_t* g_unsealed_plain_text;


#include "sgx_uae_service.h"

void get_extended_epid_group_id(uint32_t* p_group_id)
{
    sgx_status_t ret = sgx_get_extended_epid_group_id(p_group_id);
    if(ret != SGX_SUCCESS)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Failed to get EPID group id. Error code: 0x%x\n", ret);
        return;
    }
    fprintf(stderr, "Successfully got EPID group id %u\n", *p_group_id);
}

int main()
{
    fprintf(stderr, "Getting EPID group id... ");
    uint32_t group_id;
    get_extended_epid_group_id(&group_id);

    return 0;
}