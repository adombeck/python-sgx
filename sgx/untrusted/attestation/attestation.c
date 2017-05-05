#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Python.h>

#include "sgx_uae_service.h"


// Took this from the Intel SGX SDK's RemoteAttestation app
void PRINT_BYTE_ARRAY(void *mem, uint32_t len)
{
    if(!mem || !len)
    {
        fprintf(stderr, "\n( null )\n");
        return;
    }
    uint8_t *array = (uint8_t *)mem;
    uint32_t i = 0;
    for(i = 0; i < len; i++)
    {
        fprintf(stderr, "%02x", array[i]);
    }
    fprintf(stderr, "\n");
}


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


void get_quote_size(uint8_t* revocation_list, uint32_t revocation_list_size, uint32_t* p_quote_size)
{
    sgx_status_t ret =  sgx_get_quote_size(revocation_list_size ? revocation_list : NULL, p_quote_size);
    if(ret != SGX_SUCCESS)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Failed to call sgx_get_quote_size. Error code: 0x%x\n", ret);
        return;
    }

    fprintf(stderr, "Successfully got quote size %u\n", *p_quote_size);
}


void get_quote(sgx_report_t report,
               uint16_t quote_type,
               sgx_spid_t spid,
               sgx_quote_nonce_t nonce,
               uint8_t* revocation_list,
               uint32_t revocation_list_size,
               char* p_quote,
               uint32_t* p_quote_size,
               sgx_report_t* p_qe_report)
{
    fprintf(stderr, "p_quote_size in get_quote(): %u\n", *p_quote_size);

    memset(p_quote, 0, *p_quote_size);

    sgx_status_t ret = sgx_get_quote(&report,
                                     quote_type,
                                     &spid,
                                     &nonce,
                                     revocation_list_size ? revocation_list : NULL,
                                     revocation_list_size,
                                     p_qe_report,
                                     (sgx_quote_t *) p_quote,
                                     *p_quote_size);

    if(ret != SGX_SUCCESS)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Failed to call sgx_get_quote. Error code: 0x%x\n", ret);
        return;
    }

    fprintf(stderr, "Successfully got quote\n");

    fprintf(stderr, "quote:\n");
    PRINT_BYTE_ARRAY(p_quote, *p_quote_size);

    fprintf(stderr, "quote size: %u\n", *p_quote_size);

    fprintf(stderr, "qe_report:\n");
    PRINT_BYTE_ARRAY(p_qe_report, sizeof(sgx_report_t));

    fprintf(stderr, "qe_report.mac:\n");
    PRINT_BYTE_ARRAY(&p_qe_report->mac, sizeof(sgx_mac_t));
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