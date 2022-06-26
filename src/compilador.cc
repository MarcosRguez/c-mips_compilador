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
#include "../headers/mapeos.hh"

#include <iostream>

Compilador::Compilador(const std::string& source) :
		source_{source},
		source_index_{0} {}

std::string Compilador::Compilar() {
	Tokenizar();
	/// Preprocesar
	/*
	EliminarComentarios(source_);
	EliminarDirectivas(source_);
	EliminarCorchetes(source_);
	EliminarEspacios(source_);
	EliminarLineasVacias(source_);
	*/
	/// Gettín redi
	unsigned indent_last{0};
	unsigned indent_current{0};
	/// Bucle
	while (source_index_ != source_.size()) {
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

void Compilador::Tokenizar() {
	for (auto& i : source_) {
		if (i == '\n' || i == '\t') i = ' ';
	}
	std::string palabra;
	for (int i{0}; i < source_.size(); i++) {
		if (isalnum(source_[i]) || source_[i] == '_') {
			while (isalnum(source_[i]) || source_[i] == '_') {
				palabra.push_back(source_[i++]);
			}
			if (m_keyword.find(palabra) != m_keyword.end()) {
				tokens_.push_back(std::make_pair(token_t::KEYWORD, static_cast<unsigned>(m_keyword.at(palabra))));
			} else if (m_tipos.find(palabra) != m_tipos.end()) {
				tokens_.push_back(std::make_pair(token_t::TYPE, static_cast<unsigned>(m_tipos.at(palabra))));
			} else { /// identificador
				tokens_.push_back(std::make_pair(token_t::IDENTIFIER, 0));
			}
			palabra.clear();
		} else {
			if (m_symbol.find(source_[i]) != m_symbol.end()) {
				tokens_.push_back(std::make_pair(token_t::SYMBOL, static_cast<unsigned>(m_symbol.at(source_[i]))));
			} else if (m_operator.find(source_[i]) != m_operator.end()) {
				tokens_.push_back(std::make_pair(token_t::OPERATOR, static_cast<unsigned>(m_operator.at(source_[i]))));
			}
		}
	}
}

void Compilador::SelZeroIndent() {}
void Compilador::SelNonZeroIndent() {}