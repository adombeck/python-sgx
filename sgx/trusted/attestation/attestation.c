#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <Python.h>

#include "sgx_tkey_exchange.h"
#include "sgx_ukey_exchange.h"
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


// This is only used in main() (which is only there for debugging)
// Took this from the Intel SGX SDK's RemoteAttestation app
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
//    fprintf(stderr, "g_b in process_msg2():\n");
//    PRINT_BYTE_ARRAY(&public_key, sizeof(sgx_ec256_public_t));
//
//    fprintf(stderr, "sign_gb_ga.r in process_msg2():\n");
//    PRINT_BYTE_ARRAY(&key_signature.x, 32);
//
//    fprintf(stderr, "sign_gb_ga.s in process_msg2():\n");
//    PRINT_BYTE_ARRAY(&key_signature.y, 32);
//
//    fprintf(stderr, "MAC in process_msg2():\n");
//    PRINT_BYTE_ARRAY(mac, 16);
//
//    fprintf(stderr, "QE target info in process_msg2():\n");
//    PRINT_BYTE_ARRAY(&qe_target_info, sizeof(sgx_target_info_t));
//
//    fprintf(stderr, "revocation list in process_msg2():\n");
//    PRINT_BYTE_ARRAY(revocation_list, revocation_list_size);
//
//    fprintf(stderr, "revocation list size in process_msg2(): %u\n", revocation_list_size);

    sgx_ra_msg2_t msg2;
    msg2.g_b = public_key;
    msg2.spid = spid;
    msg2.quote_type = quote_type;
    msg2.kdf_id = kdf_id;
    msg2.sign_gb_ga = key_signature;
    memcpy(msg2.mac, mac, sizeof(msg2.mac));
    msg2.sig_rl_size = revocation_list_size;
    memcpy(msg2.sig_rl, revocation_list, msg2.sig_rl_size);

//    fprintf(stderr, "Authenticated bytes:\n");
//    PRINT_BYTE_ARRAY((uint8_t*) &msg2.g_b, 148);

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
//    fprintf(stderr, "quote in get_msg3():\n");
//    PRINT_BYTE_ARRAY(quote, quote_size);
//
//    fprintf(stderr, "quote size get_msg3(): %u\n", quote_size);
//
//    fprintf(stderr, "qe_report in get_msg3():\n");
//    PRINT_BYTE_ARRAY(&qe_report, sizeof(sgx_report_t));
//
//    fprintf(stderr, "qe_report.mac in get_msg3():\n");
//    PRINT_BYTE_ARRAY(&qe_report.mac, sizeof(sgx_mac_t));

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


int main()
{
    fprintf(stderr, "&g_sp_pub_key: %p\n", &g_sp_pub_key);

    sgx_ra_context_t context = INT_MAX;
    initialize_remote_attestation(g_sp_pub_key, 0, &context);

    sgx_ec256_public_t enclave_public_key;
    get_new_public_key(context, &enclave_public_key);
    fprintf(stderr, "Enclave public key:\n");
    PRINT_BYTE_ARRAY(&enclave_public_key, sizeof(enclave_public_key));

    return 0;
}
