#pragma once
#include "json/json.h"
class print
{
public:
	float ratox;//ÿǧ��֮һ���׵�������
	float ratoy;//ÿǧ��֮һ���׵�������
	//void setDPI(int, int);

	static void drawQRCode(CDC*, CRect&, const char*);
	static int printList(Json::Value, int, CDC*, int, int, int, int);
	static void printTag(Json::Value, int, CDC*, int, int, int, int);
};
