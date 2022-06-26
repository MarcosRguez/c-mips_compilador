/**
 * @file enums.hh
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 26-06-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

enum class token_t {
	KEYWORD,
	TYPE,
	IDENTIFIER,
	LITERAL,
	SYMBOL,
	OPERATOR
};

enum class tipos_e {
	VOID,
	CHAR,
	INT,
	FLOAT,
	DOUBLE
};

enum class keyword_e {
	FOR,
	WHILE,
	DO
};

enum class symbol_e {
	PARENTESIS_A,
	PARENTESIS_C,
	CORCHETE_A,
	CORCHETE_C,
	LLAVE_A,
	LLAVE_C,
	COMILLAAN_A,
	COMILLAAN_C,
	PUNTOYCOMA,
	ALMOHADILLA
};

enum class operator_e {
	ASIGNACION,
	COMA,
	PUNTERO,
	REFERENCIA,
	MIEMBRO
};