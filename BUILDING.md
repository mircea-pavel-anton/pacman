# Building Locally

## On Windows
1. Just open this project folder in Visual Studio 2019 (or newer), with 
CMake Extension installed as part of "Desktop C++ Dev Workload"
2. From top toolbar, change mode to **Release**, and click **Build** > **Build All**.
3. Done. That's it. Your .EXE will be in `{project_dir}/out/Release/bin`
 
## On MacOS & Linux
1. Open this project directory in your terminal.
2. Ensure CMake 3.20 or higher is installed. Run `cmake --version`
3. If installed, run the following command:

```bash
mkdir build
cd build
cmake . . -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --target all 
```
4. Your executable (.app bundle for MacOS) and binary for Linux will be found in `{project_dir}/build/bin`
