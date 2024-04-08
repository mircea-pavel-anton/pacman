# Building Locally

## On Windows
1. Just open this project folder in Visual Studio 2019 (or newer), with 
CMake plugin installed.
2. From top toolbar, change mode to **Release**, and click **Build** > **Build All**.
3. Done. That's it. Your .EXE will be in `{project_dir}/out/Release/bin`
4. If the game fails to start, make sure `openal32.dll` is present right beside your executable. If not, copy it from SFML directory `${SFML_HOME}/bin/openal32.dll`
 
## On MacOS & Linux
1. Open this project directory in your terminal.
2. Ensure CMake 3.20 or higher is installed. Run `cmake --version`
3. Once installed, run the following commands:

```bash
mkdir build
cd build
cmake . . -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --target all 

```
4. Your executable (.app bundle for MacOS) and binary for Linux will be found in `{project_dir}/build/bin`

## Using CMake GUI (all destkop platforms)

- Download CMake GUI installer for your OS from the official site. (supported on all desktop OS)
- On MacOS, to add Cmake CLI to path, run the following command:
  ```bash
   sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
  ```
- This is how the home screen of CMake GUI looks like:
![cmake_screenshot](apple_cmake/cmake_gui.png)

- Set "Source Folder" to this project root. Then set "Build Folder" to a NEW _relative_ folder `/build` or `/out`. See image above. Then click "**Configure**", 
- In the next screen, Choose **Unix Makefiles** on Linux & Mac. Choose **Visual Studio** on Windows. Then click OK. Click Configure once more, then **Generate**.
- Finally, open build folder, then run `make all` in Terminal (Linux and macOS). On Windows, double-click the generated VS project `.sln` file.
