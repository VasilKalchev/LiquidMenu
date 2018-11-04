/**
@file
Contains the overloaded functions DataType recognizeType(... variable);
 for LiquidMenu library.
*/

#include "LiquidMenu.h"

DataType recognizeType(bool variable) {
	return DataType::BOOL;
	variable = variable;
}

DataType recognizeType(char variable) {
	return DataType::CHAR;
	variable = variable;
}

DataType recognizeType(char* variable) {
	return DataType::CHAR_PTR;
	variable = variable;
}

DataType recognizeType(const char* variable) {
	return DataType::CONST_CHAR_PTR;
	variable = variable;
}

DataType recognizeType(int8_t variable) {
	return DataType::INT8_T;
	variable = variable;
}

DataType recognizeType(uint8_t variable) {
	return DataType::UINT8_T;
	variable = variable;
}

DataType recognizeType(int16_t variable) {
	return DataType::INT16_T;
	variable = variable;
}

DataType recognizeType(uint16_t variable) {
	return DataType::UINT16_T;
	variable = variable;
}

DataType recognizeType(int32_t variable) {
	return DataType::INT32_T;
	variable = variable;
}

DataType recognizeType(uint32_t variable) {
	return DataType::UINT32_T;
	variable = variable;
}

DataType recognizeType(float variable) {
	return DataType::FLOAT;
	variable = variable;
}

DataType recognizeType(double variable) {
	return DataType::DOUBLE;
	variable = variable;
}


DataType recognizeType(boolFnPtr variable) {
	return DataType::BOOL_GETTER;
	variable = variable;
}


DataType recognizeType(int8tFnPtr variable) {
	return DataType::INT8_T_GETTER;
	variable = variable;
}


DataType recognizeType(uint8tFnPtr variable) {
	return DataType::UINT8_T_GETTER;
	variable = variable;
}


DataType recognizeType(int16tFnPtr variable) {
	return DataType::INT16_T_GETTER;
	variable = variable;
}

DataType recognizeType(uint16tFnPtr variable) {
	return DataType::UINT8_T_GETTER;
	variable = variable;
}


DataType recognizeType(int32tFnPtr variable) {
	return DataType::INT32_T_GETTER;
	variable = variable;
}


DataType recognizeType(uint32tFnPtr variable) {
	return DataType::UINT32_T_GETTER;
	variable = variable;
}


DataType recognizeType(floatFnPtr variable) {
	return DataType::FLOAT_GETTER;
	variable = variable;
}

DataType recognizeType(doubleFnPtr variable) {
	return DataType::DOUBLE_GETTER;
	variable = variable;
}

DataType recognizeType(charFnPtr variable) {
	return DataType::CHAR_GETTER;
	variable = variable;
}


DataType recognizeType(charPtrFnPtr variable) {
	return DataType::CHAR_PTR_GETTER;
	variable = variable;
}


DataType recognizeType(constcharPtrFnPtr variable) {
	return DataType::CONST_CHAR_PTR_GETTER;
	variable = variable;
}
