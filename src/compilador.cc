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

EvalExpr_t Compilador::EvaluadorExpresiones(const int index, const int n_tokens) {
	/// caso base: literal
	/// caso base: identificador
	/// caso base: funccall
	EvalExpr_t resultado;
	if (n_tokens == 0) return resultado;
	if (n_tokens == 1) {
		assert(tokens_[index].first == token_t::LITERAL || tokens_[index].first == token_t::IDENTIFIER);
		if (tokens_[index].first == token_t::LITERAL) {
			resultado.is_literal = true;
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
			return resultado;
		} else if (tokens_[index].first == token_t::IDENTIFIER) {
			resultado.is_register = true;
			if (!FindVarTable(identificadores_.front())) {
				throw std::runtime_error{"variable no declarada"};
			}
			for (const auto& i : current_func.variables_) {
				if (i.identificador == identificadores_.front()) {
					resultado.out_reg = i.registro;
					identificadores_.pop();
					return resultado;
				}
			}
		} else {
			throw std::runtime_error{"en fin, la hipocresìa."};
		}
	}
	/// expresión compuesta
	/// comprobar si el un funccall
	if (tokens_[index].first == token_t::IDENTIFIER && (tokens_[index + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[index + 1].second) == symbol_e::PARENTESIS_A)) {
		if (NextMatching(index + 1) == index + n_tokens - 1) {
			FuncCall(index);
			resultado.is_register = true;
			resultado.out_reg = "$v0";
			return resultado;
		}
	}
	// /// hacemos una copia
	// std::vector<std::pair<token_t, unsigned>> copia;
	// for (int i{index}; i < index + n_tokens; i++) {
	// 	copia.push_back(tokens_[i]);
	// }
	/// encontramos el operador con más precedencia
	int op_index{InxOperador(index, n_tokens)};
	/// si el operador en cuestión es de ariedad 2
	if (Aridad(static_cast<operator_e>(tokens_[op_index].second)) == 2) {
		/// evaluamos lo primero y sustituimos el primer operando por el outreg
		EvalExpr_t operando1;
		/// comprobamos si la expr está entre paréntesis
		if (tokens_[op_index - 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[op_index - 1].second) == symbol_e::PARENTESIS_C) {
			/// comprobamos si funca las maracas 🧛‍♂️🧜‍♀️🧶
			if (tokens_[PrevMatching(op_index - 1) - 1].first == token_t::IDENTIFIER) {
				operando1 = EvaluadorExpresiones(PrevMatching(op_index - 1) - 1, (op_index - 1) - PrevMatching(op_index - 1) - 1);
			} else {
				operando1 = EvaluadorExpresiones(PrevMatching(op_index - 1) + 1, (op_index - 1) - PrevMatching(op_index - 1) + 1);
			}
		} else {
			operando1 = EvaluadorExpresiones(op_index - 1, 1);
			if (operando1.is_literal) {
				if (operando1.contenido[0] == "0") {
					operando1.out_reg = "$zero";
				} else {
					operando1.out_reg = EncontrarRegistroLibre(current_func.registros_temporales_);
					current_func.registros_temporales_.at(operando1.out_reg) = true;
				}
				operando1.is_register = true;
			}
		}
		/// write buffer
		if (operando1.is_literal && operando1.is_register) {
			resultado.contenido.push_back("li " + operando1.out_reg + ',' + operando1.contenido[0]);
		} else {
			WriteBuffer(operando1.contenido);
		}
		/// evaluamos los segundo y sustituimos el primer operando por el outreg
		EvalExpr_t operando2;
		/// Es una expr entre paréntesis
		if (tokens_[op_index + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[op_index + 1].second) == symbol_e::PARENTESIS_A) {
			operando2 = EvaluadorExpresiones(op_index + 2, NextMatching(op_index + 1) - op_index + 1);
		} /// Es una funccall
		else if (tokens_[op_index + 1].first == token_t::IDENTIFIER && (tokens_[op_index + 2].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[op_index + 2].second) == symbol_e::PARENTESIS_A)) {
			operando2 = EvaluadorExpresiones(op_index + 1, NextMatching(op_index + 2) - op_index + 1);
		} else {
			operando2 = EvaluadorExpresiones(op_index + 1, 1);
			if (operando2.is_literal) {
				if (operando2.contenido[0] == "0") {
					operando2.out_reg = "$zero";
				} else {
					operando2.out_reg = EncontrarRegistroLibre(current_func.registros_temporales_);
					current_func.registros_temporales_.at(operando2.out_reg) = true;
				}
				operando2.is_register = true;
			}
		}
		/// write buffer
		if (operando2.is_literal && operando2.is_register) {
			resultado.contenido.push_back("li " + operando2.out_reg + ',' + operando2.contenido[0]);
		} else {
			WriteBuffer(operando2.contenido);
		}
		/// operamos el operando y sustituimos la operación por el outreg
		resultado.out_reg = EncontrarRegistroLibre(current_func.registros_temporales_);
		resultado.contenido.push_back(GetInstruction(static_cast<operator_e>(tokens_[op_index].second)) + ' ' + resultado.out_reg + ',' + operando1.out_reg + ',' + operando2.out_reg);
		/// liberamos los registros temporales
		ClearRegs(current_func.registros_temporales_);
	}
	return resultado;
}
void Compilador::ClearRegs(registros_t& regs) {
	for (auto& i : regs) {
		i.second = false;
	}
}

