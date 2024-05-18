# Snake Game

This is a classic Snake game built using the Raylib library in C. The goal of the game is to navigate the snake to eat food, grow in length, and avoid colliding with itself. This README provides instructions on setting up, building, and running the game, as well as an overview of the game mechanics.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Controls](#controls)
- [Gameplay](#gameplay)
- [Credits](#credits)
- [License](#license)

## Features

- Classic Snake gameplay.
- Simple and intuitive controls.
- Score tracking.
- Dynamic snake growth.
- Game over detection.

## Prerequisites

To compile and run this game, you'll need:

- A C compiler (GCC recommended).
- Raylib library installed on your system.

## Installation

### Installing Raylib

Follow the official Raylib installation guide for your operating system: [Raylib Installation](https://github.com/raysan5/raylib?tab=readme-ov-file#build-and-installation).

### Cloning the Repository

Clone this repository to your local machine:

```bash
git clone https://github.com/Mahmoud-Khaled_FS/snake_c.git
cd snake_c
```

## Usage

### Building the Game

To build the game, run the following command in your terminal:

```bash
make build
```

### Running the Game

After building the game, run the executable:

```bash
./snake
```

## Controls

- Arrow Keys (w,a,s,d): Move the snake in the desired direction (Up, Down, Left, Right).
- Space: Pause the game or start new one.

## Gameplay

- The snake starts at a fixed position on the screen and moves in the direction of the arrow keys pressed by the player.
- The objective is to eat the food that appears randomly on the screen.
- Each time the snake eats food, it grows in length, and the player's score increases.
- The game ends if the snake collides with the walls or itself.

## Credits

- Developed by Mahmoud Khaled.
- Built using the Raylib library.

## License

This project is licensed under the MIT [License](https://opensource.org/license/mit) - see the LICENSE file for details.

---

Feel free to contribute to this project by opening issues and submitting pull requests. Enjoy playing the Snake game!
