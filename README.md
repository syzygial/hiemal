<p align="center">
  <img src="docs/_static/hiemal.png">
</p>

![Hiemal Version](https://img.shields.io/badge/version-0.1.0-blue)
[![tests](https://github.com/hiemal-audio/hiemal/actions/workflows/run_tests.yml/badge.svg)](https://github.com/hiemal-audio/hiemal/actions/workflows/run_tests.yml)

> [!WARNING]
> This project is very young and under active development, so the public API should not be considered stable and might change with little notice.

Hiemal is a lightweight C audio library which aims to provide a simple 
interface for building signal chains for low-latency, real-time audio 
processing including DSP, file i/o, etc.  

# Supported Platforms

Currently Hiemal only supports x86_64 Linux systems, although future support for different platforms and operating systems will be added.

# Installing

Hiemal can be built from source with cmake:

```
mkdir build && cd build
cmake ..
make
make install
```

# Documentation
Documentation for Hiemal can be found at [https://www.hiemal.dev](https://www.hiemal.dev)

# License

Hiemal is available under the [MIT License](LICENSE)
