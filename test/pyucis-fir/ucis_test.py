"""Tests for ucis."""

import unittest
from validate_ucis_xml import validate_ucis

#from usr.local.google.home.davjohn.progs.fc4sc.test.pyucis-fir import ucis
#from google3.testing.pybase import googletest


#class UcisTest(googletest.TestCase):
class UcisTest(unittest.TestCase):

  def test_validate_ucis_xml(self):
    self.assertTrue(validate_ucis('../../examples/fir/coverage_results.xml'))



if __name__ == '__main__':
  #googletest.main()
  unittest.main()
