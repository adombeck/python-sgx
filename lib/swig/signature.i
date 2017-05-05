// ----------------------------------------------------------------------------
// map sgx_ec256_signature_t to Python bytes
// ----------------------------------------------------------------------------

%typemap(in) sgx_ec256_signature_t signature {
    assert(sizeof(sgx_ec256_signature_t) == 64);

    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != sizeof(sgx_ec256_signature_t)) {
        PyErr_SetString(PyExc_ValueError, "Signature must be 64 bytes");
        SWIG_fail;
    }

    uint8_t* bytes = (uint8_t*) PyBytes_AsString($input);

    memcpy(&$1, bytes, sizeof(sgx_ec256_signature_t));

    // Reverse byte order to little-endian
    reverse_byte_array((uint8_t*) &$1, 32);
    reverse_byte_array((uint8_t*) &$1 + 32, 32);
}
