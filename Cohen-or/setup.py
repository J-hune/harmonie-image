from setuptools import setup
from Cython.Build import cythonize

setup(
    ext_modules = cythonize("cohenOrC.py", extra_compile_args=["-O3"])


)