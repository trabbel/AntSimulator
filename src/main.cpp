#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <fstream>
#include "colony.hpp"
#include "config.hpp"
#include "display_manager.hpp"

#include <stdio.h>      // for sprintf()

#include <iostream>     // for console output
#include <string>       // for std::string
#include <ctime>
#include <unistd.h>

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
* @param counter_pheromone:: Will the ants secret cautionary pheromone?
* @param hell_phermn_intensity_multiplier:: Multiplier for the intensity of misleading pheromone
* @param hell_phermn_evpr_multi:: Multiplier for the evaporation rate of misleading pheromone
* @param cntr_phermn_evpr_multi:: Multiplier for the evaporation rate of cautionary pheromone
* @param dilusion_max:: Maximum patience if cautionary pheromone is used
* @param dilusion_increment:: Increment of patience if ant does not follow a food trail
* @param phermn_fod_food:: Will the ants use pheromone for food as defensive strategy?
* @param cunning:: Will the malicious ants work as cunning agents?
* @param threshold:: Threshold for mode changing in cunning ants
*/
const bool DISPLAY_GUI = true;
const int SIMULATION_STEPS = 50000;		// Only used in the data recording, NOT IN GUI
const int SIMULATION_ITERATIONS = 100;
float malicious_fraction = 0.1250;
int malicious_timer_wait = 100;	
bool malicious_ants_focus = true;
AntTracingPattern ant_tracing_pattern = AntTracingPattern::FOOD;
bool counter_pheromone = false;
float hell_phermn_intensity_multiplier = 1.0;
float hell_phermn_evpr_multi = 5.0;
float cntr_phermn_evpr_multi = 1.0;
int dilusion_max = 250;
int dilusion_increment = 2;
bool phermn_for_food = true;
bool cunning = true;
int threshold = 0;

std::string getExperimentSpecificName(int iteration)
{
std::string DISPLAY_GUI_string = "_DISPLAY_GUI-" + std::to_string(DISPLAY_GUI);
std::string SIMULATION_STEPS_string = "_SIM_STEPS-" + std::to_string(SIMULATION_STEPS);
std::string SIMULATION_ITERATIONS_string = "_SIM_ITERS-" + std::to_string(SIMULATION_ITERATIONS);
std::string malicious_fraction_string = "_mal_frac-" + std::to_string(malicious_fraction);
std::string malicious_timer_wait_string = "_mal_delay-" + std::to_string(malicious_timer_wait);
std::string malicious_ants_focus_string = "_mal_ants_focus-" + std::to_string(malicious_ants_focus);
std::string ant_tracing_pattern_string = "_ant_tracing-" + std::to_string(ant_tracing_pattern);
std::string counter_pheromone_string = "_ctr_pherm-" + std::to_string(counter_pheromone);
std::string hell_phermn_intensity_multiplier_string = "_hell_phermn_intens-" + std::to_string(hell_phermn_intensity_multiplier);
std::string hell_phermn_evpr_multi_string = "_hell_phermn_evpr-" + std::to_string(hell_phermn_evpr_multi);
std::string dilusion_max_string = "_dil_max-" + std::to_string(dilusion_max);
std::string dilusion_increment_string = "_dil_incr-" + std::to_string(dilusion_increment);
std::string iteration_string = "_iter-" + std::to_string(iteration);

return SIMULATION_STEPS_string
		+ SIMULATION_ITERATIONS_string
		+ malicious_fraction_string
		+ malicious_timer_wait_string
		+ malicious_ants_focus_string
		+ ant_tracing_pattern_string
		+ counter_pheromone_string
		+ hell_phermn_intensity_multiplier_string
		+ hell_phermn_evpr_multi_string
		+ dilusion_max_string
		+ dilusion_increment_string
		+ iteration_string;
}

void loadUserConf()
{	
	std::ifstream conf_file("conf.txt");
	if (conf_file) {
		conf_file >> malicious_fraction;
		conf_file >> malicious_timer_wait;
		conf_file >> Conf::ANTS_COUNT;
	}
	else {
		// malicious_fraction = 0.25;
		// malicious_timer_wait = 100;
		std::cout << "Couldn't find 'conf.txt', loading default" << std::endl;
	}
}

void setStaticVariables()
{
	WorldCell::setHellPhermnEvprMulti(hell_phermn_evpr_multi);
	Ant::resetFoodBitsCounters();
	Ant::setDilusionMax(dilusion_max);
	Ant::setDilusionIncrement(dilusion_increment);
}

void initWorld(World& world, Colony& colony)
{
	setStaticVariables();
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
					int food_intensity = 100;
					world.addFoodAt(position.x/10, position.y/10, food_intensity);
					//world.addFoodAt(position.x/10+1200, position.y/10, food_intensity);
					//world.addFoodAt(position.x/10, position.y/10+1000, food_intensity);
					//world.addFoodAt(position.x/10+1200, position.y/10+1000, food_intensity);
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
	world.update(dt);
}

