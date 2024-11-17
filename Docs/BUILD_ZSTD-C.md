
# Zstd Build Automation

This project provides automation scripts for building and setting up the Zstandard (Zstd) compression library. It includes two scripts: one for Windows (`build_zstd.bat`) and one for Linux/macOS (`build_zstd.sh`), both of which automate the process of cloning the Zstd repository, running a Python script to generate a single .c file, and cleaning up afterward.

## Prerequisites

Before running the scripts, ensure you have the following software installed:

For both Linux/macOS and Windows:

- **Git**: For cloning the Zstd repository.
    - Install Git: https://git-scm.com/

- **Python 3**: The script relies on Python to run `combine.py` to generate `zstd.c`.
    - Install Python 3: https://www.python.org/downloads/

## Usage Instructions

#### For Linux/macOS
Run the following commands in your terminal:
```shell
./build_zstd.c.sh
```

- Note: Ensure the script has executable permissions. If not, grant them using:

```shell
chmod +x ./build_zstd.c.sh
```

#### For Windows
Run the following command in Command Prompt or PowerShell:
```bash
build_zstd.c.bat
```

Upon successful execution
- The `zstd.c` file will be located at `./lib/zstd/zstd.c`.
- The temporary `build_zstd_c` directory will be removed automatically.

## Troubleshooting

#### Common Issue

- **Git Clone Fails**:
    - Ensure Git is installed and available in your PATH.

- **Python Script Fails**:
    - Verify that Python 3 is installed.
    - Ensure all necessary Python dependencies for `combine.py` are available.

- **Permission Denied (Linux/macOS)**:
    - Ensure the script has executable permissions (`chmod +x build_zstd.sh`).

- **Directory Not Found**:
    - Verify that the working directory contains the required scripts and paths.