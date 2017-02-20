%module attestation_swig

%{
#include "sgx_tkey_exchange.h"

extern void create_pse_session();
%}

extern void create_pse_session();
