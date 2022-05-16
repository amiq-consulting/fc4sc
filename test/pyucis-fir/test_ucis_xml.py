"""TODO(davjohn): DO NOT SUBMIT without one-line documentation for test_ucis_xml.

TODO(davjohn): DO NOT SUBMIT without a detailed description of test_ucis_xml.
"""


from io import BytesIO
import os
from ucis.xml import validate_ucis_xml




with open('fir.xml') as f:
  xml_file = BytesIO(bytes(bytearray(f.read(), encoding='utf-8')))
  validate_ucis_xml(xml_file)


