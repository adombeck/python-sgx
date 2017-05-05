// ----------------------------------------------------------------------------
// map sgx_report_t to Python bytes
// ----------------------------------------------------------------------------

%typemap(in) sgx_report_t report {
    assert(sizeof(sgx_report_t) == 432);

    if (!PyBytes_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytes parameter");
        SWIG_fail;
    }

    if (PyObject_Length($input) != sizeof(sgx_report_t)) {
        PyErr_SetString(PyExc_ValueError, "Expected a bytes parameter with 432 elements");
        SWIG_fail;
    }

    memcpy(&$1, PyBytes_AsString($input), sizeof(sgx_report_t));
}


// Define typemaps for p_report

// This typemap suppresses requiring the parameter as an input.
%typemap(in,numinputs=0) sgx_report_t* p_report (sgx_report_t temp) {
    memset(&temp, 0, sizeof(sgx_report_t));
    $1 = &temp;
}

%typemap(argout) sgx_report_t* p_report {
    PyObject* pybytes = PyBytes_FromStringAndSize((char*) $1, sizeof(sgx_report_t));
    $result = SWIG_Python_AppendOutput($result, pybytes);
}
