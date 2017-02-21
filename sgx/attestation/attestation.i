%module attestation_swig

%{
#include "sgx_tkey_exchange.h"
#include "sgx_uae_service.h"

extern int create_pse_session();
//extern int get_extended_epid_group_id(uint32_t* p_group_id);
%}

extern int create_pse_session();
//extern int get_extended_epid_group_id(uint32_t* p_group_id);