#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <fstream>
#include "colony.hpp"
#include "config.hpp"
#include "display_manager.hpp"

#include <cstdio>		// for printf()		
#include <stdio.h>      // for sprintf()
#include <cstdlib>		// for parsing string to int
#include <string.h>		// for strcmp
#include <iostream>     // for console output
#include <string>       // for std::string
#include <iomanip>		// for 3 decimal precision

/****************************************************************************************
************************ CHANGE THESE PARAMETERS FOR TRIALS ************************
****************************************************************************************/
/*
* @param DISPLAY_GUI:: Do you want GUI? If set to true, you can see the simulation
* @param SIMULATION_STEPS:: Number of steps of simulation (Will not be in effect for GUI)
* @param SIMULATION_ITERATIONS:: Run the same configured iteration these number of times
* @param malicious_fraction:: Probability of an ant being malicious (fraction of ants being malicious)
* @param malicious_timer_wait:: Delay after which the attack is launched
* @param malicious_ants_focus::  Should the attack be focused towards food
* @param ant_tracing_pattern::   Should malicious ants trace food pheromone or roam randomly
* @param counter_pheromone:: Will the ants secret counter pheromone?
* @param hell_phermn_intensity_multiplier:: multiplier for the intensity of TO_HELL pheromone
*/

/*
	usage:
		./AntSimulator <steps> <iterations> <mal_fraction> <mal_timer> <mal_focus> <tracing_pattern> <counter_pheromone> <fake_intensity>
		<> means required arguments
		for boolean variables, T is for true. Other key is for false
		for tracing_pattern, R is for random. Other key is for food.

	Example:
		./AntSimulator 10000 3 0.05 100 F F F 1
		This will run 3 experiments for 10000 timesteps each where the experiment is as followed:
			- there are 5% of malicious ants within the colony
			- it will launch the attack at timestep 100
			- the malicious ants will not attack towards the food
			- the malicious ants will follow food pheromone.
			- the counter pheromone will not released.
			- the fake pheromone will be 1 times stronger than usual (aka. normal)

*/
bool DISPLAY_GUI = false;
/*const bool DISPLAY_GUI = false;
const int SIMULATION_STEPS = 10000;		// Only used in the data recording, NOT IN GUI
const int SIMULATION_ITERATIONS = 5;
float malicious_fraction = 0.05;
int malicious_timer_wait = 100;	
bool malicious_ants_focus = false; //directional or uniform random
AntTracingPattern ant_tracing_pattern = AntTracingPattern::FOOD; //FOOD, RANDOM, 
bool counter_pheromone = true;
float hell_phermn_intensity_multiplier = 1; //fake food pheromone strength
*/
/*void loadUserConf() //this function is not being used as we are parse parameters from terminal
{	
	std::ifstream conf_file("conf.txt");
	if (conf_file) {
		conf_file >> malicious_fraction;
		conf_file >> malicious_timer_wait;
		conf_file >> Conf::ANTS_COUNT;
	}
	else {
		//malicious_fraction = 0.25;
		//malicious_timer_wait = 100;
		std::cout << "Couldn't find 'conf.txt', loading default" << std::endl;
	}
}
*/
void initWorld(World& world, Colony& colony)
{
	for (uint32_t i(0); i < 64; ++i) {
		float angle = float(i) / 64.0f * (2.0f * PI);
		world.addMarker(colony.position + 16.0f * sf::Vector2f(cos(angle), sin(angle)), Mode::ToHome, 10.0f, true);
	}	

	sf::Image food_map;
	if (food_map.loadFromFile("map.bmp")) {
		for (uint32_t x(0); x < food_map.getSize().x; ++x) {
			for (uint32_t y(0); y < food_map.getSize().y; ++y) {
				const sf::Vector2f position = float(world.markers.cell_size) * sf::Vector2f(to<float>(x), to<float>(y));
				if (food_map.getPixel(x, y).g > 100) {
					///////////////////
					// FOOD POSITION
					world.addFoodAt(position.x/10, position.y/10, 5);
				} else if (food_map.getPixel(x, y).r > 100) {
					world.addWall(position);
				}
			}
		}
	}

}

void updateColony(World& world, Colony& colony)
{
	const static float dt = 0.016f;
	colony.update(dt, world);
	
	if(colony.timer_count2%100 == 0){
		// std::cout << "Foraged ant=" << colony.confused_count<< std::endl;
	}
	world.update(dt);
}

void simulateAnts(int SIMULATION_ITERATIONS, int SIMULATION_STEPS, float malicious_fraction, int malicious_timer_wait, bool malicious_ants_focus, AntTracingPattern ant_tracing_pattern, 
		bool counter_pheromone, float hell_phermn_intensity_multiplier, int mode)
{
	const static float dt = 0.016f;
	std::ofstream myfile;
	myfile.open ("../AntSimData.csv");
	//x axis -> ratio of malicious ants/all ants
	//y axis -> ratio of intensity of fake/everything
	//z axis -> metric (ratio of ant coming back with food) 
	/**
	 * @brief This loop will start a new colony and run the sim for SIMULATION_STEPS number of steps
	 */
	for(int i = 0; i<SIMULATION_ITERATIONS; i++)
	{
		World world(Conf::WORLD_WIDTH, Conf::WORLD_HEIGHT);
		Colony colony(Conf::COLONY_POSITION.x, Conf::COLONY_POSITION.y, Conf::ANTS_COUNT, 
		malicious_fraction, malicious_timer_wait, malicious_ants_focus, ant_tracing_pattern, 
		counter_pheromone, hell_phermn_intensity_multiplier);
		initWorld(world, colony);	
		float ratio;
		for(int j = 0; j<SIMULATION_STEPS; j++)
		{
			updateColony(world, colony);
			
			if(colony.timer_count2%10 == 0){
				switch(mode){
					case 0:
						ratio = (float)colony.confused_count/(float)Conf::ANTS_COUNT;//change this for other metric
						myfile  << std::fixed<< std::setprecision(3) << ratio << ",";
						break;
					case 1:
						ratio = (float)colony.confused_count/(float)((1-malicious_fraction)*Conf::ANTS_COUNT);//change this for other metric
						myfile  << std::fixed<< std::setprecision(3) << ratio << ",";
						break;
				}
			}
		}
		myfile  << std::endl;
	}
	myfile.close();
}

