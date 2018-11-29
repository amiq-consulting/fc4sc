import os
import sys
import yaml
import argparse

sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'coverage_merge'))

import merge

args = {}

def report_coverage(report_missing=False):
    for mod_name, mod_data in d['modules'].items():
        if(report_missing and (mod_data['pct_cov'] == 100.00)):
            continue
        print(
"""     %6.2f : %s
""" % (mod_data['pct_cov'], mod_name))
        for inst_name, inst_data in mod_data['instances'].items():
            if(report_missing and (inst_data['pct_cov'] == 100.00)):
                continue
            print(
"""     %6.2f : %s
""" % (inst_data['pct_cov'], inst_name))
            for cp_name, cp_data in inst_data['inst_data'].items():
                if(report_missing and (cp_data['pct_cov'] == 100.00)):
                    continue
                print(
"""         %6.2f : %s """ % (cp_data['pct_cov'], cp_name))
                if(report_missing and args.report_missing_bins):
                    for bin in cp_data['misses']:
                        print(
"""                     %s """ % bin )


def reduce_to_cg_inst_summary(db):
    out_db = { 'pct_cov' : db['pct_cov'] }
    for mod_name, mod_data in db['modules'].items():
        for inst_name, inst_data in mod_data['instances'].items():
            d = {'pct_cov' : inst_data['pct_cov'],
                 'points' : {},
                 'crosses' : {}}
            out_db[inst_name] = d ;
            for cp_name, cp_data in inst_data['inst_data'].items():
                if(cp_data['item_type'] == 'point'):
                    d['points'][cp_name] = cp_data['pct_cov']
                else:
                    d['crosses'][cp_name] = cp_data['pct_cov']
    return out_db


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='FC4SC report generator')

    parser.add_argument('--xml_report',           type=str, help="Input XML report")
    parser.add_argument('--report_missing_bins',  action='store_true', help="Including missing bin details in report")
    parser.add_argument('--yaml_out',             type=str, help="Output YAML file")
    parser.add_argument('--yaml_cg_summary',      action='store_true', help="Covergroup summary to YAML")

    args = parser.parse_args()

    parser = merge.UCIS_DB_Parser()

    try:
        d = parser.get_report_data(args.xml_report)
    except IOError as e:
        sys.stderr.write("Failed to parse XML report:\n\t%s\n" % (str(e)))
        sys.stderr.flush()
        exit(1) ;

    if(args.yaml_out):
        yaml_db = d
        if(args.yaml_cg_summary):
            yaml_db = reduce_to_cg_inst_summary(d)
        with open(args.yaml_out, 'w') as yaml_file:
            yaml.dump(yaml_db, yaml_file, default_flow_style=False)

    print("""
  Overall Summary:
  
    Total Coverage: %6.2f
    
  Module Summary:
""" % (yaml_db['pct_cov']))

    report_coverage()

    if (args.report_missing_bins):
        print("""
  Missing Bins
""")
        report_coverage(report_missing=True)
print("")

