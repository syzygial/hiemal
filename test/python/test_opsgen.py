import unittest
import re

# unit under test
import gen_ops

__all__ = ["TestOpGen"]

class TestOpGen(unittest.TestCase):
  _test_name = "gen_ops"
  def test_opsgen(self):
    test_str1 = """
// [ a   : int,  b:  char]
DSP_OP (  test1){
  return 0;
}

// [a: char, b: int]
DSP_OP(test2) {
  return 0;
}
"""
    str1_ops = gen_ops.file_ops(test_str1, "DSP_OP")
    self.assertTrue(len(str1_ops) == 2)
    self.assertTrue(str1_ops[0][0] == "test1")
    self.assertTrue(str1_ops[1][0] == "test2")
    self.assertTrue(str1_ops[0][1] == ['int a', 'char b'])
    self.assertTrue(str1_ops[1][1] == ['char a', 'int b'])
  
if __name__ == "__main__":
  unittest.main()