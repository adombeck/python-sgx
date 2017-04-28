// ----------------------------------------------------------------------------
// map sgx_report_t to Python bytes
// ----------------------------------------------------------------------------

// This typemap suppresses requiring the parameter as an input.
%typemap(in,numinputs=0) sgx_report_t* p_report (sgx_report_t temp) {
  $1 = &temp;
}

%typemap(argout) sgx_report_t* p_report {
    $result = PyBytes_FromStringAndSize((char*) $1, sizeof(sgx_report_t));
}
