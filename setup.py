from setuptools import setup

from Cython.Build import cythonize
# https://stackoverflow.com/questions/36880336/setup-of-pycharm-for-cython
setup(
    ext_modules = cythonize("./Cohen-or/cohenOrC.pyx")
)