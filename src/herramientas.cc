/**
 * @file herramientas.cc
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief Implementación de funcioncitas
 * @version 0.1
 * @date 12-07-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../headers/compilador.hh"

#include <cassert>
#include <stdexcept>

int Precedencia(const operator_e op) {
	if (op == operator_e::NOT) {
		return 2;
	} else if (op == operator_e::PRODUCTO || op == operator_e::DIVISION || op == operator_e::MODULO) {
		return 4;
	} else if (op == operator_e::SUMA || op == operator_e::RESTA) {
		return 5;
	} else if (op == operator_e::MENORQUE || op == operator_e::MAYORQUE || op == operator_e::MENORIGUAL || op == operator_e::MAYORIGUAL) {
		return 6;
	} else if (op == operator_e::IGUALQUE || op == operator_e::DESIGUALQUE) {
		return 7;
	} else if (op == operator_e::AND) {
		return 12;
	} else if (op == operator_e::OR) {
		return 13;
	} else if (op == operator_e::ASIGNACION) {
		return 15;
	} else if (op == operator_e::COMA) {
		return 17;
	} else {
		throw std::runtime_error{"operador sin precedencia conocida."};
	}
	return 0;
}

int Aridad(const operator_e op) {
	if (op == operator_e::NOT || op == operator_e::INCREMENTO || op == operator_e::DECREMENTO || op == operator_e::PUNTERO || op == operator_e::REFERENCIA) {
		return 1;
	} else {
		return 2;
	}
}

int OpArgs(const operator_e op) {
	if (op == operator_e::AND || op == operator_e::OR || op == operator_e::NOT) {
		return 1;
	} else if (op == operator_e::ASIGNACION) {
		return 2;
	} else {
		return 3;
	}
}

/**
 * @brief Devuelve el índice de coso que cierra
 * @param index
 * @return int
 */
int NextMatching(const tokenlist_t& cosa, int index) {
	assert(cosa[index].first == token_t::SYMBOL);
	int apertura{1};
	int cerrura{0};
	unsigned code{cosa[index].second};
	while (apertura != cerrura) {
		index++;
		if (cosa[index].first == token_t::SYMBOL) {
			if (cosa[index].second == code) {
				apertura++;
			} else if (cosa[index].second == code + 1) {
				cerrura++;
			}
		}
	}
	return index;
}

int PrevMatching(const tokenlist_t& cosa, int index) {
	assert(cosa[index].first == token_t::SYMBOL);
	int apertura{0};
	int cerrura{1};
	unsigned code{cosa[index].second};
	while (apertura != cerrura) {
		index++;
		if (cosa[index].first == token_t::SYMBOL) {
			if (cosa[index].second == code) {
				apertura++;
			} else if (cosa[index].second == code - 1) {
				cerrura++;
			}
		}
	}
	return index;
}

int InxOperador(const tokenlist_t& cosa) {
	int resultado{-1};
	int precedencia{INT16_MAX};
	for (int i{0}; i < cosa.size(); i++) {
		if (cosa[i].first == token_t::OPERATOR) {
			if (Precedencia(static_cast<operator_e>(cosa[i].second)) < precedencia) {
				precedencia = Precedencia(static_cast<operator_e>(cosa[i].second));
				resultado = i;
			}
		} else if (cosa[i].first == token_t::SYMBOL && static_cast<symbol_e>(cosa[i].second) == symbol_e::PARENTESIS_A) {
			i = NextMatching(cosa, i);
		}
	}
	return resultado;
}

bool IsLogigOp(const operator_e op) {
	if (op == operator_e::AND || op == operator_e::OR || op == operator_e::NOT) {
		return true;
	}
	return false;
}

std::string GetInstruction(const operator_e op) {
	switch (op) {
		case (operator_e::ASIGNACION):
			return "move";
			break;
		case (operator_e::SUMA):
			return "add";
			break;
		case (operator_e::RESTA):
			return "sub";
			break;
		case (operator_e::PRODUCTO):
			return "mul";
			break;
		case (operator_e::DIVISION):
			return "div";
			break;
		case (operator_e::MENORIGUAL):
			return "sle";
			break;
		case (operator_e::MENORQUE):
			return "slt";
			break;
		case (operator_e::MAYORIGUAL):
			return "sge";
			break;
		case (operator_e::MAYORQUE):
			return "sgt";
			break;
		default:
			break;
	}
	return "";
}

std::string EncontrarRegistroLibre(
		const registros_t& registros) {
	for (const auto& i : registros) {
		if (i.second == false) {
			return i.first;
		}
	}
	return NULL;
}

/**
 * @brief Establece a false (no usado) todos los registros de la lista pasada
 * @param regs lista de registros
 */
void ClearRegs(registros_t& regs) {
	for (auto& i : regs) {
		i.second = false;
	}
}

/**
 * @brief Ecuentra el próximo punto y coma
 * @param cosa lista de tokens
 * @param index índice al partir el cual buscar
 * @return índice del punto y coma
 * @note tengo que hacer que salte los literales de char y str
 */
int NextPuntoYComa(const tokenlist_t& cosa, int index) {
	while (!(cosa[index].first == token_t::SYMBOL && static_cast<symbol_e>(cosa[index].second) == symbol_e::PUNTOYCOMA)) {
		index++;
	}
	return index;
}