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
#include "../headers/utilidades.hh"
#include "../headers/compilador.hh"

/**
 * @brief Punto de entrada
 * @param argc
 * @param argv
 * @return int
 */
int main(const int argc, const char* const argv[]) {
	Timer crono;
	std::string filename{"../test/input/hello.c"};
	Compilador bench{LeerArchivo(filename)};
	std::string ruta{"../test/output/"};
	std::string extension{".asm"};
	std::string out = filename.substr(filename.find_last_of('/') + 1, filename.size() - filename.find_last_of('/') - (1 + (filename.size() - filename.find_last_of('.'))));
	CrearArchivo(ruta + out + ".asm", bench.Compilar());
	std::cout << "La compilación duró: " << crono.elapsed() << " ms" << std::endl;
	return 0;
}