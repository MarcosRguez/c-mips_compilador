/**
 * @file utilidades.hh
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-04-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <string>
#include <vector>

typedef std::vector<std::string> archivo_t;

archivo_t LeerArchivo(const std::string&);
void EliminarComentarios(archivo_t&);
void CrearArchivo(const std::string&, const std::string&, const archivo_t&);
void CrearArchivo(const std::string&, const std::string&);
std::string LineaUnica(const archivo_t&);
std::string LeerArchivoSTR(const std::string& filename);
void EliminarLineasVacias(archivo_t&);
void EliminarCaracteresFantasmas(archivo_t&);
void EliminarIndentacion(archivo_t&);
void EliminarEspacios(archivo_t&);
bool MatchingBrackets(const std::string& sample);
void EliminarCorchetes(archivo_t&);
void EliminarBarrasInvertidas(archivo_t&);
unsigned EncontrarFinFuncion(const archivo_t&, unsigned);
void EliminarDirectivas(archivo_t&);
unsigned EncontrarTextSegment(archivo_t&);