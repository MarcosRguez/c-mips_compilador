/**
 * @file herramientas.cc
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 12-07-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../headers/compilador.hh"

int Precedencia(const operator_e op) {
	if (op == operator_e::PRODUCTO || op == operator_e::DIVISION || op == operator_e::MODULO) {
		return 4;
	} else if (op == operator_e::SUMA || op == operator_e::RESTA) {
		return 5;
	} else if (op == operator_e::MENORQUE || op == operator_e::MAYORQUE || op == operator_e::MENORIGUAL || op == operator_e::MAYORIGUAL) {
		return 6;
	} else if (op == operator_e::IGUALQUE || op == operator_e::DESIGUALQUE) {
		return 7;
	} else if (op == operator_e::COMA) {
		return 17;
	} else {
		throw std::runtime_error{"operador sin precedencia conocida."};
	}
}

int Aridad(const operator_e op) {
	if (op == operator_e::NOT || op == operator_e::INCREMENTO || op == operator_e::DECREMENTO || op == operator_e::PUNTERO || op == operator_e::REFERENCIA) {
		return 1;
	} else {
		return 2;
	}
}