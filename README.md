# Catan Frontier Settlements

Multiplayer implementation of the classic Catan board game built in **C++ and Qt**, featuring a **client–server architecture**, real‑time gameplay synchronization, customizable game sessions, in‑game chat, and match statistics.

![Gameplay](Catan/gui/qt/assets/gameplay.png)

**Tech focus:**  
C++17 • Qt6 • Client–Server Architecture • Network Programming • Protobuf • CMake • State Machine Architecture • Design Patterns

## Features

In addition to the full core gameplay, this version includes several additional features:

- **Parallel Game Sessions**  
  The server supports multiple game rooms simultaneously, allowing different groups to play separate matches.
- **Customizable Game Rooms**  
  The host can configure game parameters such as number of players, victory point threshold, and map options.
- **In‑Game Chat**  
  Players can communicate and negotiate during gameplay.
- **Game History and Statistics**  
  Match statistics and history are recorded and available for review after each game.
- **Custom and Random Maps**  
  Supports both randomized standard/extended maps and fully custom board configurations.
- **ASCII Map Prototype**  
  Used during early development phases for testing core game mechanics.

## Installation

The following is required to build and run the project:

- C++ compiler, version at least C++17
- Qt 6 SDK, recommended version at least 6.2
- CMake, version at least 3.16
- [nlohmann](https://github.com/nlohmann/json)
- [protobuf](https://protobuf.dev/)
- [Catch2](https://github.com/catchorg/Catch2)

Additional libraries (nlohmann, protobuf, and catch2) can be installed using the command:

```
vcpkg install json nlohmann-json protobuf catch2
```

Note: The Catch2 library will be installed by running the cmake file.

## Client configuration

Before running the application, you need to configure the client to connect to the server:

- File path: System local app storage location resources/config.ini
- Settings: Update the server_ip in this file to the address of the machine running the server.

## Build process

- clone the repository:
```
git clone https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2025-2026/catan-frontier-settlements
cd catan-frontier-settlements
```
- create build directory and run cmake:
```
mkdir build && cd build
cmake ..
make
```

## Demo:
[demo video](https://drive.google.com/file/d/1Pya67nALP-7X76x5xOPm5f9Pppxsmphd/view?usp=sharing)

## Contributions

This project was developed as part of a university team assignment.  
The following describes ***my primary personal contributions to the project***.

My work focused on the **core gameplay engine**, responsible for controlling the rules, progression, and state of the game.
I authored the **GameSession**, **GameController**, and **Move system**, which together implement gameplay logic.

### Primary contributions:

- Designed and implemented the **GameSession system**, managing the full lifecycle of a match.
- Designed and implemented the **game state machine**, handling turn order, game phases, and valid state transitions.
- Implemented the **core GameController**, coordinating gameplay logic and interaction with the GUI.
- Implemented the **complete move system**, defining all player actions and their effects on the game state.
- Implemented **global gameplay rules and move validation logic**, later used to drive **GUI move highlighting and visual feedback (e.g., board shake) during building actions**.

### Architecture and design patterns used:

- **State Machine** – controlling game phases, turn progression, and enabling/disabling available actions in the GUI.
- **Command Pattern** – representing player actions as game moves.
- **Observer Pattern (Qt signals/slots)** – synchronizing gameplay state changes with the GUI layer.
  
This architecture allowed the **GameSession and controller to drive the GUI state**, ensuring that actions are only available when valid within the current game phase.

### Additional contributions:

- Implemented most **unit tests using Catch2**.
- Implemented the **AudioManager** and integrated gameplay audio.
- Implemented several **Qt GUI elements** (houses, cities, roads, robber).
- Performed **GUI refactoring and styling improvements** across the application.
- Implemented the **game configuration system**.
- Maintained and populated **GameHistory match data tracking**.
- Organized **code refactoring and project structure improvements**.
- Set up **Git hooks and development workflow**.
- Created **architecture sketches** used by the team and worked on **initial UML class diagram**.
- Helped coordinate development tasks and made issues for others.

### Collaboration

I collaborated on the **design of move synchronization and GUI–model communication flow**, ensuring proper integration between the gameplay engine, session management, move processing system and the networking layer which was primarily implemented by @MatijaRadulovic, finalizing real‑time multiplayer communication between clients and the server.

## Team members:

 - <a href="https://gitlab.com/markocv">Marko Cvijetinović 7/2022</a>
 - <a href="https://gitlab.com/andjaa">Andjela Spasic 166/2022</a>
 - <a href="https://gitlab.com/jov580">Jovana Lazic 21/2022</a>
 - <a href="https://gitlab.com/LazarRajcic">Lazar Rajcic 50/2022</a>
 - <a href="https://gitlab.com/MatijaRadulovic">Matija Radulovic 5/2022</a>
