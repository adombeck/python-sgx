#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Python.h>

#include "sgx_uae_service.h"


static char error_message[256];
static int error_status = 0;


void clear_exception() {
    error_status = 0;
}


char* check_exception() {
    if (error_status)
        return error_message;
    else
        return NULL;
}


void get_extended_epid_group_id(uint32_t* p_group_id)
{
    sgx_status_t ret = sgx_get_extended_epid_group_id(p_group_id);
    if(ret != SGX_SUCCESS)
    {
        snprintf(error_message, 256, "Failed to get EPID group id. Error code: 0x%x\n", ret);
        error_status = 1;
    }
}


void initialize_quoting_enclave(sgx_target_info_t* p_qe_target_info, sgx_epid_group_id_t* p_gid)
{
    sgx_status_t ret = sgx_init_quote(p_qe_target_info, p_gid);
    if(ret != SGX_SUCCESS)
    {
        snprintf(error_message, 256, "Failed to call sgx_init_quote. Error code: 0x%x\n", ret);
        error_status = 1;
    }
}


void get_quote_size(uint8_t* revocation_list, uint32_t revocation_list_size, uint32_t* p_quote_size)
{
    sgx_status_t ret =  sgx_get_quote_size(revocation_list_size ? revocation_list : NULL, p_quote_size);
    if(ret != SGX_SUCCESS)
    {
        snprintf(error_message, 256, "Failed to call sgx_get_quote_size. Error code: 0x%x\n", ret);
        error_status = 1;
    }
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
        snprintf(error_message, 256, "Failed to call sgx_get_quote. Error code: 0x%x\n", ret);
        error_status = 1;
    }
}

