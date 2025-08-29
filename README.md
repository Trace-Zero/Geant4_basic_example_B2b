# Geant4_basic_exammple_b2b


This repository contains a minimal implementation of the Geant4 **B2b basic example**.  
It provides the original source (`.cc` and `.hh`) files, a CMake build script, and documentation for compiling and running the application.  

---

## Overview
The Geant4 basic example B2b demonstrates:
- Detector construction with parameterized geometry
- Primary particle generation
- Event, run, and stepping actions
- Output suitable for histogramming or ntuple storage (optionally with ROOT)

This repository preserves the example in a minimal, clean structure for reference and reproducibility.

---

## Requirements
- Geant4 (11.x recommended)
- CMake ≥ 3.16
- C++17 compiler (GCC or Clang)
- [Optional] ROOT libraries for data storage

Activate your Geant4 environment before compiling:
```bash
source /path/to/geant4-install/bin/geant4.sh
