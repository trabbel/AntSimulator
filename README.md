# AntSimulator

Very simple ants simulator.

# Installation

## Prerequisites

In order to compile this project you will need to:
 - have [SFML](https://www.sfml-dev.org/index.php) installed on your system. If you don't know how to do it see [this link](https://www.sfml-dev.org/tutorials/2.5/#getting-started).
 - have [CMake](https://cmake.org/) installed

## Compilation

Detailed explanation [here](https://preshing.com/20170511/how-to-build-a-cmake-based-project/)

### On Linux with `install.sh`
- Go in the repo folder

`cd the/repo/location`

- Execute `install.sh` script

`./install.sh`

### On Windows with CMake GUI and Visual Studio
 - Install the right SFML version or compile it (see [this](https://www.sfml-dev.org/tutorials/2.5/start-vc.php))
 - Run CMake
 - Select the repo location
 
![Cmake 1](https://github.com/johnBuffer/AntSimulator/blob/master/img/cmake_1.PNG)
 - Click on `Configure`, if you have installed the `x64` version of SFML, in the pop up window select `x64` in the `Optionnal platform for generator` drop down

![Cmake 2](https://github.com/johnBuffer/AntSimulator/blob/master/img/cmake_2.PNG)
 - Click on `Finish`
 - Click on `Generate`

![Cmake 3](https://github.com/johnBuffer/AntSimulator/blob/master/img/cmake_3.PNG)
 - You can now open the generated project and build it.

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

# 	Headless mode usage
	./AntSimulator <steps> <iterations> <mal_fraction> <mal_timer> <mal_focus> <tracing_pattern> <counter_pheromone> <fake_intensity> <display_GUI>
<> means required arguments

for boolean variables, T is for true. Other key is for false

for tracing_pattern, R is for random. Other key is for food.

#	Example
	./AntSimulator 10000 3 0.05 100 F F F 1 F
This will run 3 experiments in headless mode for 10000 timesteps each where the experiment is as followed:
			
    - there are 5% of malicious ants within the colony
	- it will launch the attack at timestep 100
	- the malicious ants will not attack towards the food
	- the malicious ants will follow food pheromone.
	- the counter pheromone will not released.
	- the fake pheromone will be 1 times stronger than usual (aka. normal)