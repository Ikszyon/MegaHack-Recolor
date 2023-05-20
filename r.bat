:: this will only work for MegaHack and quickldr

@echo off

set /p GMD_PATH=<env\GMD_PATH
set /p MOD_NAME=<env\MOD_NAME
set /p MOD_LDER=<env\MOD_LDER
set /p GMD_LNCH=<env\GMD_LNCH

if "%MOD_LDER%"=="quickldr" (
    xcopy /y "%~dp0Build\Release\%MOD_NAME%.dll" "%GMD_PATH%\quickldr\%MOD_NAME%.dll"

    >nul find "%MOD_NAME%" "%GMD_PATH%\quickldr\settings.txt" || (
        echo. >> "%GMD_PATH%\quickldr\settings.txt"
        echo %MOD_NAME%.dll >> "%GMD_PATH%\quickldr\settings.txt"
    )
) else (
    xcopy /y "%~dp0Build\Release\%MOD_NAME%.dll" "%GMD_PATH%\extensions\%MOD_NAME%.dll"
)

if "%GMD_LNCH%"=="1" (
    call start steam://rungameid/322170
)

@echo on
