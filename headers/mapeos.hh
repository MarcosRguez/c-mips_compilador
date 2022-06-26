/**
 * @file mapeos.hh
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 26-06-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../headers/enums.hh"

#include <map>

const std::map<char, operator_e> m_operator{
		{'=', operator_e::ASIGNACION},
		{',', operator_e::COMA},
		{'*', operator_e::PUNTERO},
		{'&', operator_e::REFERENCIA}};

const std::map<char, symbol_e> m_symbol{
		{'#', symbol_e::ALMOHADILLA},
		{'(', symbol_e::PARENTESIS_A},
		{')', symbol_e::PARENTESIS_C},
		{'[', symbol_e::CORCHETE_A},
		{']', symbol_e::CORCHETE_C},
		{'{', symbol_e::LLAVE_A},
		{'}', symbol_e::LLAVE_C},
		{'<', symbol_e::COMILLAAN_A},
		{'>', symbol_e::COMILLAAN_C},
		{';', symbol_e::PUNTOYCOMA}};

const std::map<std::string, tipos_e>
		m_tipos{
				{"void", tipos_e::VOID},
				{"char", tipos_e::CHAR},
				{"int", tipos_e::INT},
				{"float", tipos_e::FLOAT},
				{"double", tipos_e::DOUBLE}};

const std::map<std::string, keyword_e>
		m_keyword{
				{"for", keyword_e::FOR},
				{"while", keyword_e::WHILE},
				{"do", keyword_e::DO}};