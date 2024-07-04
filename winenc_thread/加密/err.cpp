#include "stdafx.h"
#include <Windows.h>
#include "err.h"

void ShowError(char *pszText) {
	char szErr[MAX_PATH] = {0};
	::wsprintf(
		szErr,
		"%s err 0x%x \n", pszText);
	MessageBox(
		NULL,
		szErr, 
		"ERROR",
		MB_OK|MB_ICONERROR
		);
}