int Compilador::InxOperador(const int index, const int n_tokens) {
	int resultado{-1};
	int precedencia{INT16_MAX};
	for (int i{index}; i < index + n_tokens; i++) {
		if (tokens_[i].first == token_t::OPERATOR) {
			if (Precedencia(static_cast<operator_e>(tokens_[i].second)) < precedencia) {
				resultado = i;
			}
		} else if (tokens_[i].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i].second) == symbol_e::PARENTESIS_A) {
			i = NextMatching(i);
		}
	}
	return resultado;
}

bool Compilador::FindVarTable(const std::string& nombre) {
	bool existe{false};
	for (const auto& i : current_func.variables_) {
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

EvalExpr_t Compilador::EvaluadorBool(int index, int n_tokens) {
	EvalExpr_t resultado;
	if (n_tokens == 0) return resultado;
	if (n_tokens == 1) {
		assert(tokens_[index].first == token_t::LITERAL && static_cast<literal_e>(tokens_[index].second) == literal_e::BOOL);
		resultado.is_literal = true;
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

void Compilador::WriteBuffer(const archivo_t& buffer) {
	if (buffer.empty()) return;
	for (const auto& i : buffer) {
		text_segment_.push_back(i);
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

int Compilador::PrevMatching(int index) {
	assert(tokens_[index].first == token_t::SYMBOL);
	int apertura{0};
	int cerrura{1};
	switch (static_cast<symbol_e>(tokens_[index].second)) {
		case (symbol_e::PARENTESIS_A):
			while (apertura != cerrura) {
				switch (static_cast<symbol_e>(tokens_[--index].second)) {
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
				switch (static_cast<symbol_e>(tokens_[--index].second)) {
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
				switch (static_cast<symbol_e>(tokens_[--index].second)) {
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
				switch (static_cast<symbol_e>(tokens_[--index].second)) {
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
 * @param index	el índice del identificador
 * @return devuelve un literal en el caso o si no un registro
 */
EvalExpr_t Compilador::VarInit(const int index) {
	assert(tokens_[index].first == token_t::IDENTIFIER);
	EvalExpr_t resultado;
	if (tokens_[index + 1].first == token_t::SYMBOL) {
		if (static_cast<symbol_e>(tokens_[index + 1].second) == symbol_e::PUNTOYCOMA) {
			/// no está init
			resultado.out_reg = "$zero";
			resultado.is_register = true;
		} else if (static_cast<symbol_e>(tokens_[index + 1].second) == symbol_e::LLAVE_A) {
			/// evaluar la expresión entre los corchetes
			return EvaluadorExpresiones(index + 2, NextMatching(index + 1) - (index + 1) - 1);
		}
	} else if (tokens_[index + 1].first == token_t::OPERATOR) {
		if (static_cast<operator_e>(tokens_[index + 1].second) == operator_e::ASIGNACION) {
			return EvaluadorExpresiones(index + 2, NextPuntoYComa(index + 2) - (index + 2));
		} else {
			throw std::runtime_error{"error de sintaxis"};
		}
	} else {
		throw std::runtime_error{"error de sintaxis"};
	}
	return resultado;
}

/**
 * @brief Aquí deberíamos escribir lo de poner en un buffer en espera para evaluar el var init primero
 * @param buffer
 * @param index
 */
void Compilador::DeclararVar(archivo_t& buffer, int& index) {
	variables_t&& temp{identificadores_.front(), static_cast<tipos_e>(tokens_[index - 1].second), EncontrarRegistroLibre(current_func.registros_salvados_)};
	current_func.variables_.push_back(temp);
	identificadores_.pop();
	current_func.registros_salvados_.at(current_func.variables_.back().registro) = true;
	/// comprobamos si está inizializado
	const auto&& eval{VarInit(index)};
	/// lo ponemos
	if (eval.is_literal) {
		buffer.push_back("li " + temp.registro + ',' + eval.out_reg);
	} else if (eval.is_register) {
		buffer.push_back("move " + temp.registro + ',' + eval.out_reg);
	} else {
		WriteBuffer(eval.contenido);
		buffer.push_back("move " + temp.registro + ',' + eval.out_reg);
	}
	/// saltar los tokens analizados
	index = NextPuntoYComa(index);
}

int Compilador::FuncCall(int index) {
	if (!FindFuncTable(identificadores_.front())) {
		throw std::runtime_error{"declaración implícita de función"};
	}
	index += 2;
	int n_parameter{0};
	while (!(tokens_[index].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[index].second) == symbol_e::PARENTESIS_C)) {
		if (tokens_[index].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[index].second) == symbol_e::COMA) {
			index++;
		}
		int n_tokens{0};
		int temp{index};
		while (!(tokens_[temp].first == token_t::SYMBOL && (static_cast<symbol_e>(tokens_[temp].second) == symbol_e::PARENTESIS_C || static_cast<symbol_e>(tokens_[temp].second) == symbol_e::COMA))) {
			n_tokens++;
			temp++;
		}
		const auto&& eval{EvaluadorExpresiones(index, n_tokens)};
		if (eval.is_literal) {
			text_segment_.push_back("li $a" + std::to_string(n_parameter++) + ',');
			text_segment_.back().append(eval.contenido[0]);
		} else if (eval.is_register) {
			text_segment_.push_back("move $a" + std::to_string(n_parameter++) + ',');
			text_segment_.back().append(eval.out_reg);
		} else {
			WriteBuffer(eval.contenido);
			text_segment_.push_back("move $a" + std::to_string(n_parameter++) + ',' + "$t0");
		}
		index += n_tokens;
	}
	text_segment_.push_back("jal " + identificadores_.front());
	identificadores_.pop();
	return index;
}

/**
 * @brief Genera el código fuente,
 * recorre los tokens previamente generados y hace cosas según lo que se encuentra
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
					int index{i + 2};
					int n_params{0};
					while (!(tokens_[index].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[index].second) == symbol_e::PARENTESIS_C)) {
						if (tokens_[index].first == token_t::IDENTIFIER) {
							assert(tokens_[index - 1].first == token_t::TYPE);
							assert(tokens_[index + 1].first == token_t::SYMBOL && (static_cast<symbol_e>(tokens_[index + 1].second) == symbol_e::COMA || static_cast<symbol_e>(tokens_[index + 1].second) == symbol_e::PARENTESIS_C));
							current_func.variables_.push_back({identificadores_.front(), static_cast<tipos_e>(tokens_[index - 1].second), "$a" + std::to_string(n_params++)});
							identificadores_.pop();
						}
						index++;
					}
					i = ++index;
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
				else if ((tokens_[i + 1].first == token_t::OPERATOR && static_cast<operator_e>(tokens_[i + 1].second) == operator_e::ASIGNACION) || (tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::LLAVE_A)) {
					variables_t var;
					/// data_Segmetn
					data_segment_.push_back(identificadores_.front() + ':');
					var.identificador = identificadores_.front();
					identificadores_.pop();
					const auto& temp{static_cast<tipos_e>(tokens_[i - 1].second)};
					if (temp == tipos_e::INT) {
						var.tipo = tipos_e::INT;
						data_segment_.back().append(".word	");
					} else if (temp == tipos_e::FLOAT) {
						var.tipo = tipos_e::FLOAT;
						data_segment_.back().append(".float	");
					} else if (temp == tipos_e::DOUBLE) {
						var.tipo = tipos_e::DOUBLE;
						data_segment_.back().append(".double	");
					} else if (temp == tipos_e::CHAR) {
						var.tipo = tipos_e::CHAR;
						data_segment_.back().append(".byte	");
					} else {
						throw std::runtime_error{"tipo de var global no soportado"};
					}
					globl_vars_.push_back(var);
					VarInit(i); /// solo dios sabe si esto funciona
				}
			} else {
				/// es una llamada a función
				if (tokens_[i - 1].first != token_t::TYPE && (tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A)) {
					i = FuncCall(i);
				} else {
					/// es una declaración de variable local
					DeclararVar(text_segment_, i);
				}
			}
		} else if (token == token_t::KEYWORD) {
			auto tipo{static_cast<keyword_e>(tokens_[i].second)};
			if (tipo == keyword_e::WHILE) {
				assert(tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A);
				text_segment_.push_back(current_func.identificador + "_while" + std::to_string(current_func.bucle_while_count_++) + ':');
				const auto&& temp{EvaluadorBool(i + 2, NextMatching(i + 1) - (i + 2))};
				if (temp.is_literal) throw std::runtime_error{"desgraciado, has escrito un bucle infinito"};
				WriteBuffer(temp.contenido);
				archivo_t buffer;
				buffer.push_back("b " + current_func.identificador + "_while" + std::to_string(current_func.bucle_while_count_ - 1));
				buffer.push_back(current_func.identificador + "_while" + std::to_string(current_func.bucle_while_count_ - 1) + '_');
				cerrar_bucles_.push(buffer);
			} else if (tipo == keyword_e::FOR) {
				assert(tokens_[i + 1].first == token_t::SYMBOL && static_cast<symbol_e>(tokens_[i + 1].second) == symbol_e::PARENTESIS_A);
				int index{i + 2};
				int n_tokens{NextPuntoYComa(index) - index};
				/// inicializar el iterador
				WriteBuffer(EvaluadorExpresiones(index, n_tokens).contenido);
				/// escribir la etiqueta
				text_segment_.push_back(current_func.identificador + "_for" + std::to_string(current_func.bucle_for_count_) + ':');
				/// escribir la condición
				index += n_tokens + 1;
				n_tokens = NextPuntoYComa(index) - index;
				WriteBuffer(EvaluadorBool(index, n_tokens).contenido);
				/// poner lo de cerrar el bucle en la pila de cerrar bucles
				index += n_tokens + 1;
				n_tokens = NextPuntoYComa(index) - index;
				cerrar_bucles_.push(EvaluadorExpresiones(index, n_tokens).contenido);
			} else if (tipo == keyword_e::IF) {
			} else if (tipo == keyword_e::ELSE) {
			} else if (tipo == keyword_e::RETURN) {
				/// evaluar la expresión y hacer branch
				int n_tokens{NextPuntoYComa(i + 1) - (i + 1)};
				const auto&& eval{EvaluadorExpresiones(i + 1, n_tokens)};
				if (eval.is_literal) {
					text_segment_.push_back("li $v0," + eval.contenido[0]);
				} else if (eval.is_register) {
					text_segment_.push_back("move $v0," + eval.out_reg);
				} else {
					WriteBuffer(eval.contenido);
					text_segment_.push_back("move $v0," + eval.out_reg);
				}
				text_segment_.push_back("b " + current_func.identificador + '_');
				i += n_tokens;
			}
		} else if (token == token_t::SYMBOL) {
			/// suponemos que todas las demás llaves que no sean de funciones las hemos saltado
			if (static_cast<symbol_e>(tokens_[i].second) == symbol_e::LLAVE_C) {
				if (!cerrar_bucles_.empty()) {
					WriteBuffer(cerrar_bucles_.top());
					cerrar_bucles_.pop();
				} else {
					if (current_func.identificador != "main") {
						text_segment_.push_back(current_func.identificador + "_:");
						text_segment_.push_back("lw $ra,0($sp)");
						text_segment_.push_back("move $sp,$fp");
						text_segment_.push_back("jr $ra");
					} else {
						text_segment_.push_back("main_:");
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