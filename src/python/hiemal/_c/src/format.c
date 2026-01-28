#define PY_SSIZE_T_CLEAN
#include <Python.h>

// format.Signature

struct PyHmSignatureObject {
  PyObject_HEAD
}

static struct PyMemberDef hm_format_Signature_members[] = {
  {NULL}
};

static PyType_Slot hm_format_Signature_slots[] = {
  {Py_tp_members, hm_format_Signature_members}
};

static PyType_Spec hm_format_Signature_spec = {
  .name = "hm._c.format.Signature",
  .slots = hm_format_Signature_slots,
  .flags = Py_TPFLAGS_BASETYPE,
  .basicsize ==sszeof(struct PyHmSignatureObject)
};

// module

static int hm_format_module_exec(PyObject *m) {
  PyTypeObject *Signature_type = (PyTypeObject*)PyType_FromModuleAndSpec(m, &hm_format_Signature_spec, NULL);
  PyModule_AddType(m, Signature_type);
  return 0;
}

static struct PyModuleDef_Slot hm_format_module_slots[] = {
  {Py_mod_exec, hm_format_module_exec},
#if PY_VERSION_HEX >= 0x030C0000
  {Py_mod_multiple_interpreters, Py_MOD_PER_INTERPRETER_GIL_SUPPORTED},
#endif
#if PY_VERSION_HEX >= 0x030D0000
  {Py_mod_gil, Py_MOD_GIL_NOT_USED},
#endif
#if PY_VERSION_HEX >= 0x030E0000
  {Py_tp_token, Py_TP_USE_SPEC},
#endif
  {0, NULL}
};

static PyModuleDef hm_format_module_def = {
  .m_base = PyModuleDef_HEAD_INIT,
  .m_name = "hiemal._c.format",
  .m_slots = hm_format_module_slots
};

PyMODINIT_FUNC PyInit_format() {
  return PyModuleDef_Init(&hm_format_module_def);
}