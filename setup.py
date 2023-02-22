from setuptools import setup
import setuptools.command.build_py
import glob
import subprocess

def gen_pb_python(src, dest):
  proto_files = glob.glob(src + "/*.proto")
  subprocess.run(["protoc", "--proto_path="+src,"--python_out="+dest] + proto_files)


class build_py(setuptools.command.build_py.build_py):
  def run(self):
    setuptools.command.build_py.build_py.run(self)
    gen_pb_python("src/protobuf", self.build_lib + "/hiemal/backend/protobuf")


setup(
  name="hiemal", 
  version="0.1.0-alpha.0", 
  description="A lightweight audio library",
  long_description="Hiemal is a lightweight C audio library which aims to "
    "provide a simple interface for building signal chains for low-latency, "
    "real-time audio processing including DSP, file i/o, etc. This project "
    "is very young and under active development, so the public API should not "
    "be considered stable and might change with little notice.",
  cmdclass={'build_py': build_py},
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
