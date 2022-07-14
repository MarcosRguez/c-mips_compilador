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
	std::string source_{}; /// esto a lo mejor hace falta conservarlo más tarde para los operadores unarios o solucionar la correlación token/línea
	int source_index_{};
	archivo_t data_segment_{};
	archivo_t text_segment_{};
	tokenlist_t tokens_{};											/// Lista de tokens generados
	std::queue<std::string> identificadores_{}; /// Identificadores sin identificar
	std::stack<archivo_t> cerrar_bucles_{};			/// Pila de intrucciones para cerrar bloques
	std::vector<funciones_t> funciones_{};			/// Tabla de funciones
	std::vector<variables_t> globl_vars_{};			/// lista de variables globales
	funciones_t current_func{};									/// Función actual
	/// Colas de literales
	std::queue<bool> bool_l_{};
	std::queue<char> char_l_{};
	std::queue<int> int_l_{};
	std::queue<float> float_l_{};
	std::queue<double> double_l_{};
	std::queue<std::string> str_l_{};
	/// Métodos

	/// Métodos Secuenciales
	void Preprocesar();
	void Tokenizar();
	void Generar();

	/// Evaluador de expresiones
	EvalExpr_t EvaluadorExpresiones(const int, const int);
	EvalExpr_t EvaluadorBool(int, int); /// voy a combinarlos

	/// Métodos que hacen cosas
	int FuncCall(int);
	void DeclararVar(archivo_t& buffer, int& index);
	EvalExpr_t VarInit(const int);

	/// Métodos de ayuda
	void WriteBuffer(const archivo_t&);
	bool FindFuncTable(const std::string&) const;
	bool FindVarTable(const std::string&) const;
};