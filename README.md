# Catan Frontier Settlements

Catan: Frontier Settlements je naša implementacija poznate društvene igre Katan sa multiplayer opcijom i dodacima.

## Instalacija

Za build-ovanje i pokretanje projekta potrebno je sledeće:
- C++ komapjler, za verziju bar C++17
- Qt 6 SDK, preporučena je verzija bar 6.2
- CMake, verzija bar 3.16
- [nlohmann](https://github.com/nlohmann/json)
- [protobuf](https://protobuf.dev/)
- [Catch2](https://github.com/catchorg/Catch2)

Dodatne biblioteke(nlohmann, protobuf i catch2) je moguće instalirati korišćenjem komande:

```
vcpkg install json nlohmann-json protobuf catch2
```

Napomena: catch2 biblioteka će biti instalirana pokretanjem cmake fajla

Proces build-ovanja:

- klonirati repozitorijum:
```
git clone https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2025-2026/catan-frontier-settlements
cd catan-frontier-settlements
```
- kreirati build direktorijum i pokrenuti cmake:
```
mkdir build && cd build
cmake ..
make
```

Članovi:
 - <a href="https://gitlab.com/markocv">Marko Cvijetinović 7/2022</a>
 - <a href="https://gitlab.com/andjaa">Andjela Spasic 166/2022</a>
 - <a href="https://gitlab.com/jov580">Jovana Lazic 21/2022</a>
 - <a href="https://gitlab.com/LazarRajcic">Lazar Rajcic 50/2022</a>
 - <a href="https://gitlab.com/MatijaRadulovic">Matija Radulovic 5/2022</a>