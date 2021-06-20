#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <fstream>
#include "colony.hpp"
#include "config.hpp"
#include "display_manager.hpp"

bool DISPLAY_GUI = false;
  
void loadUserConf(float& malicious_fraction, int& malicious_timer_wait)
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

void simulateAnts(World& world, Colony& colony)
{
	const float dt = 0.016f;
	for(int i = 0; i<10000; i++)
	{
		colony.update(dt, world);			
		if(colony.timer_count2%100 == 0)
		std::cout << "Foraged ant=" << colony.confused_count<< std::endl;
		world.update(dt);
		// std::cout<<i;
	}
}

void displaySimulation(World& world, Colony& colony)
{
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

		const float dt = 0.016f;

		if (!display_manager.pause) {
			colony.update(dt, world);
			if(colony.timer_count2%100 == 0){
				std::cout << "Foraged ant=" << colony.confused_count<< std::endl;
			}
			world.update(dt);
		}

		window.clear(sf::Color(94, 87, 87));
		
		display_manager.draw();

		window.display();
		if(colony.timer_count2>10000) break;
	}
}

int main()
{
	Conf::loadTextures();


	World world(Conf::WORLD_WIDTH, Conf::WORLD_HEIGHT);


  /****************************************************************************************
   ************************ CHANGE THIS FRACTION OF MALICIOUS ANTS ************************
   ****************************************************************************************/
  float malicious_fraction = 0.05;
  int malicious_timer_wait = 100;
	loadUserConf(malicious_fraction, malicious_timer_wait);
  
	Colony colony(Conf::COLONY_POSITION.x, Conf::COLONY_POSITION.y, Conf::ANTS_COUNT, malicious_fraction, malicious_timer_wait);
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

	if(DISPLAY_GUI)
		displaySimulation(world, colony);
	else
		simulateAnts(world, colony);

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