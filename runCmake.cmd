rem Configure
cmake -S . -B build

rem Build the binaries
cmake --build build --config Release
