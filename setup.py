from setuptools import setup

setup(
  name="hiemal", 
  version="0.1.0-alpha.0", 
  description="A lightweight audio library",
  long_description="Hiemal is a lightweight C audio library which aims to"
    "provide a simple interface for building signal chains for low-latency,"
    "real-time audio processing including DSP, file i/o, etc. This project"
    "is very young and under active development, so the public API should not"
    "be considered stable and might change with little notice.",
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