// ----------------------------------------------------------------------------
// map sgx_target_info_t to Python bytes
// ----------------------------------------------------------------------------

%typemap(in) sgx_target_info_t target_info {
    assert(sizeof(sgx_target_info_t) == 512);

    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != sizeof(sgx_target_info_t)) {
        PyErr_SetString(PyExc_ValueError, "Expected a bytes parameter with 512 elements");
        SWIG_fail;
    }

    memcpy(&$1, PyBytes_AsString($input), sizeof(sgx_target_info_t));
}


%typemap(out) sgx_target_info_t target_info {
    $result = PyBytes_FromStringAndSize((char*) $1, sizeof(sgx_target_info_t));
}


// Define typemaps for p_target_info

// This typemap suppresses requiring the parameter as an input.
%typemap(in,numinputs=0) sgx_target_info_t* p_target_info (sgx_target_info_t temp) {
  $1 = &temp;
}

%typemap(argout) sgx_target_info_t* p_target_info {
    $result = PyBytes_FromStringAndSize((char*) $1, sizeof(sgx_target_info_t));
}
