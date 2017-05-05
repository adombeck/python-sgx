// ----------------------------------------------------------------------------
// map sgx_quote_nonce_t to Python bytes
// ----------------------------------------------------------------------------

%typemap(in) sgx_quote_nonce_t nonce {
    assert(sizeof(sgx_quote_nonce_t) == 16);

    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != sizeof(sgx_quote_nonce_t)) {
        PyErr_SetString(PyExc_ValueError, "Expected a bytes parameter with 16 elements");
        SWIG_fail;
    }

    memcpy(&$1, PyBytes_AsString($input), sizeof(sgx_quote_nonce_t));
}


// Define typemaps for p_nonce

// This typemap suppresses requiring the parameter as an input.
%typemap(in,numinputs=0) sgx_quote_nonce_t* p_nonce (sgx_quote_nonce_t temp) {
    memset(&temp, 0, sizeof(sgx_quote_nonce_t));
    $1 = &temp;
}

%typemap(argout) sgx_quote_nonce_t* p_nonce {
    PyObject* pybytes = PyBytes_FromStringAndSize((char*) $1, sizeof(sgx_quote_nonce_t));
    $result = SWIG_Python_AppendOutput($result, pybytes);
}
