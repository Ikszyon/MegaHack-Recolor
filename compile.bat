@echo off

cls

set /p MOD_NAME=<env\MOD_NAME
set /p VER_MSVC=<env\VER_MSVC

if not exist Build\ (
    cmake -G "%VER_MSVC%" -B build -DCMAKE_BUILD_TYPE=Release -T host=x86 -A win32
)

timeout /T 1 /nobreak >> NUL

cmake --build Build --config Release --target "%MOD_NAME%"

@echo on