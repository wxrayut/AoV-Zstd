
IF NOT EXIST build_zstd_c (
    mkdir .\build_zstd_c
)

git clone https://github.com/facebook/zstd.git .\build_zstd_c
IF %ERRORLEVEL% NEQ 0 (
    echo Git clone failed. Exiting. . .
    exit /b 1
)

IF NOT EXIST .\lib\zstd (
    mkdir .\lib\zstd
)

python ./build_zstd_c/build/single_file_libs/combine.py ^
    -r ./build_zstd_c/lib ^
    -x legacy/zstd_legacy.h ^
    -o ./lib/zstd/zstd.c ^
    ./build_zstd_c/build/single_file_libs/zstd-in.c
IF %ERRORLEVEL% NEQ 0 (
    echo Python script failed. Exiting. . .
    exit /b 1
)

IF EXIST build_zstd_c (
    rmdir /s /q build_zstd_c
    echo Removed build_zstd_c directory.
)
