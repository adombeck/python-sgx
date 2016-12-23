%module sealing

%{
#define SWIG_PYTHON_STRICT_BYTE_CHAR
#include "sgx_tseal.h"
#include "sgx_key.h"
#include "sgx_error.h"
#include "sgx_defs.h"
#include "sgx_attributes.h"
#include "sgx_tcrypto.h"
#include "sgx.h"
#include "sgx_defs.h"
#include "sgx_report.h"

//extern void seal(char* secret, uint32_t secret_len, char* plain_text, uint32_t plain_text_len, char** p_sealed_buf, uint32_t* p_sealed_len);
extern void seal(char* secret, uint32_t secret_len, char* plain_text, uint32_t plain_text_len, char** p_sealed_buf, uint32_t* p_sealed_len);
extern void unseal(char** p_secret, uint32_t* p_secret_len, char** p_plain_text, uint32_t* p_plain_text_len, char* sealed_buf);
%}

%include "cdata.i"
%include "stdint.i"
%include "typemaps.i"
%include "cstring.i"

%include "sgx_defs.h"
%include "sgx_tseal.h"

%cstring_output_allocate_size(char** p_sealed_buf, uint32_t* p_sealed_len, free(*$1));
extern void seal(char* secret, uint32_t secret_len, char* plain_text, uint32_t plain_text_len, char** p_sealed_buf, uint32_t* p_sealed_len);

%cstring_output_allocate_size(char** p_secret, uint32_t* p_secret_len, free(*$1));
%cstring_output_allocate_size(char** p_plain_text, uint32_t* p_plain_text_len, free(*$1));
extern void unseal(char** p_secret, uint32_t* p_secret_len, char** p_plain_text, uint32_t* p_plain_text_len, char* sealed_buf);
