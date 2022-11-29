#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *hm_test(PyObject *self, PyObject *args) {
  return PyUnicode_FromString("hello world");
}

static PyMethodDef hm_methods[] = {
  {"test", hm_test, METH_NOARGS, "test method"},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef hm = {
  PyModuleDef_HEAD_INIT,
  "hm",
  NULL,
  -1,
  hm_methods
};

PyMODINIT_FUNC PyInit_hm(void) {
  return PyModule_Create(&hm);
}