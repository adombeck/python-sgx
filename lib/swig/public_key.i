// ----------------------------------------------------------------------------
// map sgx_ec256_public_t to Python bytes
// ----------------------------------------------------------------------------

// Define typemaps for sgx_ec256_public_t

%typemap(in) sgx_ec256_public_t public_key {
    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != 64) {
        PyErr_SetString(PyExc_ValueError, "Public key must be 64 bytes");
        SWIG_fail;
    }

    sgx_ec256_public_t res;
    char* bytes = PyBytes_AsString($input);

    memcpy(res.gx, &bytes[0], 32);
    memcpy(res.gy, &bytes[32], 32);

    // Reverse byte order to little-endian
    reverse_byte_array(res.gx, 32);
    reverse_byte_array(res.gy, 32);

    $1 = res;
}

%typemap(out) sgx_ec256_public_t public_key {
    uint8_t tmp[64];
    memcpy(&tmp[0], $1.gx, 32);
    memcpy(&tmp[32], $1.gy, 32);

    // Reverse byte order to big-endian
    reverse_byte_array(&tmp[0], 32);
    reverse_byte_array(&tmp[32], 32);

    $result = PyBytes_FromStringAndSize((char*) tmp, 64);
}


// Define typemaps for pp_public_key

// This typemap suppresses requiring the parameter as an input.
%typemap(in,numinputs=0) sgx_ec256_public_t** pp_public_key (sgx_ec256_public_t* temp) {
  $1 = &temp;
}

%typemap(argout) sgx_ec256_public_t** pp_public_key {
    uint8_t tmp[64];
    memcpy(&tmp[0], (*$1)->gx, 32);
    memcpy(&tmp[32], (*$1)->gy, 32);

    // Reverse byte order to big-endian
    reverse_byte_array(&tmp[0], 32);
    reverse_byte_array(&tmp[32], 32);

    $result = PyBytes_FromStringAndSize((char*) tmp, 64);
}
