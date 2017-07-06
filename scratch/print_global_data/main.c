#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "pal.h"
//#include "global_data.h"


void breakpoint() {
    __asm__("int3");
}


int main()
{
    fprintf(stderr, "hello\n");
//    fprintf(stderr, "&g_global_data: %p\n", &g_global_data);
//    fprintf(stderr, "g_global_data.enclave_size: %lx\n", g_global_data.enclave_size);
//    fprintf(stderr, "g_global_data.heap_size: %lx\n", g_global_data.heap_size);
    //fprintf(stderr, "enclave_base: %x\n", get_enclave_base());
//    fprintf(stderr, "__ImageBase: %p\n", &__ImageBase);

//    fprintf(stderr, "enclave_base: %p, enclave_size: %lx\n", pal_enclave_base(), pal_enclave_size());

    breakpoint();

    return 0;
}
