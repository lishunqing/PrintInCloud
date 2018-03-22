#include "stdafx.h"
#include "print.h"
#include "QREncode/qrencode.h"

static unsigned int ra = 0;
static const char* wxurl = "https://u.wechat.com/EB-CztQG0xos67txvVXsiNY";
static HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
static HBRUSH black = CreateSolidBrush(RGB(0, 0, 0));


void print::drawQRCode(CDC *dc, CRect & rect, const char* text)
{

	HDC hdc = dc->GetSafeHdc();
	FillRect(hdc, rect, white);

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
				FillRect(hdc, f, black);
			}

			++data;
		}
	}

	QRcode_free(pQRC);
}

int print::printList(Json::Value task, int startNo, CDC *dc, int left, int top, int width, int height)
{
	auto style = task["style"];
	auto model = task["model"];

	if (style.isNull() || model.isNull())
		return -1;
	if (!style.isArray() || !model.isArray())
		return -1;

	float percent = (float)width / 100;
	CFont f;
	f.CreateFont(int(percent * 4),           //   nHeight     
		0,                                     //   nWidth     
		0,                                     //   nEscapement   
		0,                                     //   nOrientation     
		FW_LIGHT,                              //   nWeight     
		FALSE,                                 //   bItalic     
		FALSE,                                 //   bUnderline     
		0,                                     //   cStrikeOut     
		DEFAULT_CHARSET,                       //   nCharSet     
		OUT_DEFAULT_PRECIS,                    //   nOutPrecision     
		CLIP_DEFAULT_PRECIS,                   //   nClipPrecision     
		DEFAULT_QUALITY,                       //   nQuality     
		DEFAULT_PITCH | FF_SWISS,              //   nPitchAndFamily       
		_T("宋体"));
	dc->SelectObject(&f);

	int t = int(top + percent * 10);
	dc->MoveTo(int(left + percent * 3), t);
	dc->LineTo(int(left + width - percent * 3), t);
	dc->MoveTo(int(left + percent * 3), t + 1);
	dc->LineTo(int(left + width - percent * 3), t + 1);

	t = int(top + percent * 6);
	CRect rect;
	rect.top = int(top + percent * 11);
	rect.bottom = int(top + percent * 15);
	int w = int(percent * 8);//字体宽度*2
							   //抬头，款号28
	rect.left = int(left + percent * 4);//左边留3%
	rect.right = int(left + percent * 30);
	dc->ExtTextOut(rect.left + (w < rect.Width() ? int((rect.Width() - w) / 2) : 0), rect.top, ETO_CLIPPED, &rect, _T("款号"), 2, NULL);
	//抬头，名称20
	rect.left = int(left + percent * 32);
	rect.right = int(left + percent * 52);
	dc->ExtTextOut(rect.left + (w < rect.Width() ? int((rect.Width() - w) / 2) : 0), rect.top, ETO_CLIPPED, &rect, _T("名称"), 2, NULL);
	//抬头，颜色的12
	rect.left = int(left + percent * 54);
	rect.right = int(left + percent * 66);
	dc->ExtTextOut(rect.left + (w < rect.Width() ? int((rect.Width() - w) / 2) : 0), rect.top, ETO_CLIPPED, &rect, _T("颜色"), 2, NULL);
	//抬头，尺码6
	rect.left = int(left + percent * 68);
	rect.right = int(left + percent * 76);
	dc->ExtTextOut(rect.left + (w < rect.Width() ? int((rect.Width() - w) / 2) : 0), rect.top, ETO_CLIPPED, &rect, _T("尺码"), 2, NULL);
	//抬头，价格8
	rect.left = int(left + percent * 78);
	rect.right = int(left + percent * 86);
	dc->ExtTextOut(rect.left + (w < rect.Width() ? int((rect.Width() - w) / 2) : 0), rect.top, ETO_CLIPPED, &rect, _T("价格"), 2, NULL);
	//抬头，数量6
	rect.left = int(left + percent * 88);
	rect.right = int(left + percent * 96);//右边5%
	dc->ExtTextOut(rect.left + (w < rect.Width() ? int((rect.Width() - w) / 2) : 0), rect.top, ETO_CLIPPED, &rect, _T("数量"), 2, NULL);


	t = int(top + percent * 16);
	dc->MoveTo(int(left + percent * 3), t);
	dc->LineTo(int(left + width - percent * 3), t);


	int i = startNo;
	int sum = 0;
	while (true)
	{
		if (int(t + percent * 7) > (top + height))
			break;
		if ((unsigned)i >= model.size())
			break;
		auto m = model[i];

		rect.top = int(t + percent * 1);
		rect.bottom = int(t + percent * 5);//top+字体

											 //款号30
		auto code = m["modelcode"].asCString();
		w = int(percent * 2 * strlen(code));
		rect.left = int(left + percent * 4);
		rect.right = int(left + percent * 30);
		ExtTextOutA(dc->GetSafeHdc(), rect.left, rect.top, ETO_CLIPPED, &rect, code, strlen(code), NULL);
		//名称16
		auto name = m["name"].asCString();
		w = int(percent * 2 * strlen(name));
		rect.left = int(left + percent * 32);
		rect.right = int(left + percent * 52);
		ExtTextOutA(dc->GetSafeHdc(), rect.left + (w < rect.Width() ? int((rect.Width() - w) / 2) : 0), rect.top, ETO_CLIPPED, &rect, name, strlen(name), NULL);
		//颜色的14
		auto style1 = m["style1"].asCString();
		w = int(percent * 2 * strlen(style1));
		rect.left = int(left + percent * 54);
		rect.right = int(left + percent * 66);
		ExtTextOutA(dc->GetSafeHdc(), rect.left + (w < rect.Width() ? int((rect.Width() - w) / 2) : 0), rect.top, ETO_CLIPPED, &rect, style1, strlen(style1), NULL);
		//尺码11
		auto style2 = m["style2"].asCString();
		w = int(percent * 2 * strlen(style2));
		rect.left = int(left + percent * 68);
		rect.right = int(left + percent * 76);
		ExtTextOutA(dc->GetSafeHdc(), rect.left + (w < rect.Width() ? int((rect.Width() - w) / 2) : 0), rect.top, ETO_CLIPPED, &rect, style2, strlen(style2), NULL);
		CString p;
		//价格16
		double price = m["price"].asDouble();
		p.Format(L"%d", int(price));
		w = int(percent * 2 * p.GetLength());
		rect.left = int(left + percent * 78);
		rect.right = int(left + percent * 86);
		ExtTextOut(dc->GetSafeHdc(), rect.left + (w < rect.Width() ? int(rect.Width() - w) : 0), rect.top, ETO_CLIPPED, &rect, p.GetBuffer(), p.GetLength(), NULL);
		//数量11
		int amount = m["amount"].asInt();
		sum += amount;
		p.Format(L"%d", amount);
		w = int(percent * 2 * p.GetLength());
		rect.left = int(left + percent * 88);
		rect.right = int(left + percent * 96);
		ExtTextOut(dc->GetSafeHdc(), rect.left + (w < rect.Width() ? int(rect.Width() - w) : 0), rect.top, ETO_CLIPPED, &rect, p.GetBuffer(), p.GetLength(), NULL);


		dc->MoveTo(int(left + percent * 3), int(t + percent * 6));
		dc->LineTo(int(left + width - percent * 3), int(t + percent * 6));


		t += int(percent * 6);
		++i;
	}
	dc->MoveTo(int(left + percent * 3), t + 1);
	dc->LineTo(int(left + width - percent * 3), t + 1);

	CString p;
	p.Format(L"行号%d-%d，本单总计计%d行，本页总计%d件。", startNo + 1, i, i - startNo, sum);
	rect.top = int(top + percent * 3);
	rect.bottom = int(top + percent * 9);
	rect.left = int(left + percent * 2);
	rect.right = int(left + percent * 98);
	ExtTextOut(dc->GetSafeHdc(), rect.left, rect.top, ETO_CLIPPED, &rect, p.GetBuffer(), p.GetLength(), NULL);

	int l = int(left + percent * 3) - 1;
	++t;
	dc->MoveTo(l, int(top + percent * 10));
	dc->LineTo(l, t);
	++l;
	dc->MoveTo(l, int(top + percent * 10));
	dc->LineTo(l, t);

	l = int(left + percent * 31);
	dc->MoveTo(l, int(top + percent * 10));
	dc->LineTo(l, t);

	l = int(left + percent * 53);
	dc->MoveTo(l, int(top + percent * 10));
	dc->LineTo(l, t);

	l = int(left + percent * 67);
	dc->MoveTo(l, int(top + percent * 10));
	dc->LineTo(l, t);

	l = int(left + percent * 77);
	dc->MoveTo(l, int(top + percent * 10));
	dc->LineTo(l, t);

	l = int(left + percent * 87);
	dc->MoveTo(l, int(top + percent * 10));
	dc->LineTo(l, t);

	l = int(left + percent * 97);
	dc->MoveTo(l, int(top + percent * 10));
	dc->LineTo(l, t);
	++l;
	dc->MoveTo(l, int(top + percent * 10));
	dc->LineTo(l, t);

	if ((unsigned)i >= model.size())
		return -1;
	else
		return i;
}

