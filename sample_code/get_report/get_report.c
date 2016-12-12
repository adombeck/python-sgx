#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sgx_report.h"

#include "Enclave.h"


char* bin_to_strhex(unsigned char *bin, unsigned int binsz)
{
  char          hex_str[]= "0123456789abcdef";
  unsigned int  i;
  
  char* result = (char *)malloc(binsz * 2 + 1);
  (result)[binsz * 2] = 0;
  
  if (!binsz)
    return NULL;
  
  for (i = 0; i < binsz; i++)
    {
      (result)[i * 2 + 0] = hex_str[(bin[i] >> 4) & 0x0F];
      (result)[i * 2 + 1] = hex_str[(bin[i]     ) & 0x0F];
    }
  return result;
}

void print_report(sgx_report_t report)
{
  fprintf(stderr, "report.key_id: %i\n", report.key_id);
  fprintf(stderr, "report.mac: %.32s\n", bin_to_strhex(report.mac, 16));
  fprintf(stderr, "report.body.mr_enclave.m: %.32s\n", bin_to_strhex(report.body.mr_enclave.m, 32));
  fprintf(stderr, "report.body.mr_signer.m: %.32s\n", bin_to_strhex(report.body.mr_signer.m, 32));
}

int main()
{
  sgx_report_t report;
  create_report(&report);
  print_report(report);
//  fprintf(stderr, "Returning\n");
}

