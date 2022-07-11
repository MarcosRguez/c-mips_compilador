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
		current_func.registros_temporales_.emplace("$t" + std::to_string(i), false);
	}
	for (int i{0}; i <= 8; i++) {
		current_func.registros_salvados_.emplace("$s" + std::to_string(i), false);
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
	if (n_tokens == 1) {
		assert(tokens_[index].first == token_t::LITERAL || tokens_[index].first == token_t::IDENTIFIER);
		resultado.literal = true;
		if (tokens_[index].first == token_t::LITERAL) {
			switch (static_cast<literal_e>(tokens_[index].second)) {
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
		} else if (tokens_[index].first == token_t::IDENTIFIER) {
			if (!FindVarTable(identificadores_.front())) {
				throw std::runtime_error{"variable no declarada"};
			}
			for (const auto& i : variables_) {
				if (i.identificador == identificadores_.front()) {
					resultado.contenido.push_back(i.registro);
					return resultado;
				}
			}
		} else {
			throw std::runtime_error{"en fin, la hipocresìa."};
		}
	}
	bool declaracion{false};
	for (int i{index}; i < index + n_tokens; i++) {
	}
	return resultado;
}

bool Compilador::FindVarTable(const std::string& nombre) {
	bool existe{false};
	for (const auto& i : variables_) {
		if (i.identificador == nombre) {
			existe = true;
			break;
		}
	}
	return existe;
}

bool Compilador::FindFuncTable(const std::string& nombre) {
	bool existe{false};
	for (const auto& i : funciones_) {
		if (i.identificador == nombre) {
			existe = true;
			break;
		}
	}
	return existe;
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
	/// meneamos las maracas 🐯🦄
	for (int i{index}; i < index + n_tokens; i++) {
		const auto& token{tokens_[i].first};
		if (token == token_t::IDENTIFIER) {
		}
	}
	return resultado;
}

void Compilador::WriteBuffer(const archivo_t& buffer, write_buffer_e ans) {
	if (buffer.empty()) return;
	if (ans == write_buffer_e::END) {
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

/**
 * @brief para inizializar las variables
 * @param buffer el buffer donde se anexa el literal
 * @param index	el índice del identificador
 */
void Compilador::var_init(archivo_t& buffer, int& index) {
	if (tokens_[index + 1].first == token_t::SYMBOL) {
		if (static_cast<symbol_e>(tokens_[index + 1].second) == symbol_e::PUNTOYCOMA) {
			/// no está init
			buffer.back().push_back('0');
		} else if (static_cast<symbol_e>(tokens_[index + 1].second) == symbol_e::LLAVE_A) {
			/// evaluar la expresión entre los corchetes
			const auto&& temp{EvaluadorExpresiones(index + 2, NextMatching(index + 1) - (index + 1))};
			if (temp.literal) {
				buffer.back().append(temp.contenido[0]);
			} else {
				WriteBuffer(temp.contenido, write_buffer_e::ANS);
			}
			/// saltar los tokens analizados
			index = NextPuntoYComa(index);
		}
	} else if (tokens_[index + 1].first == token_t::OPERATOR) {
		if (static_cast<operator_e>(tokens_[index + 1].second) == operator_e::ASIGNACION) {
			const auto&& temp{EvaluadorExpresiones(index + 2, NextPuntoYComa(index + 2) - (index + 2))};
			if (temp.literal) {
				buffer.back().append(temp.contenido[0]);
			} else {
				WriteBuffer(temp.contenido, write_buffer_e::ANS);
			}
			index = NextPuntoYComa(index);
		} else {
			throw std::runtime_error{"error de sintaxis"};
		}
	} else {
		throw std::runtime_error{"error de sintaxis"};
	}
}

void Compilador::DeclararVar(archivo_t& buffer, int& index) {
	variables_t&& temp{identificadores_.front(), static_cast<tipos_e>(tokens_[index - 1].second), EncontrarRegistroLibre(current_func.registros_salvados_)};
	variables_.push_back(temp);
	identificadores_.pop();
	current_func.registros_salvados_.at(variables_.back().registro) = true;
	/// lo ponemos
	buffer.push_back("li ");
	buffer.back().append(variables_.back().registro);
	buffer.back().append(",");
	/// comprobamos si está inizializado
	var_init(buffer, index);
}

/**
 * @brief Genera el código fuente
 */
void Compilador::Generar() {
	for (int i{0}; i < tokens_.size(); i++) {
		auto token = tokens_[i].first;
		if (token == token_t::IDENTIFIER) {
			if (current_func.identificador.empty()) {
				assert(tokens_[i - 1].first == token_t::TYPE);
				/// Es una definición de función (o declaración)
				if (tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A) {
					/// actualizamos el alcance
					current_func.identificador = identificadores_.front();
					identificadores_.pop();
					/// analizamos los argumentos 🤒🦍
					/// ponemos la funcion en la tabla de funciones
					funciones_.push_back(current_func);
					/// Escribimos la entrada de subrutina
					text_segment_.push_back(current_func.identificador + ':');
					/// entrada de función marco de pila...
					if (current_func.identificador != "main") {
						text_segment_.push_back("move $fp,$sp");
						text_segment_.push_back("addi $sp,$sp,-4");
						text_segment_.push_back("sw $ra,0($sp)");
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
					var_init(data_segment_, i); /// solo dios sabe si esto funciona
				}
			} else { /// convertir esto en una función
				/// es una llamada a función
				if (tokens_[i - 1].first != token_t::TYPE && (tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A)) {
					if (!FindFuncTable(identificadores_.front())) {
						throw std::runtime_error{"declaración implícita de función"};
					}
					int index{i + 2};
					while (!(tokens_[index].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[index].second) == symbol_e::PARENTESIS_C)) {
						int n_tokens{0};
						int temp{index};
						int n_parameter{0};
						while (!((tokens_[temp].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[temp].second) == symbol_e::PARENTESIS_C) || (tokens_[temp].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[temp].second) == symbol_e::COMA))) {
							n_tokens++;
							temp++;
						}
						const auto&& eval{EvaluadorExpresiones(index, n_tokens)};
						if (eval.literal) {
							text_segment_.push_back("li $a" + std::to_string(n_parameter++) + ',');
							text_segment_.back().append(eval.contenido[0]);
						} else {
							WriteBuffer(eval.contenido, write_buffer_e::END);
							text_segment_.push_back("move $a" + std::to_string(n_parameter++) + ',' + "$t0");
						}
						index += n_tokens;
					}
					text_segment_.push_back("jal " + identificadores_.front());
					identificadores_.pop();
				} else {
					/// es una declaración de variable local (o una llamada a función)
					DeclararVar(text_segment_, i);
				}
			}
		} else if (token == token_t::KEYWORD) {
			auto tipo{static_cast<keyword_e>(tokens_[i].second)};
			if (tipo == keyword_e::WHILE) {
				assert(tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A);
				text_segment_.push_back(current_func.identificador + "_while" + std::to_string(current_func.bucle_while_count_++) + ':');
				const auto&& temp{EvaluadorBool(i + 2, NextMatching(i + 1) - (i + 2))};
				if (temp.literal) throw std::runtime_error{"desgraciado, has escrito un bucle infinito"};
				WriteBuffer(temp.contenido, write_buffer_e::END);
				archivo_t buffer;
				buffer.push_back("b " + current_func.identificador + "_while" + std::to_string(current_func.bucle_while_count_ - 1));
				buffer.push_back(current_func.identificador + "_while" + std::to_string(current_func.bucle_while_count_ - 1) + '_');
				cerrar_bucles_.push(buffer);
			} else if (tipo == keyword_e::FOR) {
				assert(tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A);
				int index{0};
				int n_tokens{0};
				index = i + 2;
				n_tokens = NextPuntoYComa(index) - index;
				/// inicializar el iterador
				WriteBuffer(EvaluadorExpresiones(index, n_tokens).contenido, write_buffer_e::END);
				/// escribir la etiqueta
				text_segment_.push_back(current_func.identificador + "_for" + std::to_string(current_func.bucle_for_count_) + ':');
				/// escribir la condición
				index += n_tokens + 1;
				n_tokens = NextPuntoYComa(index) - index;
				WriteBuffer(EvaluadorBool(index, n_tokens).contenido, write_buffer_e::END);
				/// poner lo de cerrar el bucle en la pila de cerrar bucles
				index += n_tokens + 1;
				n_tokens = NextPuntoYComa(index) - index;
				cerrar_bucles_.push(EvaluadorExpresiones(index, n_tokens).contenido);
			} else if (tipo == keyword_e::IF) {
			} else if (tipo == keyword_e::ELSE) {
			} else if (tipo == keyword_e::RETURN) {
				/// evaluar la expresión y hacer branch
			}
		} else if (token == token_t::SYMBOL) {
			/// suponemos que todas las demás llaves que no sean de funciones las hemos saltado
			if (static_cast<symbol_e>(tokens_[i].second) == symbol_e::LLAVE_C) {
				if (!cerrar_bucles_.empty()) {
					WriteBuffer(cerrar_bucles_.top(), write_buffer_e::END);
					cerrar_bucles_.pop();
				} else {
					if (current_func.identificador != "main") {
						text_segment_.push_back(current_func.identificador + "_return:");
						text_segment_.push_back("lw $ra,0($sp)");
						text_segment_.push_back("move $sp,$fp");
						text_segment_.push_back("jr $ra");
					} else {
						text_segment_.push_back("li $v0,10");
						text_segment_.push_back("syscall");
					}
					current_func.identificador.clear(); /// Creo que aquí hay que borrar más cosas
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