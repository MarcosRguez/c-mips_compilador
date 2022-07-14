/**
 * @file compilador.hh
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief clase que compila
 * @version 0.1
 * @date 25-06-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../headers/herramientas.hh"

#include <string>
#include <queue>
#include <stack>

class Compilador {
 public:
	/// Constructor estándar
	Compilador(const std::string&);

	/// Para compilar
	std::string Compilar();

 private:
	/// Atributos
	std::string source_{};
	int source_index_{};
	archivo_t data_segment_{};
	archivo_t text_segment_{};
	tokenlist_t tokens_{};
	std::queue<std::string> identificadores_{}; /// Identificadores sin identificar
	std::queue<bool> bool_literales_{};
	std::queue<int> int_literales_{};
	std::queue<std::string> str_literales_{};
	std::stack<archivo_t> cerrar_bucles_{};
	std::vector<funciones_t> funciones_{}; /// Tabla de funciones
	std::vector<variables_t> globl_vars_{};
	funciones_t current_func{};
	/// Métodos

	void Tokenizar();
	void Preprocesar();
	void Generar();

	EvalExpr_t EvaluadorExpresiones(int, int);
	EvalExpr_t EvaluadorBool(int, int);

	void ClearRegs(registros_t& regs);
	int FuncCall(int);
	void DeclararVar(archivo_t& buffer, int& index);
	EvalExpr_t VarInit(const int);
	bool FindVarTable(const std::string&);
	bool FindFuncTable(const std::string&);
	void WriteBuffer(const archivo_t&);
};