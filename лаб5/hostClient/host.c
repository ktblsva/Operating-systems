#include <windows.h>
#include "host_resource.h"
#include <stdio.h>
//cmd.exe /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat" `& powershell 
LRESULT CALLBACK
DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {
	MSG msg;
	DialogBox(hInstance,(LPCTSTR)IDD_HOSTTEST,NULL,
	          (DLGPROC)DlgProc);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK
DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			return TRUE;
		case WM_COPYDATA:
			char* strText, *strText1, *oper;
			char* pch = strtok ((char*)(
			                  ((COPYDATASTRUCT*)lParam)->lpData)," ");
			strText = pch;
			oper = strtok(NULL, " ");
			strText1 = strtok(NULL, " ");
			float result = -1;
			if(oper[0] == '+')
			{
				result = atof(strText) + atof(strText1);
			}
			if(oper[0] == '-')
			{
				result = atof(strText) - atof(strText1);
			}
			if(oper[0] == '*')
			{
				result = atof(strText) * atof(strText1);
			}
			if(oper[0] == '/')
			{
				result = atof(strText) / atof(strText1);
			}
			char res[100];
			sprintf(res, "%0.2f", result);
			SetDlgItemText(hDlg, IDC_EDIT1, res);
		
			        break;
		case WM_COMMAND:
			switch (LOWORD(wParam) ) {
				case IDOK:
					PostQuitMessage(0);
					return TRUE;
				case IDCANCEL:
					PostQuitMessage(0);
					return TRUE;
					
				case IDC_BTN1:
					char message[100];
					GetDlgItemText(hDlg, IDC_EDIT1, message, 100);
					HWND hWnd;
					COPYDATASTRUCT data;
					hWnd=FindWindow(NULL,"Client");
					data.cbData=strlen(message);
					data.lpData=message;
					SendMessage (hWnd, WM_COPYDATA, (WPARAM)GetFocus(),
					            (LPARAM)&data);
					SetDlgItemText(hDlg, IDC_EDIT1, "");
					break;
			}
			break;
		default:
			return FALSE;
	}
}
