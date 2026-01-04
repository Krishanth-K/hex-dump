# `hex-dump`: A Hex Viewer and File Type Finder

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-orange.svg)

> A fast, modern command-line tool for viewing file contents in hexadecimal and identifying file types using magic number signatures.

`hex-dump` is a C++ application for developers, reverse engineers, and digital forensics analysts who need a quick way to inspect file internals and determine a file's true type, regardless of its extension.

---

## Table of Contents

- [About The Project](#about-the-project)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building](#building)
- [Usage](#usage)

---

## About The Project

This tool serves two primary purposes: a hex viewer and a file type finder.

As a **hex viewer**, it provides a clean, formatted hexadecimal and ASCII representation of a file's contents.

As a **file type finder**, it reads a file's raw byte content and searches for known "magic number" signatures that identify its true type. For example, it can identify a PNG image even if it's been renamed with a `.txt` extension.

Efficiency is a core design goal. `hex-dump` uses memory-mapped I/O (`mmap`) to handle large files with minimal overhead and the Aho-Corasick algorithm to search for all known file signatures in a single, efficient pass.

---

## Features

- **Dual Functionality**: Acts as both a hex viewer and a file type finder.
- **High-speed Scanning**: Utilizes memory-mapped files to minimize I/O overhead.
- **Efficient Multi-pattern Search**: Employs the Aho-Corasick algorithm to find all known signatures in one pass.
- **Cross-Platform**: Built with standard C++ and CMake, targeting POSIX-compliant systems (Linux, macOS).

---

## Getting Started

Follow these instructions to get a copy of the project up and running on your local machine.

### Prerequisites

- **C++20 Compliant Compiler**:
  - GCC 10 or newer
  - Clang 12 or newer
- **CMake**:
  - Version 3.10 or newer
- **Git**

### Building

1.  **Clone the repository**
    ```sh
    git clone https://github.com/your_username/hex-dump.git
    cd hex-dump
    ```
2.  **Create a build directory and configure**
    ```sh
    mkdir build && cd build
    cmake ..
    ```
3.  **Compile the project**
    ```sh
    make
    ```
The compiled binary, `hex-dump`, will be located in the `build` directory.

---

## Usage

Run the tool from the command line, passing the path of the file you wish to inspect as an argument.

```sh
./build/hex-dump /path/to/your/file.ext
```

### Example Output

```
[+] Hexdump of the first 100 bytes:
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  |.PNG........IHDR|
00000010: 00 00 01 f4 00 00 01 f4 08 06 00 00 00 1f 15 c4  |................|
...

[+] Matches found: 1
  Offset: 0x0 | Pattern: 89 50 4e 47 0d 0a 1a 0a | File type: png
```


