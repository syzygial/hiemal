#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

// #include <hiemal/buffer.h>
#include "intern/buffer.h"

// python wrapper around buffer_t object
typedef struct {
  PyObject_HEAD
  buffer_t *buf;
} PyBufObject;

static PyObject *PyBuf_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyBufObject *self = (PyBufObject*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}

static int PyBuf_init(PyBufObject *self, PyObject *args, PyObject *kwargs) {
  static char *kw_list[] = {"buf_size", NULL};

  unsigned int buf_size;

  if(!PyArg_ParseTupleAndKeywords(args, kwargs, "I", kw_list, &buf_size)) {
    return -1;
  }

  if (buffer_init(&(self->buf), buf_size, RING) != 0) {
    return -1;
  }
  return 0;
}

static void PyBuf_dealloc(PyBufObject *self) {
  if(self->buf != NULL) {
    buffer_delete(&(self->buf));
  }
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyMemberDef PyBuf_members[] = { {NULL} };

static PyObject *PyBuf_write(PyBufObject *self, PyObject *args, PyObject *kwargs) {
  PyBytesObject *bytes;
  void *dat;
  Py_ssize_t len;
  if (!PyArg_ParseTuple(args, "O!", &PyBytes_Type, &bytes)) {
    return NULL;
  }
  PyBytes_AsStringAndSize((PyObject*)bytes, (char**)&dat, &len);
  buffer_write(self->buf, (const void*)dat, (unsigned int)len);
  Py_RETURN_NONE;
}

static PyMethodDef PyBuf_methods[] = { 
  {"write", (PyCFunction) PyBuf_write, METH_VARARGS | METH_KEYWORDS, "write to buffer"},
  {NULL} 
};

static PyObject *PyBuf_getraw(PyBufObject *self, void *unused) {
  return PyMemoryView_FromMemory((char*)self->buf->buf, \
    (Py_ssize_t)self->buf->buf_len_bytes, _Py_MEMORYVIEW_C);
}

static PyGetSetDef PyBuf_getset[] = {
  {"raw", (getter)PyBuf_getraw, NULL, NULL, NULL},
  {NULL}
};

static PyTypeObject PyBufType = {
  PyVarObject_HEAD_INIT(NULL,0)
  .tp_name = "_hm_pywrap.Buffer",
  .tp_doc = NULL,
  .tp_basicsize = sizeof(PyBufObject),
  .tp_new = (newfunc) PyBuf_new,
  .tp_init = (initproc) PyBuf_init,
  .tp_dealloc = (destructor) PyBuf_dealloc,
  .tp_members = PyBuf_members,
  .tp_methods = PyBuf_methods,
  .tp_getset = PyBuf_getset,
};

static PyObject *hm_test(PyObject *self, PyObject *args) {
  return PyUnicode_FromString("hello world");
}

static PyMethodDef hm_methods[] = {
  {"test", hm_test, METH_NOARGS, "test method"},
  {NULL, NULL, 0, NULL}
};


static struct PyModuleDef hm_wrapper = {
  PyModuleDef_HEAD_INIT,
  "_hm_pywrap",
  NULL,
  -1,
  hm_methods
};

PyMODINIT_FUNC PyInit__hm_pywrap(void) {
  PyObject *m;
  if(PyType_Ready(&PyBufType) < 0) {
    return NULL;
  }

  m = PyModule_Create(&hm_wrapper);
  if (m == NULL) {
    return NULL;
  }
  Py_INCREF(&PyBufType);
  if(PyModule_AddObject(m, "Buffer", (PyObject*)&PyBufType) < 0) {
    Py_DECREF(&PyBufType);
    Py_DECREF(m);
    return NULL;
  }
  return m;
}