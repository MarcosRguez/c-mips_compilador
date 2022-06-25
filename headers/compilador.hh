/**
 * @file compilador.hh
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief clase que compila
 * @version 0.1
 * @date 25-06-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <string>

class Compilador {
 public:
	/// Constructores
	Compilador(const std::string&); /// Constructor estándar
	/// Destructores
	// ~Compilador();
	/// Métodos
	std::string Compilar();

 protected:
 private:
	/// Atributos
	const std::string source_;
};