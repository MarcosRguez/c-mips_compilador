/**
 * @file utilidades.cc
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief Implementación de utilidades genéricas
 * @version 0.1
 * @date 05-05-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../headers/utilidades.hh"

#include <exception>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

/**
 * @brief Esta función lee un archivo y guarda cada línea en una entrada de un
 * vector.
 * @param[in] filename El nombre del archivo.
 * @return Devuelve un vector de cadenas donde cada una corresponde a una línea.
 */
archivo_t LeerArchivo(const std::string& filename) {
	std::fstream file;
	file.open(filename, std::fstream::in);
	if (!file.is_open()) {
		throw std::domain_error("¿Archivo no encontrado!");
	}
	std::string linea;
	archivo_t lineas;
	while (std::getline(file, linea)) {
		lineas.push_back(linea);
	}
	file.close();
	return lineas;
}

/**
 * @brief Hace lo que piensas que hace
 * @param filename Nombre de archivo
 * @return Cadena con el contenido del archivo
 */
std::string LeerArchivoSTR(const std::string& filename) {
	std::fstream file;
	file.open(filename, std::fstream::in);
	std::stringstream str;
	str << file.rdbuf();
	file.close();
	return str.str();
}

/**
 * @brief Elimina los comentarios de doble barra al principio de cada línea
 * @param[out] archivo El archivo a operar.
 */
void EliminarComentarios(archivo_t& archivo) {
	for (std::size_t i = 0; i < archivo.size(); i++) {
		// Comentarios
		int posicion{-1};
		posicion = archivo[i].find("//");
		if (posicion != -1) {
			archivo[i].erase(posicion, archivo[i].size());
		}
	}
	/*  Adaptar
		posicion = -1;
		posicion = linea.find("#");
		if (posicion != -1) {
			archivo[i].replace(posicion, 1, "//");
		}
	*/
}

/**
 * @brief Esta función crea un archivo.
 * @param[in] filename_out El nombre del archivo.
 * @param[in] contenido El contenido del archivo en forma de vector de cadenas,
 * donde cada índice es una línea.
 */
void CrearArchivo(const std::string& filename_out, const std::string& ruta,
									const archivo_t& contenido) {
	std::fstream file(ruta);
	file.close();
	file.open(filename_out, std::fstream::out | std::fstream::trunc);
	for (std::size_t i = 0; i < contenido.size(); i++) {
		file << contenido[i] << std::endl;
	}
	file.close();
}

/**
 * @brief Comprueba si los paréntesis, corchetes y llaves se cierran correctamente
 * @param sample texto
 * @return true
 * @return false
 */
bool MatchingBrackets(const std::string& sample) {
	int parentesis = 0;
	int corchetes = 0;
	int llaves = 0;
	/// 1: parentesis++ ; 2: corchetes++ ; 3: llaves++ ;
	/// -1: parentesis-- ; -2: corchetes-- ; -3: llaves-- ;
	int last_increased = 0;
	int pre_last_increased = 0;
	for (std::size_t i = 0; i < sample.length(); i++) {
		pre_last_increased = last_increased;
		/// conteo de cada tipo
		if (sample[i] == '(') {
			parentesis++;
			last_increased = 1;
		} else if (sample[i] == ')') {
			parentesis--;
			last_increased = -1;
		} else if (sample[i] == '[') {
			corchetes++;
			last_increased = 2;
		} else if (sample[i] == ']') {
			corchetes--;
			last_increased = -2;
		} else if (sample[i] == '{') {
			llaves++;
			last_increased = 3;
		} else if (sample[i] == '}') {
			llaves--;
			last_increased = -3;
		}
		/// incongruencia
		if ((parentesis < 0) || (corchetes < 0) || (llaves < 0)) {
			return false;
		}
		/// inanidación
		if ((last_increased < 0) && (pre_last_increased > 0) && (pre_last_increased != abs(last_increased)) && (i > 0)) {
			return false;
		}
	}
	/// están bien cerrados
	if ((parentesis == 0) && (corchetes == 0) && (llaves == 0)) {
		return true;
	}
	/// no están bien cerrados
	return false;
}

