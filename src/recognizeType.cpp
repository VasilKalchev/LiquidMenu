/**
@file
Contains the overloaded functions DataType recognizeType(... variable);
 for LiquidMenu library.
*/

#include "LiquidMenu.h"

DataType recognizeType(bool variable) {
	return DataType::BOOL;
}

DataType recognizeType(char variable) {
	return DataType::CHAR;
}

DataType recognizeType(char* variable) {
	return DataType::CHAR_PTR;
}

DataType recognizeType(const char* variable) {
	return DataType::CONST_CHAR_PTR;
}

DataType recognizeType(int8_t variable) {
	return DataType::INT8_T;
}

DataType recognizeType(uint8_t variable) {
	return DataType::UINT8_T;
}

DataType recognizeType(int16_t variable) {
	return DataType::INT16_T;
}

DataType recognizeType(uint16_t variable) {
	return DataType::UINT16_T;
}

DataType recognizeType(int32_t variable) {
	return DataType::INT32_T;
}

DataType recognizeType(uint32_t variable) {
	return DataType::UINT32_T;
}

DataType recognizeType(float variable) {
	return DataType::FLOAT;
}

DataType recognizeType(double variable) {
	return DataType::DOUBLE;
}


DataType recognizeType(boolFnPtr variable) {
	return DataType::BOOL_GETTER;
}


DataType recognizeType(int8tFnPtr variable) {
	return DataType::INT8_T_GETTER;
}


DataType recognizeType(uint8tFnPtr variable) {
	return DataType::UINT8_T_GETTER;
}


DataType recognizeType(int16tFnPtr variable) {
	return DataType::INT16_T_GETTER;
}

DataType recognizeType(uint16tFnPtr variable) {
	return DataType::UINT16_T_GETTER;
}


DataType recognizeType(int32tFnPtr variable) {
	return DataType::INT32_T_GETTER;
}


DataType recognizeType(uint32tFnPtr variable) {
	return DataType::UINT32_T_GETTER;
}


DataType recognizeType(floatFnPtr variable) {
	return DataType::FLOAT_GETTER;
}

DataType recognizeType(doubleFnPtr variable) {
	return DataType::DOUBLE_GETTER;
}

DataType recognizeType(charFnPtr variable) {
	return DataType::CHAR_GETTER;
}


DataType recognizeType(charPtrFnPtr variable) {
	return DataType::CHAR_PTR_GETTER;
}


DataType recognizeType(constcharPtrFnPtr variable) {
	return DataType::CONST_CHAR_PTR_GETTER;
}
