#pragma once

class print
{
public:
	float ratox;//每千分之一毫米的像素数
	float ratoy;//每千分之一毫米的像素数
	//void setDPI(int, int);

	static void drawQRCode(CDC*, CRect&, CString);
};
