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

const std::map<std::string, operator_e> m_operator{
		/// aritméticos
		{"+", operator_e::SUMA},
		{"-", operator_e::RESTA},
		{"*", operator_e::PRODUCTO},
		{"/", operator_e::DIVISION},
		{"%", operator_e::MODULO},
		/// aritméticos de asignación
		{"+=", operator_e::SUMA_A},
		{"-=", operator_e::RESTA_A},
		{"*=", operator_e::PRODUCTO_A},
		{"/=", operator_e::DIVISION_A},
		{"%=", operator_e::MODULO_A},
		/// incremento y decremento
		{"++", operator_e::INCREMENTO},
		{"--", operator_e::DECREMENTO},
		/// Lógica
		{"!", operator_e::NOT},
		{"&&", operator_e::AND},
		{"||", operator_e::OR},
		/// comparación
		{"<", operator_e::MENORQUE},
		{">", operator_e::MAYORQUE},
		{"<=", operator_e::MENORIGUAL},
		{">=", operator_e::MAYORIGUAL},
		{"==", operator_e::IGUALQUE},
		{"!=", operator_e::DESIGUALQUE},
		/// otros
		{"=", operator_e::ASIGNACION},
		{",", operator_e::COMA},
		{"*", operator_e::PUNTERO},
		{"&", operator_e::REFERENCIA},
		/// miscelánea
		{"<?", operator_e::MINIMO},
		{">?", operator_e::MAXIMO},
};

const std::map<char, symbol_e> m_symbol{
		{',', symbol_e::COMA},
		{'#', symbol_e::ALMOHADILLA},
		{'(', symbol_e::PARENTESIS_A},
		{')', symbol_e::PARENTESIS_C},
		{'[', symbol_e::CORCHETE_A},
		{']', symbol_e::CORCHETE_C},
		{'{', symbol_e::LLAVE_A},
		{'}', symbol_e::LLAVE_C},
		{'<', symbol_e::COMILLAAN_A},
		{'>', symbol_e::COMILLAAN_C},
		{';', symbol_e::PUNTOYCOMA},
		{'\'', symbol_e::COMILLA},
		{'\"', symbol_e::COMILLAS}};

const std::map<std::string, tipos_e>
		m_tipos{
				{"void", tipos_e::VOID},
				{"bool", tipos_e::BOOL},
				{"char", tipos_e::CHAR},
				{"int", tipos_e::INT},
				{"float", tipos_e::FLOAT},
				{"double", tipos_e::DOUBLE}};

const std::map<std::string, keyword_e>
		m_keyword{
				/// Calificadores de tipo
				{"const", keyword_e::CONST},
				{"static", keyword_e::STATIC},
				/// control de flujo
				{"if", keyword_e::IF},
				{"else", keyword_e::ELSE},
				{"while", keyword_e::WHILE},
				{"do", keyword_e::DO},
				{"for", keyword_e::FOR},
				{"return", keyword_e::RETURN}};

const std::map<std::string, directivas_e> m_directivas{
		{"include", directivas_e::INCLUDE}};

const std::map<std::string, literal_e> m_literal{
		{"true", literal_e::BOOL},
		{"false", literal_e::BOOL}};