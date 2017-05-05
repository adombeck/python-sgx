// ----------------------------------------------------------------------------
// map sgx_mac_t to Python bytes
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

    memcpy($1, PyBytes_AsString($input), 16);
}


// Define typemaps for p_mac

// This typemap suppresses requiring the parameter as an input.
%typemap(in,numinputs=0) sgx_mac_t* p_mac (sgx_mac_t temp) {
    memset(&temp, 0, sizeof(sgx_mac_t));
    $1 = &temp;
}

%typemap(argout) sgx_mac_t* p_mac {
    PyObject* pybytes = PyBytes_FromStringAndSize((char*) $1, sizeof(sgx_mac_t));
    $result = SWIG_Python_AppendOutput($result, pybytes);
}
