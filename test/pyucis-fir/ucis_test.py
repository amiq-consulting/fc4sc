"""Tests for ucis."""

import unittest
from validate_ucis_xml import validate_ucis



class UcisTest(unittest.TestCase):

  def test_validate_ucis_xml(self):
    self.assertTrue(validate_ucis('../../examples/fir/coverage_results.xml'))



if __name__ == '__main__':
  unittest.main()
