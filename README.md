# File Signature Detector

A command-line tool for identifying file types based on their file signatures. This tool uses the Aho-Corasick algorithm for efficient multi-pattern searching.

## Description

This project is a C++ application that scans a given file for known file signatures (also known as "magic numbers") to determine the file's type. It uses a memory-mapped file for efficient reading and the Aho-Corasick algorithm to search for all signatures in a single pass.

## Building and Running

This project uses CMake for building.

### Prerequisites

*   A C++ compiler that supports C++20 (e.g., GCC 10 or later, Clang 12 or later)
*   CMake (version 3.10 or later)
*   Make

### Build Steps

1.  Clone the repository:
    ```bash
    git clone <repository-url>
    cd file-signature-detector
    ```

2.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```

3.  Run CMake and build the project:
    ```bash
    cmake ..
    make
    ```

    This will create an executable named `file-signature-detector` in the `build` directory.

## Usage

To use the tool, provide the path to the file you want to analyze as a command-line argument:

```bash
./build/file-signature-detector /path/to/your/file
```

The tool will then output a hexdump of the beginning of the file and a list of any matching file signatures it found.

## Project Structure

```
.
├── CMakeLists.txt
├── include
│   ├── aho_corasick.hpp
│   └── cli.hpp
├── misc
│   ├── common_signatures.csv
│   ├── file_signatures_subset.csv
│   └── filtered_signatures.csv
├── src
│   ├── aho_corasick.cpp
│   ├── cli.cpp
│   └── main.cpp
└── README.md
```

*   `src/main.cpp`: The main entry point for the application.
*   `src/cli.cpp`, `include/cli.hpp`: Contains functions for the command-line interface, including hexdump and file signature definitions.
*   `src/aho_corasick.cpp`, `include/aho_corasick.hpp`: Implementation of the Aho-Corasick algorithm.
*   `misc/`: Contains CSV files with file signatures. (Note: Currently, the signatures are hardcoded in `src/cli.cpp`).

## License

T# Binary Filetype Detection Engine

A low-level, mmap-based filetype detection engine written in C++.

This project identifies the *real* type of a file based on its binary structure — not its extension — using a two-phase detection pipeline inspired by malware scanners and forensic tools.

If a file lies, this tool doesn’t believe it.

---

## 🚀 What This Is

This is **not** a wrapper around `libmagic`.
This is **not** string-matching magic bytes and calling it a day.

This engine:

1. Scans raw bytes efficiently using **Aho–Corasick**
2. Generates *candidate* filetypes from known signatures
3. Performs **structural validation** to eliminate false positives

The result is **high-confidence filetype detection**, even for:

* renamed files
* embedded containers
* large disk images
* malware samples

---

## 🧠 Design Philosophy

> Signatures propose. Validators prove.

Magic bytes alone are unreliable. Real formats have rules.

This engine enforces those rules.

---

## 🧱 Architecture Overview

```
Raw File
  ↓
[mmap]
  ↓
Aho–Corasick Signature Scan
  ↓
Candidate Filetypes
  ↓
Structural Validators
  ↓
Confirmed Filetypes
```

Each stage exists because the previous one is insufficient on its own.

---

## 📦 Components

### 1. MappedFile (Zero-Copy File Access)

Uses `mmap` to treat the file as a read-only byte array.

* No heap allocation
* Kernel-managed paging
* Efficient random access
* Handles multi-gigabyte files

```cpp
MappedFile mf("sample.iso");
const std::byte* data = mf.getData();
size_t size = mf.getSize();
```

---

### 2. Signatures + Aho–Corasick

All known file signatures are loaded into an Aho–Corasick trie.

Each signature contains:

* byte pattern
* expected offset (optional)
* associated filetype
* weight (confidence contribution)

This allows:

* single-pass scanning
* O(n) performance
* simultaneous matching of hundreds of signatures

Signatures **do not confirm** a filetype — they only nominate candidates.

---

### 3. Candidate Scoring

Signature hits are aggregated per filetype using weights:

```cpp
scores[filetype] += signature.weight;
```

This produces a ranked list of *possible* formats.

Still not trusted.

---

### 4. Structural Validators (The Important Part)

Each file format has a dedicated validator that enforces rules from the format specification.

Validators are selected via a lookup table:

```cpp
validators["pdf"] = PDFValidator;
validators["zip"] = ZIPValidator;
```

If validation fails, the filetype is rejected — even if signatures matched.

---

## 🧪 Currently Supported Validators

| Format | Validation Checks                          |
| ------ | ------------------------------------------ |
| PE     | DOS header, e_lfanew sanity, NT header     |
| ZIP    | Local header + End of Central Directory    |
| PDF    | %PDF- header, %%EOF, xref presence         |
| ISO    | Primary Volume Descriptor (CD001 @ 0x8001) |

Validators are intentionally conservative to minimize false positives.

---

## 📄 Example Output

```
[+] Matches found: 2
  Offset: 0x8001 | Pattern: 43 44 30 30 31 | File type: iso
  Offset: 0x9001 | Pattern: 43 44 30 30 31 | File type: iso

Confirmed filetypes:
  iso
```

---

## ⚙️ Build & Run

### Build

```
mkdir build
cd build
cmake ..
make
```

### Run

```
./detector path/to/file
```

Large files (ISOs, disk images, dumps) are fully supported.

---

## 🧩 Extending the Engine

### Adding a New Filetype

1. Add one or more signatures
2. Implement a `FileValidator`
3. Register the validator

```cpp
validators["elf"] = std::make_shared<ELFValidator>();
```

That’s it.

