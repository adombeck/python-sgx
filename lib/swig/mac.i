// ----------------------------------------------------------------------------
// map sgx_ec256_signature_t to Python bytes
// ----------------------------------------------------------------------------

%typemap(in) sgx_mac_t mac {
    assert(sizeof(sgx_mac_t) == 16);

    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != sizeof(sgx_mac_t)) {
        PyErr_SetString(PyExc_ValueError, "MAC must be 16 bytes");
        SWIG_fail;
    }

    uint8_t* bytes = (uint8_t*) PyBytes_AsString($input);

    memcpy($1, bytes, 16);

    // Reverse byte order to little-endian
    reverse_byte_array($1, 16);
}
