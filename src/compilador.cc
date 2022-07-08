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
#include <cassert>

Compilador::Compilador(const std::string& source) :
		source_{source},
		source_index_{0} {
	for (int i{0}; i <= 9; i++) {
		registros_temporales_.emplace("$t" + std::to_string(i), false);
	}
	for (int i{0}; i <= 8; i++) {
		registros_salvados_.emplace("$s" + std::to_string(i), false);
	}
}

std::string Compilador::Compilar() {
	try {
		/// En matchingamos los brakets
		if (!MatchingBrackets(source_)) throw std::runtime_error("los brakest no matchingn");
		/// Generar tokens
		Tokenizar();
		/// Generar código fuente
		data_segment_.push_back(".data");
		text_segment_.push_back(".text");
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

Eval_f_t Compilador::EvaluadorExpresiones(const int index, const int n_tokens) {
	/// caso base: literal
	/// caso base: identificador
	Eval_f_t resultado;
	if (n_tokens == 0) return resultado;
	for (int i{index}; i < index + n_tokens; i++) {
		if (tokens_[i].first == token_t::LITERAL) {
			/// Devolver el literal
			resultado.literal = true;
			switch (static_cast<literal_e>(tokens_[i].second)) {
				case (literal_e::INT):
					resultado.contenido.push_back(std::to_string(int_literales_.front()));
					int_literales_.pop();
					break;
				case (literal_e::STR):
					resultado.contenido.push_back(str_literales_.front());
					int_literales_.pop();
					break;
				default:
					throw std::runtime_error{"literal desconocido"};
					break;
			}
		} else if (tokens_[i].first == token_t::IDENTIFIER) {
			/// comprobamos si existe en la tabla de variables
		}
	}
	return resultado;
}

Eval_f_t Compilador::EvaluadorBool(int index, int n_tokens) {
	Eval_f_t resultado;
	if (n_tokens == 0) return resultado;
	if (n_tokens == 1) {
		assert(tokens_[index].first == token_t::LITERAL && static_cast<literal_e>(tokens_[index].second) == literal_e::BOOL);
		resultado.literal = true;
		bool temp{bool_literales_.front()};
		bool_literales_.pop();
		if (temp) {
			resultado.contenido.push_back("1");
		} else {
			resultado.contenido.push_back("0");
		}
		return resultado;
	}
	for (int i{index}; i < index + n_tokens; i++) {
	}
	return resultado;
}

void Compilador::WriteBuffer(const archivo_t& buffer, bool ans) {
	if (buffer.empty()) return;
	if (!ans) {
		for (const auto& i : buffer) {
			text_segment_.push_back(i);
		}
	} else {
		for (const auto& i : buffer) {
			text_segment_.insert(text_segment_.end() - 1, i);
		}
	}
}

/**
 * @brief Devuelve el índice de coso que cierra
 * @param index
 * @return int
 */
int Compilador::NextMatching(int index) {
	assert(tokens_[index].first == token_t::SYMBOL);
	int apertura{1};
	int cerrura{0};
	switch (static_cast<symbol_e>(tokens_[index].second)) {
		case (symbol_e::PARENTESIS_A):
			while (apertura != cerrura) {
				switch (static_cast<symbol_e>(tokens_[++index].second)) {
					case (symbol_e::PARENTESIS_A):
						apertura++;
						break;
					case (symbol_e::PARENTESIS_C):
						cerrura++;
						break;
					default:
						break;
				}
			}
			break;
		case (symbol_e::CORCHETE_A):
			while (apertura != cerrura) {
				switch (static_cast<symbol_e>(tokens_[++index].second)) {
					case (symbol_e::CORCHETE_A):
						apertura++;
						break;
					case (symbol_e::CORCHETE_C):
						cerrura++;
						break;
					default:
						break;
				}
			}
			break;
		case (symbol_e::LLAVE_A):
			while (apertura != cerrura) {
				switch (static_cast<symbol_e>(tokens_[++index].second)) {
					case (symbol_e::LLAVE_A):
						apertura++;
						break;
					case (symbol_e::LLAVE_C):
						cerrura++;
						break;
					default:
						break;
				}
			}
			break;
		case (symbol_e::COMILLAAN_A):
			while (apertura != cerrura) {
				switch (static_cast<symbol_e>(tokens_[++index].second)) {
					case (symbol_e::COMILLAAN_A):
						apertura++;
						break;
					case (symbol_e::COMILLAAN_C):
						cerrura++;
						break;
					default:
						break;
				}
			}
			break;
		default:
			throw std::runtime_error{"error coño"};
			break;
	}
	return index;
}

std::string Compilador::EncontrarRegistroLibre(
		const registros_t& registros) const {
	for (const auto& i : registros) {
		if (i.second == false) {
			return i.first;
		}
	}
	return NULL;
}

int Compilador::NextPuntoYComa(int index) {
	while (!(tokens_[index].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[index].second) == symbol_e::PUNTOYCOMA)) {
		index++;
	}
	return index;
}

void Compilador::var_init(archivo_t& segment, int& i) {
	if (tokens_[i + 1].first == token_t::SYMBOL) {
		if (static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PUNTOYCOMA) {
			/// no está init
			text_segment_.back().push_back('0');
		} else if (static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::LLAVE_A) {
			/// evaluar la expresión entre los corchetes
			const auto&& temp{EvaluadorExpresiones(i + 2, NextMatching(i + 1) - (i + 1))};
			if (temp.literal) {
				text_segment_.back().append(temp.contenido[0]);
			} else {
				WriteBuffer(temp.contenido, true);
			}
			/// saltar los tokens analizados
			i = NextPuntoYComa(i);
		}
	} else if (tokens_[i + 1].first == token_t::OPERATOR) {
		if (static_cast<operator_e>(tokens_[i + 1].second) == operator_e::ASIGNACION) {
			const auto&& temp{EvaluadorExpresiones(i + 2, NextPuntoYComa(i + 2) - (i + 2))};
			if (temp.literal) {
				text_segment_.back().append(temp.contenido[0]);
			} else {
				WriteBuffer(temp.contenido, true);
			}
			i = NextPuntoYComa(i);
		} else {
			throw std::runtime_error{"error de sintaxis"};
		}
	} else {
		throw std::runtime_error{"error de sintaxis"};
	}
}

/**
 * @brief Genera el código fuente
 */
void Compilador::Generar() {
	std::string func_name;
	bool end_while_loop{false};
	bool end_for_loop{false};
	int local_for_n_bucle{0};
	for (int i{0}; i < tokens_.size(); i++) {
		auto token = tokens_[i].first;
		if (token == token_t::IDENTIFIER) {
			if (func_name.empty()) {
				assert(tokens_[i - 1].first == token_t::TYPE);
				/// Es una definición de función (o declaración)
				if (tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A) {
					/// Escribimos la entrada de subrutina
					text_segment_.push_back(identificadores_.front() + ':');
					/// actualizamos el alcance
					func_name = identificadores_.front();
					identificadores_.pop();
					/// entrada de función marco de pila...
					if (func_name != "main") {
						text_segment_.push_back("move $fp,$sp");
						text_segment_.push_back("addi $sp,$sp,-4");
						text_segment_.push_back("sw $ra,0($sp)");
					} else {
						/// skip main argrs
					}
				}
				/// es una variable global
				else if (tokens_[i + 1].first == token_t::OPERATOR && static_cast<operator_e>(tokens_[i + 1].second) == operator_e::ASIGNACION) {
					/// data_Segmetn
					data_segment_.push_back(identificadores_.front() + ':');
					identificadores_.pop();
					const auto& temp{static_cast<tipos_e>(tokens_[i - 1].second)};
					if (temp == tipos_e::INT) {
						data_segment_.back().append(".word");
					} else if (temp == tipos_e::FLOAT) {
						data_segment_.back().append(".float");
					} else if (temp == tipos_e::DOUBLE) {
						data_segment_.back().append(".double");
					} else if (temp == tipos_e::CHAR) {
						data_segment_.back().append(".byte");
					} else {
						throw std::runtime_error{"tipo de var global no soportado"};
					}
					var_init(data_segment_, i);
				}
			} else {
				/// es una declaración de variable local
				variables_t&& temp{static_cast<tipos_e>(tokens_[i - 1].second), identificadores_.front(), EncontrarRegistroLibre(registros_salvados_)};
				variables_.push_back(temp);
				identificadores_.pop();
				registros_salvados_.at(variables_.back().registro) = true; ///¿Poner los registros salvados únicos para cada función?
				/// lo ponemos
				text_segment_.push_back("li ");
				text_segment_.back().append(variables_.back().registro);
				text_segment_.back().append(",");
				/// comprobamos si está inizializado
				var_init(text_segment_, i);
			}
		} else if (token == token_t::KEYWORD) {
			auto tipo{static_cast<keyword_e>(tokens_[i].second)};
			if (tipo == keyword_e::WHILE) {
				assert(tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A);
				text_segment_.push_back(func_name + "_while" + std::to_string(bucle_while_count_++) + ':');
				const auto&& temp{EvaluadorBool(i + 2, NextMatching(i + 1) - (i + 2))};
				if (temp.literal) throw std::runtime_error{"desgraciado, has escrito un bucle infinito"};
				WriteBuffer(temp.contenido, false);
				// text_segment_.push_back(alcance + "_while" + std::to_string(bucle_for_count_++) + "_fin" + ':');
				/// establecer flag y analizar token '}'
				end_while_loop = true;
			} else if (tipo == keyword_e::FOR) {
				assert(tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A);
				int n_tokens{0};
				while (!(tokens_[i + 2 + n_tokens].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 2 + n_tokens].second) == symbol_e::PUNTOYCOMA)) {
					n_tokens++;
				}
				WriteBuffer(EvaluadorExpresiones(i + 2, n_tokens).contenido, false);
				text_segment_.push_back(func_name + "_for" + std::to_string(local_for_n_bucle));
				// WriteBuffer(EvaluadorExpresiones(i + 2 + n_tokens, n_tokens));

			} else if (tipo == keyword_e::RETURN) {
				/// evaluar la expresión y hacer branch
			}
		} else if (token == token_t::SYMBOL) {
			/// suponemos que todas las demás llaves que no sean de funciones las hemos saltado
			if (static_cast<symbol_e>(tokens_[i].second) == symbol_e::LLAVE_C) {
				if (end_while_loop) {
					text_segment_.push_back("b " + func_name + "_while" + std::to_string(bucle_while_count_ - 1));
					text_segment_.push_back(func_name + "_while" + std::to_string(bucle_while_count_ - 1) + '_');
					end_while_loop = false; /// aquí hay que usar el stack de cierres de bucles
				} else if (end_for_loop) {
				} else {
					if (func_name != "main") {
						text_segment_.push_back(func_name + "_return:");
						text_segment_.push_back("lw $ra,0($sp)");
						text_segment_.push_back("move $sp,$fp");
						text_segment_.push_back("jr $ra");
						func_name.clear();
					} else {
						text_segment_.push_back("li $v0,10");
						text_segment_.push_back("syscall");
						func_name.clear();
					}
				}
			}
		} else {
			// throw std::runtime_error{"se analizó un token imprevisto"};
		}
	}
}

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
				i--;
				tokens_.push_back(std::make_pair(token_t::LITERAL, static_cast<unsigned>(literal_e::INT)));
				int_literales_.push(stoi(palabra));
				palabra.clear();
			} else {
				while (isalnum(source_[i]) || source_[i] == '_') {
					palabra.push_back(source_[i++]);
				}
				i--;
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
				} else if (m_literal.find(palabra) != m_literal.end()) {
					/// bool true false
					tokens_.push_back(std::make_pair(token_t::LITERAL, static_cast<unsigned>(m_literal.at(palabra))));
					if (palabra == "true") {
						bool_literales_.push(true);
					} else {
						bool_literales_.push(false);
					}
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