/**
 * @brief Crear archivo a partir de cadena
 * @param filename_out nombre de archivo resultante
 * @param contenido texto
 */
void CrearArchivo(const std::string& filename_out,
									const std::string& contenido) {
	std::fstream file;
	file.open(filename_out, std::fstream::out | std::fstream::trunc);
	file << contenido;
	file.close();
}

/**
 * @brief Convierte un texto en formato vector de cadenas a una sola cadena.
 * @param[in] texto El texto objetivo.
 * @return Una cadena con el contenido del vector.
 */
std::string LineaUnica(const archivo_t& texto) {
	std::string linea;
	for (std::size_t i = 0; i < texto.size(); i++) {
		linea += texto[i];
		if (i != (texto.size() - 1)) {
			linea += '\n';
		}
	}
	return linea;
}

/**
 * @brief Elimina las líneas vacías del archivo
 * @param[out] archivo El archivo a operar.
 */
void EliminarLineasVacias(archivo_t& archivo) {
	for (std::size_t i = 0; i < archivo.size(); i++) {
		if (archivo[i] == "" || archivo[i] == "\n") {
			archivo.erase(archivo.begin() + i);
			i--;
		}
	}
}

/**
 * @brief Elimina los caracteres maliciosos como "\r"
 * @param[out] archivo El archivo a operar.
 */
void EliminarCaracteresFantasmas(archivo_t& archivo) {
	for (std::size_t i = 0; i < archivo.size(); i++) {
		while (archivo[i].back() == ' ') {
			archivo[i].pop_back();
		}
	}
	for (std::size_t i = 0; i < archivo.size(); i++) {
		int posicion = -1;
		posicion = archivo[i].find("\r");
		if (posicion != -1) {
			archivo[i].pop_back();
		}
	}
}

/**
 * @brief Elimina la indentación del archivo
 * @param archivo objetivo
 */
void EliminarIndentacion(archivo_t& archivo) {
	for (std::size_t i{0}; i < archivo.size(); ++i) {
		while (archivo[i][0] == ' ' || archivo[i][0] == '\t') {
			archivo[i].erase(0, 1);
		}
	}
}

void EliminarCorchetes(archivo_t& archivo) {
	for (unsigned i{0}; i < archivo.size(); i++) {
		if (archivo[i].back() == '{' || archivo[i].back() == '}') {
			archivo[i].pop_back();
		}
	}
}

void EliminarEspacios(archivo_t& archivo) {
	for (unsigned i{0}; i < archivo.size(); i++) {
		while (archivo[i].back() == ' ') {
			archivo[i].pop_back();
		}
	}
}

void EliminarBarrasInvertidas(archivo_t& archivo) {
	for (std::size_t i{0}; i < archivo.size(); ++i) {
		for (std::size_t j{0}; j < archivo[i].size(); ++j) {
			if (archivo[i][j] == '\\') {
				archivo[i].erase(j, 1);
			}
		}
	}
}

/// Esto de aquí abajo está obsoleto

// unsigned EncontrarFinFuncion(const archivo_t& archivo, unsigned index) {
// 	std::string linea;
// 	unsigned indent_current;
// 	do {
// 		index++;
// 		linea = archivo[index];
// 		indent_current = linea.find_first_not_of(' ');
// 	} while (indent_current != 0 || index >= archivo.size());
// 	return index;
// }

// void EliminarDirectivas(archivo_t& archivo) {
// 	for (unsigned i{0}; i < archivo.size(); i++) {
// 		if (archivo[i].front() == '#') {
// 			archivo.erase(archivo.begin() + i);
// 			i--;
// 		}
// 	}
// }

// unsigned EncontrarTextSegment(archivo_t& archivo) {
// 	for (unsigned i{0}; i < archivo.size(); i++) {
// 		if (archivo[i] == ".text") {
// 			return i;
// 		}
// 	}
// 	return 0;
// }