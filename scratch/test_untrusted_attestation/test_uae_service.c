#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <Python.h>

#include "sgx_ukey_exchange.h"

void print_buf(char* buf, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        if (i > 0) printf(":");
        printf("%02X", (unsigned char) buf[i]);
    }
    printf("\n");
}

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

    printf("Successfully initialized quote\n");

    printf("EPID group ID: ");
    print_buf((char*) gid, 4);

    printf("QE target info:\n");
    print_buf((char*) &qe_target_info, sizeof(sgx_target_info_t));

}


int main()
{
    initialize_quote();

    return 0;
}
