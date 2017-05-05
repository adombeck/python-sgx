#!/usr/bin/env python3.5
"""A setuptools based setup module.
See:
https://packaging.python.org/en/latest/distributing.html
https://github.com/pypa/sampleproject
"""

# Always prefer setuptools over distutils
import os
import sh
import subprocess
import platform
# To use a consistent encoding
from codecs import open
from distutils.version import LooseVersion

from setuptools import setup, find_packages

import importlib
config = importlib.import_module("sgx.config")

GRAPHENE_DIR = os.path.abspath("./graphene/")


# Check if swig >= 3.0.10 is installed
out = sh.grep(sh.dpkg("-s", "swig", "swig3.0", _ok_code=[0,1]), '^Version:')
version = out.split(" ")[-1].split("-")[0]
if not version:
  exit("Error: Couldn't find swig")
if LooseVersion(version) < LooseVersion("3.0.10"):
  exit("Error: swig version is lower than 3.0.10. Install swig 3.0.10 or higher.")


# Check if apport is installed (it produces error messages when executing Python with Graphene)
try:
    sh.dpkg("-s", "python3-apport")
    exit("Error: python3-apport is installed. Please uninstall it.")
except sh.ErrorReturnCode_1:
    pass

here = os.path.abspath(os.path.dirname(__file__))


# Link graphene's pal launcher to /usr/bin/graphene-pal
pal = "/usr/bin/graphene-pal"
if not os.path.islink(pal):
    subprocess.check_call(["ln", "-s", os.path.join(GRAPHENE_DIR, "Runtime/pal-Linux-SGX"), pal])


# Create the key directory
sh.install("-m", "755", "-d", config.KEY_DIR)


# Create the data directory
sh.install("-m", "755", "-d", config.DATA_DIR)


# Copy trusted-ra-manager to `/usr/local/bin/`.
# If trusted-ra-manager is included in the `scripts` argument to setup(), setup() creates a script
# which uses run_script() to execute the actual script, thereby ignoring our custom shebang to run python3-sgx.
sh.cp("scripts/trusted-ra-manager", "/usr/local/bin/")


# Create python3 launcher in the data directory
sh.cp("launcher/python3.manifest.template", config.DATA_DIR)
create_manifest = sh.Command(os.path.abspath("utils/create_manifest.py"))
sign_manifest = sh.Command(os.path.abspath("utils/sign_manifest.py"))
create_manifest(config.DATA_DIR)
sign_manifest(config.DATA_DIR)


# Get the long description from the README file
with open(os.path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

dist = setup(
    name='sgx',

    # Versions should comply with PEP440.  For a discussion on single-sourcing
    # the version across setup.py and the project code, see
    # https://packaging.python.org/en/latest/single_source_version.html
    version='0.1a0',

    description='Intel SGX Python wrapper',
    long_description=long_description,

    # The project's main homepage.
    # url='https://github.com/pypa/sampleproject',

    # Author details
    author='Adrian Dombeck',
    author_email='adrian.dombeck@rub.de',
    # Choose your license
    # license='MIT',

    # See https://pypi.python.org/pypi?%3Aaction=list_classifiers
    classifiers=[
        # How mature is this project? Common values are
        #   3 - Alpha
        #   4 - Beta
        #   5 - Production/Stable
        'Development Status :: 3 - Alpha',

        # Indicate who your project is intended for
        'Intended Audience :: Developers',
        'Topic :: Software Development',

        # Pick your license as you wish (should match "license" above)
        # 'License :: OSI Approved :: MIT License',

        # Specify the Python versions you support here. In particular, ensure
        # that you indicate whether you support Python 2, Python 3 or both.
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.5',
    ],

    # What does your project relate to?
    keywords='sgx security development',

    include_package_data=True,

    # You can just specify the packages manually here if your project is
    # simple. Or you can use find_packages().
    packages=find_packages(exclude=['contrib', 'docs', 'tests']),

    scripts=["scripts/python3-sgx",
             "scripts/untrusted-ra-manager"],

    # Alternatively, if you want to distribute just a my_module.py, uncomment
    # this:
    #   py_modules=["my_module"],

    # List run-time dependencies here.  These will be installed by pip when
    # your project is installed. For an analysis of "install_requires" vs pip's
    # requirements files see:
    # https://packaging.python.org/en/latest/requirements.html
    install_requires=['sh', ],

    # List additional groups of dependencies here (e.g. development
    # dependencies). You can install these using the following syntax,
    # for example:
    # $ pip install -e .[dev,test]
    # extras_require={
    #    'dev': ['check-manifest'],
    #    'test': ['coverage'],
    # },

    # If there are data files included in your packages that need to be
    # installed, specify them here.  If using Python 2.6 or less, then these
    # have to be included in MANIFEST.in as well.
    # package_data={
    #     'sample': ['package_data.dat'],
    # },

    # Although 'package_data' is the preferred approach, in some case you may
    # need to place data files outside of your packages. See:
    # http://docs.python.org/3.4/distutils/setupscript.html#installing-additional-files # noqa
    # In this case, 'data_file' will be installed into '<sys.prefix>/my_data'
    # data_files=[('my_data', ['data/data_file'])],

    # To provide executable scripts, use entry points in preference to the
    # "scripts" keyword. Entry points provide cross-platform support and allow
    # pip to create the appropriate form of executable for the target platform.
    # entry_points={
    #     'console_scripts': [
    #         'sample=sample:main',
    #     ],
    # },
)
