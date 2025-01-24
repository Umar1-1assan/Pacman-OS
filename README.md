# Pacman Game - Operating System Principles (C)

This project implements a multi-threaded version of the classic Pacman game using **Operating System (OS) principles**. The game makes extensive use of **threads** and **semaphores** for parallel processing, optimizing the game for **multi-core systems**. The game introduces several advanced features such as **ghosts** that behave independently, **big pills** for eating ghosts, and a **key-lock mechanism** that controls when each ghost appears, providing a challenging and dynamic gameplay experience.

## Features:
- **Parallel Processing with Threads**: The game utilizes threads to handle multiple components of the game simultaneously. Each ghost, the player, and other game events are managed by separate threads, ensuring smooth, responsive gameplay.
- **Semaphores for Synchronization**: Semaphores are used to synchronize the movement and actions of the different threads. For example, they are used to ensure that the ghosts don’t collide with each other or with the player, and that the game state is updated correctly across multiple threads.
- **Optimized for Multi-Core Systems**: The game code is optimized to take advantage of multi-core processors, with each core handling different tasks like movement, collision detection, and event handling, ensuring better performance and responsiveness.
- **Ghosts**: Multiple ghosts are managed as separate threads, each with its own movement pattern. These threads are synchronized with semaphores to avoid race conditions and ensure the game runs smoothly.
- **Big Pills**: The player can collect big pills that allow them to eat ghosts, temporarily turning them into edible targets. The game manages the interaction between the player and the ghosts using semaphore signals to indicate when a ghost becomes vulnerable.
- **Key Lock Mechanism**: A key-lock feature is implemented so that each ghost only appears at different times based on whether the player has collected a key. This introduces an additional layer of strategy, as players must time their movements carefully to avoid or capture ghosts.

## Operating System Principles:
- **Threads and Parallelism**: The game uses threads to handle the different entities (player, ghosts, pills) and game events in parallel. This reflects real-world OS principles of multitasking and parallel processing, where different processes run concurrently to improve performance.
- **Semaphores and Synchronization**: Semaphores are used to control access to shared resources, such as the game state and entity positions. They help synchronize the threads, preventing race conditions and ensuring proper coordination between the player and ghosts.
- **Multi-Core Optimization**: The game is designed to run efficiently on multi-core processors by distributing tasks across cores. For example, one core can handle ghost movement, another core can handle player input and movement, and a third core can manage game updates and rendering.
  
## Gameplay:
- **Player Movement**: The player moves around the grid, collecting pellets and avoiding ghosts. The player can also collect big pills, which turn ghosts into vulnerable targets. The player and ghosts are each controlled by their own threads, allowing for independent movement and actions.
- **Ghost Behavior**: Each ghost has its own movement pattern, controlled by a separate thread. The ghosts avoid or chase the player depending on their AI behavior. The key-lock mechanism ensures that ghosts appear at different times based on whether the player has the key.
- **Collision Detection**: Collision detection between the player and ghosts, as well as between the player and pills, is handled through synchronized threads. The player’s ability to eat ghosts is managed by semaphores, which indicate when a ghost is vulnerable after a big pill is collected.
  
## Technologies Used:
- **C Programming**: The game is written in C, making use of standard libraries such as pthreads for thread management and synchronization.
- **Threads (pthreads)**: To manage the parallel execution of multiple game components.
- **Semaphores**: For synchronizing actions between threads and preventing race conditions.
- **Multi-Core Optimization**: Leveraging multi-core processors for better performance.

## Learning Outcomes:
- **Understanding OS Concepts**: By using threads and semaphores, this project provides a practical understanding of concurrency, synchronization, and parallelism in operating systems.
- **Performance Optimization**: Optimizing the game for multi-core systems gives insights into how to write efficient, high-performance code that takes full advantage of modern hardware.
- **Game Design in C**: This project showcases how to design and implement a game in C, dealing with low-level memory management, process synchronization, and real-time event handling.

## Getting Started:
1. Clone the repository.
2. Compile the C code using a C compiler that supports pthreads.
3. Run the executable to start playing the game.
