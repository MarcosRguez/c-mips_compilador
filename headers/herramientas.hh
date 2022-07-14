/**
 * @file herramientas.hh
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief Cosas útiles no genéricas
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

/// Alias de tipo

using registros_t = std::map<std::string, bool>;
using tokenlist_t = std::vector<std::pair<token_t, unsigned>>;

/// Structs

struct EvalExpr_t {
	archivo_t contenido{};
	bool is_literal{false};
	bool is_register{false};
	bool direct_init{false};
	std::string out_reg{};
};

struct variables_t {
	std::string identificador{};
	tipos_e tipo{};
	std::string registro{};
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
	int if_count_{};
};

/// Funciones

int Precedencia(const operator_e op);
int Aridad(const operator_e op);
std::string GetInstruction(const operator_e op);
int OpArgs(const operator_e op);
int InxOperador(const tokenlist_t& cosa);
int NextMatching(const tokenlist_t& cosa, int index);
int PrevMatching(const tokenlist_t& cosa, int index);
std::string EncontrarRegistroLibre(
		const registros_t& registros);
int NextPuntoYComa(const tokenlist_t& cosa, int index);
void ClearRegs(registros_t& regs);
