# Catan Frontier Settlements

**Catan: Frontier Settlements** is our implementation of the famous board game. The project is built as a networked multiplayer application, allowing players to connect from different devices and play together in real time.

![Gameplay](Catan/gui/qt/assets/gameplay.png)

In addition to the full core gameplay, this version includes several extra features:
- **Parallel Game Sessions:** The server supports multiple game rooms at once, allowing different groups to play separate matches simultaneously.
- **Customizable Game Rooms:** The host can configure settings such as the number of players, victory point threshold, and map options.
- **In-Game Chat:** Players can communicate and negotiate during the match.
- **Game History and Statistics:** After each game, players can review match data and statistics. 
- **Custom and Random maps:** The game supports randomized standard and extended boards from the original game, along with any custom map configuration.
- **Ascii map:** Made in the initial phase of the project for testing.

---

## Installation

The following is required to build and run the project:

- C++ compiler with C++17 support
- Qt 6 SDK, recommended version 6.2 or newer
- CMake 3.16 or newer

### Optional

- Conan 2, if building with Conan
- Protobuf, if building with plain CMake
- Catch2, if building tests with plain CMake

Additional libraries (nlohmann, protobuf, and catch2) can be installed using the command:

```bash
vcpkg install json nlohmann-json protobuf catch2
```

Note: The Catch2 library will be installed by running the cmake file.

---

## Client configuration

Before running the application, configure the client to connect to the server:

- File path: local app storage location `resources/config.ini`
- Setting to update: `server_ip`
- Set it to the address of the machine running the server

---

## Build process

- clone the repository:
```bash
git clone https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2025-2026/catan-frontier-settlements
cd catan-frontier-settlements
```
### Plain CMake build

If you build without Conan, make sure Qt and Protobuf are installed locally and available to CMake.

You may need to point CMake to your Qt installation:

```bash
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64
cmake --build build
```

---

### Conan build

If you build with Conan, make sure Conan is installed and provide your local Qt path.

**Option 1: environment variable**

```bash
export QT_DIR=/path/to/Qt/6.x/gcc_64
```

**Option 2: Conan profile/config**

Add this to your local Conan profile:

```ini
[conf]
user.qt:path=/path/to/Qt/6.x/gcc_64
```

Then run:

```bash
conan install . --build=missing
conan build .
```

### Optional: create a Conan package

If you want to package the project into your local Conan cache:

```bash
export QT_DIR=/path/to/Qt/6.x/gcc_64 # if not yet done
conan create . --build=missing
```

## Demo:
[Demo video](https://drive.google.com/file/d/1Pya67nALP-7X76x5xOPm5f9Pppxsmphd/view?usp=sharing)

## Team members:

 - <a href="https://gitlab.com/markocv">Marko Cvijetinović 7/2022</a>
 - <a href="https://gitlab.com/andjaa">Andjela Spasic 166/2022</a>
 - <a href="https://gitlab.com/jov580">Jovana Lazic 21/2022</a>
 - <a href="https://gitlab.com/LazarRajcic">Lazar Rajcic 50/2022</a>
 - <a href="https://gitlab.com/MatijaRadulovic">Matija Radulovic 5/2022</a>