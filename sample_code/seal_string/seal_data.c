#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sgx_tseal.h"

#define SECRET_LEN 256
#define PLAIN_TEXT_LEN 256

// Global data
sgx_sealed_data_t* g_sealed_buf;
uint8_t* g_secret;
uint8_t* g_plain_text;
uint8_t* g_unsealed_secret;
uint8_t* g_unsealed_plain_text;


int seal_data_in_enclave(uint8_t* secret, uint8_t* plain_text, sgx_sealed_data_t* sealed_buf)
{
    fprintf(stderr, "Sealing data\n");

    uint32_t secret_len = SECRET_LEN;
    uint32_t plain_text_len = PLAIN_TEXT_LEN;
    uint32_t sealed_len = sizeof(sgx_sealed_data_t) + secret_len + plain_text_len;

    // Seal the secret data
    sgx_status_t ret = sgx_seal_data(plain_text_len, plain_text, secret_len, secret, sealed_len, sealed_buf);
    if(ret != SGX_SUCCESS)
    {
        fprintf(stderr, "Failed to seal data\n");
        return -1;
    }

    return 0;
}


int unseal_data_in_enclave(uint8_t* secret, uint8_t* plain_text, sgx_sealed_data_t* sealed_buf)
{
    fprintf(stderr, "Unsealing data\n");

    uint32_t unsealed_data_len = sgx_get_encrypt_txt_len(sealed_buf);
    fprintf(stderr, "unsealed_data_len: %u\n", unsealed_data_len);

    uint32_t plain_text_len = sgx_get_add_mac_txt_len(sealed_buf);
    fprintf(stderr, "plain_text_len: %u\n", plain_text_len);

    // Unseal current sealed buf
    sgx_status_t ret = sgx_unseal_data(sealed_buf, plain_text, &plain_text_len, secret, &unsealed_data_len);
    if(ret != SGX_SUCCESS)
    {
        fprintf(stderr, "Failed to unseal data.\n");
        return -1;
    }

    return 0;
}


void set_global_data()
{
    // Allocate memory to save the sealed data.
    uint32_t sealed_len = sizeof(sgx_sealed_data_t) + SECRET_LEN + PLAIN_TEXT_LEN;

    g_sealed_buf = (sgx_sealed_data_t*) malloc(sealed_len);
    if(g_sealed_buf == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        abort();
    }
    memset(g_sealed_buf, 0, sealed_len);

    g_secret = (uint8_t*) malloc(SECRET_LEN);
    if(g_secret == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        free(g_sealed_buf);
        abort();
    }

    g_plain_text = (uint8_t*) malloc(PLAIN_TEXT_LEN);
    if(g_plain_text == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        free(g_sealed_buf);
        free(g_secret);
        abort();
    }

    g_unsealed_secret = (uint8_t*) malloc(SECRET_LEN);
    if(g_unsealed_secret == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        free(g_sealed_buf);
        free(g_secret);
        free(g_plain_text);
        abort();
    }

    g_unsealed_plain_text = (uint8_t*) malloc(PLAIN_TEXT_LEN);
    if(g_unsealed_plain_text == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        free(g_sealed_buf);
        free(g_secret);
        free(g_plain_text);
        free(g_unsealed_secret);
        abort();
    }

    return;
}

void release_global_data()
{
    fprintf(stderr, "Freeing g_sealed_buf\n");
    free(g_sealed_buf);
    g_sealed_buf = NULL;

    fprintf(stderr, "Freeing g_secret\n");
    free(g_secret);
    g_secret = NULL;

    fprintf(stderr, "Freeing g_plain_text\n");
    free(g_plain_text);
    g_plain_text = NULL;

    fprintf(stderr, "Freeing g_unsealed_secret\n");
    free(g_unsealed_secret);
    g_unsealed_secret = NULL;

    fprintf(stderr, "Freeing g_unsealed_plain_text\n");
    free(g_unsealed_plain_text);
    g_unsealed_plain_text = NULL;

    return;
}


int main()
{
    int ret;

    set_global_data();

    strcpy((char*) g_secret, "foobar");
    strcpy((char*) g_plain_text, "baobab");

    ret = seal_data_in_enclave(g_secret, g_plain_text, g_sealed_buf);
    if(ret != 0)
    {
        fprintf(stderr, "Error: seal_data_in_enclave() returned %u\n", ret);
        release_global_data();
        abort();
    }

    ret = unseal_data_in_enclave(g_unsealed_secret, g_unsealed_plain_text, g_sealed_buf);
    if(ret != 0)
    {
        fprintf(stderr, "Error: unseal_data_in_enclave() returned %u\n", ret);
        release_global_data();
        abort();
    }

    fprintf(stderr, "Got unsealed secret: %s\n", g_unsealed_secret);
    fprintf(stderr, "Got unsealed plaintext: %s\n", g_unsealed_plain_text);

    // Release resources
    release_global_data();

    return 0;
}
