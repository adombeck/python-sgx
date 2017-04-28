#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Python.h>

#include "sgx_uae_service.h"


void print_target_info(sgx_target_info_t target_info)
{
    int i;
    int offset = 0;
    unsigned char* charPtr = (unsigned char*) &target_info;
    fprintf(stderr, "sizeof(target_info): %lx\n", sizeof(target_info));
//    fprintf(stderr, "target_info: ");
//    for(i = 0; i < sizeof(target_info); i++)
//        fprintf(stderr, "%02hhx", charPtr[i]);
//    fprintf(stderr, "\n");

    fprintf(stderr, "target_info.mr_enclave:");
    for(i = 0; i < sizeof(target_info.mr_enclave); i++)
        fprintf(stderr, "%02hhx", charPtr[offset + i]);
    fprintf(stderr, "\n");
    offset += i;

    fprintf(stderr, "target_info.attributes:");
    for(i = 0; i < sizeof(target_info.attributes); i++)
        fprintf(stderr, "%02hhx", charPtr[offset + i]);
    fprintf(stderr, "\n");
    offset += i;

    fprintf(stderr, "target_info.reserved1:");
    for(i = 0; i < sizeof(target_info.reserved1); i++)
        fprintf(stderr, "%02hhx", charPtr[offset + i]);
    fprintf(stderr, "\n");
    offset += i;

    fprintf(stderr, "target_info.misc_select:");
    for(i = 0; i < sizeof(target_info.misc_select); i++)
        fprintf(stderr, "%02hhx", charPtr[offset + i]);
    fprintf(stderr, "\n");
    offset += i;

    fprintf(stderr, "target_info.reserved2:");
    for(i = 0; i < sizeof(target_info.reserved2); i++)
        fprintf(stderr, "%02hhx", charPtr[offset + i]);
    fprintf(stderr, "\n");
    offset += i;

    fprintf(stderr, "\n");
}


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


void initialize_quoting_enclave(sgx_target_info_t* p_qe_target_info)
{
    sgx_epid_group_id_t gid = {0};

    sgx_status_t ret = sgx_init_quote(p_qe_target_info, &gid);
    if(ret != SGX_SUCCESS)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Failed to call sgx_init_quote. Error code: 0x%x\n", ret);
        return;
    }

    fprintf(stderr, "Successfully initialized quoting enclave\n");
    print_target_info(*p_qe_target_info);
}


void get_quote(uint8_t* p_quote)
{

}


int main()
{
    fprintf(stderr, "Getting EPID group id... ");
    uint32_t group_id;
    get_extended_epid_group_id(&group_id);

    sgx_target_info_t qe_target_info;
    initialize_quoting_enclave(&qe_target_info);
    print_target_info(qe_target_info);

    return 0;
}