void displaySimulation(int SIMULATION_ITERATIONS, int SIMULATION_STEPS, float malicious_fraction, int malicious_timer_wait, bool malicious_ants_focus, AntTracingPattern ant_tracing_pattern, 
		bool counter_pheromone, float hell_phermn_intensity_multiplier)
{
	World world(Conf::WORLD_WIDTH, Conf::WORLD_HEIGHT);
	Colony colony(Conf::COLONY_POSITION.x, Conf::COLONY_POSITION.y, Conf::ANTS_COUNT, 
	malicious_fraction, malicious_timer_wait, malicious_ants_focus, ant_tracing_pattern, 
	counter_pheromone, hell_phermn_intensity_multiplier);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	initWorld(world, colony);
	sf::RenderWindow window(sf::VideoMode(Conf::WIN_WIDTH, Conf::WIN_HEIGHT), "AntSim", sf::Style::Fullscreen, settings);
	window.setFramerateLimit(60);

	DisplayManager display_manager(window, window, world, colony);

	sf::Vector2f last_clic;

	while (window.isOpen())
	{
		display_manager.processEvents();
		// Add food on clic
		if (display_manager.clic) {
			const sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
			const sf::Vector2f world_position = display_manager.displayCoordToWorldCoord(sf::Vector2f(to<float>(mouse_position.x), to<float>(mouse_position.y)));
			const float clic_min_dist = 2.0f;
			if (getLength(world_position - last_clic) > clic_min_dist) {
				if (display_manager.wall_mode) {
					world.addWall(world_position);
				}
				else if (display_manager.remove_wall) {
					world.removeWall(world_position);
				}
				else {
					world.addFoodAt(world_position.x, world_position.y, 20);
				}
				last_clic = world_position;
			}
		}

		if (!display_manager.pause) {
			updateColony(world, colony);
		}

		window.clear(sf::Color(94, 87, 87));
		
		display_manager.draw();

		window.display();
		// if(colony.timer_count2>SIMULATION_STEPS) break;
	}
}

int main(int argc,char* argv[])
{
	Conf::loadTextures();
	
	//loadUserConf(); //this function is not used.
	if(argc < 10){
		//insufficient arguments -> not run
		std::cout << "Insufficient arguments. Program terminated." << std::endl;
		printf("usage:\n\t./AntSimulator <steps> <iterations> <mal_fraction> <mal_timer> <mal_focus> <tracing_pattern> <counter_pheromone> <fake_intensity>\n\t\t<> means required arguments\n\t\tfor boolean variables, T is for true. Other key is for false\n\t\tfor tracing_pattern, R is for random. Other key is for food.\n\nExample:\n\t./AntSimulator 10000 3 0.05 100 F F F 1\n\t\tThis will run 3 experiments for 10000 timesteps each where the experiment is as followed:\n\t\t\t- there are 5%% of malicious ants within the colony\n\t\t\t- it will launch the attack at timestep 100\n\t\t\t- the malicious ants will not attack towards the food\n\t\t\t- the malicious ants will follow food pheromone.\n\t\t\t- the counter pheromone will not released.\n\t\t\t- the fake pheromone will be 1 times stronger than usual (aka. normal)\n");
		return 2;
	}
		//set the arguments here.
		int SIMULATION_STEPS = atoi(argv[1]);		// Only used in the data recording, NOT IN GUI
		int SIMULATION_ITERATIONS = atoi(argv[2]);
		float malicious_fraction = std::stof(argv[3]);
		int malicious_timer_wait = atoi(argv[4]);
		bool malicious_ants_focus = false;
		if(strcmp(argv[5],("T"))==0){
			malicious_ants_focus = true;
		}
		//bool malicious_ants_focus = false; //directional or uniform random
		AntTracingPattern ant_tracing_pattern = AntTracingPattern::FOOD;
		if(strcmp(argv[6],("R"))==0){
			ant_tracing_pattern = AntTracingPattern::RANDOM;
		}
		bool counter_pheromone = false;
		if(strcmp(argv[7],("T"))==0){
			counter_pheromone = true;
		}
		float hell_phermn_intensity_multiplier = std::stof(argv[8]);; //fake food pheromone strength
		if(strcmp(argv[9],("T"))==0){
			DISPLAY_GUI = true;
		}
		int mode = 0;
		if(argc>10 && atoi(argv[10])==1){
			mode = 1;
		}
	if(DISPLAY_GUI)
		displaySimulation(SIMULATION_ITERATIONS, SIMULATION_STEPS, malicious_fraction, malicious_timer_wait, malicious_ants_focus,ant_tracing_pattern, 
		counter_pheromone, hell_phermn_intensity_multiplier);
	else
		simulateAnts(SIMULATION_ITERATIONS, SIMULATION_STEPS, malicious_fraction, malicious_timer_wait, malicious_ants_focus,ant_tracing_pattern, 
		counter_pheromone, hell_phermn_intensity_multiplier,mode);

	// Free textures
	Conf::freeTextures();

	return 0;
}

#if defined(_WIN32)
#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline,
                     int cmdshow) {
  return main();
}
#endif