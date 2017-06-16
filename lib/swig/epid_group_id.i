// ----------------------------------------------------------------------------
// map sgx_epid_group_id_t to Python bytes
// ----------------------------------------------------------------------------

%typemap(in) sgx_epid_group_id_t epid_group_id {
    assert(sizeof(sgx_epid_group_id_t) == 4);

    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != sizeof(sgx_epid_group_id_t)) {
        PyErr_SetString(PyExc_ValueError, "Expected a bytes parameter with 4 elements");
        SWIG_fail;
    }

    memcpy(&$1, PyBytes_AsString($input), sizeof(sgx_epid_group_id_t));

    // Reverse byte order to little-endian
    reverse_byte_array((uint8_t*) &$1, 4);
}

// Define typemaps for p_epid_group_id

// This typemap suppresses requiring the parameter as an input.
%typemap(in,numinputs=0) sgx_epid_group_id_t* p_epid_group_id (sgx_epid_group_id_t temp) {
    memset(&temp, 0, sizeof(sgx_epid_group_id_t));
    $1 = &temp;
}

%typemap(argout) sgx_epid_group_id_t* p_epid_group_id {
    uint8_t tmp[4];
    memcpy(tmp, $1, 4);

    // Reverse byte order to big-endian
    reverse_byte_array(tmp, 4);

    PyObject* pybytes = PyBytes_FromStringAndSize((char*) tmp, sizeof(sgx_epid_group_id_t));
    $result = SWIG_Python_AppendOutput($result, pybytes);
}
