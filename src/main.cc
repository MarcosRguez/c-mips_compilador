/**
 * @file main.cc
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 25-06-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

#include "../headers/timer.hh"

/**
 * @brief Punto de entrada
 * @param argc
 * @param argv
 * @return int
 */
int main(const int argc, const char* const argv[]) {
	Timer crono;

	std::cout << "La compilación duró: " << crono.elapsed() << " ms" << std::endl;
	return 0;
}