#!/usr/bin/env python3

import glob
import os

import sys
sys.path.append('..')
from sgx.config import GRAPHENE_DIR, PYTHON_VERSION, LIBPROTOBUF_VERSION

RUNTIME = os.path.join(GRAPHENE_DIR, "Runtime")

manifest_templates = glob.glob("*.manifest.template")

if not manifest_templates:
    sys.exit("Cannot find manifest.template in current directory")

for manifest_template in manifest_templates:
    manifest = manifest_template[:-9]
    with open(manifest_template) as f_template:
        with open(manifest, "w+") as f_manifest:
            for line in f_template:
                line = line.replace("$(RUNTIME)", RUNTIME) 
                line = line.replace("$(PYTHON_VERSION)", PYTHON_VERSION) 
                line = line.replace("$(LIBPROTOBUF_VERSION)", LIBPROTOBUF_VERSION) 
                f_manifest.write(line)

