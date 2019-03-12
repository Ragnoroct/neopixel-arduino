## Test Without Arduino
```bash
# Compile simulator on windows
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl /EHsc main/simulator.cpp && rm simulator.obj && .\simulator.exe
```