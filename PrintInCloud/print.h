#pragma once
#include "json/json.h"
class print
{
public:
	float ratox;//每千分之一毫米的像素数
	float ratoy;//每千分之一毫米的像素数
	//void setDPI(int, int);

	static void drawQRCode(CDC*, CRect&, const char*);
	static int printList(Json::Value, int, CDC*, int, int, int, int);
	static void printTag(Json::Value, int, CDC*, int, int, int, int);
};
