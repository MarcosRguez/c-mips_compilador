/**
 * @file enums.hh
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief Clasificación de tokens (Gramática)
 * @version 0.1
 * @date 26-06-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

/**
 * @brief Tipo primario
 */
enum class token_t {
	KEYWORD,
	TYPE,
	IDENTIFIER,
	LITERAL,
	SYMBOL,
	OPERATOR,
	REGISTER /// especial para evaluar expresiones
};

/// Subtipos

enum class literal_e {
	BOOL,
	CHAR,
	INT,
	FLOAT,
	DOUBLE,
	STR
};

enum class directivas_e {
	INCLUDE,
	PRAGMA,
	DEFINE
};

enum class tipos_e {
	VOID,
	BOOL,
	CHAR,
	INT,
	FLOAT,
	DOUBLE
};

enum class keyword_e {
	/// Calificadores de tipo
	CONST,
	STATIC,
	/// control de flujo
	IF,
	ELSE,
	WHILE,
	DO,
	FOR,
	RETURN
};

enum class symbol_e {
	COMA,
	PARENTESIS_A,
	PARENTESIS_C,
	CORCHETE_A,
	CORCHETE_C,
	LLAVE_A,
	LLAVE_C,
	COMILLAAN_A,
	COMILLAAN_C,
	PUNTOYCOMA,
	ALMOHADILLA,
	COMILLA,
	COMILLAS,
};

enum class operator_e {
	/// Aritméticos
	SUMA,
	RESTA,
	PRODUCTO,
	DIVISION,
	MODULO,
	SUMA_A,
	RESTA_A,
	PRODUCTO_A,
	DIVISION_A,
	MODULO_A,
	/// incremento y decremento
	INCREMENTO,
	DECREMENTO,
	/// Lógica
	NOT,
	AND,
	OR,
	/// comparación
	MENORQUE,
	MAYORQUE,
	MENORIGUAL,
	MAYORIGUAL,
	IGUALQUE,
	DESIGUALQUE,
	/// otros
	ASIGNACION,
	COMA,
	PUNTERO,
	REFERENCIA,
	MIEMBRO,
	/// miscelánea
	MINIMO,
	MAXIMO
};