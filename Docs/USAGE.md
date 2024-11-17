
# AoV Zstd Build and Usage Guide

This guide provides instructions for building and using the AoV Zstd compression tool. 

## Requirements

Before building, ensure the following tools are installed on your system:

- **GCC Compiler**: Required for compiling the C source files.
- **Make** (Optional): For using the provided `Makefile` on Linux/macOS systems.

If the `lib/zstd/zstd.c` file does not exist, you will need to follow the instructions in the [BUILD_ZSTD.C.md](https://github.com/wxrayut/AoV-Zstd/blob/main/Docs/BUILD_ZSTD-C.md) to generate it

## Build Instructions

To get started, first clone the repository and navigate into the project directory:

```bash
git clone https://github.com/wxrayut/AoV-Zstd.git
cd AoV-Zstd
```

#### For Linux/macOS

Install `libzstd-dev`:
```
sudo apt install libzstd-dev
```

Create a build directory
```
mkdir build
```

Run the following command in your terminal to compile using the provided `Makefile`:
```
make
```

- After building, you can run the tests to verify everything is working correctly. To do this, simply run the following command:

    - Decompress with directory option: Decompress files in a directory.
    ```
    make decompress_with_dir_option
    ```

    - Compress with directory option: Compress files from a directory:
    ```
    make compress_with_dir_option
    ```

    - Decompress with file option: Decompress a specific file:
    ```
    make decompress_with_file_option
    ```

    - Compress with file option: Compress a specific file:
    ```
    make compress_with_file_option
    ```

- To run all the tests at once, use the following command:
    ```
    make test
    ```

#### For Windows
Run the following command in Command Prompt or PowerShell:
```bash
build_AoV_Zstd.bat
```

## Usage Instructions

The general syntax for using the AoV Zstd tool is:
```
AoV-Zstd [MODE] [OPTIONS] [-f FILE | -D DIRECTORY]
```

#### Modes
```
-c,  --compress             Compress the specified file or directory.
-d,  --decompress           Decompress the specified file or directory.
```

#### Options
```
-l,  --clevel  LEVEL        Set the compression level (e.g., 1-22, with 22 being the highest).
                            Default level is 19.
-D,  --dir     DIRECTORY    Specify a directory to compress or decompress.
                            Recommended for handling multiple files in a directory.
-f,  --file    FILE         Specify a single file to compress or decompress.
-o,  --output  OUTPUT       Specify the output path for the result (file or directory).
                            If not provided, the input file or directory will be used.
-V,  --verbose VERBOSE      Enable verbose output, showing detailed progress.
-v,  --version VERSION      Display the program version.
-h,  --help    HELP         Display this help message.
```

## Example Usage

- Decompress files in a directory:
```
./AoV-Zstd --decompress --dir ./tests/106_XiaoQiao/skill -o ./output -V
```

- Decompress a single file:
```
./AoV-Zstd --decompress --file ./tests/106_XiaoQiao/imprint/10620_imprint.xml -o ./10620_imprint_decompressed.xml --verbose
```

## Troubleshooting

#### Common Issue

- **Missing Tools**:
    - Ensure GCC, Make, and Git are installed and available in your PATH.

- **Missing Files**:
    - If `lib/zstd/zstd.c` is missing, refer to the instructions in [BUILD_ZSTD.C.md](https://github.com/wxrayut/AoV-Zstd/blob/main/Docs/BUILD_ZSTD-C.md).