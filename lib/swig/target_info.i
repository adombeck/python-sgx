// ----------------------------------------------------------------------------
// map sgx_target_info_t to Python bytes
// ----------------------------------------------------------------------------

%typemap(in) sgx_target_info_t target_info {
    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != sizeof(sgx_target_info_t)) {
        PyErr_SetString(PyExc_ValueError, "Expected a bytes parameter with sizeof(sgx_target_info_t) elements");
        SWIG_fail;
    }

    sgx_target_info_t res;
    char* bytes = PyBytes_AsString($input);
    uint32_t offset = 0;

    memcpy(&(res.mr_enclave), &(bytes[offset]), sizeof(res.mr_enclave));
    offset += sizeof(res.mr_enclave);

    memcpy(&(res.attributes), &(bytes[offset]), sizeof(res.attributes));
    offset += sizeof(res.attributes);

    memcpy(&(res.reserved1), &(bytes[offset]), sizeof(res.reserved1));
    offset += sizeof(res.reserved1);

    memcpy(&(res.misc_select), &(bytes[offset]), sizeof(res.misc_select));
    offset += sizeof(res.misc_select);

    memcpy(&(res.reserved2), &(bytes[offset]), sizeof(res.reserved2));
    offset += sizeof(res.reserved2);

    $1 = res;
}

%typemap(out) sgx_target_info_t target_info {
    uint32_t size = sizeof(sgx_target_info_t);
    uint32_t offset = 0;

    uint8_t tmp[size];

    memcpy(&tmp[offset], &($1.mr_enclave), sizeof($1.mr_enclave));
    offset += sizeof($1.mr_enclave);

    memcpy(&tmp[offset], &($1.attributes), sizeof($1.attributes));
    offset += sizeof($1.attributes);

    memcpy(&tmp[offset], &($1.reserved1), sizeof($1.reserved1));
    offset += sizeof($1.reserved1);

    memcpy(&tmp[offset], &($1.misc_select), sizeof($1.misc_select));
    offset += sizeof($1.misc_select);

    memcpy(&tmp[offset], &($1.reserved2), sizeof($1.reserved2));
    offset += sizeof($1.reserved2);

    $result = PyBytes_FromStringAndSize((char*) tmp, size);
}


// Define typemaps for pp_qe_target_info

// This typemap suppresses requiring the parameter as an input.
%typemap(in,numinputs=0) sgx_target_info_t** pp_target_info (sgx_target_info_t* temp) {
  $1 = &temp;
}

%typemap(argout) sgx_target_info_t** pp_target_info {
    uint32_t size = sizeof(sgx_target_info_t);
    uint32_t offset = 0;

    uint8_t tmp[size];

    memcpy(&tmp[offset], &((*$1)->mr_enclave), sizeof((*$1)->mr_enclave));
    offset += sizeof((*$1)->mr_enclave);

    memcpy(&tmp[offset], &((*$1)->attributes), sizeof((*$1)->attributes));
    offset += sizeof((*$1)->attributes);

    memcpy(&tmp[offset], &((*$1)->reserved1), sizeof((*$1)->reserved1));
    offset += sizeof((*$1)->reserved1);

    memcpy(&tmp[offset], &((*$1)->misc_select), sizeof((*$1)->misc_select));
    offset += sizeof((*$1)->misc_select);

    memcpy(&tmp[offset], &((*$1)->reserved2), sizeof((*$1)->reserved2));
    offset += sizeof((*$1)->reserved2);

    $result = PyBytes_FromStringAndSize((char*) tmp, size);
}
