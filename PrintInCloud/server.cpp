#include "stdafx.h"
#include "server.h"


LPCTSTR Host = L"528539490.lcloudl.xyz";
//LPCTSTR Host = L"rsd44zk7.qcloud.la";
LPCTSTR Path = L"/weapp/print/task";

CString getTaks(TCHAR* args, DWORD&err) {
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	CString ret = L"";

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP Example/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, Host,
			INTERNET_DEFAULT_HTTPS_PORT, 0);



	// Create an HTTP request handle.
	if (hConnect)
		if (*args) {
			CString path;
			path.Format(L"%s?id=%s", Path, args);
			hRequest = WinHttpOpenRequest(hConnect, L"GET", path.GetBuffer(),
				NULL, WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				WINHTTP_FLAG_SECURE);
		}else{
			hRequest = WinHttpOpenRequest(hConnect, L"GET", Path,
				NULL, WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				WINHTTP_FLAG_SECURE);
		}


	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);


	// End the request.
	if (bResults) 
		bResults = WinHttpReceiveResponse(hRequest, NULL);



	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		DWORD dwReadBytes, dwSizeDW = sizeof(dwSizeDW), dwContentSize, dwIndex = 0;

		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwContentSize, &dwSizeDW, &dwIndex);
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				// Read the Data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
					dwSize, &dwDownloaded))
					err = GetLastError();
				else
				{
					int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, pszOutBuffer, -1, NULL, 0) + 1;
					wchar_t * pUnicode = new wchar_t[unicodeLen];
					MultiByteToWideChar(CP_UTF8, 0, pszOutBuffer, -1, pUnicode, unicodeLen);
					ret += CString(pUnicode);
					delete pUnicode;
				}

				// Free the memory allocated to the buffer.
				delete[] pszOutBuffer;
			}

		} while (dwSize > 0);
	}



	// Report any errors.
	if (!bResults)
		err = GetLastError();
	else
		err = 0;

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return ret;
}
