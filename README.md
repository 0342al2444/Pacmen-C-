# Pacmen

Pacmen is a 2-player co-op, Pac-Man inspired game built in C++20 with raylib.

## Project overview

- Pacmen: 2-player co-op Pac-Man inspired game in C++20 + raylib
- Controls: P1 WASD, P2 Arrow Keys
- Goal: collect all pellets, avoid ghosts, win screen when pellets are gone
- Menu with Start button, scoreboard panel

## Requirements (Windows)

- Visual Studio with Desktop development with C++
- CMake
- vcpkg (manifest mode)
- Optional: Ninja (if you support it)

## Quick start (copy-paste commands)

```bat
cd C:\dev
git clone https://github.com/0342al2444/Pacmen-C-.git
cd Pacmen-C-
setx VCPKG_ROOT "C:\dev\vcpkg"
cmake -S . -B build -G "Visual Studio 18 2026" -A x64 -DCMAKE_TOOLCHAIN_FILE="C:\dev\vcpkg\scripts\buildsystems\vcpkg.cmake"
cmake --build build --config Debug
.\build\Debug\pacmen.exe
```

If missing DLLs, copy vcpkg debug DLLs next to the exe:

```bat
xcopy /y ".\build\vcpkg_installed\x64-windows\debug\bin\*.dll" ".\build\Debug\"
```

## Repository structure

- `src/core`, `src/game`, `src/render`, `src/entities`, `src/systems`
- `assets/maps/level1.txt`

## Development notes

- Close `pacmen.exe` before rebuilding (Windows locks the exe)
- Map symbols: `#` wall, `.` pellet, `P/Q` spawns, `G` ghost spawn
