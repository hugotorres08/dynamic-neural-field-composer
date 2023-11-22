@echo off

set PROJECT_ROOT=%CD%

:: Install x64-release configuration
cmake --build "%PROJECT_ROOT%\build\x64-release" --config Release --target install

:: Install x64-debug configuration
cmake --build "%PROJECT_ROOT%\build\x64-debug" --config Debug --target install

pause
exit /b 0