void oneExperiment(int i)
{
	std::ofstream myfile;
	const static float dt = 0.016f;
	const static int datapoints_to_record = 100;
	static int skip_steps = SIMULATION_STEPS/datapoints_to_record;
	static std::string file_name_prefix =  "data_pff";//"../data_exp-counters-multi-iteration/AntSimData";
	static int x = 0;
	
	myfile.open(file_name_prefix+getExperimentSpecificName(i)+".csv");
	//std::cout<<file_name_prefix+getExperimentSpecificName(i)<<std::endl;
	float food_found_per_ant = 0.0;
	float food_delivered_per_ant = 0.0;
	float fraction_of_ants_found_food = 0.0;
	float fraction_of_ants_delivered_food = 0.0;
	
	setStaticVariables();
	World world(Conf::WORLD_WIDTH, Conf::WORLD_HEIGHT);
	Colony colony(Conf::COLONY_POSITION.x, Conf::COLONY_POSITION.y, Conf::ANTS_COUNT, 
	malicious_fraction, malicious_timer_wait, malicious_ants_focus, ant_tracing_pattern, 
	counter_pheromone, hell_phermn_intensity_multiplier, phermn_for_food, cunning, threshold);
	initWorld(world, colony);	

	for(int j = 0; j<SIMULATION_STEPS; j++)
	{
		updateColony(world, colony);
		if(j%skip_steps == 0)
		{
			food_found_per_ant = float(Ant::getFoodBitsTaken())/float(1024); // Total  number of Ants 
			food_delivered_per_ant = float(Ant::getFoodBitsDelivered())/float(1024); // Total  number of Ants 
			fraction_of_ants_found_food = float(Colony::getAntsThatFoundFood())/float(1024);
			fraction_of_ants_delivered_food = float(Colony::getAntsThatDeliveredFood())/float(1024);
			myfile  << (food_found_per_ant)<< "," << food_delivered_per_ant << "," 
					<< fraction_of_ants_found_food << "," << fraction_of_ants_delivered_food<<std::endl;
		}
	}
	myfile.close();
	std::cout<<"Iteration "<<x++<<" Done"<<std::endl;

}

void simulateAnts()
{
	/**
	 * @brief This loop will start a new colony and run the sim for SIMULATION_STEPS number of steps
	 */
	std::vector<int> dilusion_max_set = {50, 100, 250, 500, 750, 1000};
	std::vector<int> dilusion_increment_power = {1, 2, 5, 10, 50, 100, 1000};
	for(int i = 0; i<SIMULATION_ITERATIONS; i++)
	{oneExperiment(i);
		/*for(int dil_max = 0; dil_max<dilusion_max_set.size(); dil_max++)
		{
			for(int dil_incr = 0; dil_incr<dilusion_increment_power.size(); dil_incr++)
			{	
				dilusion_max = dilusion_max_set.at(dil_max);
				dilusion_increment = dilusion_max/dilusion_increment_power.at(dil_incr);
				oneExperiment(i);
			}
		}*/
		std::cout<<"###########################"<<std::endl;
		std::cout<<"Experiment "<<i<<" Done"<<std::endl;
		std::cout<<"###########################"<<std::endl;
	}
	std::cout<<"########## DONE ##########"<<std::endl;
}

void displaySimulation()
{
	setStaticVariables();
	World world(Conf::WORLD_WIDTH, Conf::WORLD_HEIGHT);
	Colony colony(Conf::COLONY_POSITION.x, Conf::COLONY_POSITION.y, Conf::ANTS_COUNT, 
	malicious_fraction, malicious_timer_wait, malicious_ants_focus, ant_tracing_pattern, 
	counter_pheromone, hell_phermn_intensity_multiplier, phermn_for_food, cunning, threshold);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	initWorld(world, colony);
	sf::RenderWindow window(sf::VideoMode(Conf::WIN_WIDTH, Conf::WIN_HEIGHT), "AntSim", sf::Style::Fullscreen, settings);
	window.setFramerateLimit(60);

	DisplayManager display_manager(window, window, world, colony);

	sf::Vector2f last_clic;
	int c = 0;
	int C = -100;
	int i = 0;

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
			//std::cout<<i<<std::endl;
			//if (i==500 or i==1500 or i==10000){sleep(10);}
			i++;
		}

		if(c++>C)
		{
			c = 0;
				window.clear(sf::Color(94, 87, 87));
				
				display_manager.draw();

				window.display();
		}
	}
}

int main()
{
	Conf::loadTextures();
	
	loadUserConf();
	if(DISPLAY_GUI)
		displaySimulation();
	else
		simulateAnts();

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
