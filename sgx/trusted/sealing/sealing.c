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


// XXX: Use sgx_seal_data_ex and add seal_to_signer option (default to False, i.e. seal to enclave by default, because
//      a remote attester cannot trust secrets sealed to signer to remain secret)


int seal_data(uint8_t* secret, uint32_t secret_len, uint8_t* plain_text, uint32_t plain_text_len, sgx_sealed_data_t* sealed_buf)
{
    fprintf(stderr, "Sealing data\n");

    uint32_t sealed_len = sizeof(sgx_sealed_data_t) + secret_len + plain_text_len;

    // Seal the secret data
    sgx_status_t ret = sgx_seal_data(plain_text_len, plain_text, secret_len, secret, sealed_len, sealed_buf);
    if(ret != SGX_SUCCESS)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Failed to seal data\n");
        return ret;
    }

    return 0;
}


void seal(char* secret, uint32_t secret_len, char* plain_text, uint32_t plain_text_len, char** p_sealed_buf, uint32_t* p_sealed_len)
{
    uint32_t sealed_len = sizeof(sgx_sealed_data_t) + secret_len + plain_text_len;

    sgx_sealed_data_t* sealed_buf = malloc(sealed_len);
    if(sealed_buf == NULL)
    {
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        fprintf(stderr, "Out of memory\n");
        abort();
    }
    memset(sealed_buf, 0, sealed_len);

    int ret = seal_data((uint8_t*) secret, secret_len, (uint8_t*) plain_text, plain_text_len, sealed_buf);

    if(ret != 0)
        // XXX: Throw Python exception. See http://www.swig.org/Doc1.1/HTML/Exceptions.html
        return;

    *p_sealed_buf = (char*) sealed_buf;
    *p_sealed_len = sealed_len;
}


int unseal_data(uint8_t* secret, uint8_t* plain_text, sgx_sealed_data_t* sealed_buf)
{
    fprintf(stderr, "Unsealing data\n");

    uint32_t unsealed_data_len = sgx_get_encrypt_txt_len(sealed_buf);
    fprintf(stderr, "unsealed_data_len: %u\n", unsealed_data_len);

    uint32_t plain_text_len = sgx_get_add_mac_txt_len(sealed_buf);
    fprintf(stderr, "plain_text_len: %u\n", plain_text_len);

    // Unseal current sealed buf
    sgx_status_t ret = sgx_unseal_data((sgx_sealed_data_t*) sealed_buf, plain_text, &plain_text_len, secret, &unsealed_data_len);
    if(ret != SGX_SUCCESS)
    {
        fprintf(stderr, "Failed to unseal data.\n");
        return ret;
    }

    return 0;
}


void unseal(char** p_secret, uint32_t* p_secret_len, char** p_plain_text, uint32_t* p_plain_text_len, char* sealed_buf)
{
    uint32_t secret_len = sgx_get_encrypt_txt_len((sgx_sealed_data_t*) sealed_buf);
    uint32_t plain_text_len = sgx_get_add_mac_txt_len((sgx_sealed_data_t*) sealed_buf);

    uint8_t* secret = (uint8_t*) calloc(secret_len, sizeof(uint8_t));
    if(secret == NULL)
    {
        // XXX: Return error message
        fprintf(stderr, "Out of memory\n");
        abort();
    }

    uint8_t* plain_text = (uint8_t*) calloc(plain_text_len, sizeof(uint8_t));
    if(plain_text == NULL)
    {
        // XXX: Return error message
        fprintf(stderr, "Out of memory\n");
        free(secret);
        abort();
    }

    int ret = unseal_data(secret, plain_text, (sgx_sealed_data_t*) sealed_buf);
    // XXX: Return error message
    if(ret != 0)
        return;

    *p_secret = (char*) secret;
    *p_secret_len = secret_len;
    *p_plain_text = (char*) plain_text;
    *p_plain_text_len = plain_text_len;
}


void set_global_data()
{
    // Allocate memory to save the sealed data.
    uint32_t sealed_len = sizeof(sgx_sealed_data_t) + SECRET_LEN + PLAIN_TEXT_LEN;

    g_sealed_buf = (sgx_sealed_data_t*) calloc(1, sealed_len);
    if(g_sealed_buf == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        abort();
    }

    g_secret = (uint8_t*) calloc(SECRET_LEN, sizeof(uint8_t));
    if(g_secret == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        free(g_sealed_buf);
        abort();
    }

    g_plain_text = (uint8_t*) calloc(PLAIN_TEXT_LEN, sizeof(uint8_t));
    if(g_plain_text == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        free(g_sealed_buf);
        free(g_secret);
        abort();
    }

    g_unsealed_secret = (uint8_t*) calloc(SECRET_LEN, sizeof(uint8_t));
    if(g_unsealed_secret == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        free(g_sealed_buf);
        free(g_secret);
        free(g_plain_text);
        abort();
    }

    g_unsealed_plain_text = (uint8_t*) calloc(PLAIN_TEXT_LEN, sizeof(uint8_t));
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


char *charstring(char *s){
    return s;
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

    ret = seal_data(g_secret, SECRET_LEN, g_plain_text, PLAIN_TEXT_LEN, g_sealed_buf);
    if(ret != 0)
    {
        fprintf(stderr, "Error: seal_data() returned %i\n", ret);
        release_global_data();
        abort();
    }

    ret = unseal_data(g_unsealed_secret, g_unsealed_plain_text, g_sealed_buf);
    if(ret != 0)
    {
        fprintf(stderr, "Error: unseal_data() returned %i\n", ret);
        release_global_data();
        abort();
    }

    fprintf(stderr, "Got unsealed secret: %s\n", g_unsealed_secret);
    fprintf(stderr, "Got unsealed plaintext: %s\n", g_unsealed_plain_text);

    // Release resources
    release_global_data();

    return 0;
}
