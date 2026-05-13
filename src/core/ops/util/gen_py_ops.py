#!/usr/bin/env python3

DIRECT_CONVERSIONS = {
  "[]": ["O", "PyObject*"] # e.g. double[]
}

DERIVED_CONVERSIONS = {
  "len": "ssize_t" # e.g. {"b": "double[]", "b_order": len(b)}
}

def _is_direct_conversion(pyop_type: str) -> str:
  for conv in DIRECT_CONVERSIONS.keys():
    if pyop_type.count(conv) > 0:
      return conv
  return ""

def _is_derived_conversion(pyop_type: str) -> str:
  for conv in DERIVED_CONVERSIONS.keys():
    if pyop_type.count(conv) > 0:
      return conv
  return ""

def _gen_parse_args(op_type: str, op: dict):
  parse_args_str =\
"""
  static const char* fmt_str = "{fmt_str}";
  {parsed_var_decls}
  PyArg_ParseTuple(args, fmt_str, {parsed_var_ref});
"""
  fmt_str = "OO" if op_type == "dsp" else "O"
  parsed_var_decls = ""
  parsed_var_ref = []
  if op_type == "source":
    parsed_var_decls = \
"""
  PyObject* dest = NULL;
"""
    parsed_var_ref.append("&dest")
  elif op_type == "sink":
    parsed_var_decls = \
"""
  PyObject* src = NULL;
"""
    parsed_var_ref.append("&src")
  elif op_type == "dsp":
    parsed_var_decls = \
"""
  PyObject* src = NULL;
  PyObject* dest = NULL;
"""
    parsed_var_ref.extend(["&src", "&dest"])
  for p, p_type in op["py_params"].items():
    direct_conv = _is_direct_conversion(p_type)
    if direct_conv:
      fmt_str += DIRECT_CONVERSIONS[direct_conv][0]
  return parse_args_str.format(fmt_str=fmt_str, 
    parsed_var_decls=parsed_var_decls,
    parsed_var_ref=", ".join(parsed_var_ref))

def gen_py_wrapper(op_type: str, op: dict):
  pyop_function_str =\
"""
int {function_name} (PyObject* self, PyObject* args) {{
  {parse_args}
  {convert_args}
  {invoke_fn}
  {cleanup}
  return Py_None;
}}
"""

  if "py_params" not in op.keys():
    return {}
  pyop_wrapper = {}
  pyop_function_name = "hm_pywrap_{}_{}".format(op["name"], op_type)
  pyop_function = pyop_function_str.format(
    function_name=pyop_function_name,
    parse_args=_gen_parse_args(op_type, op),
    convert_args="",
    invoke_fn="",
    cleanup=""
  )
  pyop_method = '{{"{}", {}, METH_VARARGS, NULL}}'.format(op["name"], pyop_function_name)
  
  fmt_str = ""

  pyop_wrapper["pyop_function"] = pyop_function
  pyop_wrapper["pyop_method"] = pyop_method
  return pyop_wrapper