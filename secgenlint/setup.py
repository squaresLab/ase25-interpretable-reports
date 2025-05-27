# -*- coding: utf-8 -*-

import os
import sys
from setuptools import setup, find_packages

# ??
if sys.hexversion < 0x3000000:
    print('SECGENLint requires Python 3.6 or newer!')
    sys.exit()

dir = os.path.dirname(os.path.abspath(__file__))
version = {}
with open(os.path.join(dir, "secgenlint", "version.py")) as fp:
    exec(fp.read(), version)

with open('requirements.txt') as f:
    requirements = f.read().splitlines()

with open('README.md') as f:
    long_description = f.read()


setup(
    name='secgenlint',
    version=version["__version__"],
    python_requires='>=3.6',
    # description='SECGENLint A Linter to measure compliance against [SECGEN](https://github.com/TQRG/secgen) convention.',
    # long_description=long_description,
    # author='Claudia Mamede',
    # author_email='cmamede@andrew.cmu.edu',
    # url='<retracted>',
    # download_url='<retracted>',
    include_package_data=True,
    packages=['secgenlint'],
    package_data={
        'secgenlint': ['config/rules.yml', 'entities/patterns.jsonl'],
    },
    entry_points={
        "console_scripts": ['secgenlint = secgenlint.cli:main']
    },
    install_requires=requirements,
    classifiers=[
        'Programming Language :: Python :: 3.0',
        'Environment :: Console',
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
    ],
)