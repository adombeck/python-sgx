%module attestation_swig

%{
#define SWIG_PYTHON_STRICT_BYTE_CHAR
#include "sgx_key.h"
#include "sgx_error.h"
#include "sgx_defs.h"
#include "sgx_attributes.h"
#include "sgx_tcrypto.h"
#include "sgx.h"
#include "sgx_defs.h"
#include "sgx_report.h"

extern void create_pse_session();
%}

%include "cdata.i"
%include "stdint.i"
%include "typemaps.i"
%include "cstring.i"

%include "sgx_defs.h"

extern void create_pse_session();