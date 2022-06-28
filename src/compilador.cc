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
#include <stdexcept>

Compilador::Compilador(const std::string& source) :
		source_{source},
		source_index_{0} {}

std::string Compilador::Compilar() {
	try {
		/// Generar tokens
		Tokenizar();
		/// Generar código fuente
		Generar();
	} catch (const std::runtime_error& error) {
		std::cerr << error.what() << std::endl;
		exit(EXIT_FAILURE);
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

/**
 * @brief Genera el código fuente
 */
void Compilador::Generar() {}

/**
 * @brief Preprocesa el source_
 */
void Compilador::Preprocesar() {
	for (auto& i : source_) {
		if (i == '\n' || i == '\t') i = ' ';
	}
	char last;
	for (int i{0}; i < source_.size(); i++) {
		if (i == 0) {
			last = source_[i];
			continue;
		}
		if (source_[i] == ' ' && last == ' ') {
			source_.erase(--i, 1);
		}
		last = source_[i];
	}
}

/**
 * @brief Genera los tokens
 */
void Compilador::Tokenizar() {
	/// Preprocesar
	Preprocesar();
	/// preprocesado acabado
	std::string palabra;
	bool preprocesado{false};
	bool string_literal{false};
	// token_t prev{};
	for (int i{0}; i < source_.size(); i++) {
		if (source_[i] == ' ') continue;
		if (string_literal && !preprocesado) {
			char prev;
			while (!(source_[i] == '\"' && prev != '\\')) {
				palabra.push_back(source_[i++]);
				prev = source_[i - 1];
			}
			str_literales_.push(palabra);
			tokens_.push_back(std::make_pair(token_t::LITERAL, static_cast<unsigned>(literal_e::STR)));
			palabra.clear();
			string_literal = false;
		} else if (preprocesado && string_literal) {
			while (source_[i] != '>') {
				palabra.push_back(source_[i++]);
			}
			preprocesado = false;
			string_literal = false;
			palabra.clear();
		} else if (isalnum(source_[i]) || source_[i] == '_') {
			/// num literal
			if (isdigit(source_[i])) {
				while (isdigit(source_[i]) || source_[i] == '.') {
					palabra.push_back(source_[i++]);
				}
				tokens_.push_back(std::make_pair(token_t::LITERAL, static_cast<unsigned>(literal_e::INT)));
				int_literales_.push(stoi(palabra));
				palabra.clear();
			} else {
				while (isalnum(source_[i]) || source_[i] == '_') {
					palabra.push_back(source_[i++]);
				}
				/// Flags
				if (preprocesado) {
					if (m_directivas.find(palabra) == m_directivas.end()) {
						throw std::runtime_error{"Error en el caractér " + std::to_string(i + 1) + ": directiva desconocida."};
					}
					palabra.clear();
					continue;
				}
				/// Regular
				if (m_keyword.find(palabra) != m_keyword.end()) {
					tokens_.push_back(std::make_pair(token_t::KEYWORD, static_cast<unsigned>(m_keyword.at(palabra))));
				} else if (m_tipos.find(palabra) != m_tipos.end()) {
					tokens_.push_back(std::make_pair(token_t::TYPE, static_cast<unsigned>(m_tipos.at(palabra))));
				} else { /// identificador
					tokens_.push_back(std::make_pair(token_t::IDENTIFIER, 0));
					identificadores_.push(palabra);
				}
			}
			palabra.clear();
		} else {
			/// Regular
			if (m_symbol.find(source_[i]) != m_symbol.end()) {
				tokens_.push_back(std::make_pair(token_t::SYMBOL, static_cast<unsigned>(m_symbol.at(source_[i]))));
				if (m_symbol.at(source_[i]) == symbol_e::COMILLAS) {
					string_literal = true;
				} else if (m_symbol.at(source_[i]) == symbol_e::ALMOHADILLA) {
					preprocesado = true;
				} else if (m_symbol.at(source_[i]) == symbol_e::COMILLAAN_A && preprocesado) {
					string_literal = true;
				}
				continue;
			}
			while (source_[i] != ' ') {
				palabra.push_back(source_[i++]);
			}
			if (m_operator.find(palabra) != m_operator.end()) {
				tokens_.push_back(std::make_pair(token_t::OPERATOR, static_cast<unsigned>(m_operator.at(palabra))));
			}
		}
		palabra.clear();
	}
}