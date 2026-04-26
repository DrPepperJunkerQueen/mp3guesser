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

The project follows an Object-Oriented Programming (OOP) approach, strictly separating data management (Core) from the graphical user interface (GUI). *Note: Each `.cpp` file has a corresponding `.h` header file.*

### ⚙️ Configuration & Entry Point
- `App.cpp / .h`: The main entry point of the application. It initializes the wxWidgets library and launches the first window.
- `Definitions.h`: A central header file containing global constants, file paths, scoring multipliers, and round time limits.

### 🧠 Data Models & Core Logic
- `Profile.cpp / .h`: Represents the player. It manages statistics (total score, highscore) and handles saving/loading profile data to and from disk.
- `Song.h`: A structure storing normalized data for a single track, including the file path, cleaned title, main artists, featured artists, and duration.
- `SongLibrary.cpp / .h`: A powerful music library manager responsible for:
  - Scanning directories and reading MP3 metadata using **TagLib**.
  - Advanced string normalization (diacritics removal, tag cleaning).
  - Extracting main and featured artists (handling "Feat." tags).
  - Managing a caching system to speed up subsequent launches significantly.

### 🖼️ User Interface (GUI - wxWidgets)
- `BackgroundFrame.cpp / .h`: A base class (inheriting from `wxFrame`) that centralizes the logic for loading and manually painting the window background from a PNG image. Most main game windows inherit from this class to ensure visual consistency.
- `ProfileFrame.cpp / .h`: The start screen for profile management and triggering manual music library rescans.
- `SelectProfileDialog.cpp / .h`: A custom dialog featuring a `wxListBox` that allows users to select an existing profile to load.
- `GameFrame.cpp / .h`: The post-login main menu ("Welcome, [Name]!"). It holds references to the user profile and the loaded song library.
- `ModeSelectFrame.cpp / .h`: A screen for choosing the game mode (e.g., "Straight Up 20" or infinite mode).
- `PlaySessionFrame.cpp / .h`: The core game engine. This complex class handles:
  - Audio playback via **SFML Audio**, starting from random offsets in the song.
  - Round timers and time limits.
  - Advanced answer validation logic (ignoring "The" prefixes, parentheses, case sensitivity, and punctuation).
  - Score calculation based on speed and identified artist types.
  - The `OverrideDialog` functionality allows players to validate their answers in case of minor typos manually.

## 📝 License

This project was created for educational purposes.
