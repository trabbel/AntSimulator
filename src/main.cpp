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

#include <boost/date_time/posix_time/posix_time.hpp>


//-----------------------------------------------------------------------------
// Format current time (calculated as an offset in current day) in this form:
//
//     "hh:mm:ss.SSS" (where "SSS" are milliseconds)
//-----------------------------------------------------------------------------
std::string now_str()
{
    // Get current time from the clock, using microseconds resolution
    const boost::posix_time::ptime now = 
        boost::posix_time::microsec_clock::local_time();

    // Get the time offset in current day
    const boost::posix_time::time_duration td = now.time_of_day();

    //
    // Extract hours, minutes, seconds and milliseconds.
    //
    // Since there is no direct accessor ".milliseconds()",
    // milliseconds are computed _by difference_ between total milliseconds
    // (for which there is an accessor), and the hours/minutes/seconds
    // values previously fetched.
    //
    const long hours        = td.hours();
    const long minutes      = td.minutes();
    const long seconds      = td.seconds();
    const long milliseconds = td.total_milliseconds() -
                              ((hours * 3600 + minutes * 60 + seconds) * 1000);

    //
    // Format like this:
    //
    //      hh:mm:ss.SSS
    //
    // e.g. 02:15:40:321
    //
    //      ^          ^
    //      |          |
    //      123456789*12
    //      ---------10-     --> 12 chars + \0 --> 13 chars should suffice
    //  
    // 
    char buf[40];
    sprintf(buf, "%02ld:%02ld:%02ld.%03ld", 
        hours, minutes, seconds, milliseconds);

    return buf;
}

const bool DISPLAY_GUI = false;
const int SIMULATION_STEPS = 1000;
const int SIMULATION_ITERATIONS = 2;
float malicious_fraction = 0.05;
int malicious_timer_wait = 100;	
  
void loadUserConf()
{	
	std::ifstream conf_file("conf.txt");
	if (conf_file) {
		conf_file >> malicious_fraction;
		conf_file >> malicious_timer_wait;
		conf_file >> Conf::ANTS_COUNT;
	}
	else {
		malicious_fraction = 0.25;
		malicious_timer_wait = 100;
		std::cout << "Couldn't find 'conf.txt', loading default" << std::endl;
	}
}

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
	initWorld(world, colony);

	if(colony.timer_count2%100 == 0){
		// std::cout << "Foraged ant=" << colony.confused_count<< std::endl;
	}
	world.update(dt);
}

void simulateAnts()
{
	const static float dt = 0.016f;
	std::cout << "In Sim" << now_str() << '\n'; 
	std::ofstream myfile;
	myfile.open ("../AntSimData.csv");
	std::cout << "File Open" << now_str() << '\n'; 
	World world(Conf::WORLD_WIDTH, Conf::WORLD_HEIGHT);
	Colony colony(Conf::COLONY_POSITION.x, Conf::COLONY_POSITION.y, Conf::ANTS_COUNT, malicious_fraction, malicious_timer_wait);
	initWorld(world, colony);	
	std::cout << "World colon init" << now_str() << '\n'; 
	for(int i = 0; i<SIMULATION_ITERATIONS; i++)
	{
		std::cout << "Iterating" << now_str() << '\n'; 
		for(int j = 0; j<SIMULATION_STEPS; j++)
		{
			updateColony(world, colony);

			if(colony.timer_count2%10 == 0)
				myfile  << colony.confused_count<< ",";
		}
		std::cout << "Iterating" << now_str() << '\n'; 
		myfile  << std::endl;
	}
	myfile.close();
}

void displaySimulation()
{
	World world(Conf::WORLD_WIDTH, Conf::WORLD_HEIGHT);
	Colony colony(Conf::COLONY_POSITION.x, Conf::COLONY_POSITION.y, Conf::ANTS_COUNT, malicious_fraction, malicious_timer_wait);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
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
		if(colony.timer_count2>SIMULATION_STEPS) break;
	}
}

int main()
{
	Conf::loadTextures();
  /****************************************************************************************
   ************************ CHANGE THIS FRACTION OF MALICIOUS ANTS ************************
   ****************************************************************************************/
	
	loadUserConf();
	std::cout << now_str() << '\n'; 
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