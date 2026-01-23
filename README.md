# EsoProg

<p align="center">
  <img src="https://img.shields.io/badge/language-c++%2020-blue?style=for-the-badge&logo=cplusplus" alt="Language: C++20" />
  <img src="https://img.shields.io/badge/license-mit-yellow?style=for-the-badge&logo=opensourceinitiative&logoColor=white" alt="License: MIT"/>
  <img src="https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey?style=for-the-badge" alt="Platforms: Windows | macOS | Linux" />
  <!-- TODO: Add build status when workflow is completed (ci.yaml) -->
</p>

> Playground of Esoteoric Languages!

---

## Table of Content (TOC)

- [About the Project](#about-the-project)
  - [Introduction](#introduction)
  - [Supported Languages](#supported-languages)
  - [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Basic Usage](#basic-usage)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)

---

## About the Project

> Playground of Esoteoric Languages!

### Introduction

**EsoProg** is a playground for various types of esoteoric languages.
Esoteoric languages are type of languages which are designed to non-conventional and hard to write.
In EsoProg you can write programs in these languages with ease!

![Piet Empty Program Example](./ReadMe_Resources/Empty_Program_Example.png)
![Piet Quest Example](./ReadMe_Resources/PietQuest_Running.png)

### Supported Languages

The languages which are supported by **EsoProg** are as following:
- Piet
- BrainF\*ck
- COW
- Auld

### Features

- **Performance:** Written in C++20 for modern type-safety and high performance.

- **Support:** Supports various types of Esoteoric languages from image-based to pointer-based.

- **Editor:** Comes with an ImGui-based editor for writing in these languages.

---

## Getting Started

### Prerequisites

1. **C++20 Supported Compiler:**
  - GCC 11.1+
  - Clang 13.0+
  - MSVC 16.11+

2. **Premake:** Premake v5

### Installation

1. **Clone the Repository:**

```bash
git clone https://github.com/yanniknelson/EsoProg.git
```

2. **Initialize Submodules:**

```bash
git submodule update --init --recursive
```

3. **Build the Project:**

- **For Windows:**

```bash
./Scripts/Setup-Windows.bat
```

- **For Linux/MacOS:**

```bash
./Scripts/Setup-Linux.bat
```

> After building you'll have the respective executable for your OS.

### Basic Usage

Consult [Examples](./App/Examples) folder for basic usage and programs.

---

## Roadmap

Currently Piet has been done and BrainF\*ck is under-development. COW and Auld development will start after that.

**Current Development Stage:**
- Piet [✓]
- BrainF\*ck [-]
- COW []
- Auld []

---

## Contributing

If you find this interesting then consider contributing to it! All contributions are welcomed whole-heartedly.

> See the contributing guideline: [CONTRIBUTING.md](./contributing.md)

---

## License

EsoProg is under **MIT License** and all its third-party vendors are under their own licenses and terms and conditions.