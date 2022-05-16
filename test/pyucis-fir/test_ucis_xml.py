
from io import BytesIO
import os
from ucis.xml import validate_ucis_xml




with open('../../examples/fir/coverage_results.xml') as f:
  xml_file = BytesIO(bytes(bytearray(f.read(), encoding='utf-8')))
  validate_ucis_xml(xml_file)


