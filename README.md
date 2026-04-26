# mp3guesser

A music guessing game ("Name That Tune" style) built in C++ using modern multimedia libraries and a graphical user interface.

## 🎵 About the Project

**mp3guesser** is a desktop application that allows users to test their music library knowledge. The program plays snippets of MP3 tracks, and the player's goal is to correctly guess the title or the artist.

### Key Features:
- **Profile System:** Create and select user profiles.
- **Statistics:** Track high scores and total points earned.
- **Metadata Support:** Uses TagLib for accurate reading of track information.
- **High-Quality Sound:** Audio engine based on the SFML library.
- **Native Interface:** GUI built with wxWidgets.

## 🛠️ Technologies

- **Language:** C++17 (or newer)
- **GUI:** [wxWidgets](https://www.wxwidgets.org/)
- **Audio:** [SFML](https://www.sfml-dev.org/)
- **Metadata:** [TagLib](https://taglib.org/)
- **Package Management:** [vcpkg](https://vcpkg.io/) (Manifest Mode)

## 🚀 Installation and Compilation

This project uses `vcpkg` in manifest mode, which means all dependencies are automatically downloaded and compiled when the project is first opened in Visual Studio.

### Requirements:
- Windows 10/11
- Visual Studio 2022/2026 (with "Desktop development with C++" workload installed)
- `vcpkg` installed on your system

### Steps:
1. Clone the repository:
   ```bash
   git clone [https://github.com/DrPepperJunkerQueen/mp3guesser.git](https://github.com/DrPepperJunkerQueen/mp3guesser.git)
   cd mp3guesser
   ```
2. Ensure the `vcpkg.json` file is in the project's root directory.
3. Open the `mp3guesser.sln` file in Visual Studio.
4. Visual Studio should automatically detect the manifest file and begin downloading libraries (this may take several minutes during the first run).
5. Ensure the project settings are set to **C++17** or **C++20** standard and the SubSystem is set to **Windows (/SUBSYSTEM:WINDOWS)**.
6. Build the project (F7) and run it.

## 📁 File Structure

- `App.h / App.cpp`: Application entry point and wxWidgets initialization.
- `ProfileFrame.h / ProfileFrame.cpp`: Main window for profile selection and creation.
- `Profile.h / Profile.cpp`: Logic for managing user data (scores, name).
- `vcpkg.json`: Project dependency list.

## 📝 License

This project was created for educational purposes.
