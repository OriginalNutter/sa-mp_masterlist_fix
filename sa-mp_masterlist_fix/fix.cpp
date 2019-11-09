#include <windows.h>
#include <iostream>
#include <detours.h>

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "ws2_32.lib")

#define MASTERLIST_HOST_DEFAULT "03dl.is-very.cool"
#define MASTERLIST_PATH_DEFAULT "/list/internet"

#define HOSTEDLIST_HOST_DEFAULT "03dl.is-very.cool"
#define HOSTEDLIST_PATH_DEFAULT "/list/hosted"

int (WINAPI *pSend)(SOCKET s, const char* buf, int len, int flags) = send;
int WINAPI HOOK_send(SOCKET s, const char* buf, int len, int flags);
struct hostent* FAR(WINAPI *pgethostbyname)(const char *cp) = gethostbyname;
struct hostent* FAR WINAPI HOOK_gethostbyname(const char *cp);

char g_szInternetHost[128];
char g_szHostedHost[128];

char g_szInternetHTTPHeaders[256 + 200];
int g_iInternetHTTPHeadersLen;

char g_szHostedHTTPHeaders[256 + 200];
int g_iHostedHTTPHeadersLen;

struct hostent* FAR WINAPI HOOK_gethostbyname(const char *cp)
{
	int iCurTab = *(int*)(0x4EE6E8); // current tab (fav/internet/hosted)
	if (!strcmp(cp, "lists.sa-mp.com"))
	{
		if (iCurTab == 1)
		{
			return pgethostbyname(g_szInternetHost);
		}
		else if (iCurTab == 2)
		{
			return pgethostbyname(g_szHostedHost);
		}
	}
	
	return pgethostbyname(cp);
}

int WINAPI HOOK_send(SOCKET s, const char* buf, int len, int flags)
{
	if (strstr(buf, "GET /0.3.DL/internet") != NULL)
		return pSend(s, g_szInternetHTTPHeaders, g_iInternetHTTPHeadersLen, flags);

	if (strstr(buf, "GET /0.3.DL/hosted") != NULL)
		return pSend(s, g_szHostedHTTPHeaders, g_iHostedHTTPHeadersLen, flags);

	return pSend(s, buf, len, flags);
}

void LoadConfig()
{
	FILE *fileConfig;
	bool bConfigWritten = false;
	char szInternetPath[128];
	char szHostedPath[128];

	if (fopen_s(&fileConfig, "masterlist_fix.cfg", "r") == 0)
	{
		if (fgets(g_szInternetHost, sizeof(g_szInternetHost), fileConfig) != NULL && fgets(szInternetPath, sizeof(szInternetPath), fileConfig) != NULL)
		{
			g_szInternetHost[strcspn(g_szInternetHost, "\r\n")] = 0;
			szInternetPath[strcspn(szInternetPath, "\r\n")] = 0;

			bConfigWritten = true;
		}
		fclose(fileConfig);
	}

	if (!bConfigWritten)
	{
		strcpy_s(g_szInternetHost, sizeof(g_szInternetHost), MASTERLIST_HOST_DEFAULT);
		strcpy_s(szInternetPath, sizeof(szInternetPath), MASTERLIST_PATH_DEFAULT);
	}

	sprintf_s(g_szInternetHTTPHeaders, sizeof(g_szInternetHTTPHeaders), "\
GET %s HTTP/1.1\r\n\
Content-Type: text/html\r\n\
Host: %s\r\n\
Accept: text/html, */*\r\n\
User-Agent: Mozilla/3.0 (compatible; SA:MP v0.3.DL)\r\n\r\n", szInternetPath, g_szInternetHost);

	g_iInternetHTTPHeadersLen = strlen(g_szInternetHTTPHeaders);

	strcpy_s(g_szHostedHost, sizeof(g_szHostedHost), HOSTEDLIST_HOST_DEFAULT);
	strcpy_s(szHostedPath, sizeof(szHostedPath), HOSTEDLIST_PATH_DEFAULT);

	sprintf_s(g_szHostedHTTPHeaders, sizeof(g_szHostedHTTPHeaders), "\
GET %s HTTP/1.1\r\n\
Content-Type: text/html\r\n\
Host: %s\r\n\
Accept: text/html, */*\r\n\
User-Agent: Mozilla/3.0 (compatible; SA:MP v0.3.DL)\r\n\r\n", szHostedPath, g_szHostedHost);

	g_iHostedHTTPHeadersLen = strlen(g_szHostedHTTPHeaders);
}

void InitializeFix()
{
	LoadConfig();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)pSend, HOOK_send);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)pgethostbyname, HOOK_gethostbyname);
	DetourTransactionCommit();
}

void GetRidOfMe()
{
	// we'll never.
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)pSend, HOOK_send);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)pgethostbyname, HOOK_gethostbyname);
	DetourTransactionCommit();
}