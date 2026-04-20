# Five or More

A simple game project developed in C using GTK2. This project demonstrates how to build a game interface with a menu, game field, and score display.

## Table of Contents

- [Description](#description)
- [Installation](#installation)
- [Usage](#usage)

## Description

**Five or More** is a game where you interact with a dynamic game field. The user interface is built using GTK2.

## Installation

1. **Clone the repository:**

   ```bash
   git clone https://gitlab.uliege.be/AleksandrPavlov/info0030_groupe25.git
   cd info0030_groupe25
   ```

2. **Build the project:**

   The project uses a Makefile. Simply run:
   
   ```bash
   make
   ```

   This will compile the project and produce the executable named `five`.

3. **Dependencies:**

   - GTK2 development libraries (`gtk+-2.0`)
   - Standard C libraries

   Install GTK2 on your system if it's not already available. For example, on Ubuntu:

   ```bash
   sudo apt-get install libgtk2.0-dev
   ```

## Usage

After building, run the game with:

```bash
./five
```
