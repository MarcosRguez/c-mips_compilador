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

using registros_t = std::map<std::string, bool>;

struct variables_t {
	tipos_e tipo;
	std::string nombre;
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
	int bucle_while_count_{};
	int bucle_for_count_{};
	registros_t registros_temporales_{};
	registros_t registros_salvados_{};
	/// Métodos

	void Tokenizar();
	void Preprocesar();
	void Generar();

	archivo_t EvaluadorExpresiones(int, int);
	archivo_t EvaluadorBool(int, int);

	std::string EncontrarRegistroLibre(
			const registros_t&) const;
	int NextMatching(int);
	void WriteBuffer(const archivo_t&);

	// std::string& linea();

	/// Chuchangas friend?

	void Funciones();
	void LlamadaFunciones();
	void Ostream();
	void Istream();
	void For();
	void While();
	void Elif();
	void ReturnBool();
	void Return();
	void GlobalVar();
	void LocalVar();
};