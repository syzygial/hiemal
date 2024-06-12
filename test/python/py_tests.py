import unittest
import io

def run_test(test_name):
  if test_name.startswith("py:"):
    test_name = test_name.split("py:")[-1]
  try:
    uut = __import__(test_name, globals(), locals(), [], 0)
  except ModuleNotFoundError:
    raise ValueError("Could not find file for test {}".format(test_name))
  suite = unittest.TestSuite()
  for test_case in uut.__all__:
    test_case = getattr(uut, test_case)
    suite.addTests([test_case(i) for i in dir(test_case) if i.startswith("test")])
  test_buf = io.StringIO()
  runner = unittest.TextTestRunner(stream=test_buf)
  return runner.run(suite).wasSuccessful()

if __name__ == "__main__":
  import sys
  import unittest
  for test in sys.argv[1:]:
    try:
      _tmp = __import__(test, globals(), locals(), ['*'], 0)
      sys.argv.remove(test)
      for attr in _tmp.__all__:
        globals()[attr] = _tmp.__dict__[attr]
    except ModuleNotFoundError:
      pass
  unittest.main()