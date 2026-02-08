# Catan Frontier Settlements

Catan: Frontier Settlements is our implementation of the famous board game. This project is built as a network multiplayer application, allowing players to connect from different devices over a network to play together in real-time.

Beyond the full core gameplay, this version has several additional features:
- Parallel Game Sessions: The server can host multiple game rooms at once, enabling different groups to play separate matches simultaneously.
- Customizable Game Rooms: The host can choose custom settings for their game: the number of players, victory point threshold and map options.
- In-Game Chat: For players to interact and negotiate during the game.
- Game History and Statistics: After the game, players can view game statistics and match information.
- Custom and Random maps: Game supports randomized boards and custom map configuration along with map extension for 6 players. 


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

- File path: resources/config.ini
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

## Team members:

 - <a href="https://gitlab.com/markocv">Marko Cvijetinović 7/2022</a>
 - <a href="https://gitlab.com/andjaa">Andjela Spasic 166/2022</a>
 - <a href="https://gitlab.com/jov580">Jovana Lazic 21/2022</a>
 - <a href="https://gitlab.com/LazarRajcic">Lazar Rajcic 50/2022</a>
 - <a href="https://gitlab.com/MatijaRadulovic">Matija Radulovic 5/2022</a>