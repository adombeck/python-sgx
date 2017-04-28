// ----------------------------------------------------------------------------
// map sgx_ec256_public_t to Python bytes
// ----------------------------------------------------------------------------

// Define typemaps for sgx_ec256_public_t

%typemap(in) sgx_ec256_public_t public_key {
    assert(sizeof(sgx_ec256_public_t) == 64);

    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != sizeof(sgx_ec256_public_t)) {
        PyErr_SetString(PyExc_ValueError, "Expected a bytes parameter with 64 elements");
        SWIG_fail;
    }

    uint8_t* bytes = (uint8_t*) PyBytes_AsString($input);

    // Reverse byte order to little-endian
    reverse_byte_array(&bytes[0], 32);
    reverse_byte_array(&bytes[32], 32);

    memcpy(&$1, bytes, sizeof(sgx_ec256_public_t));
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


// Define typemaps for p_public_key

// This typemap suppresses requiring the parameter as an input.
%typemap(in,numinputs=0) sgx_ec256_public_t* p_public_key (sgx_ec256_public_t temp) {
    memset(&temp, 0, sizeof(sgx_ec256_public_t));
    $1 = &temp;
}

%typemap(argout) sgx_ec256_public_t* p_public_key {
    uint8_t tmp[64];
    memcpy(&tmp[0], $1->gx, 32);
    memcpy(&tmp[32], $1->gy, 32);

    // Reverse byte order to big-endian
    reverse_byte_array(&tmp[0], 32);
    reverse_byte_array(&tmp[32], 32);

    $result = PyBytes_FromStringAndSize((char*) tmp, 64);
}
