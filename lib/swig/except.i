%exception {
  char *err;
  clear_exception();
  $action
  if ((err = check_exception())) {
    PyErr_SetString(PyExc_Exception, err);
    SWIG_fail;
  }
}