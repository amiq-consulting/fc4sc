""" Test script to compare coverage_results.xml with spec

Use pyucis to validate fir example coverage result generated
by FC4SC against specification.
"""

from io import BytesIO
from ucis.xml import validate_ucis_xml




with open('../../examples/fir/coverage_results.xml', encoding='utf-8') as f:
    xml_file = BytesIO(bytes(bytearray(f.read(), encoding='utf-8')))
    validate_ucis_xml(xml_file)
