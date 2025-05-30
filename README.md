# Email: yuvali532@gmail.com
# COUP Game

## Overview

Coup is a strategic role-based bluffing game where players compete to be the last one standing. Each player is secretly assigned a unique role with special abilities and can perform actions like gathering coins, launching a coup, bribing others, or arresting opponents. Some roles can block or interfere with these actions, leading to dynamic and tactical interactions.

This project implements:

* A modular object-oriented game engine in C++
* Full support for 6 unique roles, each with its own logic
* A graphical user interface (GUI) built using SFML
* Doctest-based test suite for validating core game mechanics
* A Makefile for streamlined compilation and testing


## 🎮 Game Instructions & Flow

1. Each player is assigned a hidden role with special powers.
2. On each turn, a player may:
   - **Gather**: Take 1 coin.
   - **Tax**: All players take 2 coins, but the Governor takes 3 (can be blocked by Governor).
   - **Bribe**: Pay 4 coins to gain 2 extra turns (can be blocked by Judge).
   - **Sanction**: Pay 3 coins to weaken another player (extra cost if Judge is targeted).
   - **Coup**: Pay 7 coins to eliminate a player (can be blocked by General).
   - **Arrest**: Neutralize a player temporarily.
   - **Spy**: View another player's coins and block their arrest ability.
3. Certain roles can **block** or modify these actions.
4. Game ends when only one player remains active.


## 🧩 Roles

Each of these inherits from `Player` and overrides specific methods to define role-specific actions:

- **Governor**: Can block other players' Tax actions, and when using Tax themselves, gains 3 coins instead of 2.
- **Baron**: Can invest 3 coins to receive 6 in return. If targeted with a sanction, receives 1 coin as compensation.
- **General**: Can block Coup actions. If the General is arrested, neither their coins nor the arresting player’s coins are affected.
- **Merchant**: If starting a turn with at least 3 coins, gains 1 extra coin. When arrested, pays 2 coins (instead of giving 1 to the attacker).
- **Judge**: Can block Bribe actions. If sanctioned, the attacker must pay an additional coin as a penalty.
- **Spy**: Views other players' coin counts and prevents them from arresting next turn.


## 🪟 GUI Features

- Welcome screen
- Player count and name input with validation
- Visual role indication and in-game status
- Turn-based action buttons with visual feedback
- Popups for target selection and error messages


## 📁 Project Structure

- `main.cpp` – Entry point to launch the game via GUI
- `Game.cpp` / `Game.hpp` – Core logic: turn handling, player management, and game rules
- `Player.cpp` / `Player.hpp` – Base class for all roles, handles coin balance, status, and shared actions
- `Roles/` – Directory with all role-specific classes (Spy, Judge, General, Baron, Governor, Merchant)
- `GameGUI.cpp` – SFML-based GUI rendering and user interaction
- `test.cpp` – Doctest-based test suite
- `Makefile` – Automation for build, test, Valgrind, and GUI launch


### ✅ Unit Tests

Using Doctest framework, we test the core game engine, turn mechanics, player state transitions, and unique behaviors of each role.


## Valgrind

Valgrind is used to detect memory leaks and invalid memory use.
By default, `make valgrind` runs the GUI version of the game (`main_exec`).  
You can change the target manually in the Makefile to run memory checks on the test suite instead.

### 🔍 Results

- **No memory leaks were detected in the game logic**.
- Valgrind reports **small memory leaks** when running the GUI version due to internal  
  allocations from **SFML** and system libraries (e.g., `libgallium`, `libGLX_mesa`).  
  These are **not caused by the code** and are safe to ignore.


## External Libraries

### C++ Standard:

* `<iostream>`, `<vector>`, `<string>`, `<stdexcept>` — core logic and data handling

### SFML:

* `sfml-graphics` — GUI drawing
* `sfml-window` — input and window events
* `sfml-system` — base utilities

---

## Makefile Targets

| Target          | Purpose                                   |
| --------------- | ----------------------------------------- |
| `make Main`     | Compile and run the game via GUI          |
| `make test`     | Compile and run unit tests                |
| `make valgrind` | Run memory checks with Valgrind           |
| `make clean`    | Remove compiled binaries and object files |

---

## How to Run

1. Ensure dependencies (`clang++`, `SFML`, `valgrind`) are installed.
2. In the project root directory:

### 🔹 Run the GUI Game:

```bash
make Main
./Main
```

### 🔹 Run Tests:

```bash
make test
```

### 🔹 Check Memory:

```bash
make valgrind
```

### 🔹 Clean Build:

```bash
make clean
```

---

Enjoy the game and may the best strategist win! 🏆
