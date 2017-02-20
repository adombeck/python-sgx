#!/usr/bin/env python3

import glob
import os

from config import GRAPHENE_DIR

RUNTIME = os.path.join(GRAPHENE_DIR, "Runtime")

manifest_templates = glob.glob("*.manifest.template")

for manifest_template in manifest_templates:
    manifest = manifest_template[:-9]
    with open(manifest_template) as f_template:
        with open(manifest, "w+") as f_manifest:
            for line in f_template:
                line = line.replace("$(RUNTIME)", RUNTIME) 
                f_manifest.write(line)

