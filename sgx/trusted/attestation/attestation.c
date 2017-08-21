#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <Python.h>

#include "sgx_tkey_exchange.h"
#include "sgx_uae_service.h"
#include "sgx_utils.h"

sgx_status_t sgx_ra_get_ga(sgx_ra_context_t context, sgx_ec256_public_t* g_a);
sgx_status_t sgx_ra_proc_msg2_trusted(sgx_ra_context_t context, const sgx_ra_msg2_t* p_msg2, const sgx_target_info_t* p_qe_target, sgx_report_t* p_report, sgx_quote_nonce_t* p_nonce);
sgx_status_t sgx_ra_get_msg3_trusted(sgx_ra_context_t context, uint32_t quote_size, sgx_report_t* qe_report, sgx_ra_msg3_t* p_msg3, uint32_t msg3_size);

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


void initialize_remote_attestation(sgx_ec256_public_t public_key, int use_pse, sgx_ra_context_t* p_context)
{
    sgx_status_t ret = sgx_ra_init(&public_key, use_pse, p_context);
    if(ret != SGX_SUCCESS)
    {
        snprintf(error_message, 256, "Failed to call sgx_ra_init. Error code: 0x%x\n", ret);
        error_status = 1;
    }
}


void close_remote_attestation(sgx_ra_context_t context)
{
    sgx_status_t ret = sgx_ra_close(context);
    if(ret != SGX_SUCCESS)
    {
        snprintf(error_message, 256, "Failed to call sgx_ra_close. Error code: 0x%x\n", ret);
        error_status = 1;
    }
}


void get_new_public_key(sgx_ra_context_t context, sgx_ec256_public_t* p_enclave_public_key)
{
    sgx_status_t ret = sgx_ra_get_ga(context, p_enclave_public_key);
    if(ret != SGX_SUCCESS)
    {
        snprintf(error_message, 256, "Failed to call sgx_ra_get_ga. Error code: 0x%x\n", ret);
        error_status = 1;
    }
}


void process_msg2(sgx_ra_context_t context,
                  sgx_target_info_t qe_target_info,
                  sgx_ec256_public_t public_key,
                  sgx_spid_t spid,
                  uint16_t quote_type,
                  uint16_t kdf_id,
                  sgx_ec256_signature_t key_signature,
                  sgx_mac_t mac,
                  uint8_t* revocation_list,
                  uint32_t revocation_list_size,
                  sgx_report_t* p_report,
                  sgx_quote_nonce_t* p_nonce
                  )
{
    sgx_ra_msg2_t msg2;
    msg2.g_b = public_key;
    msg2.spid = spid;
    msg2.quote_type = quote_type;
    msg2.kdf_id = kdf_id;
    msg2.sign_gb_ga = key_signature;
    memcpy(msg2.mac, mac, sizeof(msg2.mac));
    msg2.sig_rl_size = revocation_list_size;
    memcpy(msg2.sig_rl, revocation_list, msg2.sig_rl_size);

    sgx_status_t ret = sgx_ra_proc_msg2_trusted(context, &msg2, &qe_target_info, p_report, p_nonce);
    if(ret != SGX_SUCCESS)
    {
        snprintf(error_message, 256, "Failed to call sgx_ra_proc_msg2_trusted. Error code: 0x%x\n", ret);
        error_status = 1;
    }
}


void get_msg3(sgx_ra_context_t context,
              uint8_t* quote,
              uint32_t quote_size,
              sgx_report_t qe_report,
              sgx_mac_t* p_mac,
              char* platform_service_security_properties
              )
{
    uint32_t msg3_size = sizeof(sgx_ra_msg3_t) + quote_size;
    sgx_ra_msg3_t* p_msg3 = malloc(msg3_size);
    if(!p_msg3)
    {
        snprintf(error_message, 256, "Failed to allocate memory for msg3\n");
        error_status = 1;
        return;
    }
    memset(p_msg3, 0, msg3_size);

    memcpy(&p_msg3->quote, quote, quote_size);

    sgx_status_t ret = sgx_ra_get_msg3_trusted(context, quote_size, &qe_report, p_msg3, msg3_size);
    if(ret != SGX_SUCCESS)
    {
        snprintf(error_message, 256, "Failed to call sgx_ra_get_msg3_trusted. Error code: 0x%x\n", ret);
        error_status = 1;
        goto cleanup;
    }

    memcpy(p_mac, &p_msg3->mac, sizeof(sgx_mac_t));
    memcpy(platform_service_security_properties, &p_msg3->ps_sec_prop, sizeof(sgx_ps_sec_prop_desc_t));

cleanup:
    // Clear msg3 and free it
    memset(p_msg3, 0, msg3_size);
    free(p_msg3);
}
