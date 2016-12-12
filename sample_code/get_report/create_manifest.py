#!/usr/bin/env python3

import os
import glob

from config import GRAPHENE_DIR

SHIMPATH = os.path.join(GRAPHENE_DIR, "LibOS/shim/src/libsysdb.so")
LIBCDIR = os.path.join(GRAPHENE_DIR, "LibOS/glibc-build")

manifest_templates = glob.glob("*.manifest.template")

for manifest_template in manifest_templates:
    manifest = manifest_template[:-9]
    with open(manifest_template) as f_template:
        with open(manifest, "w+") as f_manifest:
            for line in f_template:
                line = line.replace("$(SHIMPATH)", SHIMPATH)
                line = line.replace("$(LIBCDIR)", LIBCDIR) 
                f_manifest.write(line)

