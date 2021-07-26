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
	./AntSimulator <steps> <iterations> <mal_fraction> <mal_timer> <mal_focus> <tracing_pattern> <counter_pheromone> <fake_intensity> <display_GUI> [mode] [dump]
<> means required arguments

for boolean variables, T is for true. Other key is for false

for tracing_pattern, R is for random. Other key is for food.

The mode argument is not a required argument. Default one will make .csv output have the ratio of good ants that got food and going back and all the ants. Please see **current metric** section for more detail.

#	Example
	./AntSimulator 10000 3 0.05 100 F F F 1 F 1 0 0
This will run 3 experiments in headless mode for 10000 timesteps each where the experiment is as followed:
			
    - there are 5% of malicious ants within the colony
	- it will launch the attack at timestep 100
	- the malicious ants will not attack towards the food
	- the malicious ants will follow food pheromone.
	- the counter pheromone will not released.
	- the fake pheromone will be 1 times stronger than usual (aka. normal)
    - the .csv result will be # of ants got food and coming back/# of good ants 
    - the result will NOT print out to the stdout.

#   Current Metric

The proportion of benevolent ants bringing the food back to the home divided by all ants (include malicious). If we want to have it divide by only all benevolant, please add `1` at the end of command. `2` and `3` are similar to `0` and `1` in terms of the divisor but the denominator for `2` and `3` is cumulative food count. To sum up,

   - `0` : # of benevolent ants bringing food back to home/# of all ants [Default]
   - `1` : # of benevolent ants bringing food back to home/# of benevolent ants
   - `2` : # of food at home/# of all ants (cumulative food count/# of all ants)
   - `3` : # of food at home/# of benevolent ants (cumulative food count/# of benevolent ants)

# Automatic Script for gathering experiment data

There is a automatic script for running a set of experiments (like change in ratio of malicious ants as a linear increment function or such) in `data_gathering.py`. Inside you can edit the code to suit your work but the basic idea behind it is to use simple string manipulation and python subprocess to run the script. Ideally, bash script would work too but to export the result into numpy matrix (`.npy` file) for further plotting so python is required.

# Known Issue
   - [X] Cumulative metric is not strictly increasing. [19 July 2021]
   - [ ] The automatic script does not work. [19 July 2021]
   		- [X] A quick fix for now: add `dump` function to print out the csv file and redirect `stdout` to a new buffer file for python to read. It is inefficient but it works as a quick fix for now. To use that feature (or test), please change `dump` argument to `1`. [26 July 2021]

   

