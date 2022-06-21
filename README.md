# Ant Simulator

This repository contains the code used for the paper [Hacking the Colony: On the Disruptive Effect of Misleading
Pheromone and How to Defend Against It](https://arxiv.org/pdf/2202.01808.pdf). We extended the methods with two new ant behaviors: pheromone for food and cunning ants. Parameters can be changed in [main.cpp](https://github.com/trabbel/AntSimulator/blob/master/src/main.cpp).

# Installation

## Prerequisites

In order to compile this project you will need to:
 - have [SFML](https://www.sfml-dev.org/index.php) installed on your system. If you don't know how to do it see [this link](https://www.sfml-dev.org/tutorials/2.5/#getting-started).
 - have [CMake](https://cmake.org/) installed

Please note that you might need reasonably modern computer in order to run this simulator at the intended framerates.


## Compilation

Detailed explanation [here](https://preshing.com/20170511/how-to-build-a-cmake-based-project/)

### On Linux with `install.sh`
- Go in the repo folder

`cd the/repo/location`

- Execute `install.sh` script

`./install.sh`

- Run the simulation

`./build/AntSimulator`

# Commands

|Command|Action|
|---|---|
|**P**|Pause/Unpause the simulation|
|**M**|Toggle markers drawing|
|**A**|Toggle ants drawing|
|**S**|Toggle max speed mode|
|**W**|Toggle Wall mode|
|**E**|Toggle Wall erase mode|
|**Right click**|Add food|
|**Left click**|Move view|
|**Wheel**|Zoom|
