#!/usr/bin/env python3

import glob
import os
import argparse

DIR = os.path.dirname(__file__)

import sys
sys.path.append(os.path.join(DIR, '..'))
from sgx.config import PYTHON_VERSION, LIBPROTOBUF_VERSION

GRAPHENE_DIR = os.path.abspath(os.path.join(DIR, "../graphene/"))
RUNTIME = os.path.join(GRAPHENE_DIR, "Runtime")


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("PATH", nargs='?', default=".", help="Path to the manifest template or the directory containing the manifest templates")
    return parser.parse_args()


def get_manifest_templates(path):
    manifests = glob.glob(path + "/*.manifest.template")
    if not manifests:
        sys.exit("Cannot find manifest templates in directory %r" % path)
    return manifests


def main():
    args = parse_args()
    path = os.path.abspath(args.PATH)

    if path.endswith(".manifest.template"):
        if not os.path.isfile(path):
            sys.exit("Cannot find file %r" % path)
        manifest_templates = [path]
    else:
        manifest_templates = get_manifest_templates(path)

    for manifest_template in manifest_templates:
        manifest = manifest_template[:-9]
        with open(manifest_template) as f_template:
            with open(manifest, "w+") as f_manifest:
                for line in f_template:
                    line = line.replace("$(RUNTIME)", RUNTIME)
                    line = line.replace("$(PYTHON_VERSION)", PYTHON_VERSION)
                    line = line.replace("$(LIBPROTOBUF_VERSION)", LIBPROTOBUF_VERSION)
                    f_manifest.write(line)


if __name__ == "__main__":
    main()
