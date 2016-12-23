#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h> 
#include <unistd.h>
#include <link.h>

#include "sgx_report.h"
#include "sgx_error.h"
#include "util.h"
#include "se_memcpy.h"
#include "trts_inst.h"

struct unknown_struct {
   void*  pointers[3];
   struct unknown_struct* ptr;
};


void assert_exists(char* path)
{
  if (access(path, F_OK) == -1) {
		fprintf(stderr, "File %s doesn't exist\n", path);
    exit(-1);
	}
}

int listdir(char* path)
{
  DIR           *d;
  struct dirent *dir;
  d = opendir(path);
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      fprintf(stderr, "%s\n", dir->d_name);
    }

    closedir(d);
  }

  return(0);
}

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

sgx_status_t my_sgx_create_report(sgx_report_t *report)
{
  // Create a report with REPORT_DATA = TARGET_INFO = NULL
  // Took this from linux-sgx/sdk/selib/sgx_create_report.cpp

  // allocate memory
  // 
  // To minimize the effort of memory management, the three elements allocation 
  // are combined in a single malloc. The calculation for the required size has
  // an assumption, that
  // the elements should be allocated in descending order of the alignment size. 
  //
  // If the alignment requirements are changed, the allocation order needs to
  // change accordingly.
  //
  // Current allocation order is:
  //     report -> target_info -> report_data
  //
  // target_info: 512-byte aligned, 512-byte length
  // report_data: 128-byte aligned,  64-byte length
  // report:      512-byte aligned, 432-byte length
  //
  size_t size = ROUND_TO(sizeof(sgx_target_info_t), TARGET_INFO_ALIGN_SIZE) +
                ROUND_TO(sizeof(sgx_report_data_t), REPORT_DATA_ALIGN_SIZE) +
                ROUND_TO(sizeof(sgx_report_t), REPORT_ALIGN_SIZE);
  size += MAX(MAX(TARGET_INFO_ALIGN_SIZE, REPORT_DATA_ALIGN_SIZE), REPORT_ALIGN_SIZE) - 1;

  void *buffer = malloc(size);
  if(buffer == NULL)
  {
      return SGX_ERROR_OUT_OF_MEMORY;
  }
  memset(buffer, 0, size);
  size_t buf_ptr = (size_t) buffer;

  buf_ptr = ROUND_TO(buf_ptr, REPORT_ALIGN_SIZE);
  sgx_report_t *tmp_report = (sgx_report_t*) buf_ptr;
  buf_ptr += sizeof(*tmp_report);

  buf_ptr = ROUND_TO(buf_ptr, TARGET_INFO_ALIGN_SIZE);
  sgx_target_info_t *tmp_target_info = (sgx_target_info_t*) buf_ptr;
  buf_ptr += sizeof(*tmp_target_info);

  buf_ptr = ROUND_TO(buf_ptr, REPORT_DATA_ALIGN_SIZE);
  sgx_report_data_t *tmp_report_data = (sgx_report_data_t*) buf_ptr;

  // Do EREPORT
  fprintf(stderr, "Calling do_ereport... ");
  do_ereport(tmp_target_info, tmp_report_data, tmp_report);
  fprintf(stderr, "do_ereport called\n");

  // Copy data to the user buffer
  memcpy_s(report, sizeof(*report), tmp_report, sizeof(*tmp_report));

  // cleanup
  // memset_s(buffer, size, 0, size);
  free(buffer);

  return SGX_SUCCESS;
}

int main()
{
//  void* handle; 
//  handle = dlopen(NULL, RTLD_NOW);
//	struct unknown_struct* p = (struct unknown_struct*) handle;
//  p = (*p).ptr;
//	struct link_map* map = (struct link_map*)((*p).ptr);
// 	
//  fprintf(stderr, "Libraries loaded:\n");
//  handle = dlopen(NULL, RTLD_NOW); 
//	while (map) {
//		fprintf(stderr, "%s\n", (*map).l_name);
//    map = (*map).l_next;
//  }	

  sgx_report_t report;
  sgx_status_t sgx_ret;

  sgx_ret = my_sgx_create_report(&report);
  if(SGX_SUCCESS != sgx_ret)
    {
        fprintf(stderr, "sgx_create_report failed");
    }

  print_report(report);
}

