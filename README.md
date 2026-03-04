> **Note:** The cross-platform build pipeline and Doxygen documentation setup described below were generated and co-authored with Gemini (Google AI) to ensure a seamless workflow for both Windows and Linux developers.

## 🛠️ Build Prerequisites

This project uses **CMake** for the build system and **Doxygen** to generate code documentation.

The CMake configuration is completely cross-platform. It is designed to automatically find your local Doxygen installation, regardless of your OS.

### For Linux Users (Ubuntu/Debian)
You will need to install Doxygen globally on your system. Run the following command in your terminal:

`sudo apt update && sudo apt install doxygen`

### For Windows Users
To keep your system PATH clean, this project expects a portable version of Doxygen to be placed directly inside the project directory.

1. Download the latest Windows binary `.zip` file from the [official Doxygen download page](https://www.doxygen.nl/download.html).
2. Create a folder named `bin` in the root directory of this project.
3. Extract **only** the `doxygen.exe` and `libclang.dll` files from the downloaded `.zip` and place them into the `bin/` folder.
   *(Note: These binary files are ignored by Git, so they will not be committed to the repository).*

---

## 🚀 How to Build

### 1. Build the Main Project
Once your dependencies are in place, you can configure and build the C++ project. From the root of the project, run:

`cmake -B build`
`cmake --build build`

### 2. Generate the Documentation
To keep daily compile times fast, the documentation does not generate automatically on every build. When you have updated the code comments and want to generate fresh HTML docs, run this specific target:

`cmake --build build --target generate_docs`

*(If you are using CLion, you can simply select `generate_docs` from the run configuration dropdown at the top right and click the Build hammer).*

*Note: The HTML documentation will generate inside the `docs/` folder. You can open `docs/html/index.html` in your browser to view it. The `docs/` folder is ignored by Git, so everyone should generate their own local copy.*