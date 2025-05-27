import os
import sys
from setuptools import setup, find_packages

# Check Python version
if sys.version_info[0] != 3 and sys.version_info[1] != 9 and sys.version_info[2] != 20:
    print('SECGEN requires Python 3.9.20!')
    sys.exit()

dir = os.path.dirname(os.path.abspath(__file__))
version = {}
with open(os.path.join(dir, "secgen", "version.py")) as fp:
    exec(fp.read(), version)

with open('requirements.txt') as f:
    requirements = f.read().splitlines()

with open('README.md') as f:
    long_description = f.read()

setup(
    name='secgen',
    version=version["__version__"],
    python_requires='==3.9.20',
    description='SECGEN A convention for making security vulnerability reports more structured and untimely more interpretable.',
    long_description=long_description,
    author='<retracted>',
    author_email='<retracted>',
    url='<retracted>',
    download_url='<retracted>',
    packages=['secgen'],
    package_data={'secgen': ['parsers/*']},
    entry_points={
        "console_scripts": ['secgen = secgen.cli:main']
    },
    install_requires=requirements,
    classifiers=[
        'Programming Language :: Python :: 3.9.20',
        'Environment :: Console',
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
    ],
)