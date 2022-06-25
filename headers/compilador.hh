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

#include "../headers/utilidades.hh"

#include <string>

const std::vector<std::string> builtin_tipos{
		"char",
		"bool"
		"int",
		"float",
		"double",
};

class Compilador {
 public:
	/// Constructor estándar
	Compilador(const archivo_t&);

	/// Para compilar
	std::string Compilar();

 private:
	/// Atributos
	archivo_t source_;
	int source_index_;
	archivo_t data_segment_;
	archivo_t text_segment_;
	/// Métodos

	void SelZeroIndent();
	void SelNonZeroIndent();

	std::string& linea();

	/// Chuchangas friend?

	void Funciones();
	void LlamadaFunciones();
	void Ostream();
	void Istream();
	void For();
	void While();
	void Elif();
	void ReturnBool();
	void Return();
	void GlobalVar();
	void LocalVar();
};