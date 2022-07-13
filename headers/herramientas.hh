/**
 * @file herramientas.hh
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 13-07-2022
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
#include <stack>

using registros_t = std::map<std::string, bool>;

int Precedencia(const operator_e op);
int Aridad(const operator_e op);
std::string GetInstruction(const operator_e op);

struct variables_t {
	std::string identificador{};
	tipos_e tipo{};
	std::string registro{};
};

enum class write_buffer_e {
	END,
	ANS
};

struct EvalExpr_t {
	archivo_t contenido{};
	bool is_literal{false};
	bool is_register{false};
	std::string out_reg{};
};

struct funciones_t {
	std::string identificador{};
	tipos_e return_tipo{};
	std::vector<tipos_e> args_{};
	std::vector<variables_t> variables_;
	registros_t registros_temporales_{};
	registros_t registros_salvados_{};
	int bucle_while_count_{};
	int bucle_for_count_{};
};