void print::printTag(Json::Value task, int id, CDC *dc, int left, int top, int width, int height)
{
	auto style = task["style"];
	auto model = task["model"];
	unsigned int ti = (unsigned)time(NULL);

	if (style.isNull() || model.isNull())
		return;
	if (!style.isArray() || !model.isArray())
		return;
	auto m = model[id];
	if (m.isNull())
		return;
	const float percent = (float)width / 100;
	const int edge = int(percent * 7);
	const int fontl = int(percent * 12);
	const int fontm = int(percent * 9);
	const int fonts = int(percent * 4);
	const int qrsize = int(percent * 60);
	CFont fl,fm,fs;
	fl.CreateFont(fontl,                //   nHeight     
		0,                                     //   nWidth     
		0,                                     //   nEscapement   
		0,                                     //   nOrientation     
		FW_HEAVY,                              //   nWeight     
		FALSE,                                 //   bItalic     
		FALSE,                                 //   bUnderline     
		0,                                     //   cStrikeOut     
		DEFAULT_CHARSET,                       //   nCharSet     
		OUT_DEFAULT_PRECIS,                    //   nOutPrecision     
		CLIP_DEFAULT_PRECIS,                   //   nClipPrecision     
		DEFAULT_QUALITY,                       //   nQuality     
		DEFAULT_PITCH | FF_SWISS,              //   nPitchAndFamily       
		_T("宋体"));

	fm.CreateFont(fontm,                //   nHeight     
		0,                                     //   nWidth     
		0,                                     //   nEscapement   
		0,                                     //   nOrientation     
		FW_NORMAL,                             //   nWeight     
		FALSE,                                 //   bItalic     
		FALSE,                                 //   bUnderline     
		0,                                     //   cStrikeOut     
		DEFAULT_CHARSET,                       //   nCharSet     
		OUT_DEFAULT_PRECIS,                    //   nOutPrecision     
		CLIP_DEFAULT_PRECIS,                   //   nClipPrecision     
		DEFAULT_QUALITY,                       //   nQuality     
		DEFAULT_PITCH | FF_SWISS,              //   nPitchAndFamily       
		_T("宋体"));

	fs.CreateFont(fonts,                //   nHeight     
		0,                                     //   nWidth     
		0,                                     //   nEscapement   
		0,                                     //   nOrientation     
		FW_LIGHT,                              //   nWeight     
		FALSE,                                 //   bItalic     
		FALSE,                                 //   bUnderline     
		0,                                     //   cStrikeOut     
		DEFAULT_CHARSET,                       //   nCharSet     
		OUT_DEFAULT_PRECIS,                    //   nOutPrecision     
		CLIP_DEFAULT_PRECIS,                   //   nClipPrecision     
		DEFAULT_QUALITY,                       //   nQuality     
		DEFAULT_PITCH | FF_SWISS,              //   nPitchAndFamily       
		_T("宋体"));

	int yy = top + edge;
	dc->SelectObject(&fl);
	dc->TextOut(int(left + (width - fontl * 3) / 2), yy, _T("合格证"));
	yy += fontl + edge;

	dc->SelectObject(&fm);
	CStringA text;
	text.Format("款号：%s",m["modelcode"].asCString());
	ExtTextOutA(dc->GetSafeHdc(), int(left + edge), yy, 0, NULL, text.GetBuffer(), text.GetLength(), NULL);
	yy += int(fontm + percent * 2);

	text.Format("品名：%s", m["name"].asCString());
	ExtTextOutA(dc->GetSafeHdc(), int(left + edge), yy, 0, NULL, text.GetBuffer(), text.GetLength(), NULL);
	yy += int(fontm + percent * 2);

	text.Format("%s：%s", style[unsigned(0)]["stylename"].asCString(), m["style1"].asCString());
	ExtTextOutA(dc->GetSafeHdc(), int(left + edge), yy, 0, NULL, text.GetBuffer(), text.GetLength(), NULL);
	yy += int(fontm + percent * 2);

	text.Format("%s：%s", style[unsigned(1)]["stylename"].asCString(), m["style2"].asCString());
	ExtTextOutA(dc->GetSafeHdc(), int(left + edge), yy, 0, NULL, text.GetBuffer(), text.GetLength(), NULL);
	yy += int(fontm + percent * 2);

	text.Format("检验员：%02d", rand() % 99 + 1);
	ExtTextOutA(dc->GetSafeHdc(), int(left + edge), yy, 0, NULL, text.GetBuffer(), text.GetLength(), NULL);
	yy += int(fontm + edge);

	dc->SelectObject(&fs);
	dc->TextOut(int(left + edge), yy, _T("执行标准：FZ/T73005-2002"));
	yy += int(fonts + percent );;

	dc->SelectObject(&fs);
	dc->TextOut(int(left + edge), yy, _T("质量等级：合格品"));
	yy += int(fonts + percent);;

	dc->SelectObject(&fs);
	text.Format("安全技术类别：%s",  m["style3"].asCString());
	ExtTextOutA(dc->GetSafeHdc(), int(left + edge), yy, 0, NULL, text.GetBuffer(), text.GetLength(), NULL);

	yy += int(fonts + percent);;

	dc->SelectObject(&fs);
	dc->TextOut(int(left + edge), yy, _T("洗涤方式请参考服装上洗涤说明标签"));

	yy += int(fontm + edge - fonts);
	dc->SelectObject(&fm);
	dc->TextOut(int(left + edge), yy, _T("价格："));
	dc->SelectObject(&fl);
	text.Format("￥%.0f", m["price"].asDouble());
	ExtTextOutA(dc->GetSafeHdc(), int(left + width / 2 - percent*8), yy - (fontl - fontm) / 2, 0, NULL, text.GetBuffer(), text.GetLength(), NULL);

	yy += int(fontl + edge);

	CRect r(left + (width - qrsize) / 2, yy, left + (width + qrsize) / 2, yy + qrsize);
	text.Format("%s?1=%s&2=%d&3=%d&4=%u", wxurl, m["modelcode"].asCString(), m["modelid"].asInt(), ra++, time(NULL));
	//text.Format(wxurl);
	drawQRCode(dc, r, text.GetBuffer());
}