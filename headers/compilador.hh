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
#include "../headers/enums.hh"

#include <string>
#include <map>
#include <queue>

class Compilador {
 public:
	/// Constructor estándar
	Compilador(const std::string&);

	/// Para compilar
	std::string Compilar();

 private:
	/// Atributos
	std::string source_{};
	int source_index_{};
	archivo_t data_segment_{};
	archivo_t text_segment_{};
	std::vector<std::pair<token_t, unsigned>> tokens_{};
	std::queue<std::string> identificadores_{};
	std::queue<std::string> str_literales_{};
	/// Métodos

	void Tokenizar();

	void SelZeroIndent();
	void SelNonZeroIndent();

	// std::string& linea();

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