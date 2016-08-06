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
