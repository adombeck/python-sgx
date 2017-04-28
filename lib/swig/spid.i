// ----------------------------------------------------------------------------
// map spid_t to Python bytes
// ----------------------------------------------------------------------------

%typemap(in) sgx_spid_t spid {
    assert(sizeof(sgx_spid_t) == 16);

    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != sizeof(sgx_spid_t)) {
        PyErr_SetString(PyExc_ValueError, "Expected a bytes parameter with 16 elements");
        SWIG_fail;
    }

    memcpy(&$1, PyBytes_AsString($input), sizeof(sgx_spid_t));
}
