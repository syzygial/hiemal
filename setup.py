from setuptools import setup, Extension, Distribution
import setuptools.command.build_py
import setuptools.command.build_ext

import os
import glob
import subprocess
import shutil

class cmake_wrapper:
  def __init__(self, cmake_build_dir='build', install_lib=True):
    self.build_dir = cmake_build_dir
    self.cache_file = self.build_dir + '/CMakeCache.txt'
    if os.path.isfile(self.cache_file):
      self.cache = self.parse_cmake_cache()
      self.install_dir = self.cache['CMAKE_INSTALL_PREFIX']
    else:
      self.cache = None
      self.install_dir = None
    self.state = self.get_cmake_state()
    self.install_lib = install_lib
  def parse_cmake_cache(self):
    with open(self.cache_file, "r") as f:
      cmake_cache_str = f.read()
    # filter out comments and empty strings https://stackoverflow.com/a/3845449
    cmake_cache_var = [cache_var for cache_var in cmake_cache_str.split('\n') \
      if not cache_var.startswith(('#', '//')) and cache_var]
    cmake_dict = dict()
    for var in cmake_cache_var:
      var_components = var.split('=')
      var_name = var_components[0].split(':')[0]
      var_value = var_components[1]
      cmake_dict[var_name] = var_value
    return cmake_dict
  def get_cmake_state(self):
    if not os.path.isfile(self.cache_file):
      return 0 # cmake not run
    elif not os.path.isfile(self.build_dir + '/libhiemal.so'):
      return 1 # cmake run but library not built
    elif not os.path.isdir(self.install_dir):
      return 2 # library build but not installed
    else: # library installed
      return 3
  def run_cmake(self):
    old_dir = os.getcwd()
    os.chdir(self.build_dir)
    if self.state == 0:
      print("running cmake")
      subprocess.run(["cmake", ".."])
      self.state = 1
    if self.state == 1:
      print("running make")
      subprocess.run(["make"])
      self.state = 2
    if self.state == 2:
      if self.install_lib:
        print("running make install")
        subprocess.run(["make", "install"])
      else:
        print("skipping make install step")
      self.state = 3
    if self.state == 3:
      print("finished building libhiemal")
    os.chdir(old_dir)

cmake = cmake_wrapper()
cmake.run_cmake()

def gen_pb_python(src, dest):
  proto_files = glob.glob(src + "/*.proto")
  subprocess.run(["protoc", "--proto_path="+src,"--python_out="+dest] + proto_files)

class build_py(setuptools.command.build_py.build_py):
  def run(self):
    setuptools.command.build_py.build_py.run(self)
    gen_pb_python("src/protobuf", self.build_lib + "/hiemal/backend/protobuf")

class build_ext(setuptools.command.build_ext.build_ext):
  def run(self):
    lib_dir = self.build_lib + '/hiemal/lib'
    include_dir = self.build_lib + '/hiemal/include'
    #import pdb;pdb.set_trace()
    try:
      os.mkdir(lib_dir)
    except:
      pass

    self.copy_file(cmake.install_dir + '/lib/libhiemal.so', lib_dir + '/libhiemal.so')
    self.copy_tree(cmake.install_dir + '/include', include_dir)
    setuptools.command.build_ext.build_ext.run(self)

def main():
  hm_pywrap = Extension( name="hiemal.backend._hm_pywrap", \
    sources=["src/python/hiemal/backend/src/_hm_pywrap.c"], \
    libraries=["hiemal"], \
    library_dirs=[cmake.install_dir + "/lib"], \
    include_dirs=[cmake.install_dir + "/include"],
    runtime_library_dirs=["$ORIGIN/../lib"],
    extra_compile_args=["-g"])

  setup(
    name="hiemal", 
    version="0.1.0-alpha.0", 
    description="A lightweight audio library",
    long_description="Hiemal is a lightweight C audio library which aims to "
      "provide a simple interface for building signal chains for low-latency, "
      "real-time audio processing including DSP, file i/o, etc. This project "
      "is very young and under active development, so the public API should not "
      "be considered stable and might change with little notice.",
    ext_modules=[hm_pywrap],
    cmdclass={'build_py': build_py, 'build_ext': build_ext},
    packages=['hiemal', 'hiemal.tools', 'hiemal.backend', 'hiemal.backend.protobuf'],
    package_dir={'hiemal': "src/python/hiemal"},
    url="https://www.hiemal.dev/",
    download_url="https://github.com/hiemal-labs/hiemal",
    project_urls={
      "Issue Tracker": "https://github.com/hiemal-labs/hiemal/issues",
      "Source Code": "https://github.com/hiemal-labs/hiemal",
      "Documentation": "https://www.hiemal.dev/"
    },
    author="syzygial",
    author_email="info@hiemal.dev",
    classifiers=[
      "Development Status :: 2 - Pre-Alpha",
      "Topic :: Multimedia :: Sound/Audio",
      "License :: OSI Approved :: MIT License"
    ],
    license="MIT"
  )

if __name__ == "__main__":
  main()