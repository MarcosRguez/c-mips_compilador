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

#include "../headers/utilidades.hh"
#include "../headers/enums.hh"

#include <string>
#include <map>
#include <queue>
#include <stack>

using registros_t = std::map<std::string, bool>;

struct variables_t {
	tipos_e tipo;
	std::string nombre;
	std::string registro;
};

struct Eval_f_t {
	archivo_t contenido{};
	bool literal{false};
};

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
	std::vector<std::pair<token_t, unsigned>> tokens_{};
	std::queue<std::string> identificadores_{};
	std::queue<std::string> str_literales_{};
	std::queue<int> int_literales_{};
	std::queue<bool> bool_literales_{};
	int bucle_while_count_{};
	int bucle_for_count_{};
	registros_t registros_temporales_{};
	registros_t registros_salvados_{};
	std::vector<variables_t> variables_{};
	std::stack<int> cerrar_bucles_{};
	/// Métodos

	void Tokenizar();
	void Preprocesar();
	void Generar();

	Eval_f_t EvaluadorExpresiones(int, int);
	Eval_f_t EvaluadorBool(int, int);

	std::string EncontrarRegistroLibre(
			const registros_t&) const;
	int NextMatching(int);
	int NextPuntoYComa(int);
	void WriteBuffer(const archivo_t&, bool);
};