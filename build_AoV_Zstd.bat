@echo off

:: Create the build directory if it doesn't exist
if not exist "build" (
    echo Creating build directory. . .
    mkdir "build"
)

echo Building AoV_Zstd Project. . .
echo.

:: Compile Zstandard library
echo [1/9] Compiling Zstandard library. . .
gcc -c -o ./build/zstd.o ./lib/zstd/*.c -I./include/zstd/ -fPIC
if errorlevel 1 (
    echo [Error] Failed to compile Zstandard library!
    exit /b 1
)

:: Compile args.c
echo [2/9] Compiling args.c. . .
gcc -c -o ./build/args.o ./src/args.c -I./src/ -I./include/zstd/ -fPIC
if errorlevel 1 (
    echo [Error] Failed to compile args.c!
    exit /b 1
)

:: Compile io.c
echo [3/9] Compiling io.c. . .
gcc -c -o ./build/io.o ./src/io.c -I./src/ -fPIC
if errorlevel 1 (
    echo [Error] Failed to compile io.c!
    exit /b 1
)

:: Compile message.c
echo [4/9] Compiling message.c. . .
gcc -c -o ./build/message.o ./src/message.c -I./src/ -fPIC
if errorlevel 1 (
    echo [Error] Failed to compile message.c!
    exit /b 1
)

:: Compile utils.c
echo [5/9] Compiling utils.c. . .
gcc -c -o ./build/utils.o ./src/utils.c -I./src/ -I./include/zstd/ -fPIC
if errorlevel 1 (
    echo [Error] Failed to compile utils.c!
    exit /b 1
)

:: Compile version.c
echo [6/9] Compiling version.c. . .
gcc -c -o ./build/version.o ./src/version.c -I./src/ -fPIC
if errorlevel 1 (
    echo [Error] Failed to compile version.c!
    exit /b 1
)

:: Compile zstandard.c
echo [7/9] Compiling zstandard.c. . .
gcc -c -o ./build/zstandard.o ./src/zstandard.c -I./src/ -I./include/zstd/ -fPIC
if errorlevel 1 (
    echo [Error] Failed to compile zstandard.c!
    exit /b 1
)

:: Compile main.c
echo [8/9] Compiling main.c. . .
gcc -c -o ./build/main.o ./src/main.c -I./include/ -I./include/zstd/ -fPIC
if errorlevel 1 (
    echo [Error] Failed to compile main.c!
    exit /b 1
)

:: Compile the icon file
echo [9/9] Compiling icon file. . .
windres ./icon.rc -O coff -o ./build/icon.o
if errorlevel 1 (
    echo [Error] Failed to compile icon file!
    exit /b 1
)

:: Linking all object files directly into the executable
echo.
echo Creating AoV_Zstd.exe. . .
gcc -o AoV_Zstd.exe ./build/*.o
if errorlevel 1 (
    echo [Error] Failed to create executable!
    exit /b 1
)

:: Clean up unnecessary object files
echo Cleaning up unnecessary object files. . .
for %%f in (build\*.o) do del "%%f"

:: Success message
echo.
echo Build successful! AoV_Zstd.exe has been created.

:: Sleep for a 5 seconds to let the user see the message
timeout /t 5 /nobreak > nul

:: Run the executable for test.
AoV_Zstd.exe --decompress --dir ./tests/141_Diaochan -o ./output
