import os
import sys
import yaml
import argparse
import xml.etree.ElementTree as ET

sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

from ucis_parser import UCIS_DB_Parser


class UCIS_DB_Reporter(UCIS_DB_Parser):
    def get_report_data(self, filename):
        """ Parse covergroup types """
        parseTree = ET.parse(filename)
        parseRoot = parseTree.getroot()
        data = {
            'modules' : {},
            'pct_cov' : 0
        }
        for instanceCoverages in self.findall_ucis_children(parseRoot, "instanceCoverages"):
            module_data = {
                'pct_cov' : 0,
                'weight' : 1,
                'instances' : {}
            }
            data['modules'][instanceCoverages.get('moduleName')] = module_data
            covergroupCoverage = self.find_ucis_element(instanceCoverages, "covergroupCoverage")
            self.get_covergroup_report_data(covergroupCoverage, module_data)
            module_data['pct_cov'] = sum([cg['pct_cov']*cg['weight'] for cg in module_data['instances'].values()]) \
                                 / float(sum([cg['weight'] for cg in module_data['instances'].values()]))


        data['pct_cov'] = sum([cg['pct_cov']*cg['weight'] for cg in data['modules'].values()]) \
                                 / float(sum([cg['weight'] for cg in data['modules'].values()]))
        return data

    def get_covergroup_report_data(self, covergroupCoverage, module_data):
        for cgInstance in self.findall_ucis_children(covergroupCoverage, "cgInstance"):
            options = self.find_ucis_element(cgInstance, 'options')
            cg_data = {
                # 'inst_name' : cgInstance.get('name'),
                'weight': int(options.get('weight')),
                'inst_data': {},
                'pct_cov': 0,
            }
            cg_cp_bin_map = {}
            module_data['instances'][cgInstance.get('name')] = cg_data
            self.get_coverpoint_report_data(cgInstance, cg_cp_bin_map, cg_data)
            self.get_cross_report_data(cgInstance, cg_cp_bin_map, cg_data)
            # Avoid divide by zero if no coverage captured
            if not cg_data['inst_data']:
                cg_data['pct_cov'] = 0
            else:
                cg_data['pct_cov'] = sum([cp['pct_cov'] * cp['weight'] for cp in cg_data['inst_data'].values()]) \
                                             / float(sum([cp['weight'] for cp in cg_data['inst_data'].values()]))

    def get_coverpoint_report_data(self, cgInstance, cg_cp_bin_map, cg_data):
        for coverpoint in self.findall_ucis_children(cgInstance, "coverpoint"):
            options = self.find_ucis_element(coverpoint, 'options')
            cp_name = coverpoint.get('name')
            cp_data = {
                'item_type' : 'point',
                'bin_count': 0,
                'bin_hits': 0,
                'bin_misses': 0,
                'hits' : [],
                'misses' : [],
                'weight': int(options.get('weight')),
                'pct_cov': 0,
            }
            cg_data['inst_data'][cp_name] = cp_data
            cg_cp_bin_map[cp_name] = {}
            for bin_idx, bin in enumerate(self.findall_ucis_children(coverpoint, "coverpointBin")):
                cp_data['bin_count'] += 1
                bin_name = bin.get('name')
                cg_cp_bin_map[cp_name][bin_idx] = bin_name
                hits = int(bin.get('alias'))  # Alias is number of hits
                if (hits > 0):
                    cp_data['bin_hits'] += 1
                    cp_data['hits'].append(bin_name)
                else:
                    cp_data['bin_misses'] += 1
                    cp_data['misses'].append(bin_name)
            cp_data['pct_cov'] = 100 * ((cp_data['bin_count'] - cp_data['bin_misses']) / float(cp_data['bin_count']))

    def collect_cross_bins(self, exprs, cg_cp_bin_map, parrent_bins):
        expr_name = exprs[0].text
        new_bins = []
        for parrent_bin_tuple in parrent_bins:
            for expr_bin_idx in cg_cp_bin_map[expr_name]:
                new_bins.append(parrent_bin_tuple + tuple([expr_bin_idx]))
        if(len(exprs) > 1):
            return self.collect_cross_bins(exprs[1:], cg_cp_bin_map, new_bins)
        else:
            return new_bins

    def get_cross_bin_name_from_tuple(self, cg_cp_bin_map, exprs, bin_tuple):
        names = []
        for expr_idx, bin_idx in enumerate(bin_tuple):
            expr_name = exprs[expr_idx].text
           #expr_bin_name = "%s(%s)" % (expr_name, cg_cp_bin_map[expr_name][bin_idx])
            expr_bin_name = cg_cp_bin_map[expr_name][bin_idx]
            names.append(expr_bin_name)
        names.reverse()
        return " : ".join(names)

    def get_cross_report_data(self, cgInstance, cg_cp_bin_map, cg_data):
        for cross in self.findall_ucis_children(cgInstance, "cross"):
            options = self.find_ucis_element(cross, 'options')
            cr_name = cross.get('name')
            cr_data = {
                'bin_count': 0,
                'item_type': "cross",
                'bin_hits': 0,
                'bin_misses': 0,
                'hits': [],
                'misses': [],
                'weight': int(options.get('weight')),
                'pct_cov': 0,
                'bin_hit_data': None
            }
            cg_data['inst_data'][cr_name] = cr_data
            cg_cp_bin_map[cr_name] = {}
            exprs = self.findall_ucis_children(cross,'crossExpr')
            all_cross_bins = self.collect_cross_bins(exprs, cg_cp_bin_map, [tuple()])
            cr_data['bin_count'] = len(all_cross_bins)
            bin_hits = {}
            cr_data['bin_hit_data'] = bin_hits
            for cbin in all_cross_bins:
               bin_hits[cbin] = 0

            #
            for bin_idx, bin in enumerate(self.findall_ucis_children(cross, "crossBin")):
                bin_name = bin.get('name')
                cg_cp_bin_map[cr_name][bin_idx] = bin_name
                expr_indexes = self.findall_ucis_children(bin,'index')
                bin_tuple = ()
                for i in expr_indexes:
                   bin_tuple = bin_tuple + tuple([int(i.text)])
                bin_content = self.find_ucis_element(bin, 'contents')
                bin_hit_count = int(bin_content.get('coverageCount'))
                bin_hits[bin_tuple] = bin_hit_count
            for bin_tuple, hits in bin_hits.items():
                if(hits > 0):
                    cr_data['bin_hits'] += 1
                    cr_data['hits'].append(self.get_cross_bin_name_from_tuple(cg_cp_bin_map, exprs, bin_tuple))
                else:
                    cr_data['bin_misses'] += 1
                    cr_data['misses'].append(self.get_cross_bin_name_from_tuple(cg_cp_bin_map, exprs, bin_tuple))
            cr_data['pct_cov'] = 100 * ((cr_data['bin_count'] - cr_data['bin_misses']) / float(cr_data['bin_count']))

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
    parser.add_argument('--quiet',                action='store_true', help="Do write anything to standard output")

    args = parser.parse_args()

    parser = UCIS_DB_Reporter()
    
    if not args.xml_report:
        raise ValueError("No input XML provided!")
    if not args.yaml_out:
        raise ValueError("No output YAML name provided!")
    
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

    if not args.quiet:
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

    if not args.quiet:
        print("")

