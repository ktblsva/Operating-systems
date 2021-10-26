#include <windows.h>
#include "resource.h"
#include <stdio.h>
LRESULT CALLBACK
DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {
	MSG msg;
	DialogBox(hInstance,(LPCTSTR)IDD_DLGTEST,NULL,
	          (DLGPROC)DlgProc);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK
DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	char strText[100], strText2[100], result[100];
	char oper[2];
	float res;
	switch (message) {
		case WM_INITDIALOG:
			return TRUE;
		case WM_COPYDATA:
			SetDlgItemText(hDlg,IDC_EDIT3,(char*)(
			                  ((COPYDATASTRUCT*)lParam)->lpData));
		
			        break;
		case WM_COMMAND:
			switch (LOWORD(wParam) ) {
				case IDOK:
						PostQuitMessage(0);
						return TRUE;
				case IDCANCEL:
						PostQuitMessage(0);
						return TRUE;
					
				case IDC_BUTTON1:
						GetDlgItemText(hDlg,IDC_EDIT1,strText, 100);
						GetDlgItemText(hDlg,IDC_EDIT2,strText2, 100);
						oper[0] = '+';
						oper[1] = '\0';
						sprintf(result, "%s %s %s", strText, oper, strText2);
						HWND hWnd;
						COPYDATASTRUCT data;
						hWnd=FindWindow(NULL,"Host");
						data.cbData=strlen(result);
						data.lpData=result;
						SendMessage (hWnd, WM_COPYDATA, (WPARAM)GetFocus(), (LPARAM)&data);
						return TRUE;
						// res = atof(strText) + atof(strText2);
						// sprintf(result, "%0.2f", res);
						// SetDlgItemText(hDlg, IDC_EDIT3, result);
						break;

				case IDC_BUTTON2:
						GetDlgItemText(hDlg, IDC_EDIT1, strText, 100);
                        GetDlgItemText(hDlg, IDC_EDIT2, strText2, 100);
						oper[0] = '-';
						oper[1] = '\0';
						sprintf(result, "%s %s %s", strText, oper, strText2);
						hWnd=FindWindow(NULL,"Host");
						data.cbData=strlen(result);
						data.lpData=result;
						SendMessage (hWnd, WM_COPYDATA, (WPARAM)GetFocus(), (LPARAM)&data);
						return TRUE;
                        // res = atof(strText) - atof(strText2);
					    // sprintf(result, "%0.2f", res);
                        // SetDlgItemText(hDlg, IDC_EDIT3, result);
                        break;
				case IDC_BUTTON3:
						GetDlgItemText(hDlg, IDC_EDIT1, strText, 100);
                        GetDlgItemText(hDlg, IDC_EDIT2, strText2, 100);
						oper[0] = '*';
						oper[1] = '\0';
						sprintf(result, "%s %s %s", strText, oper, strText2);
						hWnd=FindWindow(NULL,"Host");
						data.cbData=strlen(result);
						data.lpData=result;
						SendMessage (hWnd, WM_COPYDATA, (WPARAM)GetFocus(), (LPARAM)&data);
						return TRUE;
                        // res = atof(strText) * atof(strText2);
					    // sprintf(result, "%0.2f", res);
                        // SetDlgItemText(hDlg, IDC_EDIT3, result);
                        break;
				case IDC_BUTTON4:
						GetDlgItemText(hDlg, IDC_EDIT1, strText, 100);
                        GetDlgItemText(hDlg, IDC_EDIT2, strText2, 100);
						oper[0] = '/';
						oper[1] = '\0';
						sprintf(result, "%s %s %s", strText, oper, strText2);
						hWnd=FindWindow(NULL,"Host");
						data.cbData=strlen(result);
						data.lpData=result;
						SendMessage (hWnd, WM_COPYDATA, (WPARAM)GetFocus(), (LPARAM)&data);
						return TRUE;
                        // res = atof(strText) / atof(strText2);
					    // sprintf(result, "%0.2f", res);
                        // SetDlgItemText(hDlg, IDC_EDIT3, result);
                        break;
			}
			
			break;
		default:
			return FALSE;
	}
	// sprintf(result, "%s %s %s", strText, oper, strText2);
	// HWND hWnd;
	// COPYDATASTRUCT data;
	// hWnd=FindWindow(NULL,"Host");
	// data.cbData=strlen(result);
	// data.lpData=result;
	// SendMessage (hWnd, WM_COPYDATA, (WPARAM)GetFocus(), (LPARAM)&data);
	// return TRUE;
}
