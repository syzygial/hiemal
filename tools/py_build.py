# PEP 517 build backend

import os
import subprocess

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

# Required hooks

def build_wheel(wheel_directory, config_settings=None, metadata_directory=None):
  pass

def build_sdist(sdist_directory, config_settings=None):
  pass


# Optional hooks

def get_requires_for_build_wheel(config_settings=None):
  pass

def prepare_metadata_for_build_wheel(metadata_directory, config_settings=None):
  pass

def get_requires_for_build_sdist(config_settings=None):
  pass