/**
 * @file compilador.cc
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 25-06-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../headers/compilador.hh"
#include "../headers/utilidades.hh"

#include <iostream>

Compilador::Compilador(const archivo_t& source) :
		source_{source},
		source_index_{0} {}

std::string Compilador::Compilar() {
	/// Preprocesar
	EliminarComentarios(source_);
	EliminarDirectivas(source_);
	EliminarCorchetes(source_);
	EliminarEspacios(source_);
	EliminarLineasVacias(source_);
	/// Gettín redi
	unsigned indent_last{0};
	unsigned indent_current{0};
	/// Bucle
	while (source_index_ != source_.size()) {
		indent_current = linea().find_first_not_of('\t');
		if (indent_current == 0) {
			SelZeroIndent();
		} else {
			SelNonZeroIndent();
		}
		indent_last = indent_current;
		source_index_++;
	}
	/// Devolver resultado
	archivo_t resultado;
	for (const auto& i : data_segment_) {
		resultado.push_back(i);
	}
	for (const auto& i : text_segment_) {
		resultado.push_back(i);
	}
	return LineaUnica(resultado);
}

void Compilador::SelZeroIndent() {
	/// Global var
	std::string tipo = linea().substr(linea().find_first_of(' '));
	/// Función
}
void Compilador::SelNonZeroIndent() {}

std::string& Compilador::linea() {
	return source_[source_index_];
}