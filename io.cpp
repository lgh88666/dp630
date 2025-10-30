

#include"io.h"



int IOControl::getOutPut(ZMC_HANDLE handle, string interface1) {
	uint8 GetValue[2];
	if (interface1 == "DOA") {
		//非0，则失败
		if (ZAux_GetModbusOut(handle, 224, 239, GetValue)) {
			return 0;
		}
		//赋值存储
		for (int i = 0; i < 16; i++) {
			DOA[i] = (GetValue[(int)(i / 8)] & (0x1 << (i % 8))) >> (i % 8);
		}
		return 1;

	}
	else if (interface1 == "DOB") {
		//非0，则失败
		if (ZAux_GetModbusOut(handle, 240, 255, GetValue)) {
			return 0;
		}
		//赋值存储
		for (int i = 0; i < 16; i++) {
			DOB[i] = (GetValue[(int)(i / 8)] & (0x1 << (i % 8))) >> (i % 8);
		}
		return 1;

	}
	else {
		//参数传递错误直接返回0
		return 0;
	}
}

int IOControl::getInput(ZMC_HANDLE handle, string interface1) {
	uint8 GetValue[2];
	if (interface1 == "DIA") {
		//非0，则失败
		if (ZAux_GetModbusIn(handle, 224,239, GetValue)) {
			return 0;
		}
		//赋值存储
		for (int i = 0; i < 16; i++) {
			DIA[i] = (GetValue[(int)(i / 8)] & (0x1 << (i % 8))) >> (i % 8);
		}
		return 1;
		
	}
	else if (interface1 == "DIB") {
		//非0，则失败
		if (ZAux_GetModbusIn(handle, 240, 255, GetValue)) {
			return 0;
		}
		//赋值存储
		for (int i = 0; i < 16; i++) {
			DIB[i] = (GetValue[(int)(i / 8)] & (0x1 << (i % 8))) >> (i % 8);
		}
		return 1;
		
	}
	else {
		//参数传递错误直接返回0
		return 0;
	}
}


int IOControl::setOutput(ZMC_HANDLE handle, int ionum, int value, string interface1) {
	if (interface1 == "DOA") {
		//非0，即失败
		if (ZAux_Direct_SetOp(handle, ionum, value)) {
			return 0;
		}
		//存储
		DOA[ionum - 224] = value;
		return 1;	
	}
	else if (interface1 == "DOB") {
		//非0，即失败
		if (ZAux_Direct_SetOp(handle, ionum, value)) {
			return 0;
		}
		//存储
		DOB[ionum - 240] = value;
		return 1;
	}
	else {
		//参数传递错误直接返回0
		return 0;
	}

}