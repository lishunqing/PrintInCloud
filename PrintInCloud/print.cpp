#include "stdafx.h"
#include "print.h"
#include "QREncode/qrencode.h"

void print::drawQRCode(CDC *dc, CRect & rect, const char* text)
{
	FillRect(dc->GetSafeHdc(), rect, CreateSolidBrush(RGB(255, 255, 255)));

	QRcode* pQRC = QRcode_encodeString(text, 0, QR_ECLEVEL_H, QR_MODE_8, 1);

	if (pQRC == NULL)
		return;

	float top = (float)rect.top;
	float left = (float)rect.left;
	float wstep = (float)rect.Width() / pQRC->width;
	float hstep = (float)rect.Height() / pQRC->width;
	unsigned char* data = pQRC->data;
	for (int y = 0; y < pQRC->width; y++)
	{
		for (int x = 0; x < pQRC->width; x++)
		{
			if (*data & 1)
			{
				CRect f(
					int(left + wstep * x + 0.5),
					int(top + hstep * y + 0.5), 
					int(left + wstep * x + wstep + 0.5),
					int(top + hstep * y + hstep + 0.5));
				FillRect(dc->GetSafeHdc(), f, CreateSolidBrush(RGB(0, 0, 0)));
			}

			++data;
		}
	}

	QRcode_free(pQRC);
}
