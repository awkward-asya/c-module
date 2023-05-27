from distutils.core import setup, Extension

module = Extension(
    'seam_carving', # название модуля внутри Python
    sources=['seam_carving_module.c'])

setup(name='seam_carving',
      version='1.1',
      description='Module for content aware image resizing using seam carving algorithm',
      ext_modules=[module])
