# LCOM

In this repository there is available every LCOM lab solved and the final project of the course (grade: 17/20) that will be the focus of this README.

# SpaceCOM Shooter

This project was developed for the **Laboratório de Computadores** (Computer Laboratory) 2023-2024 course at **FEUP** on the **MINIX** Operating System. It is a original space shooter game, which the goal is to destroy the maximum number of incoming asteroids and try to not collide with them. The game has a variety of features to make it more difficult, like the speed of asteroids incresing during the game, more fun, like power that be activated to help the player, etc. For more details checkout out our [official report](https://github.com/GuilhermeRego/LCOM/blob/main/proj/doc/lcom_proj.pdf).

![menu_start_selected](https://github.com/user-attachments/assets/d6e6efbe-3dbe-43bb-94be-6a85b97131b3)

## Architecture

The architecture of the project is modular, with each module handling a specific aspect of the game, using the **MVC** model. The main components are:

- **Game Logic**: Managed in the `game/` directory, this includes the main game loop, state transitions, and game mechanics.
- **Game States**: Each game state (e.g., menu, game, settings) is managed by its own controller in the `game/gameStates/` directory.
- **Sprites**: Managed in the `sprites/` directory, this includes loading, drawing, and animating sprites.
- **Utilities**: General utility functions and hardware interaction code are located in the `utils/` directory. This includes keyboard and mouse input, real-time clock (RTC), timer, and video functions. These are the functions that were developed throughout the whole semester during classes.
- **Images**: All XPM image files used in the game are stored in the `imgs/` directory.

## Features

- **Multiple Game States**: The game includes various states such as menu, game, settings, instructions, game over, leaderboard, and pause.
- **Sprite Management**: Efficient loading, drawing, and animating of sprites.
- **Real-Time Clock Display**: Toggle the display of the real-time clock in the menu.
- **Keyboard and Mouse Input**: Responsive controls for navigating menus and playing the game.
- **High Scores**: Functional to display high scores.
- **Pause Functionality**: Ability to pause the game and resume later.
- **Settings Menu**: Customize game settings to enhance the gaming experience.
- **Game Logic**: The whole game logic and mechanisms using the hardware interaction, including collision detection, firing lasers upon mouse click, change cannon's direction, etc.
- **Dynamic Difficulty**: As the player increases his score, the Difficulty Controller makes the game harder, with more asteroids spawning and them getting faster.
- **Powerups**: To make the game more fun, we added power-ups for the player to catch with the mouse during the game. There are two powerups: the bomb, when the player clicks on the bomb, every asteroid on the screen is exploded, and the coin, that adds 5 to the current score.
- **Instructions Menu**: There is an intuitive menu for instructions on how to play the game.

## Prints of the Game

### Gameplay:

![image](https://github.com/user-attachments/assets/aa903357-bb4a-43e9-bded-035ffde2d036)

### Pause:

![image](https://github.com/user-attachments/assets/52384f8f-3690-4cd4-a4a6-f94d3fab54c3)

### Instructions Menu:

![image](https://github.com/user-attachments/assets/92913f7b-e1b8-493f-9b27-0b846e106ab5)

### Settings Menu:

![image](https://github.com/user-attachments/assets/2f2ddaa0-442e-49f5-93fc-d9ab7543f937)

## Authors

- Diogo Ramos (up202207954)
- Gabriel Braga (up202207784)
- Guilherme Rego (up202207041)
