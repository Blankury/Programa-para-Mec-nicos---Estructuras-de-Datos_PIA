#include<Windows.h>
#include <CommCtrl.h>
#include "Resource.h"
#include <string>
#include<fstream>
#include<time.h>

using namespace std;

#define ID_Timer 90000
time_t tempfecha; struct tm *fecha_actual;

struct usuarios {
	string nombre;
	string apellido_paterno;
	string apellido_materno;
	string usuario;
	string contraseña;
	usuarios *siguiente_u;
	usuarios *anterior_u;
}*inicio_u, *actual_u;

struct paquetes_disp {
	char nombre[MAX_PATH];
	string clave;
	string costo;
	char descripcion[MAX_PATH];
	paquetes_disp *siguiente_p;
	paquetes_disp *anterior_p;
}*inicio_p, *actual_p;

struct mecanic {
	bool dias[5] = {false, false, false, false, false};
	string num_empleado;
	string apellido_paterno;
	string apellido_materno;
	string turno;
	string telefono;
	char especialidad[MAX_PATH];
	char foto[MAX_PATH];
	char nombre[MAX_PATH];
	mecanic *siguiente_m;
	mecanic *anterior_m;
}*inicio_m, *actual_m;

struct clientes {
	string placas;
	string marca;
	string modelo;
	string año_v;
	string kilometraje;
	string telefono;
	char direccion[MAX_PATH];
	char nombre[MAX_PATH];
	string apellido_paterno;
	string apellido_materno;
	clientes *siguiente_c;
	clientes *anterior_c;
}*inicio_c, *actual_c;

struct cita {
	int num;
	string estado;
	char cliente[MAX_PATH];
	char paquete[MAX_PATH];
	char mecanico[MAX_PATH];
	char fecha[MAX_PATH] ;
	char foto[MAX_PATH];
	string hora;
	string dia_s;
	string telefono;
	cita *siguiente_cs;
	cita *anterior_cs;
}*inicio_cs, *actual_cs;

#pragma region callbacks
BOOL CALLBACK login(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK registrar_usuarios(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK menu(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK paquetes(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK buscar(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK borrar_editar_paquetes(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK mecanicos(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK borrar_editar_mecanicos(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Lista(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK borrar_editar_cliente(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK cliente(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Lista_clientes(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK citas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK borrar_editar_citas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK mostrar_citas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK reporte_citas(HWND, UINT, WPARAM, LPARAM);
#pragma endregion

#pragma region funciones
void guardado_general(paquetes_disp *inicio_p, mecanic *inicio_m, clientes *inicio_c, cita *inicio_cs);
void cargado_general();
void validar_nombres(char temp[MAX_PATH], bool &correcto);
void cargar_usuarios();
int part(string*, int, int);
void Quicksort(string*, int, int);
void heapSort(string*, int);
void heapify(string*, int);
#pragma endregion

bool verf = false, done = false;
int num_cita = 101010, cont, op_ed, tipo_busqueda = 0;
char fotos[MAX_PATH];
string fecha, hora, dia_semana, mes, esptemp, sem;
HWND cbCombo1, cbCombo2, cbCombo3, cbCombo4, cbCombo5, cbCombo6;

fstream arp, armec, arcl, arci;

HINSTANCE hGlobalInst;

int WINAPI WinMain(HINSTANCE instancia, HINSTANCE hprev, LPSTR comandos, int cShow) {

	inicio_u = actual_u = NULL;
	inicio_p = actual_p = NULL;
	inicio_m = actual_m = NULL;
	inicio_c = actual_c = NULL;
	inicio_cs = actual_cs = NULL;

	cargar_usuarios();
	cargado_general();

	hGlobalInst = instancia;

	HWND iniciar_sesion = CreateDialog(instancia, MAKEINTRESOURCE(Login), NULL, login);
	ShowWindow(iniciar_sesion, cShow);


	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

BOOL CALLBACK login (HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND:
		if (LOWORD(wParam) == continuar && HIWORD(wParam) == BN_CLICKED) {
			bool correcto = true;
			HWND husuario = GetDlgItem(handler, IDC_EDIT1);
			HWND hcontraseña = GetDlgItem(handler, IDC_EDIT2);
			int lusuario = GetWindowTextLength(husuario);
			int lcontraseña = GetWindowTextLength(hcontraseña);
			char temp_us[MAX_PATH];
			char temp_cont[MAX_PATH];
			if (lusuario < 1) {
				MessageBox(handler, "Ingresa un usuario", "Error", MB_OK);
				break;
			}
			GetWindowText(husuario, temp_us, ++lusuario);

			string temp_usST = temp_us;

			while (temp_usST.compare(actual_u->usuario) != 0) {
				if (actual_u->siguiente_u == NULL) {
					MessageBox(handler, "El usuario no existe", "Error", MB_OK);
					actual_u = inicio_u;
					correcto = false;
					break;
				}
				actual_u = actual_u->siguiente_u;
			}
			if (correcto != true) break;

			if (lcontraseña < 1) {
				MessageBox(handler, "Ingresa la contraseña", "Error", MB_OK);
				break;
			}
			GetWindowText(hcontraseña, temp_cont, ++lcontraseña);

			string temp_contST = temp_cont;

			if (temp_contST.compare(actual_u->contraseña) != 0) {
				MessageBox(handler, "Contraseña incorrecta", "Error", MB_OK);
				correcto = false;
			}

			if (correcto == true) {
				//MessageBox(handler, "Este programa actualmente tiene archivos binarios cargados; puede simplemente agregar más paquetes, mecánicos, citas y clientes y presionar <<Salir>> para guardar los datos nuevos, o simplemente ir a la carpeta del proyecto PIA_ESTDAT y borrar todos los archivos .bin y .txt EXCEPTUANDO el archivo txt de <<usuarios>> para comenzar el programa desde cero.\n", "ACLARACIÓN", MB_ICONEXCLAMATION | MB_OK);
				HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
				ShowWindow(menu_principal, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
				DialogBox(hGlobalInst, MAKEINTRESOURCE(Registrar), handler, registrar_usuarios);

		}
		break;
	case WM_CLOSE:
		PostQuitMessage(1021);
		break;
	case WM_DESTROY:
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK menu (HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:{
		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT, sizeof(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno), (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str());

		cbCombo1 = GetDlgItem(handler, IDC_COMBO1);
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "Selecciona el mes");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "enero");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "febrero");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "marzo");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "abril");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "mayo");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "junio");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "julio");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "agosto");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "septiembre");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "octubre");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "noviembre");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "diciembre");
		SendMessage(cbCombo1, CB_SETCURSEL, 0, 0);
		cbCombo2 = GetDlgItem(handler, IDC_COMBO8);
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "Selecciona el mes");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "enero");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "febrero");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "marzo");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "abril");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "mayo");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "junio");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "julio");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "agosto");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "septiembre");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "octubre");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "noviembre");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "diciembre");
		SendMessage(cbCombo2, CB_SETCURSEL, 0, 0);
		cbCombo3 = GetDlgItem(handler, IDC_COMBO4);
		SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM) "Selecciona la semana");
		SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM) "1");
		SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM) "2");
		SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM) "3");
		SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM) "4");
		SendMessage(cbCombo3, CB_SETCURSEL, 0, 0);
		cbCombo5 = GetDlgItem(handler, IDC_COMBO9);
		SendMessage(cbCombo5, CB_ADDSTRING, 0, (LPARAM) "Selecciona la semana");
		SendMessage(cbCombo5, CB_ADDSTRING, 0, (LPARAM) "1");
		SendMessage(cbCombo5, CB_ADDSTRING, 0, (LPARAM) "2");
		SendMessage(cbCombo5, CB_ADDSTRING, 0, (LPARAM) "3");
		SendMessage(cbCombo5, CB_ADDSTRING, 0, (LPARAM) "4");
		SendMessage(cbCombo5, CB_SETCURSEL, 0, 0);
		cbCombo4 = GetDlgItem(handler, IDC_COMBO3);
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "Selecciona la especialidad");
		while (actual_p != NULL) {
			SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM)actual_p->nombre);
			actual_p = actual_p->siguiente_p;
		}
		actual_p = inicio_p;
		SendMessage(cbCombo4, CB_SETCURSEL, 0, 0);
		cbCombo6 = GetDlgItem(handler, IDC_COMBO10);
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "Selecciona el mes");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "enero");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "febrero");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "marzo");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "abril");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "mayo");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "junio");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "julio");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "agosto");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "septiembre");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "octubre");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "noviembre");
		SendMessage(cbCombo6, CB_ADDSTRING, 0, (LPARAM) "diciembre");
		SendMessage(cbCombo6, CB_SETCURSEL, 0, 0);
		
		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == Paquetes && HIWORD(wParam) == BN_CLICKED) {
			HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(ventana_paquetes), NULL, paquetes);
			ShowWindow(ventana, SW_SHOW);
			DestroyWindow(handler);
		}
		else if (LOWORD(wParam) == ID_MODIFICAROELIMINAR_PAQUETES && HIWORD(wParam) == BN_CLICKED) {
			if (inicio_p == NULL) {
				MessageBox(handler, "No hay paquetes registrados.", "Error", MB_OK);
				break;
			}
			else {
				op_ed = 1;
				HWND ventana_buscar = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Pregunta), NULL, buscar);
				ShowWindow(ventana_buscar, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		else if (LOWORD(wParam) == ID_MECANICOS_REGISTRARMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (inicio_p == NULL) {
				MessageBox(handler, "No hay paquetes registrados.", "Error", MB_OK);
				break;
			}
			else {
				HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Mecanicos), NULL, mecanicos);
				ShowWindow(ventana, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		else if (LOWORD(wParam) == ID_REGISTRODEMECANICOS_REPORTE && HIWORD(wParam) == BN_CLICKED) {
			if (inicio_m == NULL) {
				MessageBox(handler, "No hay mecánicos registrados.", "Error", MB_OK);
				break;
			}
			else {
				DialogBox(hGlobalInst, MAKEINTRESOURCE(Reporte1), handler, Lista);
			}
		}
		else if (LOWORD(wParam) == ID_MODIFICAROELIMINAR_MECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (inicio_m == NULL) {
				MessageBox(handler, "No hay mecánicos registrados.", "Error", MB_OK);
				break;
			}
			else {
				op_ed = 2;
				HWND ventana_buscar = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Pregunta), NULL, buscar);
				ShowWindow(ventana_buscar, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		if (LOWORD(wParam) == ID_REGISTRODECLIENTES_REGISTRAR && HIWORD(wParam) == BN_CLICKED) {
			HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(CLIENTES), NULL, cliente);
			ShowWindow(ventana, SW_SHOW);
			DestroyWindow(handler);
		}
		if (LOWORD(wParam) == ID_REGISTRODECLIENTES_REPORTE && HIWORD(wParam) == BN_CLICKED) {
			if (inicio_c == NULL) {
				MessageBox(handler, "No hay clientes registrados.", "Error", MB_OK);
				break;
			}
			else {
				DialogBox(hGlobalInst, MAKEINTRESOURCE(Reporte1), handler, Lista_clientes);
			}
		}
		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (inicio_c == NULL || inicio_m == NULL) {
				MessageBox(handler, "No hay clientes y/o mecánicos registrados.", "Error", MB_OK);
				break;
			}
			else {
				HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(CITAS), NULL, citas);
				ShowWindow(ventana, SW_SHOW);

				SetTimer(ventana, ID_Timer, 1000, (TIMERPROC)NULL);


				DestroyWindow(handler);
			}
		}
		if (LOWORD(wParam) == ID_MODIFICAROELIMINAR_CLIENTES && HIWORD(wParam) == BN_CLICKED) {
			if (inicio_c == NULL) {
				MessageBox(handler, "No hay clientes registrados.", "Error", MB_OK);
				break;
			}
			else {
				op_ed = 3;
				HWND ventana_buscar = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Pregunta), NULL, buscar);
				ShowWindow(ventana_buscar, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		if (LOWORD(wParam) == ID_MODIFICAROELIMINAR_CITAS && HIWORD(wParam) == BN_CLICKED) {
			if (inicio_cs== NULL) {
				MessageBox(handler, "No hay citas registradas.", "Error", MB_OK);
				break;
			}
			else {
				op_ed = 4;
				HWND ventana_buscar = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Pregunta), NULL, buscar);
				ShowWindow(ventana_buscar, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		if (LOWORD(wParam) == SALIR && HIWORD(wParam) == BN_CLICKED) {
			guardado_general(inicio_p, inicio_m, inicio_c, inicio_cs);
			PostQuitMessage(1021);
		}
		if (LOWORD(wParam) == IDC_BUTTON4 && HIWORD(wParam) == BN_CLICKED) {
			actual_cs = inicio_cs;
			bool buscar = false;
			HWND hclave = GetDlgItem(handler, IDC_EDIT2);
			int lclave = GetWindowTextLength(hclave);
			char temp_cl[MAX_PATH];
			if (lclave < 1) {
				MessageBox(handler, "Debe ingresar la clave de cita", "Error", MB_OK);
				break;
			}
			GetWindowText(hclave, temp_cl, ++lclave);
			string temp_clST = temp_cl;
			while (actual_cs != NULL) {
				if (stoi(temp_clST) == actual_cs->num) {
					buscar = true;
					break;
				}
				actual_cs = actual_cs->siguiente_cs;
			}
			if (buscar != true) {
				MessageBox(handler, "El número de cita no existe.", "Error", MB_OK);
				break;
			}

			DialogBox(hGlobalInst, MAKEINTRESOURCE(cita_datos), handler, mostrar_citas);
		}
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			tipo_busqueda = 1;
			actual_m = inicio_m, actual_cs = inicio_cs;;
			bool existe = false;
			HWND hclave = GetDlgItem(handler, IDC_EDIT1);
			HWND hmes = GetDlgItem(handler, IDC_COMBO1);
			int lclave = GetWindowTextLength(hclave);
			int lmes = GetWindowTextLength(hmes);
			int n = 0, inf = 0, mitad, sup;
			int combo1 = SendMessage(cbCombo1, CB_GETCURSEL, 0, 0);
			string arr_num[20];
			char temp_cl[MAX_PATH];
			char temp_mes[MAX_PATH];
			if (lclave < 1) {
				MessageBox(handler, "Debe ingresar el numero de empleado.", "Error", MB_OK);
				break;
			}
			if (combo1 == 0) {
				MessageBox(NULL, "Seleccione el mes.", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			GetWindowText(hclave, temp_cl, ++lclave);
			GetWindowText(hmes, temp_mes, ++lmes);

			string temp_clST = temp_cl;

			while (actual_m != NULL) {
				arr_num[n] = actual_m->num_empleado;
				n++;
				actual_m = actual_m->siguiente_m;
			}
			actual_m = inicio_m;
			Quicksort(arr_num, 0, n - 1);
			sup = n;

			while (inf <= sup)
			{
				mitad = (inf + sup) / 2;
				if (arr_num[mitad] == temp_clST) {
					existe = true;
					break;
				}

				if (arr_num[mitad] > temp_clST) {
					sup = mitad;
					mitad = (inf + sup) / 2;
				}
				if (arr_num[mitad] < temp_clST) {
					inf = mitad;
					mitad = (inf + sup) / 2;
				}
			}
			if (existe != true) {
				MessageBox(handler, "El número de empleado no existe.", "Error", MB_OK);
				break;
			}

			while (actual_m != NULL) {
				if (temp_clST.compare(actual_m->num_empleado) == 0) {
					break;
				}
				actual_m = actual_m->siguiente_m;
			}
			
			mes = temp_mes;
			DialogBox(hGlobalInst, MAKEINTRESOURCE(Lista_citas), handler, reporte_citas);
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			actual_cs = inicio_cs;
			tipo_busqueda = 2;
			int combo1 = SendMessage(cbCombo2, CB_GETCURSEL, 0, 0);
			int combo2 = SendMessage(cbCombo4, CB_GETCURSEL, 0, 0);
			int combo3 = SendMessage(cbCombo3, CB_GETCURSEL, 0, 0);
			HWND hmes = GetDlgItem(handler, IDC_COMBO8);
			HWND hsem = GetDlgItem(handler, IDC_COMBO4);
			HWND hesp = GetDlgItem(handler, IDC_COMBO3);
			int lmes = GetWindowTextLength(hmes);
			int lsem = GetWindowTextLength(hsem);
			int lesp = GetWindowTextLength(hesp);
			char temp_mes[MAX_PATH];
			char temp_sem[MAX_PATH];
			char temp_esp[MAX_PATH];
			if (combo1 == 0 || combo2 == 0 || combo3 == 0) {
				MessageBox(NULL, "Seleccione semana, mes y especialidad.", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			GetWindowText(hmes, temp_mes, ++lmes);
			GetWindowText(hsem, temp_sem, ++lsem);
			GetWindowText(hesp, temp_esp, ++lesp);
			esptemp = temp_esp;
			sem = temp_sem;
			mes = temp_mes;
			DialogBox(hGlobalInst, MAKEINTRESOURCE(Lista_citas), handler, reporte_citas);
		}
		if (LOWORD(wParam) == IDC_BUTTON3 && HIWORD(wParam) == BN_CLICKED) {
			actual_cs = inicio_cs;
			tipo_busqueda = 3;
			int combo1 = SendMessage(cbCombo5, CB_GETCURSEL, 0, 0);
			int combo2 = SendMessage(cbCombo6, CB_GETCURSEL, 0, 0);
			HWND hmes = GetDlgItem(handler, IDC_COMBO10);
			HWND hsem = GetDlgItem(handler, IDC_COMBO9);
			int lmes = GetWindowTextLength(hmes);
			int lsem = GetWindowTextLength(hsem);
			char temp_mes[MAX_PATH];
			char temp_sem[MAX_PATH];
			if (combo1 == 0 || combo2 == 0) {
				MessageBox(NULL, "Seleccione semana y mes.", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			GetWindowText(hmes, temp_mes, ++lmes);
			GetWindowText(hsem, temp_sem, ++lsem);
			sem = temp_sem;
			mes = temp_mes;
			DialogBox(hGlobalInst, MAKEINTRESOURCE(Lista_citas), handler, reporte_citas);
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(1021);
		break;
	case WM_DESTROY:
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK registrar_usuarios(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			bool correcto = true;
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			HWND hap_p = GetDlgItem(handler, IDC_EDIT2);
			HWND hap_m = GetDlgItem(handler, IDC_EDIT3);
			HWND husser = GetDlgItem(handler, IDC_EDIT4);
			HWND hcont = GetDlgItem(handler, IDC_EDIT5);
			int lnombre = GetWindowTextLength(hnombre);
			int lap_p = GetWindowTextLength(hap_p);
			int lap_m = GetWindowTextLength(hap_m);
			int lusser = GetWindowTextLength(husser);
			int lcont = GetWindowTextLength(hcont);
			char temp_n[MAX_PATH];
			char temp_ap[MAX_PATH];
			char temp_am[MAX_PATH];
			char temp_uss[MAX_PATH];
			char temp_cont[MAX_PATH];
			if (lnombre < 1 || lusser < 1 || lap_p < 1 || lap_m < 1 || lcont < 1) {
				MessageBox(handler, "Llena todos los campos", "Error", MB_OK);
				break;
			}
			GetWindowText(hnombre, temp_n, ++lnombre);
			GetWindowText(hap_p, temp_ap, ++lap_p);
			GetWindowText(hap_m, temp_am, ++lap_m);
			GetWindowText(husser, temp_uss, ++lusser);
			GetWindowText(hcont, temp_cont, ++lcont);
			validar_nombres(temp_n, correcto);
			validar_nombres(temp_ap, correcto);
			validar_nombres(temp_am, correcto);
			if (correcto != true) break;
			for (int i = 0; temp_uss[i]; i++) {
				if (isalnum(temp_uss[i]) == 0) {
					MessageBox(NULL, "El nombre de usuario solo acepta numeros y letras.", "", MB_OK);
					correcto = false;
					break;
				}
			}
			while (actual_u != NULL) {
				if (actual_u->usuario.compare(temp_uss) == 0) {
					MessageBox(NULL, "Nombre usuario no disponible.", "", MB_OK);
					correcto = false;
					break;
				}
				actual_u = actual_u->siguiente_u;
			}
			actual_u = inicio_u;
			if (correcto != true) break;
			if (inicio_u == NULL) {
				inicio_u = new usuarios;
				inicio_u->nombre, temp_n;
				inicio_u->apellido_paterno = temp_ap;
				inicio_u->apellido_materno = temp_am;
				inicio_u->usuario = temp_uss;
				inicio_u->contraseña = temp_cont;
				inicio_u->siguiente_u = NULL;
				inicio_u->anterior_u = NULL;
				actual_u = inicio_u;
			}
			else {
				while (actual_u->siguiente_u != NULL)
					actual_u = actual_u->siguiente_u;
				actual_u->siguiente_u = new usuarios;
				actual_u->siguiente_u->anterior_u = actual_u;
				actual_u = actual_u->siguiente_u;
				actual_u->nombre = temp_n;
				actual_u->apellido_paterno = temp_ap;
				actual_u->apellido_materno = temp_am;
				actual_u->usuario = temp_uss;
				actual_u->contraseña = temp_cont;
				actual_u->siguiente_u = NULL;
				actual_u = inicio_u;
			}
			EndDialog(handler, Login);
		}
		break;
	case WM_CLOSE:
		break;
	case WM_DESTROY:
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}
BOOL CALLBACK paquetes(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT, sizeof(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno), (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str());
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			bool correcto = true;
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			HWND hclave = GetDlgItem(handler, IDC_EDIT2);
			HWND hcosto = GetDlgItem(handler, IDC_EDIT3);
			HWND hdesc = GetDlgItem(handler, IDC_EDIT4);
			int lnombre = GetWindowTextLength(hnombre);
			int lclave = GetWindowTextLength(hclave);
			int lcosto = GetWindowTextLength(hcosto);
			int ldesc = GetWindowTextLength(hdesc);
			char temp_n[MAX_PATH];
			char temp_cl[MAX_PATH];
			char temp_cos[MAX_PATH];
			char temp_desc[MAX_PATH];
			if (lnombre < 1 || lclave < 1 || lcosto < 1 || ldesc < 1) {
				MessageBox(handler, "Llena todos los campos", "Error", MB_OK);
				break;
			}
			GetWindowText(hnombre, temp_n, ++lnombre);
			GetWindowText(hclave, temp_cl, ++lclave);
			GetWindowText(hcosto, temp_cos, ++lcosto);
			GetWindowText(hdesc, temp_desc, ++ldesc);
			_strupr(temp_cl);
			int numpuntos = 0;
			for (int i = 0; temp_cos[i]; i++) {
				if (temp_cos[0] == '0') {
					MessageBox(handler, "El costo no puede ser 0 o empezar en 0.", "Error", MB_OK);
					correcto = false;
					break;
				}
				if (temp_cos[i] < 48 || temp_cos[i] > 57) {
					if (!(temp_cos[i] == '.')) {
						MessageBox(handler, "Hay caracteres en el costo.", "Error", MB_OK);
						correcto = false;
						break;
					}
				}
				if ((temp_cos[i] <= 47 || temp_cos[i] >= 58)) {
					if (temp_cos[i] == 46 || temp_cos[i] == 46) {
						if ((temp_cos[i] == '.')) {
							numpuntos++;
						}
						continue;
					}
					MessageBox(handler, "Hay caracteres en el costo.", "Error", MB_OK);
					correcto = false;
					break;
				}
				if (numpuntos == 2) {
					MessageBox(handler, "No puedes poner más de 2 puntos a el costo.", "Error", MB_OK);
					correcto = false;
					break;
				}
			}
			if (correcto == true) {
				int i;
				for (i = 0; temp_cl[i]; i++) {
					if (!(isalpha(temp_cl[i]) == 0) && i <= 1) {
						continue;
					}
					else {
						if ((i > 1) && (temp_cl[i] >= 48 && temp_cl[i] <= 57)) {
							continue;
						}
						else {
							MessageBox(handler, "La clave es incorrecta.", "Error", MB_OK);
							correcto = false;
							break;
						}
					}
				}
				if (correcto == true && i != 5) {
					MessageBox(handler, "La clave debe tener una logitud de 5.", "Error", MB_OK);
					correcto = false;
					break;
				}
			}
			if (correcto != true) break;
			else {
				string temp_clST = temp_cl;
				while (actual_p != NULL) {
					if (temp_clST.compare(actual_p->clave) == 0) {
						correcto = false;
						MessageBox(handler, "La clave ya existe.", "Error", MB_OK);
						break;
					}
					actual_p = actual_p->siguiente_p;
				}
				actual_p = inicio_p;
				if (correcto != true) break;
				if (inicio_p == NULL) {
					inicio_p = new paquetes_disp;
					strcpy(inicio_p->nombre, temp_n);
					inicio_p->clave = temp_cl;
					inicio_p->costo = temp_cos;
					strcpy(inicio_p->descripcion, temp_desc);
					inicio_p->siguiente_p = NULL;
					inicio_p->anterior_p = NULL;
					actual_p = inicio_p;
				}
				else {
					while (actual_p->siguiente_p != NULL)
						actual_p = actual_p->siguiente_p;
					actual_p->siguiente_p = new paquetes_disp;
					actual_p->siguiente_p->anterior_p = actual_p;
					actual_p = actual_p->siguiente_p;
					strcpy(actual_p->nombre, temp_n);
					actual_p->clave = temp_cl;
					actual_p->costo = temp_cos;
					strcpy(actual_p->descripcion, temp_desc);
					actual_p->siguiente_p = NULL;
					actual_p = inicio_p;
				}

				HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
				ShowWindow(menu_principal, SW_SHOW);
				DestroyWindow(handler);
			}

		}
		break;
	case WM_CLOSE: {
				   HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
				   ShowWindow(menu_principal, SW_SHOW);
				   DestroyWindow(handler);
				   break;
	}
	case WM_DESTROY:
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK buscar(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
		switch (op_ed) {
		case 2:
			SendDlgItemMessage(handler, send, WM_SETTEXT, sizeof("Num de empleado:"), (LPARAM)"Num de empleado");
			break;
		case 3: 
			SendDlgItemMessage(handler, send, WM_SETTEXT, sizeof("Placa de vehículo:"), (LPARAM)"Placa de vehículo");
			break;
		case 4:
			SendDlgItemMessage(handler, send, WM_SETTEXT, sizeof("Clave de cita:"), (LPARAM)"Clave de cita");
			break;
		}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			switch (op_ed) {
			case 1: {
				bool correcto = false;
				HWND hclave = GetDlgItem(handler, IDC_EDIT1);
				int lclave = GetWindowTextLength(hclave);
				char temp_clave[MAX_PATH];
				if (lclave < 1) {
					MessageBox(handler, "Ingresa la clave de paquete", "Error", MB_OK);
					break;
				}
				GetWindowText(hclave, temp_clave, ++lclave);
				_strupr(temp_clave);
				string temp_clST = temp_clave;
				while (actual_p != NULL) {
					if (temp_clST.compare(actual_p->clave) == 0) {
						correcto = true;
						break;
					}
					actual_p = actual_p->siguiente_p;
				}
				if (correcto == false) {
					MessageBox(handler, "La clave del paquete no existe.", "Error", MB_OK);
					actual_p = inicio_p;
					break;
				}
				else {
					if (actual_p->siguiente_p == NULL && actual_p->anterior_p == NULL) {
						delete actual_p;
						inicio_p = actual_p = NULL;
					}
					else if (actual_p->anterior_p == NULL) {
						inicio_p = actual_p->siguiente_p;
						delete actual_p;
						inicio_p->anterior_p = NULL;
						actual_p = inicio_p;
					}
					else if (actual_p->siguiente_p == NULL) {
						actual_p->anterior_p->siguiente_p = NULL;
						delete actual_p;
						actual_p = inicio_p;
					}
					else {
						actual_p->anterior_p->siguiente_p = actual_p->siguiente_p;
						actual_p->siguiente_p->anterior_p = actual_p->anterior_p;
						delete actual_p;
						actual_p = inicio_p;
					}
					MessageBox(handler, "El paquete se eliminó.", "", MB_OK);
					HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
					ShowWindow(ventana, SW_SHOW);
					DestroyWindow(handler);
				}
				break;
			}
			case 2: {
				bool correcto = false;
				HWND hnum = GetDlgItem(handler, IDC_EDIT1);
				int lnum = GetWindowTextLength(hnum);
				char temp_num[MAX_PATH];
				if (lnum < 1) {
					MessageBox(handler, "Ingresa el número de empleado", "Error", MB_OK);
					break;
				}
				GetWindowText(hnum, temp_num, ++lnum);
				string temp_numST = temp_num;
				while (actual_m != NULL) {
					if (temp_numST.compare(actual_m->num_empleado) == 0) {
						correcto = true;
						break;
					}
					actual_m = actual_m->siguiente_m;
				}

				if (correcto == false) {
					MessageBox(handler, "El número de empleado no existe.", "Error", MB_OK);
					actual_m = inicio_m;
					break;
				}
				else {
					if (actual_m->siguiente_m == NULL && actual_m->anterior_m == NULL) {
						delete actual_m;
						inicio_m = actual_m = NULL;
					}
					else if (actual_m->anterior_m == NULL) {
						inicio_m = actual_m->siguiente_m;
						delete actual_m;
						inicio_m->anterior_m = NULL;
						actual_m = inicio_m;
					}
					else if (actual_m->siguiente_m == NULL) {
						actual_m->anterior_m->siguiente_m = NULL;
						delete actual_m;
						actual_m = inicio_m;
					}
					else {
						actual_m->anterior_m->siguiente_m = actual_m->siguiente_m;
						actual_m->siguiente_m->anterior_m = actual_m->anterior_m;
						delete actual_m;
						actual_m = inicio_m;
					}
					MessageBox(handler, "Mécanico despedido.", "", MB_OK);
					HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
					ShowWindow(ventana, SW_SHOW);
					DestroyWindow(handler);
					break;
				}
			}
			case 3: {
				bool correcto = false;
				HWND hplac = GetDlgItem(handler, IDC_EDIT1);
				int lplac = GetWindowTextLength(hplac);
				char temp_plac[MAX_PATH];
				if (lplac < 1) {
					MessageBox(handler, "Ingresa la placa", "Error", MB_OK);
					break;
				}
				GetWindowText(hplac, temp_plac, ++lplac);
				_strupr(temp_plac);
				string temp_pST = temp_plac;
				while (actual_c != NULL) {
					if (temp_pST.compare(actual_c->placas) == 0) {
						correcto = true;
						break;
					}
					actual_c = actual_c->siguiente_c;
				}
				if (correcto == false) {
					MessageBox(handler, "La placa de vehículo no existe.", "", MB_OK);
					actual_c = inicio_c;
					break;
				}
				else {
					if (actual_c->siguiente_c == NULL && actual_c->anterior_c == NULL) {
						delete actual_c;
						inicio_c = actual_c = NULL;
					}
					else if (actual_c->anterior_c == NULL) {
						inicio_c = actual_c->siguiente_c;
						delete actual_c;
						inicio_c->anterior_c = NULL;
						actual_c = inicio_c;
					}
					else if (actual_c->siguiente_c == NULL) {
						actual_c->anterior_c->siguiente_c = NULL;
						delete actual_c;
						actual_c = inicio_c;
					}
					else {
						actual_c->anterior_c->siguiente_c = actual_c->siguiente_c;
						actual_c->siguiente_c->anterior_c = actual_c->anterior_c;
						delete actual_c;
						actual_c = inicio_c;
					}
					MessageBox(handler, "El cliente se eliminó.", "", MB_OK);
					HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
					ShowWindow(ventana, SW_SHOW);
					DestroyWindow(handler);
				}
			}
			case 4: {
				bool correcto = false;
				HWND hc = GetDlgItem(handler, IDC_EDIT1);
				int lc = GetWindowTextLength(hc);
				char temp_c[MAX_PATH];
				if (lc < 1) {
					MessageBox(handler, "Ingresa el número de cita", "Error", MB_OK);
					break;
				}
				GetWindowText(hc, temp_c, ++lc);
				_strupr(temp_c);
				string temp_cST = temp_c;
				while (actual_cs != NULL) {
					if (stoi(temp_cST) == actual_cs->num) {
						correcto = true;
						break;
					}
					actual_cs = actual_cs->siguiente_cs;
				}
				if (correcto == false) {
					MessageBox(handler, "El número de cita no existe.", "", MB_OK);
					actual_cs = inicio_cs;
					break;
				}
				else {
					if (actual_cs->estado == "Pendiente") {
						MessageBox(handler, "La cita está pendiente, no puede eliminarla del registro.", "", MB_OK);
						actual_cs = inicio_cs;
						break;
					}
					if (actual_cs->siguiente_cs == NULL && actual_cs->anterior_cs == NULL) {
						delete actual_cs;
						inicio_cs = actual_cs = NULL;
					}
					else if (actual_cs->anterior_cs == NULL) {
						inicio_cs = actual_cs->siguiente_cs;
						delete actual_cs;
						inicio_cs->anterior_cs = NULL;
						actual_cs = inicio_cs;
					}
					else if (actual_cs->siguiente_cs == NULL) {
						actual_cs->anterior_cs->siguiente_cs = NULL;
						delete actual_cs;
						actual_cs = inicio_cs;
					}
					else {
						actual_cs->anterior_cs->siguiente_cs = actual_cs->siguiente_cs;
						actual_cs->siguiente_cs->anterior_cs = actual_cs->anterior_cs;
						delete actual_cs;
						actual_cs = inicio_cs;
					}
					MessageBox(handler, "El cliente se eliminó.", "", MB_OK);
					HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
					ShowWindow(ventana, SW_SHOW);
					DestroyWindow(handler);
				}

				}
			}
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			switch (op_ed) {
			case 1:{
				bool correcto = false;
				cont = 0;
				HWND hclave = GetDlgItem(handler, IDC_EDIT1);
				int lclave = GetWindowTextLength(hclave);
				char temp_clave[MAX_PATH];
				if (lclave < 1) {
					MessageBox(handler, "Ingresa la clave", "Error", MB_OK);
					break;
				}
				GetWindowText(hclave, temp_clave, ++lclave);
				_strupr(temp_clave);
				string temp_clST = temp_clave;
				while (actual_p != NULL) {
					if (temp_clST.compare(actual_p->clave) == 0) {
						correcto = true;
						break;
					}
					actual_p = actual_p->siguiente_p;
					cont++;
				}
				if (correcto == false) {
					MessageBox(handler, "La clave no existe", "Ocurrió un problema", MB_OK);
					actual_p = inicio_p;
					break;
				}
				else {
					HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(ventana_paquetes), NULL, borrar_editar_paquetes);
					ShowWindow(ventana, SW_SHOW);
					DestroyWindow(handler);
				}
				break;
			}
			case 2: {
				bool correcto = false;
				cont = 0;
				HWND hnum = GetDlgItem(handler, IDC_EDIT1);
				int lnum = GetWindowTextLength(hnum);
				char temp_num[MAX_PATH];
				if (lnum < 1) {
					MessageBox(handler, "Ingresa el número de empleado", "Error", MB_OK);
					break;
				}
				GetWindowText(hnum, temp_num, ++lnum);
				_strupr(temp_num);
				string temp_numlST = temp_num;
				while (actual_m != NULL) {
					if (temp_numlST.compare(actual_m->num_empleado) == 0) {
						correcto = true;
						break;
					}
					actual_m = actual_m->siguiente_m;
					cont++;
				}
				if (correcto == false) {
					MessageBox(handler, "El número de empleado no existe", "Ocurrió un problema", MB_OK);
					actual_p = inicio_p;
					break;
				}
				else {
					HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Mecanicos), NULL, borrar_editar_mecanicos);
					ShowWindow(ventana, SW_SHOW);
					DestroyWindow(handler);
				}
				break;
			}
			case 3: {
				bool correcto = false;
				HWND hplac = GetDlgItem(handler, IDC_EDIT1);
				int lplac = GetWindowTextLength(hplac);
				char temp_plac[MAX_PATH];
				if (lplac < 1) {
					MessageBox(handler, "Ingresa la placa", "Error", MB_OK);
					break;
				}
				GetWindowText(hplac, temp_plac, ++lplac);
				_strupr(temp_plac);

				string temp_pST = temp_plac;
				while (actual_c != NULL) {
					if (temp_pST.compare(actual_c->placas) == 0) {
						correcto = true;
						break;
					}
					actual_c = actual_c->siguiente_c;
				}
				if (correcto == false) {
					MessageBox(handler, "La placa de vehículo no existe.", "", MB_OK);
					actual_c = inicio_c;
					break;
				}
				else {
					HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(CLIENTES), NULL, borrar_editar_cliente);
					ShowWindow(ventana, SW_SHOW);
					DestroyWindow(handler);
				}
				break;
			}
			case 4: {
				bool correcto = false;
				HWND hc = GetDlgItem(handler, IDC_EDIT1);
				int lc = GetWindowTextLength(hc);
				char temp_c[MAX_PATH];
				if (lc < 1) {
					MessageBox(handler, "Ingresa el número de cita", "Error", MB_OK);
					break;
				}
				GetWindowText(hc, temp_c, ++lc);
				_strupr(temp_c);
				string temp_cST = temp_c;
				while (actual_cs != NULL) {
					if (stoi(temp_cST) == actual_cs->num) {
						correcto = true;
						break;
					}
					actual_cs = actual_cs->siguiente_cs;
				}
				if (correcto == false) {
					MessageBox(handler, "El número de cita no existe.", "", MB_OK);
					actual_cs = inicio_cs;
					break;
				}
				else {
					if (actual_cs->estado != "Pendiente"){
						MessageBox(handler, "La cita ya se atendió o fue cancelada, no es posible editar sus datos.", "", MB_OK);
						actual_cs = inicio_cs;
						break;
					}
					else {
						HWND ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(CITAS), NULL, borrar_editar_citas);
						ShowWindow(ventana, SW_SHOW);
						DestroyWindow(handler);
					}
				}

			}
			}
		}
		break;
	case WM_CLOSE:
		break;
	case WM_DESTROY:
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK borrar_editar_paquetes(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:

		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT, sizeof(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno), (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str());
		SendDlgItemMessage(handler, IDC_EDIT1, WM_SETTEXT, sizeof(actual_p->nombre), (LPARAM)actual_p->nombre);
		SendDlgItemMessage(handler, IDC_EDIT2, WM_SETTEXT, sizeof(actual_p->clave.c_str()), (LPARAM)actual_p->clave.c_str());
		SendDlgItemMessage(handler, IDC_EDIT3, WM_SETTEXT, sizeof(actual_p->costo.c_str()), (LPARAM)actual_p->costo.c_str());
		SendDlgItemMessage(handler, IDC_EDIT4, WM_SETTEXT, sizeof(actual_p->descripcion), (LPARAM)actual_p->descripcion);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			bool correcto = true;
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			HWND hclave = GetDlgItem(handler, IDC_EDIT2);
			HWND hcosto = GetDlgItem(handler, IDC_EDIT3);
			HWND hdesc = GetDlgItem(handler, IDC_EDIT4);
			int lnombre = GetWindowTextLength(hnombre);
			int lclave = GetWindowTextLength(hclave);
			int lcosto = GetWindowTextLength(hcosto);
			int ldesc = GetWindowTextLength(hdesc);
			char temp_n[MAX_PATH];
			char temp_cl[MAX_PATH];
			char temp_cos[MAX_PATH];
			char temp_desc[MAX_PATH];
			if (lnombre < 1 || lclave < 1 || lcosto < 1 || ldesc < 1) {
				MessageBox(handler, "Llena todos los campos", "Error", MB_OK);
				break;
			}
			GetWindowText(hnombre, temp_n, ++lnombre);
			GetWindowText(hclave, temp_cl, ++lclave);
			GetWindowText(hcosto, temp_cos, ++lcosto);
			GetWindowText(hdesc, temp_desc, ++ldesc);
			_strupr(temp_cl);
			int numpuntos = 0;
			for (int i = 0; temp_cos[i]; i++) {
				if (temp_cos[0] == '0') {
					MessageBox(handler, "El costo no puede ser 0 o empezar en 0.", "Error", MB_OK);
					correcto = false;
					break;
				}
				if (temp_cos[i] < 48 || temp_cos[i] > 57) {
					if (!(temp_cos[i] == '.')) {
						MessageBox(handler, "Hay caracteres en el costo.", "Error", MB_OK);
						correcto = false;
						break;
					}
				}
				if ((temp_cos[i] <= 47 || temp_cos[i] >= 58)) {
					if (temp_cos[i] == 46 || temp_cos[i] == 46) {
						if ((temp_cos[i] == '.')) {
							numpuntos++;
						}
						continue;
					}
					MessageBox(handler, "Hay caracteres en el costo.", "Error", MB_OK);
					correcto = false;
					break;
				}
				if (numpuntos == 2) {
					MessageBox(handler, "No puedes poner más de 2 puntos a el costo.", "Error", MB_OK);
					correcto = false;
					break;
				}
			}
			
			int i;
			for (i = 0; temp_cl[i]; i++) {
				if (!(isalpha(temp_cl[i]) == 0) && i <= 1) {
					continue;
				}
				else {
					if ((i > 1) && (temp_cl[i] >= 48 && temp_cl[i] <= 57)) {
						continue;
					}
					else {
						MessageBox(handler, "La clave es incorrecta.", "Error", MB_OK);
						correcto = false;
						break;
					}
				}
			}
			if (correcto == true && i != 5) {
				MessageBox(handler, "La clave debe tener una logitud de 5.", "Error", MB_OK);
				correcto = false;
				break;
			}

			if (correcto == false) break;
			string temp_clST = temp_cl;
			actual_p = inicio_p;
			string temp_nST = temp_n;
			while (actual_p != NULL) {
				if (temp_clST.compare(actual_p->clave) == 0) {
					if (temp_nST.compare(actual_p->nombre) != 0) {
						correcto = false;
						MessageBox(handler, "La clave ya existe.", "Error", MB_OK);
						break;
					}
				}
				actual_p = actual_p->siguiente_p;
			}
			actual_p = inicio_p;

			for (int j = 0; j < cont; j++) {
				actual_p = actual_p->siguiente_p;
			}
			if (correcto == true) {
				strcpy(actual_p->nombre, temp_n);
				actual_p->clave = temp_cl;
				actual_p->costo = temp_cos;
				strcpy(actual_p->descripcion, temp_desc);
				actual_p = inicio_p;
				HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
				ShowWindow(menu_principal, SW_SHOW);
				DestroyWindow(handler);
			}

		}
		break;
	case WM_CLOSE: {
		actual_p = inicio_p;
		HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
		ShowWindow(menu_principal, SW_SHOW);
		DestroyWindow(handler);
		break; }
	case WM_DESTROY:
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK mecanicos(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
switch (msg) {
	case WM_INITDIALOG: {
		cbCombo1 = GetDlgItem(handler, Esp);
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "Selecciona la especialidad");
		while (actual_p != NULL) {
			SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM)actual_p->nombre);
			actual_p = actual_p->siguiente_p;
		}
		actual_p = inicio_p;
		SendMessage(cbCombo1, CB_SETCURSEL, 0, 0);

		cbCombo2 = GetDlgItem(handler, Turn);
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "Selecciona el turno");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "MATUTINO");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "VESPERTINO");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "NOCTURNO");
		SendMessage(cbCombo2, CB_SETCURSEL, 0, 0);
		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT, sizeof(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno), (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str());
		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			OPENFILENAME Img;
			ZeroMemory(&Img, sizeof(OPENFILENAME));
			Img.hwndOwner = handler;
			Img.nMaxFile = MAX_PATH;
			Img.lpstrDefExt = "bmp";
			Img.lStructSize = sizeof(OPENFILENAME);
			Img.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			Img.lpstrFile = fotos;
			Img.lpstrFilter = "Selecciona una imagen en formato bmp\0*.bmp";
			if (GetOpenFileName(&Img) == TRUE) {
				HBITMAP hDirImagen;
				hDirImagen = (HBITMAP)LoadImage(NULL, fotos, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPictureControl = GetDlgItem(handler, Fotografia);
				SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
				verf = true;
			}
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
		#pragma region RCP
			bool correcto = true;
			bool dias_aux[5] = { false, false, false, false, false };
			int combo1 = SendMessage(cbCombo1, CB_GETCURSEL, 0, 0);
			int combo2 = SendMessage(cbCombo2, CB_GETCURSEL, 0, 0);
			HWND hnum_empleado = GetDlgItem(handler, IDC_EDIT1);
			HWND hnombre = GetDlgItem(handler, IDC_EDIT2);
			HWND hap_p = GetDlgItem(handler, IDC_EDIT3);
			HWND hap_m = GetDlgItem(handler, IDC_EDIT4);
			HWND htel = GetDlgItem(handler, IDC_EDIT5);
			HWND hesp = GetDlgItem(handler, Esp);
			HWND hturn = GetDlgItem(handler, Turn);
			int lnum = GetWindowTextLength(hnum_empleado);
			int lnombre = GetWindowTextLength(hnombre);
			int lap_p = GetWindowTextLength(hap_p);
			int lap_m = GetWindowTextLength(hap_m);
			int ltel = GetWindowTextLength(htel);
			int lesp = GetWindowTextLength(hesp);
			int lturn = GetWindowTextLength(hturn);
			char temp_num[MAX_PATH];
			char temp_n[MAX_PATH];
			char temp_ap[MAX_PATH];
			char temp_am[MAX_PATH];
			char temp_tel[MAX_PATH];
			char temp_esp[MAX_PATH];
			char temp_turn[MAX_PATH];
			if (lnombre < 1 || lnum < 1 || lap_p < 1 || lap_m< 1 || ltel < 1) {
				MessageBox(handler, "Llena todos los campos", "Error", MB_OK);
				break;
			}
			if (combo1 == 0 || combo2 == 0) {
				MessageBox(NULL, "Seleccione algo del combobox", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			GetWindowText(hnum_empleado, temp_num, ++lnum);
			GetWindowText(hnombre, temp_n, ++lnombre);
			GetWindowText(hap_p, temp_ap, ++lap_p);
			GetWindowText(hap_m, temp_am, ++lap_m);
			GetWindowText(htel, temp_tel, ++ltel);
			GetWindowText(hesp, temp_esp, ++lesp);
			GetWindowText(hturn, temp_turn, ++lturn);
			#pragma endregion

			if (IsDlgButtonChecked(handler, IDC_CHECK1) == BST_CHECKED) {
				dias_aux[0] = true;
			}
			if (IsDlgButtonChecked(handler, IDC_CHECK2) == BST_CHECKED) {
				dias_aux[1] = true;
			}
			if (IsDlgButtonChecked(handler, IDC_CHECK3) == BST_CHECKED) {
				dias_aux[2] = true;
			}
			if (IsDlgButtonChecked(handler, IDC_CHECK4) == BST_CHECKED) {
				dias_aux[3] = true;
			}
			if (IsDlgButtonChecked(handler, IDC_CHECK5) == BST_CHECKED) {
				dias_aux[4] = true;
			}
			if (dias_aux[0] == false && dias_aux[1] == false && dias_aux[2] == false && dias_aux[3] == false && dias_aux[4] == false) {
				MessageBox(handler, "Selecciona por lo menos un día para laborar.", "Error", MB_OK);
				break;
			}
			/*while (actual_m != NULL) {
				if ((actual_m->turno.compare(temp_turn) == 0) && (strcmp(actual_m->especialidad, temp_esp) == 0) && (strcmp(actual_m->especialidad, temp_esp) == 0)) {
					MessageBox(NULL, "Ya hay mecánico que labora en el turno con la especialidad selccionada.", "", MB_OK);
					correcto = false;
					break;
				}
				actual_m = actual_m->siguiente_m;
			}
			actual_m = inicio_m;*/
			validar_nombres(temp_n, correcto);
			validar_nombres(temp_ap, correcto);
			validar_nombres(temp_am, correcto);
			if (correcto != true) break;
			if ((lnum-1) != 5){
				MessageBox(handler, "El numero de empleado debe tener 5 números.", "Error", MB_OK);
				break;
			}
			while (actual_m != NULL) {
				string temp_numST = temp_num;
				if (temp_numST.compare(actual_m->num_empleado) == 0) {
					correcto = false;
					MessageBox(handler, "El número de empleado ya existe.", "Error", MB_OK);
					break;
				}
				actual_m = actual_m->siguiente_m;
			} actual_m = inicio_m;
			if (correcto != true) break;
			if (verf == false) {
				MessageBox(handler, "Selecciona una fotografia.", "Error", MB_OK);
				break;
			}
			if ((ltel-1) != 10) {
				MessageBox(handler, "El teléfono debe tener 10 dígitos.", "Error", MB_OK);
				correcto = false;
			}
			if (correcto != true) break;
			else {
				if (inicio_m == NULL) {
					inicio_m = new mecanic;
					for (int i = 0; i < 5; i++) {
						inicio_m->dias[i] = dias_aux[i];
					}
					inicio_m->num_empleado = temp_num;
					strcpy(inicio_m->nombre, temp_n);
					inicio_m->apellido_paterno = temp_ap;
					inicio_m->apellido_materno = temp_am;
					inicio_m->telefono = temp_tel;
					strcpy(inicio_m->especialidad, temp_esp);
					inicio_m->turno = temp_turn;
					strcpy(inicio_m->foto, fotos);
					inicio_m->siguiente_m = NULL;
					inicio_m->anterior_m = NULL;
					actual_m = inicio_m;
				}
				else {
					while (actual_m->siguiente_m != NULL)
						actual_m = actual_m->siguiente_m;
					actual_m->siguiente_m = new mecanic;
					actual_m->siguiente_m->anterior_m= actual_m;
					actual_m = actual_m->siguiente_m;
					for (int i = 0; i < 5; i++) {
						actual_m->dias[i] = dias_aux[i];
					}
					actual_m->num_empleado = temp_num;
					strcpy_s(actual_m->nombre, temp_n);
					actual_m->apellido_paterno = temp_ap;
					actual_m->apellido_materno = temp_am;
					actual_m->telefono = temp_tel;
					strcpy(actual_m->especialidad, temp_esp);
					actual_m->turno = temp_turn;
					strcpy(actual_m->foto , fotos);
					actual_m->siguiente_m = NULL;
					actual_m = inicio_m;
				}
				verf = false;
				HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
				ShowWindow(menu_principal, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		break;
	case WM_CLOSE: {
		HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
		ShowWindow(menu_principal, SW_SHOW);
		DestroyWindow(handler);
		break; }
	case WM_DESTROY:
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK borrar_editar_mecanicos(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		HBITMAP hDirImagen;
		hDirImagen = (HBITMAP)LoadImage(NULL, actual_m->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		HWND hPictureControl = GetDlgItem(handler, Fotografia);
		SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
		strcat(fotos, actual_m->foto);
		actual_p = inicio_p;
		cbCombo1 = GetDlgItem(handler, Esp);
		while (actual_p != NULL) {
			SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM)actual_p->nombre);
			actual_p = actual_p->siguiente_p;
		}
		actual_p = inicio_p;
		int val1 = SendMessage(cbCombo1, CB_FINDSTRING, -1, (LPARAM)actual_m->especialidad);

		cbCombo2 = GetDlgItem(handler, Turn);
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "MATUTINO");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "VESPERTINO");
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "NOCTURNO");

		int val2 = SendMessage(cbCombo2, CB_FINDSTRING, -1, (LPARAM)actual_m->turno.c_str());
		

		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT,NULL, (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str()); SendDlgItemMessage(handler, IDC_EDIT1, WM_SETTEXT, sizeof(actual_m->num_empleado.c_str()), (LPARAM)actual_m->num_empleado.c_str());
		SendDlgItemMessage(handler, IDC_EDIT2, WM_SETTEXT, NULL, (LPARAM)actual_m->nombre);
		SendDlgItemMessage(handler, IDC_EDIT3, WM_SETTEXT,NULL, (LPARAM)actual_m->apellido_paterno.c_str());
		SendDlgItemMessage(handler, IDC_EDIT4, WM_SETTEXT, NULL, (LPARAM)actual_m->apellido_materno.c_str());
		SendDlgItemMessage(handler, IDC_EDIT5, WM_SETTEXT, NULL, (LPARAM)actual_m->telefono.c_str());
		SendDlgItemMessage(handler, Esp, WM_SETTEXT, NULL, (LPARAM)actual_m->especialidad);
		SendMessage(cbCombo1, CB_SETCURSEL, val1, val1);
		SendMessage(cbCombo2, CB_SETCURSEL, val2, val2);
		if (actual_m->dias[0] == true) {
			HWND check1 = GetDlgItem(handler, IDC_CHECK1);
			SendMessage(check1, BM_SETCHECK, BST_CHECKED, 0);
		}
		if (actual_m->dias[1] == true) {
			HWND check2= GetDlgItem(handler, IDC_CHECK2);
			SendMessage(check2, BM_SETCHECK, BST_CHECKED, 0);
		}
		if (actual_m->dias[2] == true) {
			HWND check3 = GetDlgItem(handler, IDC_CHECK3);
			SendMessage(check3, BM_SETCHECK, BST_CHECKED, 0);
		}
		if (actual_m->dias[3] == true) {
			HWND check4 = GetDlgItem(handler, IDC_CHECK4);
			SendMessage(check4, BM_SETCHECK, BST_CHECKED, 0);
		}
		if (actual_m->dias[4] == true) {
			HWND check5 = GetDlgItem(handler, IDC_CHECK5);
			SendMessage(check5, BM_SETCHECK, BST_CHECKED, 0);
		}
		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			OPENFILENAME Img;
			ZeroMemory(&Img, sizeof(OPENFILENAME));
			Img.hwndOwner = handler;
			Img.nMaxFile = MAX_PATH;
			Img.lpstrDefExt = "bmp";
			Img.lStructSize = sizeof(OPENFILENAME);
			Img.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			Img.lpstrFile = fotos;
			Img.lpstrFilter = "Selecciona una imagen en formato bmp\0*.bmp";
			if (GetOpenFileName(&Img) == TRUE) {
				HBITMAP hDirImagen;
				hDirImagen = (HBITMAP)LoadImage(NULL, fotos, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPictureControl = GetDlgItem(handler, Fotografia);
				SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
				verf = true;
			}
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			#pragma region VARIABLES
			bool correcto = true;
			bool dias_aux[5] = { false, false, false, false, false };
			int combo1 = SendMessage(cbCombo1, CB_GETCURSEL, 0, 0);
			int combo2 = SendMessage(cbCombo2, CB_GETCURSEL, 0, 0);
			HWND hnum_empleado = GetDlgItem(handler, IDC_EDIT1);
			HWND hnombre = GetDlgItem(handler, IDC_EDIT2);
			HWND hap_p = GetDlgItem(handler, IDC_EDIT3);
			HWND hap_m = GetDlgItem(handler, IDC_EDIT4);
			HWND htel = GetDlgItem(handler, IDC_EDIT5);
			HWND hesp = GetDlgItem(handler, Esp);
			HWND hturn = GetDlgItem(handler, Turn);
			int lnum = GetWindowTextLength(hnum_empleado);
			int lnombre = GetWindowTextLength(hnombre);
			int lap_p = GetWindowTextLength(hap_p);
			int lap_m = GetWindowTextLength(hap_m);
			int ltel = GetWindowTextLength(htel);
			int lesp = GetWindowTextLength(hesp);
			int lturn = GetWindowTextLength(hturn);
			char temp_num[MAX_PATH];
			char temp_n[MAX_PATH];
			char temp_ap[MAX_PATH];
			char temp_am[MAX_PATH];
			char temp_tel[MAX_PATH];
			char temp_esp[MAX_PATH];
			char temp_turn[MAX_PATH];
			if (lnombre < 1 || lnum < 1 || lap_p < 1 || lap_m < 1 || ltel < 1) {
				MessageBox(handler, "Llena todos los campos", "Error", MB_OK);
				break;
			}
			GetWindowText(hnum_empleado, temp_num, ++lnum);
			GetWindowText(hnombre, temp_n, ++lnombre);
			GetWindowText(hap_p, temp_ap, ++lap_p);
			GetWindowText(hap_m, temp_am, ++lap_m);
			GetWindowText(htel, temp_tel, ++ltel);
			GetWindowText(hesp, temp_esp, ++lesp);
			GetWindowText(hturn, temp_turn, ++lturn);

			string temp_temp = actual_m->nombre;
			#pragma endregion
			if (IsDlgButtonChecked(handler, IDC_CHECK1) == BST_CHECKED) {
				dias_aux[0] = true;
			}
			if (IsDlgButtonChecked(handler, IDC_CHECK2) == BST_CHECKED) {
				dias_aux[1] = true;
			}
			if (IsDlgButtonChecked(handler, IDC_CHECK3) == BST_CHECKED) {
				dias_aux[2] = true;
			}
			if (IsDlgButtonChecked(handler, IDC_CHECK4) == BST_CHECKED) {
				dias_aux[3] = true;
			}
			if (IsDlgButtonChecked(handler, IDC_CHECK5) == BST_CHECKED) {
				dias_aux[4] = true;
			}
			if (dias_aux[0] == false && dias_aux[1] == false && dias_aux[2] == false && dias_aux[3] == false && dias_aux[4] == false) {
				MessageBox(handler, "Selecciona por lo menos un día para laborar.", "Error", MB_OK); 
				break;
			}
			if ((lnum-1) != 5) {
				if (correcto == false) break;
				MessageBox(handler, "El numero de empleado debe tener 5 números.", "Error", MB_OK);
				break;
			}
			if (correcto != true) break;
			if ((ltel-1) != 10) {
				MessageBox(handler, "El teléfono debe tener 10 dígitos.", "Error", MB_OK);
				correcto = false;
			}
			if (correcto != true) break;
			actual_m = inicio_m;
			string temp_numST = temp_num;
			while (actual_m != NULL) {
				if (temp_numST.compare(actual_m->num_empleado) == 0) {
					if (temp_temp.compare(actual_m->nombre) != 0) {
						correcto = false;
						MessageBox(handler, "El número de empleado ya existe.", "Error", MB_OK);

						break;
					}
				}
				actual_m = actual_m->siguiente_m;
			}
			actual_m = inicio_m;
			
			if (correcto ==  true) {
				for (int j = 0; j < cont; j++) {
					actual_m = actual_m->siguiente_m;
				}
				for (int i = 0; i < 5; i++) {
					actual_m->dias[i] = dias_aux[i];
				}
				actual_m->num_empleado = temp_num;
				strcpy(actual_m->nombre, temp_n);
				actual_m->apellido_paterno = temp_ap;
				actual_m->apellido_materno = temp_am;
				actual_m->telefono = temp_tel;
				strcpy(actual_m->especialidad, temp_esp);
				actual_m->turno = temp_turn;
				strcpy(actual_m->foto, fotos);
				actual_m = inicio_m;
				memset(fotos, 0, 250);
				HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
				ShowWindow(menu_principal, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		break;
	case WM_CLOSE:
	{
		memset(fotos, 0, 250);
		actual_m = inicio_m;
		HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
		ShowWindow(menu_principal, SW_SHOW);
		DestroyWindow(handler);

		break; }
	case WM_DESTROY:
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK Lista(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
	{
		bool existe;
		int n = 0;
		string arr_num[20];

		//Llenando el arreglo para hacer el quicksort
		while (actual_m != NULL) {
			arr_num[n] = actual_m->num_empleado;
			n++;
			actual_m = actual_m->siguiente_m;
		}
		actual_m = inicio_m;

		Quicksort(arr_num, 0, n - 1);

		HWND hLista = GetDlgItem(handler, IDC_LIST1);
		SendMessage(hLista, LB_RESETCONTENT, NULL, NULL);
		ofstream RM;
		RM.open("Reporte de mecanicos.txt", ios::out);
		RM << "LISTA DE MECANICOS ORDENADOS POR NUMERO DE EMPLEADO" << endl << endl;

		for (int i = 0; i < n; i++) {
			while (actual_m != NULL) {
				if (arr_num[i] == actual_m->num_empleado) {
					string tempn = actual_m->nombre;
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Número de empleado: " + actual_m->num_empleado).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Nombre: " + tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Teléfono: " + actual_m->telefono).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM) actual_m->especialidad);
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Turno: " + actual_m->turno).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)"");
					RM << "Número de empleado: " + actual_m->num_empleado << endl;
					RM << "Nombre: "+actual_m->apellido_paterno << " " << actual_c->apellido_materno << " " << actual_c->nombre << endl;
					RM << "Turno: "+actual_m->turno << endl;
					RM << "Especialidad: " << actual_m->especialidad << endl;
					RM << "Teléfono: " + actual_m->telefono << endl;
					break;
				}
				actual_m = actual_m->siguiente_m;
			}
			actual_m = inicio_m;
		}

		MessageBox(handler, "Se creo un archivo de texto en PIA_ESTDAT con el reporte", "", MB_OK);
		RM.close();
		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {

		}
		break;
	case WM_CLOSE:
		EndDialog(handler, Principal);
		break;
	case WM_DESTROY:
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}
BOOL CALLBACK Lista_clientes(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
	{
		string arr_n[20];
		int n = 1;

		while (actual_c != NULL) {
			arr_n[n] = actual_c->apellido_paterno + actual_c->apellido_materno + actual_c->nombre;
			n++;
			actual_c = actual_c->siguiente_c;
		}
		actual_c = inicio_c;

		heapSort(arr_n, n);

		HWND hLista = GetDlgItem(handler, IDC_LIST1);
		SendMessage(hLista, LB_RESETCONTENT, NULL, NULL);
		ofstream RC;
		RC.open("Reporte de clientes.txt", ios::out);
		RC << "LISTA DE CLIENTES ORDENADOS POR APELLIDO" << endl << endl;

		for (int i = 1; i <= n; i++) {
			while (actual_c != NULL) {
				if ((arr_n[i].compare(actual_c->apellido_paterno + actual_c->apellido_materno + actual_c->nombre))==0) {
					string dir = actual_c->direccion;
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Nombre: " + actual_c->apellido_paterno + " " + actual_c->apellido_materno + " " + actual_c->nombre).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("teléfono: " + actual_c->telefono).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Placas: " + actual_c->placas).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Marca: "+actual_c->marca).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Modelo: " +actual_c->modelo).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Kilometraje: "+actual_c->kilometraje).c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)("Año: " +actual_c->año_v).c_str());
					SendMessage(hLista, LB_ADDSTRING, sizeof(dir), (LPARAM)dir.c_str());
					SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)"");
					RC << "Nombre: "<<actual_c->apellido_paterno << " " << actual_c->apellido_materno << " " << actual_c->nombre << endl;
					RC << "Teléfono: " << actual_c->telefono << endl; 
					RC << "Placa de vehículo: "<<actual_c->placas << endl;
					RC << "Marca: " << actual_c->marca << endl; 
					RC << "Modelo: " << actual_c->modelo << endl;
					RC << "Kilometraje: " << actual_c->kilometraje << endl;
					RC << "Año del vehículo: " << actual_c->año_v << endl;
					RC << "Dirección: " << actual_c->direccion << endl<<endl;
					break;
				}
				actual_c = actual_c->siguiente_c;
			}
			actual_c = inicio_c;
		}

		MessageBox(handler, "Se creo un archivo de texto en PIA_ESTDAT con el reporte", "", MB_OK);
		RC.close();

		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {

		}
		break;
	case WM_CLOSE:
		EndDialog(handler, Principal);
		break;
	case WM_DESTROY:
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}
BOOL CALLBACK cliente(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {

		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT, sizeof(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno), (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str());
		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
#pragma region RCP
			bool correcto = true;
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			HWND hap_p = GetDlgItem(handler, IDC_EDIT2);
			HWND hap_m = GetDlgItem(handler, IDC_EDIT3);
			HWND htel = GetDlgItem(handler, IDC_EDIT4);
			HWND hdir = GetDlgItem(handler, IDC_EDIT10);
			HWND hplac = GetDlgItem(handler, IDC_EDIT5);
			HWND hmarc = GetDlgItem(handler, IDC_EDIT6);
			HWND hmod = GetDlgItem(handler, IDC_EDIT7);
			HWND haño = GetDlgItem(handler, IDC_EDIT8);
			HWND hkilo = GetDlgItem(handler, IDC_EDIT9);
			int lnombre = GetWindowTextLength(hnombre);
			int lap_p = GetWindowTextLength(hap_p);
			int lap_m = GetWindowTextLength(hap_m);
			int ltel = GetWindowTextLength(htel);
			int ldir = GetWindowTextLength(hdir);
			int lplac = GetWindowTextLength(hplac);
			int lmarc = GetWindowTextLength(hmarc);
			int lmod = GetWindowTextLength(hmod);
			int laño = GetWindowTextLength(haño);
			int lkilo = GetWindowTextLength(hkilo);
			char temp_n[MAX_PATH];
			char temp_ap[MAX_PATH];
			char temp_am[MAX_PATH];
			char temp_tel[MAX_PATH];
			char temp_dir[MAX_PATH];
			char temp_plac[MAX_PATH];
			char temp_marc[MAX_PATH];
			char temp_mod[MAX_PATH];
			char temp_año[MAX_PATH];
			char temp_kilo[MAX_PATH];
			if (lnombre < 1 || lap_p < 1 || lap_m < 1 || ldir < 1 || ltel < 1 || lplac < 1 || lmarc < 1 || lkilo < 1 || lmod < 1 || laño < 1) {
				MessageBox(handler, "Llena todos los campos", "Error", MB_OK);
				break;
			}
			GetWindowText(hnombre, temp_n, ++lnombre);
			GetWindowText(hap_p, temp_ap, ++lap_p);
			GetWindowText(hap_m, temp_am, ++lap_m);
			GetWindowText(hdir, temp_dir, ++ldir);
			GetWindowText(hplac, temp_plac, ++lplac);
			GetWindowText(hmod, temp_mod, ++lmod);
			GetWindowText(hmarc, temp_marc, ++lmarc);
			GetWindowText(haño, temp_año, ++laño);
			GetWindowText(hkilo, temp_kilo, ++lkilo);
			GetWindowText(htel, temp_tel, ++ltel);
			_strupr(temp_plac);
			_strupr(temp_am);
			_strupr(temp_ap);
			_strupr(temp_n);
			validar_nombres(temp_n, correcto);
			validar_nombres(temp_ap, correcto);
			validar_nombres(temp_am, correcto);
			if (correcto != true) break;
			if ((ltel - 1) != 10) {
				MessageBox(handler, "El teléfono debe tener 10 dígitos.", "Error", MB_OK);
				correcto = false;
			}
			for (int i = 0; temp_plac[i]; i++) {
				if (temp_plac[i] == ' ') {
					MessageBox(handler, "No incluya espacios en blanco en las placas.", "Error", MB_OK);
					correcto = false;
					break;
				}
			}
			if (correcto != true) break;
			while (actual_c != NULL) {
				string temp_placST = temp_plac;
				if (temp_placST.compare(actual_c->placas) == 0) {
					correcto = false;
					MessageBox(handler, "El número de placa ya existe.", "Error", MB_OK);
					break;
				}
				actual_c = actual_c->siguiente_c;
			} actual_c = inicio_c;

			if (correcto != true) break;
			else {
				if (inicio_c == NULL) {
					inicio_c = new clientes;
					strcpy(inicio_c->nombre, temp_n);
					inicio_c->apellido_paterno = temp_ap;
					inicio_c->apellido_materno = temp_am;
					inicio_c->telefono = temp_tel;
					inicio_c->placas = temp_plac;
					inicio_c->marca = temp_marc;
					inicio_c->modelo = temp_mod;
					inicio_c->año_v = temp_año;
					inicio_c->kilometraje = temp_kilo;
					strcpy(inicio_c->direccion, temp_dir);
					inicio_c->siguiente_c = NULL;
					inicio_c->anterior_c = NULL;
					actual_c = inicio_c;
				}
				else {
					while (actual_c->siguiente_c != NULL)
						actual_c = actual_c->siguiente_c;
					actual_c->siguiente_c = new clientes;
					actual_c->siguiente_c->anterior_c = actual_c;
					actual_c = actual_c->siguiente_c;
					strcpy_s(actual_c->nombre, temp_n);
					actual_c->apellido_paterno = temp_ap;
					actual_c->apellido_materno = temp_am;
					actual_c->telefono = temp_tel;
					actual_c->placas = temp_plac;
					actual_c->marca = temp_marc;
					actual_c->modelo = temp_mod;
					actual_c->año_v = temp_año;
					actual_c->kilometraje = temp_kilo;
					strcpy(actual_c->direccion, temp_dir);
					actual_c->siguiente_c = NULL;
					actual_c = inicio_c;
				}
				HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
				ShowWindow(menu_principal, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
			ShowWindow(menu_principal, SW_SHOW);
			DestroyWindow(handler);
		}
		break;
	case WM_CLOSE: {
				   HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
				   ShowWindow(menu_principal, SW_SHOW);
				   DestroyWindow(handler);
				   break;
	}
	case WM_DESTROY:
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK citas(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_TIMER: {
		if (done == false) {
			time(&tempfecha);
			fecha_actual = localtime(&tempfecha);
			char cTimeInfo[80];
			strftime(cTimeInfo, 80, "%Y%m%d", fecha_actual);
			fecha = cTimeInfo;
			strftime(cTimeInfo, 80, "%I%M%S", fecha_actual);
			hora = cTimeInfo;
			strftime(cTimeInfo, 80, "%u", fecha_actual);
			dia_semana = cTimeInfo;
			done = true;
		}
		break;
	}
	case WM_INITDIALOG: {
		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT, sizeof(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno), (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str()); char temp[10];
		num_cita++;
		_itoa(num_cita, temp, 10);
		SendDlgItemMessage(handler, CLAVE_CITA, WM_SETTEXT, sizeof(num_cita), (LPARAM)temp);

		cbCombo3 = GetDlgItem(handler, IDC_COMBO2);
		SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM) "Todos");
		while (actual_m != NULL) {
			string tempn = actual_m->nombre;;
			SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM)(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno).c_str());
			actual_m = actual_m->siguiente_m;
		}
		actual_m = inicio_m;
		SendMessage(cbCombo3, CB_SETCURSEL, 0, 0);
		cbCombo1 = GetDlgItem(handler, IDC_COMBO7);
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "Selecciona el cliente");
		while (actual_c != NULL) {
			string tempn = actual_c->nombre;;
			SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM)(tempn + " " + actual_c->apellido_paterno + " " + actual_c->apellido_materno).c_str());
			actual_c = actual_c->siguiente_c;
		}
		actual_c = inicio_c;
		SendMessage(cbCombo1, CB_SETCURSEL, 0, 0);
		cbCombo2 = GetDlgItem(handler, IDC_COMBO5);
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "Selecciona la especialidad");
		while (actual_p != NULL) {
			SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM)actual_p->nombre);
			actual_p = actual_p->siguiente_p;
		}
		actual_p = inicio_p;
		SendMessage(cbCombo2, CB_SETCURSEL, 0, 0);
		cbCombo4 = GetDlgItem(handler, IDC_COMBO6);
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "Selecciona la hora");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "6:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "6:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "7:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "7:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "8:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "8:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "9:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "9:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "10:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "10:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "11:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "11:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "12:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "12:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "13:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "13:30"); // 16
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "14:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "14:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "15:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "15:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "16:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "16:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "17:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "17:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "18:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "18:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "19:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "19:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "20:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "20:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "21:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "21:30"); // 32
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "22:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "22:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "23:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "23:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "24:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "24:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "00:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "00:30"); //40
		SendMessage(cbCombo4, CB_SETCURSEL, 0, 0);


		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			bool correcto = true;
			int combo1 = SendMessage(cbCombo1, CB_GETCURSEL, 0, 0);
			int combo2 = SendMessage(cbCombo2, CB_GETCURSEL, 0, 0);
			int combo3 = SendMessage(cbCombo3, CB_GETCURSEL, 0, 0);
			int combo4 = SendMessage(cbCombo4, CB_GETCURSEL, 0, 0);
			HWND hnombre = GetDlgItem(handler, IDC_COMBO7);
			HWND hesp = GetDlgItem(handler, IDC_COMBO5);
			HWND hmec = GetDlgItem(handler, IDC_COMBO2);
			HWND hfecha = GetDlgItem(handler, IDC_DATETIMEPICKER1);
			HWND hhor = GetDlgItem(handler, IDC_COMBO6);
			HWND htel = GetDlgItem(handler, tel);
			int lnombre = GetWindowTextLength(hnombre);
			int lesp = GetWindowTextLength(hesp);
			int lmec = GetWindowTextLength(hmec);
			int lfecha = GetWindowTextLength(hfecha);
			int lhor = GetWindowTextLength(hhor);
			int ltel = GetWindowTextLength(htel);
			char temp_n[MAX_PATH];
			char temp_tel[MAX_PATH];
			char temp_fecha[MAX_PATH];
			char temp_esp[MAX_PATH];
			char temp_hor[MAX_PATH];
			char temp_mec[MAX_PATH];
			if (combo1 == 0 || combo2 == 0 || combo3 == 0 || combo4 == 0) {
				MessageBox(NULL, "Seleccione algo del combobox", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			GetWindowText(hnombre, temp_n, ++lnombre);
			GetWindowText(hesp, temp_esp, ++lesp);
			GetWindowText(hhor, temp_hor, ++lhor);
			GetWindowText(htel, temp_tel, ++ltel);
			GetWindowText(hmec, temp_mec, ++lmec);
			GetWindowText(hfecha, temp_fecha, ++lfecha);

			string aux1 = temp_fecha, aux2 = "";

			for (int i = 0; aux1[i]; i++) {
				if ((aux1.c_str()[i] == ',')) {
					break;
				}
				aux2 += aux1[i];
			}
			if (aux2 == "sábado" || aux2 == "domingo") {
				MessageBox(NULL, "No se atiende sábados ni domingos", "Error", MB_OK | MB_ICONWARNING);
				break;
			}

			bool semana[5] = { false, false, false, false, false };
			int sub = 0;

			if (aux2 == "lunes") {
				semana[0] = true;
			}
			else if (aux2 == "martes") {
				semana[1] = true;
			}
			else if (aux2 == "miércoles") {
				semana[2] = true;
			}
			else if (aux2 == "jueves") {
				semana[3] = true;
			}
			else if (aux2 == "viernes") {
				semana[4] = true;
			}

			string temp_nST = temp_mec;
			while (actual_m != NULL) {
				string tempn = actual_m->nombre;
				if (temp_nST.compare(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno) == 0) {
					for (int i = 0; i < 5; i++) {
						if (actual_m->dias[i] == true && semana[i] == true) {
							sub++;
						}
					}
				}
				actual_m = actual_m->siguiente_m;
			}actual_m = inicio_m;

			for (int i = 0; i < 5; i++) {
				if (actual_m->dias[i] == true && semana[i] == true) {
					sub++;
				}
			}

			if (sub == 0) {
				MessageBox(NULL, "El mecánico no trabaja en el día seleccionado.", "Error", MB_OK | MB_ICONWARNING);
				break;
			}

			while (actual_m != NULL) {
				string tempn = actual_m->nombre;
				if (temp_nST.compare(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno) == 0) {
					if (actual_m->turno == "MATUTINO") {
						if (combo4 <= 1 || combo4 >= 16) {
							MessageBox(NULL, "El mecánico no trabaja en la hora seleccionada.", "Error", MB_OK | MB_ICONWARNING);
							correcto = false;
						}
					}
					else if (actual_m->turno == "NOCTURNO") {
						if (combo4 <= 33 || combo4 >= 40) {
							MessageBox(NULL, "El mecánico no trabaja en la hora seleccionada.", "Error", MB_OK | MB_ICONWARNING);
							correcto = false;
						}
					}
					else if (actual_m->turno == "VESPERTINO") {
						if (combo4 <= 17 || combo4 >= 32) {
							MessageBox(NULL, "El mecánico no trabaja en la hora seleccionada.", "Error", MB_OK | MB_ICONWARNING);
							correcto = false;
						}
					}

				}
				actual_m = actual_m->siguiente_m;
			}actual_m = inicio_m;
			if (!correcto) break;

			/*	string temp_nST = temp_n;
				while (actual_cs != NULL) {
					if ((temp_nST.compare(actual_cs->cliente) == 0) && (strcmp(actual_cs->paquete, temp_esp) == 0) && (strcmp(actual_cs->fecha, temp_fecha) == 0) && (strcmp(actual_cs->hora.c_str(), temp_hor) == 0)) {
						MessageBox(NULL, "Un mecánico de la especialidad ya atiende al cliente.", "", MB_OK);
						correcto = false;
						break;
					}
					actual_cs = actual_cs->siguiente_cs;
				}
				actual_cs = inicio_cs;*/

			if (inicio_cs == NULL) {
				inicio_cs = new cita;
				strcpy(inicio_cs->cliente, temp_n);
				strcpy(inicio_cs->mecanico, temp_mec);
				inicio_cs->hora = temp_hor;
				inicio_cs->telefono = temp_tel;
				inicio_cs->num = num_cita;
				strcpy(inicio_cs->paquete, temp_esp);
				strcpy(inicio_cs->fecha, temp_fecha);
				strcpy(inicio_cs->foto, fotos);
				inicio_cs->estado = "Pendiente";
				inicio_cs->siguiente_cs = NULL;
				inicio_cs->anterior_cs = NULL;
				actual_cs = inicio_cs;
			}
			else {
				while (actual_cs->siguiente_cs != NULL)
					actual_cs = actual_cs->siguiente_cs;
				actual_cs->siguiente_cs = new cita;
				actual_cs->siguiente_cs->anterior_cs = actual_cs;
				actual_cs = actual_cs->siguiente_cs;
				strcpy_s(actual_cs->cliente, temp_n);
				strcpy(actual_cs->mecanico, temp_mec);
				actual_cs->hora = temp_hor;
				actual_cs->telefono = temp_tel;
				actual_cs->num = num_cita;
				strcpy(actual_cs->fecha, temp_fecha);
				strcpy(actual_cs->paquete, temp_esp);
				strcpy(actual_cs->foto, fotos);
				actual_cs->estado = "Pendiente";
				actual_cs->siguiente_cs = NULL;
				actual_cs = inicio_cs;
			}
			HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
			ShowWindow(menu_principal, SW_SHOW);
			DestroyWindow(handler);
		}
		if (LOWORD(wParam) == IDC_COMBO5 && HIWORD(wParam) == CBN_SELCHANGE) {
			SendMessage(cbCombo3, CB_RESETCONTENT, 0, (LPARAM)actual_m->nombre);
			int combo2 = SendMessage(cbCombo2, CB_GETCURSEL, 0, 0);
			if (combo2 != 0) {
				HWND hesp = GetDlgItem(handler, IDC_COMBO5);
				int lesp = GetWindowTextLength(hesp);
				char temp_esp[MAX_PATH];
				GetWindowText(hesp, temp_esp, ++lesp);
				SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM)temp_esp);

				cbCombo3 = GetDlgItem(handler, IDC_COMBO2);

				string temp_espST = temp_esp;
				while (actual_m != NULL) {
					if (temp_espST.compare(actual_m->especialidad) == 0) {
						string tempn = actual_m->nombre;;
						SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM)(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno).c_str());
					}
					actual_m = actual_m->siguiente_m;
				}
				SendMessage(cbCombo3, CB_SETCURSEL, 0, 0);
				actual_m = inicio_m;

			}
			else {
				cbCombo3 = GetDlgItem(handler, IDC_COMBO2);
				SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM) "Todos");
				while (actual_m != NULL) {
					string tempn = actual_m->nombre;;
					SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM)(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno).c_str());
					actual_m = actual_m->siguiente_m;
				}
				actual_m = inicio_m;
				SendMessage(cbCombo3, CB_SETCURSEL, 0, 0);
			}

		}
		if (LOWORD(wParam) == IDC_COMBO2 && HIWORD(wParam) == CBN_SELCHANGE) {
			HWND hnombre = GetDlgItem(handler, IDC_COMBO2);
			int lnombre = GetWindowTextLength(hnombre);
			char temp_n[MAX_PATH];
			GetWindowText(hnombre, temp_n, ++lnombre);
			while (actual_m != NULL) {
				string temp_clST = temp_n;
				string tempn = actual_m->nombre;
				if (temp_clST.compare(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno) == 0) {
					HBITMAP hDirImagen;
					string tempf = actual_m->foto;
					strcat(fotos, tempf.c_str());
					hDirImagen = (HBITMAP)LoadImage(NULL, actual_m->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					HWND hPictureControl = GetDlgItem(handler, Fotografia);
					SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
				}
				actual_m = actual_m->siguiente_m;
			}
			actual_m = inicio_m;

		}
		if (LOWORD(wParam) == IDC_COMBO7 && HIWORD(wParam) == CBN_SELCHANGE) {
			HWND hn = GetDlgItem(handler, IDC_COMBO7);
			int ln = GetWindowTextLength(hn);
			char temp_n[MAX_PATH];
			GetWindowText(hn, temp_n, ++ln);
			string temp_nST = temp_n;
			while (actual_c != NULL) {
				string tempn = actual_c->nombre;
				if (temp_nST.compare(tempn + " " + actual_c->apellido_paterno + " " + actual_c->apellido_materno) == 0) {
					string temp_telST = actual_c->telefono;
					SendDlgItemMessage(handler, tel, WM_SETTEXT, sizeof(temp_telST.c_str()), (LPARAM)temp_telST.c_str());
				}
				actual_c = actual_c->siguiente_c;
			}
			actual_c = inicio_c;
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
			ShowWindow(menu_principal, SW_SHOW);
			DestroyWindow(handler);
		}
		break;
	case WM_CLOSE:{
		HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
		ShowWindow(menu_principal, SW_SHOW);
		DestroyWindow(handler);
		break;
	}
	case WM_DESTROY:
		break;
	default: 
		
		DefWindowProc(handler, msg, wParam, lParam); 
	}

	return FALSE;
}
BOOL CALLBACK borrar_editar_cliente(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT, sizeof(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno), (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str());
		SendDlgItemMessage(handler, IDC_EDIT1, WM_SETTEXT, sizeof(actual_c->nombre), (LPARAM)actual_c->nombre);
		SendDlgItemMessage(handler, IDC_EDIT2, WM_SETTEXT, sizeof(actual_c->apellido_paterno.c_str()), (LPARAM)actual_c->apellido_paterno.c_str());
		SendDlgItemMessage(handler, IDC_EDIT3, WM_SETTEXT, sizeof(actual_c->apellido_materno.c_str()), (LPARAM)actual_c->apellido_materno.c_str());
		SendDlgItemMessage(handler, IDC_EDIT4, WM_SETTEXT, sizeof(actual_c->telefono.c_str()), (LPARAM)actual_c->telefono.c_str());
		SendDlgItemMessage(handler, IDC_EDIT5, WM_SETTEXT, sizeof(actual_c->placas.c_str()), (LPARAM)actual_c->placas.c_str());
		SendDlgItemMessage(handler, IDC_EDIT6, WM_SETTEXT, sizeof(actual_c->marca.c_str()), (LPARAM)actual_c->marca.c_str());
		SendDlgItemMessage(handler, IDC_EDIT7, WM_SETTEXT, sizeof(actual_c->modelo.c_str()), (LPARAM)actual_c->modelo.c_str());
		SendDlgItemMessage(handler, IDC_EDIT8, WM_SETTEXT, sizeof(actual_c->año_v.c_str()), (LPARAM)actual_c->año_v.c_str());
		SendDlgItemMessage(handler, IDC_EDIT9, WM_SETTEXT, sizeof(actual_c->kilometraje.c_str()), (LPARAM)actual_c->kilometraje.c_str());
		SendDlgItemMessage(handler, IDC_EDIT10, WM_SETTEXT, sizeof(actual_c->direccion), (LPARAM)actual_c->direccion);

		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
#pragma region RCP
			bool correcto = true;
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			HWND hap_p = GetDlgItem(handler, IDC_EDIT2);
			HWND hap_m = GetDlgItem(handler, IDC_EDIT3);
			HWND htel = GetDlgItem(handler, IDC_EDIT4);
			HWND hdir = GetDlgItem(handler, IDC_EDIT10);
			HWND hplac = GetDlgItem(handler, IDC_EDIT5);
			HWND hmarc = GetDlgItem(handler, IDC_EDIT6);
			HWND hmod = GetDlgItem(handler, IDC_EDIT7);
			HWND haño = GetDlgItem(handler, IDC_EDIT8);
			HWND hkilo = GetDlgItem(handler, IDC_EDIT9);
			int lnombre = GetWindowTextLength(hnombre);
			int lap_p = GetWindowTextLength(hap_p);
			int lap_m = GetWindowTextLength(hap_m);
			int ltel = GetWindowTextLength(htel);
			int ldir = GetWindowTextLength(hdir);
			int lplac = GetWindowTextLength(hplac);
			int lmarc = GetWindowTextLength(hmarc);
			int lmod = GetWindowTextLength(hmod);
			int laño = GetWindowTextLength(haño);
			int lkilo = GetWindowTextLength(hkilo);
			char temp_n[MAX_PATH];
			char temp_ap[MAX_PATH];
			char temp_am[MAX_PATH];
			char temp_tel[MAX_PATH];
			char temp_dir[MAX_PATH];
			char temp_plac[MAX_PATH];
			char temp_marc[MAX_PATH];
			char temp_mod[MAX_PATH];
			char temp_año[MAX_PATH];
			char temp_kilo[MAX_PATH];
			if (lnombre < 1 || lap_p < 1 || lap_m < 1 || ldir < 1 || ltel < 1 || lplac < 1 || lmarc < 1 || lkilo < 1 || lmod < 1 || laño < 1) {
				MessageBox(handler, "Llena todos los campos", "Error", MB_OK);
				break;
			}
			GetWindowText(hnombre, temp_n, ++lnombre);
			GetWindowText(hap_p, temp_ap, ++lap_p);
			GetWindowText(hap_m, temp_am, ++lap_m);
			GetWindowText(hdir, temp_dir, ++ldir);
			GetWindowText(hplac, temp_plac, ++lplac);
			GetWindowText(hmod, temp_mod, ++lmod);
			GetWindowText(hmarc, temp_marc, ++lmarc);
			GetWindowText(haño, temp_año, ++laño);
			GetWindowText(hkilo, temp_kilo, ++lkilo);
			GetWindowText(htel, temp_tel, ++ltel);
#pragma endregion
			_strupr(temp_plac);
			_strupr(temp_am);
			_strupr(temp_ap);
			_strupr(temp_n);
			validar_nombres(temp_n, correcto);
			validar_nombres(temp_ap, correcto);
			validar_nombres(temp_am, correcto);
			if (correcto != true) break;
			for (int i = 0; temp_año[i]; i++) {
				if (temp_año[i] < 48 || temp_año[i] > 57) {
					MessageBox(handler, "El año del vehículo debe contener números.", "Error", MB_OK);
					correcto = false;
					break;
				}
			}
			if (correcto != true) break;
			for (int i = 0; temp_kilo[i]; i++) {
				if (temp_kilo[i] < 48 || temp_kilo[i] > 57) {
					MessageBox(handler, "El kilometraje del vehículo debe contener números.", "Error", MB_OK);
					correcto = false;
					break;
				}
			}
			if (correcto != true) break;
			if ((ltel - 1) != 10) {
				MessageBox(handler, "El teléfono debe tener 10 dígitos.", "Error", MB_OK);
				correcto = false;
			}
			for (int i = 0; temp_plac[i]; i++) {
				if (temp_plac[i] == ' ') {
					MessageBox(handler, "No incluya espacios en blanco en las placas.", "Error", MB_OK);
					correcto = false;
					break;
				}
			}
			if (correcto != true) break;
			string temp_temp = temp_n;
			actual_c = inicio_c;
			while (actual_c != NULL) {
				string temp_placST = temp_plac;
				if (temp_placST.compare(actual_c->placas) == 0) {
					if (temp_temp.compare(actual_c->nombre) != 0) {
						correcto = false;
						MessageBox(handler, "El número de placa ya existe.", "Error", MB_OK);
						break;
					}				}
				actual_c = actual_c->siguiente_c;
			}
			actual_c = inicio_c;
			while (actual_c != NULL) {
				string temp_TST = temp_tel;
				if (temp_TST.compare(actual_c->telefono) == 0) {
					break;
				}
				actual_c = actual_c->siguiente_c;
			}

			if (correcto != true) break;
			else {
				strcpy_s(actual_c->nombre, temp_n);
				actual_c->apellido_paterno = temp_ap;
				actual_c->apellido_materno = temp_am;
				actual_c->telefono = temp_tel;
				actual_c->placas = temp_plac;
				actual_c->marca = temp_marc;
				actual_c->modelo = temp_mod;
				actual_c->año_v = temp_año;
				actual_c->kilometraje = temp_kilo;
				strcpy(actual_c->direccion, temp_dir);
				actual_c = inicio_c;

				HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
				ShowWindow(menu_principal, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
			ShowWindow(menu_principal, SW_SHOW);
			DestroyWindow(handler);
		}
		break;
	case WM_CLOSE: {
		HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
		ShowWindow(menu_principal, SW_SHOW);
		DestroyWindow(handler);
		break; }
	case WM_DESTROY:
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK borrar_editar_citas(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {

		HBITMAP hDirImagen;
		hDirImagen = (HBITMAP)LoadImage(NULL, actual_cs->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		HWND hPictureControl = GetDlgItem(handler, Fotografia);
		SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
		strcat(fotos, actual_cs->foto);

		cbCombo1 = GetDlgItem(handler, IDC_COMBO7);
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "Selecciona el cliente");
		while (actual_c != NULL) {
			string tempn = actual_c->nombre;;
			SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM)(tempn + " " + actual_c->apellido_paterno + " " + actual_c->apellido_materno).c_str());
			actual_c = actual_c->siguiente_c;
		}
		actual_c = inicio_c;
		int val1 = SendMessage(cbCombo1, CB_FINDSTRING, -1, (LPARAM)actual_cs->cliente);
		SendMessage(cbCombo1, CB_SETCURSEL, val1, val1);

		actual_p = inicio_p;
		cbCombo2 = GetDlgItem(handler, IDC_COMBO5);
		SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM) "Selecciona la especialidad");
		while (actual_p != NULL) {
			SendMessage(cbCombo2, CB_ADDSTRING, 0, (LPARAM)actual_p->nombre);
			actual_p = actual_p->siguiente_p;
		}
		actual_p = inicio_p;
		int val2 = SendMessage(cbCombo2, CB_FINDSTRING, -1, (LPARAM)actual_cs->paquete);
		SendMessage(cbCombo2, CB_SETCURSEL, val2, val2);

		cbCombo3 = GetDlgItem(handler, IDC_COMBO2);
		SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM) "Todos");
		while (actual_m != NULL) {
			string tempn = actual_m->nombre;;
			SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM)(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno).c_str());
			actual_m = actual_m->siguiente_m;
		}
		actual_m = inicio_m;
		int val3 = SendMessage(cbCombo3, CB_FINDSTRING, -1, (LPARAM)actual_cs->mecanico);
		SendMessage(cbCombo3, CB_SETCURSEL, val3, val3);

		char temp[10];
		_itoa(actual_cs->num, temp, 10);
		SendDlgItemMessage(handler, CLAVE_CITA, WM_SETTEXT, sizeof(num_cita), (LPARAM)temp);
		SendDlgItemMessage(handler, tel, WM_SETTEXT, sizeof(actual_cs->telefono.c_str()), (LPARAM)actual_cs->telefono.c_str());
		
		cbCombo4 = GetDlgItem(handler, IDC_COMBO6);
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "Selecciona la hora");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "6:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "6:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "7:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "7:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "8:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "8:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "9:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "9:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "10:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "10:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "11:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "11:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "12:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "12:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "13:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "13:30"); // 16
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "14:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "14:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "15:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "15:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "16:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "16:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "17:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "17:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "18:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "18:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "19:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "19:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "20:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "20:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "21:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "21:30"); // 32
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "22:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "22:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "23:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "23:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "24:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "24:30");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "00:00");
		SendMessage(cbCombo4, CB_ADDSTRING, 0, (LPARAM) "00:30"); //40
		int val4 = SendMessage(cbCombo4, CB_FINDSTRING, -1, (LPARAM)actual_cs->hora.c_str());
		SendMessage(cbCombo4, CB_SETCURSEL, val4, val4);


		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT, sizeof(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno), (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str()); SendDlgItemMessage(handler, IDC_EDIT1, WM_SETTEXT, sizeof(actual_m->num_empleado.c_str()), (LPARAM)actual_m->num_empleado.c_str());
		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			bool correcto = true;
			int combo1 = SendMessage(cbCombo1, CB_GETCURSEL, 0, 0);
			int combo2 = SendMessage(cbCombo2, CB_GETCURSEL, 0, 0);
			int combo3 = SendMessage(cbCombo3, CB_GETCURSEL, 0, 0);
			int combo4 = SendMessage(cbCombo4, CB_GETCURSEL, 0, 0);
			HWND hnombre = GetDlgItem(handler, IDC_COMBO7);
			HWND hesp = GetDlgItem(handler, IDC_COMBO5);
			HWND hmec = GetDlgItem(handler, IDC_COMBO2);
			HWND hfecha = GetDlgItem(handler, IDC_DATETIMEPICKER1);
			HWND hhor = GetDlgItem(handler, IDC_COMBO6);
			HWND htel = GetDlgItem(handler, tel);
			int lnombre = GetWindowTextLength(hnombre);
			int lesp = GetWindowTextLength(hesp);
			int lmec = GetWindowTextLength(hmec);
			int lfecha = GetWindowTextLength(hfecha);
			int lhor = GetWindowTextLength(hhor);
			int ltel = GetWindowTextLength(htel);
			char temp_n[MAX_PATH];
			char temp_tel[MAX_PATH];
			char temp_fecha[MAX_PATH];
			char temp_esp[MAX_PATH];
			char temp_hor[MAX_PATH];
			char temp_mec[MAX_PATH];
			if (combo1 == 0 || combo2 == 0 || combo3 == 0 || combo4 == 0) {
				MessageBox(NULL, "Seleccione algo del combobox", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			GetWindowText(hnombre, temp_n, ++lnombre);
			GetWindowText(hesp, temp_esp, ++lesp);
			GetWindowText(hhor, temp_hor, ++lhor);
			GetWindowText(htel, temp_tel, ++ltel);
			GetWindowText(hmec, temp_mec, ++lmec);
			GetWindowText(hfecha, temp_fecha, ++lfecha);

			string aux1 = temp_fecha, aux2 = "";

			for (int i = 0; aux1[i]; i++) {
				if ((aux1.c_str()[i] == ',')) {
					break;
				}
				aux2 += aux1[i];
			}
			if (aux2 == "sábado" || aux2 == "domingo") {
				MessageBox(NULL, "No se atiende sábados ni domingos", "Error", MB_OK | MB_ICONWARNING);
				break;
			}

			bool semana[5] = { false, false, false, false, false };
			int sub = 0;

			if (aux2 == "lunes") {
				semana[0] = true;
			}
			else if (aux2 == "martes") {
				semana[1] = true;
			}
			else if (aux2 == "miércoles") {
				semana[2] = true;
			}
			else if (aux2 == "jueves") {
				semana[3] = true;
			}
			else if (aux2 == "viernes") {
				semana[4] = true;
			}

			string temp_nST = temp_mec;
			while (actual_m != NULL) {
				string tempn = actual_m->nombre;
				if (temp_nST.compare(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno) == 0) {
					for (int i = 0; i < 5; i++) {
						if (actual_m->dias[i] == true && semana[i] == true) {
							sub++;
						}
					}
				}
				actual_m = actual_m->siguiente_m;
			}actual_m = inicio_m;

			for (int i = 0; i < 5; i++) {
				if (actual_m->dias[i] == true && semana[i] == true) {
					sub++;
				}
			}

			if (sub == 0) {
				MessageBox(NULL, "El mecánico no trabaja en el día seleccionado.", "Error", MB_OK | MB_ICONWARNING);
				break;
			}

			while (actual_m != NULL) {
				string tempn = actual_m->nombre;
				if (temp_nST.compare(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno) == 0) {
					if (actual_m->turno == "MATUTINO") {
						if (combo4 <= 1 || combo4 >= 16) {
							MessageBox(NULL, "El mecánico no trabaja en la hora seleccionada.", "Error", MB_OK | MB_ICONWARNING);
							correcto = false;
						}
					}
					else if (actual_m->turno == "NOCTURNO") {
						if (combo4 <= 33 || combo4 >= 40) {
							MessageBox(NULL, "El mecánico no trabaja en la hora seleccionada.", "Error", MB_OK | MB_ICONWARNING);
							correcto = false;
						}
					}
					else if (actual_m->turno == "VESPERTINO") {
						if (combo4 <= 17 || combo4 >= 32) {
							MessageBox(NULL, "El mecánico no trabaja en la hora seleccionada.", "Error", MB_OK | MB_ICONWARNING);
							correcto = false;
						}
					}

				}
				actual_m = actual_m->siguiente_m;
			}actual_m = inicio_m;
			if (!correcto) break;
			strcpy_s(actual_cs->cliente, temp_n);
			strcpy(actual_cs->mecanico, temp_mec);
			actual_cs->hora = temp_hor;
			actual_cs->telefono = temp_tel;
			strcpy(actual_cs->fecha, temp_fecha);
			strcpy(actual_cs->paquete, temp_esp);
			strcpy(actual_cs->foto, fotos);
			actual_cs->estado = "Pendiente";
			actual_cs = inicio_cs;
			HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
			ShowWindow(menu_principal, SW_SHOW);
			DestroyWindow(handler);
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
			ShowWindow(menu_principal, SW_SHOW);
			DestroyWindow(handler);
		}
		if (LOWORD(wParam) == IDC_COMBO5 && HIWORD(wParam) == CBN_SELCHANGE) {
			SendMessage(cbCombo3, CB_RESETCONTENT, 0, (LPARAM)actual_m->nombre);
			int combo2 = SendMessage(cbCombo2, CB_GETCURSEL, 0, 0);
			if (combo2 != 0) {
				HWND hesp = GetDlgItem(handler, IDC_COMBO5);
				int lesp = GetWindowTextLength(hesp);
				char temp_esp[MAX_PATH];
				GetWindowText(hesp, temp_esp, ++lesp);
				SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM)temp_esp);

				cbCombo3 = GetDlgItem(handler, IDC_COMBO2);

				string temp_espST = temp_esp;
				while (actual_m != NULL) {
					if (temp_espST.compare(actual_m->especialidad) == 0) {
						string tempn = actual_m->nombre;;
						SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM)(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno).c_str());
					}
					actual_m = actual_m->siguiente_m;
				}
				SendMessage(cbCombo3, CB_SETCURSEL, 0, 0);
				actual_m = inicio_m;

			}
			else {
				cbCombo3 = GetDlgItem(handler, IDC_COMBO2);
				SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM) "Todos");
				while (actual_m != NULL) {
					string tempn = actual_m->nombre;;
					SendMessage(cbCombo3, CB_ADDSTRING, 0, (LPARAM)(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno).c_str());
					actual_m = actual_m->siguiente_m;
				}
				actual_m = inicio_m;
				SendMessage(cbCombo3, CB_SETCURSEL, 0, 0);
			}

		}
		if (LOWORD(wParam) == IDC_COMBO2 && HIWORD(wParam) == CBN_SELCHANGE) {
			HWND hnombre = GetDlgItem(handler, IDC_COMBO2);
			int lnombre = GetWindowTextLength(hnombre);
			char temp_n[MAX_PATH];
			GetWindowText(hnombre, temp_n, ++lnombre);
			while (actual_m != NULL) {
				string temp_clST = temp_n;
				string tempn = actual_m->nombre;
				if (temp_clST.compare(tempn + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno) == 0) {
					HBITMAP hDirImagen;
					string tempf = actual_m->foto;
					strcat(fotos, tempf.c_str());
					hDirImagen = (HBITMAP)LoadImage(NULL, actual_m->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					HWND hPictureControl = GetDlgItem(handler, Fotografia);
					SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
				}
				actual_m = actual_m->siguiente_m;
			}
			actual_m = inicio_m;

		}
		if (LOWORD(wParam) == IDC_COMBO7 && HIWORD(wParam) == CBN_SELCHANGE) {
			HWND hn = GetDlgItem(handler, IDC_COMBO7);
			int ln = GetWindowTextLength(hn);
			char temp_n[MAX_PATH];
			GetWindowText(hn, temp_n, ++ln);
			string temp_nST = temp_n;
			while (actual_c != NULL) {
				string tempn = actual_c->nombre;
				if (temp_nST.compare(tempn + " " + actual_c->apellido_paterno + " " + actual_c->apellido_materno) == 0) {
					string temp_telST = actual_c->telefono;
					SendDlgItemMessage(handler, tel, WM_SETTEXT, sizeof(temp_telST.c_str()), (LPARAM)temp_telST.c_str());
				}
				actual_c = actual_c->siguiente_c;
			}
			actual_c = inicio_c;
		}
		break;
	case WM_CLOSE: {
		HWND menu_principal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Principal), NULL, menu);
		ShowWindow(menu_principal, SW_SHOW);
		DestroyWindow(handler);
		break; }
	case WM_DESTROY:
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}
BOOL CALLBACK mostrar_citas(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
	{
		char temp[10];
		_itoa(actual_cs->num, temp, 10);
		SendDlgItemMessage(handler, NOMBRES, WM_SETTEXT, sizeof(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno), (LPARAM)(actual_u->nombre + " " + actual_u->apellido_paterno + " " + actual_u->apellido_materno).c_str());
		SendDlgItemMessage(handler, NUMERO, WM_SETTEXT, sizeof(actual_cs->num), (LPARAM)temp);
		SendDlgItemMessage(handler, NOMBRE_CLIENTE, WM_SETTEXT, sizeof(actual_cs->cliente), (LPARAM)actual_cs->cliente);
		SendDlgItemMessage(handler, Esp, WM_SETTEXT, sizeof(actual_cs->paquete), (LPARAM)actual_cs->paquete);
		SendDlgItemMessage(handler, NOMBRE_MEC, WM_SETTEXT, sizeof(actual_cs->mecanico), (LPARAM)actual_cs->mecanico);
		SendDlgItemMessage(handler, tel, WM_SETTEXT, sizeof(actual_cs->telefono), (LPARAM)actual_cs->telefono.c_str());
		SendDlgItemMessage(handler, Est, WM_SETTEXT, sizeof(actual_cs->estado), (LPARAM)actual_cs->estado.c_str());
		SendDlgItemMessage(handler, FECHA_CONS, WM_SETTEXT, sizeof(actual_cs->fecha), (LPARAM)actual_cs->fecha);
		SendDlgItemMessage(handler, HORA, WM_SETTEXT, sizeof(actual_cs->hora), (LPARAM)actual_cs->hora.c_str());
		HBITMAP hDirImagen;
		hDirImagen = (HBITMAP)LoadImage(NULL, actual_cs->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		HWND hPictureControl = GetDlgItem(handler, Fotografi);
		SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
		break;
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CHECK2 && HIWORD(wParam) == BN_CLICKED) {
			if (IsDlgButtonChecked(handler, IDC_CHECK2) == BST_CHECKED) {
				HWND check1 = GetDlgItem(handler, IDC_CHECK1);
				SendMessage(check1, BM_SETCHECK, BST_UNCHECKED, 0);
				HWND motivo = GetDlgItem(handler, IDC_EDIT1);
				SendMessage(motivo, EM_SETREADONLY, FALSE, 0);
			}
		}
		if (LOWORD(wParam) == IDC_CHECK1 && HIWORD(wParam) == BN_CLICKED) {
			if (IsDlgButtonChecked(handler, IDC_CHECK1) == BST_CHECKED) {
				HWND check1 = GetDlgItem(handler, IDC_CHECK2);
				SendMessage(check1, BM_SETCHECK, BST_UNCHECKED, 0);
				HWND motivo = GetDlgItem(handler, IDC_EDIT1);
				SendMessage(motivo, EM_UNDO, 0, 0);
  				SendMessage(motivo, EM_EMPTYUNDOBUFFER, 0, 0);
				SendMessage(motivo, EM_SETREADONLY, TRUE, 0);
			}
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			if (IsDlgButtonChecked(handler, IDC_CHECK1) == BST_CHECKED) {
				actual_cs->estado = "Consultada";
				actual_cs = inicio_cs;
				EndDialog(handler, Principal);
			}
			else if (IsDlgButtonChecked(handler, IDC_CHECK2) == BST_CHECKED) {
				HWND hmot = GetDlgItem(handler, IDC_EDIT1);
				int lmot = GetWindowTextLength(hmot);
				if (lmot < 1) {
					MessageBox(handler, "Por favor, haznos saber el motivo de la cancelación.", "", MB_OK);
					break;
				}
				actual_cs->estado = "Cancelada";
				actual_cs = inicio_cs;
				EndDialog(handler, Principal);
			}
			else MessageBox(handler, "Si desea dejar pendiente la cita y solo revisó datos puede cerrar la ventana.", "", MB_OK);

		}
		break;
	case WM_CLOSE:
		EndDialog(handler, Principal);
		break;
	case WM_DESTROY:
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}
BOOL CALLBACK reporte_citas(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
	{
		switch (tipo_busqueda) {
		case 1: {
			string temp2 = actual_m->nombre;
			SendDlgItemMessage(handler, Mecanicos, WM_SETTEXT, sizeof(temp2 + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno), (LPARAM)(temp2 + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno + ", Especialidad: " + actual_m->especialidad).c_str());
			SendDlgItemMessage(handler, Esp, WM_SETTEXT, sizeof("Mes: " + mes), (LPARAM)("Mes: " + mes).c_str());
			SendDlgItemMessage(handler, tel, WM_SETTEXT, sizeof("Citas pendientes."), (LPARAM)"Citas pendientes.");

			ofstream CM;
			CM.open("Consulta de citas por mecánico.txt", ios::out);
			CM << "LISTA DE CITAS DEL MECÁNICO PENDIENTES EN EL MES: "<< mes << endl << endl;
			HWND hLista = GetDlgItem(handler, IDC_LIST1);
			SendMessage(hLista, LB_RESETCONTENT, NULL, NULL);
			while (actual_cs != NULL) {
				string temp1 = actual_cs->mecanico;
				if (temp1.compare(temp2 + " " + actual_m->apellido_paterno + " " + actual_m->apellido_materno) == 0) {
					int pos = 1;
					string aux2 = "";
					string fe = actual_cs->fecha;
					for (int i = 0; fe[i]; i++) {
						if ((fe.c_str()[i] == ' ')) {
							pos++;
						}
						if (pos > 3 && (fe.c_str()[i] != ' ')) {
							aux2 += fe[i];

						}
						else if (pos > 4 && fe.c_str()[i] == ' ') break;
					}
					if (mes.compare(aux2) == 0) {
						if (actual_cs->estado.compare("Pendiente") == 0) {
							MessageBox(handler, "Se generó un archivo de texto en PIA_estdat de las citas pendientes  con todos sus datos.", "", MB_OK);
							char temp[10];
							_itoa(actual_cs->num, temp, 10);
							SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)actual_cs->cliente);
							SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)temp);
							SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)"");
							CM << "Número de cita: "<<actual_cs->num << endl;
							CM <<"Nombre del cliente: "<< actual_cs->cliente << endl;
							CM << "Teléfono del cliente: " << actual_cs->telefono << endl;
							CM << "Nombre del mecánico: "<< actual_cs->mecanico << endl;
							CM <<"Paquete de servicio: "<< actual_cs->paquete<< endl;
							CM << "Fecha de cita: "<< actual_cs->fecha <<", hora:" << actual_cs->hora<<endl;
						}
					}
				}

				actual_cs = actual_cs->siguiente_cs;
			}
			actual_cs = inicio_cs;
			CM.close();
			break;
		}
		case 2: {
			SendDlgItemMessage(handler, Mecanicos, WM_SETTEXT, sizeof("Especialidad: " + esptemp), (LPARAM)("Especialidad: " + esptemp).c_str());
			SendDlgItemMessage(handler, Esp, WM_SETTEXT, sizeof("Mes: " + mes + ", semana: " + sem), (LPARAM)("Mes: " + mes + ", semana: " + sem).c_str());
			SendDlgItemMessage(handler, tel, WM_SETTEXT, sizeof("Citas pendientes."), (LPARAM)"Citas pendientes.");
			ofstream ES;
			ES.open("Consulta de citas por especialidad.txt", ios::out);
			ES << "LISTA DE CITAS DE LA ESPECIALIDAD " << esptemp << ", EN EL MES: " << mes << ", LA SEMANA: "<< sem << endl << endl;

			HWND hLista = GetDlgItem(handler, IDC_LIST1);
			SendMessage(hLista, LB_RESETCONTENT, NULL, NULL);
			while (actual_cs != NULL) {
				string temp1 = actual_cs->paquete;
				if (temp1.compare(esptemp) == 0) {
					int pos = 1;
					string aux2 = "";
					string fe = actual_cs->fecha;
					for (int i = 0; fe[i]; i++) {
						if ((fe.c_str()[i] == ' ')) {
							pos++;
						}
						if (pos > 3 && (fe.c_str()[i] != ' ')) {
							aux2 += fe[i];

						}
						else if (pos > 4 && fe.c_str()[i] == ' ') {
							break;
						}
					}
					if (mes.compare(aux2) == 0) {
						int pos = 1;
						string aux2 = "";
						string fe = actual_cs->fecha;
						for (int i = 0; fe[i]; i++) {
							if ((fe.c_str()[i] == ' ')) {
								pos++;
							}
							if (pos > 1 && (fe.c_str()[i] != ' ')) {
								aux2 += fe[i];

							}
							else if (pos > 2 && fe.c_str()[i] == ' ') break;
						}
						int dia_t = atoi(aux2.c_str());
						string sem_t;
						if (dia_t >= 1 && dia_t <= 7) {
							sem_t = "1";
						}
						if (dia_t >=8 && dia_t <= 14) {
							sem_t = "2";
						}
						if (dia_t >= 15 && dia_t <= 21) {
							sem_t = "3";
						}
						if (dia_t >= 22 && dia_t <= 31) {
							sem_t = "4";
						}

						if (sem.compare(sem_t) == 0) {
							if (actual_cs->estado.compare("Pendiente") == 0) {
								MessageBox(handler, "Se generó un archivo de texto en PIA_estdat de las citas pendientes con todos sus datos.", "", MB_OK);
								char temp[10];
								_itoa(actual_cs->num, temp, 10);
								SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)actual_cs->cliente);
								SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)temp);
								SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)"");
								ES << "Número de cita: " << actual_cs->num << endl;
								ES << "Nombre del cliente: " << actual_cs->cliente << endl;
								ES << "Teléfono del cliente: " << actual_cs->telefono << endl;
								ES << "Nombre del mecánico: " << actual_cs->mecanico << endl;
								ES << "Paquete de servicio: " << actual_cs->paquete << endl;
								ES << "Fecha de cita: " << actual_cs->fecha << ", hora:" << actual_cs->hora << endl;
							}
						}
					}
				}
				actual_cs = actual_cs->siguiente_cs;
			}
			actual_cs = inicio_cs;
			ES.close();
			break;
		}
		case 3: {
			SendDlgItemMessage(handler, Mecanicos, WM_SETTEXT, sizeof("Todos los mecánicos."), (LPARAM)"Todos los mecánicos");
			SendDlgItemMessage(handler, Esp, WM_SETTEXT, sizeof("Mes: " + mes + ", semana: " + sem), (LPARAM)("Mes: " + mes + ", semana: " + sem).c_str());
			SendDlgItemMessage(handler, tel, WM_SETTEXT, sizeof("Citas pendientes."), (LPARAM)"Citas pendientes.");
			ofstream SE;
			SE.open("Consulta de citas por semana.txt", ios::out);
			SE << "LISTA DE CITAS DE EL MES: "<< mes << ", LA SEMANA: " << sem << endl << endl;
			HWND hLista = GetDlgItem(handler, IDC_LIST1);
			SendMessage(hLista, LB_RESETCONTENT, NULL, NULL);
			while (actual_cs != NULL) {
				int pos = 1;
				string aux2 = "";
				string fe = actual_cs->fecha;
				for (int i = 0; fe[i]; i++) {
					if ((fe.c_str()[i] == ' ')) {
						pos++;
					}
					if (pos > 3 && (fe.c_str()[i] != ' ')) {
						aux2 += fe[i];

					}
					else if (pos > 4 && fe.c_str()[i] == ' ') {
						break;
					}
				}
				if (mes.compare(aux2) == 0) {
					int pos = 1;
					string aux2 = "";
					string fe = actual_cs->fecha;
					for (int i = 0; fe[i]; i++) {
						if ((fe.c_str()[i] == ' ')) {
							pos++;
						}
						if (pos > 1 && (fe.c_str()[i] != ' ')) {
							aux2 += fe[i];

						}
						else if (pos > 2 && fe.c_str()[i] == ' ') break;
					}
					int dia_t = atoi(aux2.c_str());
					string sem_t;
					if (dia_t >= 1 && dia_t <= 7) {
						sem_t = "1";
					}
					if (dia_t >= 8 && dia_t <= 14) {
						sem_t = "2";
					}
					if (dia_t >= 15 && dia_t <= 21) {
						sem_t = "3";
					}
					if (dia_t >= 22 && dia_t <= 31) {
						sem_t = "4";
					}

					if (sem.compare(sem_t) == 0) {
						if (actual_cs->estado.compare("Pendiente") == 0) {
							MessageBox(handler, "Se generó un archivo de texto de las citas pendientes con todos sus datos.", "", MB_OK);
							char temp[10];
							_itoa(actual_cs->num, temp, 10);
							SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)actual_cs->cliente);
							SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)temp);
							SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)"");
							SE << "Número de cita: " << actual_cs->num << endl;
							SE << "Nombre del cliente: " << actual_cs->cliente << endl;
							SE << "Teléfono del cliente: " << actual_cs->telefono << endl;
							SE << "Nombre del mecánico: " << actual_cs->mecanico << endl;
							SE << "Paquete de servicio: " << actual_cs->paquete << endl;
							SE << "Fecha de cita: " << actual_cs->fecha << ", hora:" << actual_cs->hora << endl;
						}
					}
				}
				actual_cs = actual_cs->siguiente_cs;
			}
			actual_cs = inicio_cs;
			SE.close();
			break;
		}
		}
		break;
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			bool buscar = false;
			HWND hclave = GetDlgItem(handler, IDC_EDIT1);
			int lclave = GetWindowTextLength(hclave);
			char temp_cl[MAX_PATH];
			if (lclave < 1) {
				MessageBox(handler, "Debe ingresar la clave de cita", "Error", MB_OK);
				break;
			}
			GetWindowText(hclave, temp_cl, ++lclave);
			string temp_clST = temp_cl;
			while (actual_cs != NULL) {
				if (stoi(temp_clST) == actual_cs->num) {
					buscar = true;
					break;
				}
				actual_cs = actual_cs->siguiente_cs;
			}
			if (buscar != true) {
				MessageBox(handler, "El número de cita no existe.", "Error", MB_OK);
				break;
			}
			DialogBox(hGlobalInst, MAKEINTRESOURCE(cita_datos), handler, mostrar_citas);
		}
		break;
	case WM_CLOSE:
		EndDialog(handler, Principal);
		break;
	case WM_DESTROY:
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}

void cargar_usuarios() {
	fstream archivo_de_usuarios;
	int cont = 0;
	string linea;

	archivo_de_usuarios.open("usuarios.txt", ios::in);

	if (archivo_de_usuarios.fail()) {
		PostQuitMessage(1021);
	}


	if (archivo_de_usuarios.is_open()) {
		while (!archivo_de_usuarios.eof()) {
			
			if (inicio_u == NULL) {
				inicio_u = new usuarios;
				getline(archivo_de_usuarios, linea);
				while (linea != "") {
					inicio_u->nombre = linea.c_str();
					getline(archivo_de_usuarios, linea);
					inicio_u->apellido_paterno = linea.c_str();
					getline(archivo_de_usuarios, linea);
					inicio_u->apellido_materno = linea.c_str();
					getline(archivo_de_usuarios, linea);
					inicio_u->usuario = linea.c_str();
					getline(archivo_de_usuarios, linea);
					inicio_u->contraseña = linea.c_str();
					getline(archivo_de_usuarios, linea);
				}

				inicio_u->siguiente_u = NULL;
				inicio_u->anterior_u = NULL;
				actual_u = inicio_u;
			}
			else {
				while (actual_u->siguiente_u != NULL)
					actual_u = actual_u->siguiente_u;
				actual_u->siguiente_u = new usuarios;
				actual_u->siguiente_u->anterior_u = actual_u;
				actual_u = actual_u->siguiente_u;
				getline(archivo_de_usuarios, linea);
				while (linea != "") {
					actual_u->nombre = linea.c_str();
					getline(archivo_de_usuarios, linea);
					actual_u->apellido_paterno = linea.c_str();
					getline(archivo_de_usuarios, linea);
					actual_u->apellido_materno = linea.c_str();
					getline(archivo_de_usuarios, linea);
					actual_u->usuario = linea.c_str();
					getline(archivo_de_usuarios, linea);
					actual_u->contraseña = linea.c_str();
					getline(archivo_de_usuarios, linea);
					if (archivo_de_usuarios.eof())
						break;
				}
				actual_u->siguiente_u = NULL;
				actual_u = inicio_u;
			}
		}
	}

	archivo_de_usuarios.close();
}
void validar_nombres(char temp[MAX_PATH], bool &correcto) {
	if (correcto == true) {
		for (int i = 0; temp[i]; i++) {
			if (isalpha(temp[i]) == 0) {
				if (temp[i] == ' ') break;
				MessageBox(NULL, "No se aceptan números en el nombre y apellidos.", "", MB_OK);
				correcto = false;
				break;
			}
		}
	}
}
void guardado_general(paquetes_disp *inicio_p, mecanic *inicio_m, clientes *inicio_c, cita *inicio_cs) {
	arp.open("Paquetes.bin", ios::binary | ios::out | ios::trunc);
	if (arp.is_open()) {
		while (inicio_p != NULL) {
			arp.write(reinterpret_cast<char*>(inicio_p), sizeof(paquetes_disp));
			paquetes_disp* temp = inicio_p;
			temp = inicio_p;
			inicio_p = inicio_p->siguiente_p;
			delete temp;
		}
	}
	arp.close();

	armec.open("Mecanicos.bin", ios::binary | ios::out | ios::trunc);
	if (armec.is_open()) {
		while (inicio_m != NULL) {
			armec.write(reinterpret_cast<char*>(inicio_m), sizeof(mecanic));
			mecanic* temp = inicio_m;
			temp = inicio_m;
			inicio_m = inicio_m->siguiente_m;
			delete temp;
		}
	}
	armec.close();

	arcl.open("Clientes.bin", ios::binary | ios::out | ios::trunc);
	if (arcl.is_open()) {
		while (inicio_c != NULL) {
			arcl.write(reinterpret_cast<char*>(inicio_c), sizeof(clientes));
			clientes* temp = inicio_c;
			temp = inicio_c;
			inicio_c = inicio_c->siguiente_c;
			delete temp;
		}
	}
	arcl.close();

	arci.open("Citas.bin", ios::binary | ios::out | ios::trunc);
	if (arci.is_open()) {
		while (inicio_cs != NULL) {
			arci.write(reinterpret_cast<char*>(inicio_cs), sizeof(cita));
			cita* temp = inicio_cs;
			temp = inicio_cs;
			inicio_cs = inicio_cs->siguiente_cs;
			delete temp;
		}
	}
	arci.close();
	return;
}
void cargado_general() {
	arp.open("Paquetes.bin", ios::binary | ios::in | ios::ate);
	if (arp.is_open()) {
		int TotalSize = arp.tellg();
		if (TotalSize < 1) {
			arp.close();
		}
		for (int i = 0; i < TotalSize / sizeof(paquetes_disp); i++) {
			arp.seekg(i * sizeof(paquetes_disp));
			if (inicio_p == NULL) {
				inicio_p = new paquetes_disp;
				inicio_p->siguiente_p = NULL;
				inicio_p->anterior_p = NULL;
				paquetes_disp *temp = new paquetes_disp;
				arp.read(reinterpret_cast<char*>(temp), sizeof(paquetes_disp));
				strcpy(inicio_p->nombre, temp->nombre);
				inicio_p->clave = temp->clave;
				inicio_p->costo = temp->costo;
				strcpy(inicio_p->descripcion, temp->descripcion);
				actual_p = inicio_p;
				delete reinterpret_cast<char*>(temp);
			}
			else {
				while (actual_p->siguiente_p != NULL)
					actual_p = actual_p->siguiente_p;
				paquetes_disp *temp = new paquetes_disp;
				actual_p->siguiente_p = new paquetes_disp;
				arp.read(reinterpret_cast<char*>(temp), sizeof(paquetes_disp));
				actual_p->siguiente_p->anterior_p = actual_p;
				actual_p = actual_p->siguiente_p;
				actual_p->siguiente_p = NULL;
				strcpy(actual_p->nombre, temp->nombre);
				actual_p->clave = temp->clave;
				actual_p->costo = temp->costo;
				strcpy(actual_p->descripcion, temp->descripcion);
				actual_p = inicio_p;
				delete reinterpret_cast<char*>(temp);
			}
		}
		arp.close();
	}

	armec.open("Mecanicos.bin", ios::binary | ios::in | ios::ate);
	if (armec.is_open()) {
		int TotalSize = armec.tellg();
		if (TotalSize < 1) {
			armec.close();
		}
		for (int i = 0; i < TotalSize / sizeof(mecanic); i++) {
			armec.seekg(i * sizeof(mecanic));
			if (inicio_m == NULL) {
				inicio_m = new mecanic;
				inicio_m->siguiente_m = NULL;
				inicio_m->anterior_m = NULL;
				mecanic *temp = new mecanic;
				armec.read(reinterpret_cast<char*>(temp), sizeof(mecanic));
				for (int i = 0; i < 5; i++) {
					inicio_m->dias[i] = temp->dias[i];
				}
				inicio_m->num_empleado = temp->num_empleado;
				strcpy(inicio_m->nombre, temp->nombre);
				inicio_m->apellido_paterno = temp->apellido_paterno;
				inicio_m->apellido_materno = temp->apellido_materno;
				inicio_m->telefono = temp->telefono;
				strcpy(inicio_m->especialidad, temp->especialidad);
				inicio_m->turno = temp->turno;
				strcpy(inicio_m->foto, temp->foto);
				actual_m = inicio_m;
				delete reinterpret_cast<char*>(temp);
			}
			else {
				while (actual_m->siguiente_m != NULL)
					actual_m = actual_m->siguiente_m;
				mecanic *temp = new mecanic;
				actual_m->siguiente_m = new mecanic;
				armec.read(reinterpret_cast<char*>(temp), sizeof(mecanic));
				actual_m->siguiente_m->anterior_m = actual_m;
				actual_m = actual_m->siguiente_m;
				actual_m->siguiente_m =NULL;
				for (int i = 0; i < 5; i++) {
					actual_m->dias[i] = temp->dias[i];
				}
				actual_m->num_empleado = temp->num_empleado;
				strcpy_s(actual_m->nombre, temp->nombre);
				actual_m->apellido_paterno = temp->apellido_paterno;
				actual_m->apellido_materno = temp->apellido_materno;
				actual_m->telefono = temp->telefono;
				strcpy(actual_m->especialidad, temp->especialidad);
				actual_m->turno = temp->turno;
				strcpy(actual_m->foto, temp->foto);
				actual_m = inicio_m;
				delete reinterpret_cast<char*>(temp);
			}
		}
		armec.close();
	}

	arcl.open("Clientes.bin", ios::binary | ios::in | ios::ate);
	if (arcl.is_open()) {
		int TotalSize = arcl.tellg();
		if (TotalSize < 1) {
			arcl.close();
		}
		for (int i = 0; i < TotalSize / sizeof(clientes); i++) {
			arcl.seekg(i * sizeof(clientes));
			if (inicio_c == NULL) {
				inicio_c = new clientes;
				inicio_c->siguiente_c = NULL;
				inicio_c->anterior_c = NULL;
				clientes *temp = new clientes;
				arcl.read(reinterpret_cast<char*>(temp), sizeof(clientes));
				strcpy(inicio_c->nombre, temp->nombre);
				inicio_c->apellido_paterno = temp->apellido_paterno;
				inicio_c->apellido_materno = temp->apellido_materno;
				inicio_c->telefono = temp->telefono;
				inicio_c->placas = temp->placas;
				inicio_c->marca = temp->marca;
				inicio_c->modelo = temp->modelo;
				inicio_c->año_v = temp->año_v;
				inicio_c->kilometraje = temp->kilometraje;
				strcpy(inicio_c->direccion, temp->direccion);
				actual_c = inicio_c;
				delete reinterpret_cast<char*>(temp);
			}
			else {
				while (actual_c->siguiente_c != NULL)
					actual_c = actual_c->siguiente_c;
				clientes *temp = new clientes;
				actual_c->siguiente_c = new clientes;
				arcl.read(reinterpret_cast<char*>(temp), sizeof(clientes));
				actual_c->siguiente_c->anterior_c = actual_c;
				actual_c = actual_c->siguiente_c;
				actual_c->siguiente_c = NULL;
				strcpy_s(actual_c->nombre, temp->nombre);
				actual_c->apellido_paterno = temp->apellido_paterno;
				actual_c->apellido_materno = temp->apellido_materno;
				actual_c->telefono = temp->telefono;
				actual_c->placas = temp->placas;
				actual_c->marca = temp->marca;
				actual_c->modelo = temp->modelo;
				actual_c->año_v = temp->año_v;
				actual_c->kilometraje = temp->kilometraje;
				strcpy(inicio_c->direccion, temp->direccion);
				actual_c = inicio_c;
				delete reinterpret_cast<char*>(temp);
			}
		}
		arcl.close();
	}

	arci.open("Citas.bin", ios::binary | ios::in | ios::ate);
	if (arci.is_open()) {
		int TotalSize = arci.tellg();
		if (TotalSize < 1) {
			arci.close();
		}
		for (int i = 0; i < TotalSize / sizeof(cita); i++) {
			arci.seekg(i * sizeof(cita));
			if (inicio_cs == NULL) {
				inicio_cs = new cita;
				inicio_cs->siguiente_cs = NULL;
				inicio_cs->anterior_cs = NULL;
				cita *temp = new cita;
				arci.read(reinterpret_cast<char*>(temp), sizeof(cita));
				strcpy(inicio_cs->cliente, temp->cliente);
				strcpy(inicio_cs->mecanico, temp->mecanico);
				inicio_cs->hora = temp->hora;
				inicio_cs->telefono = temp->telefono;
				inicio_cs->estado = temp->estado;
				strcpy(inicio_cs->foto, temp->foto);
				inicio_cs->num = temp->num;
				strcpy(inicio_cs->paquete, temp->paquete);
				strcpy(inicio_cs->fecha, temp->fecha);
				num_cita = inicio_cs->num;
				actual_cs = inicio_cs;
				delete reinterpret_cast<char*>(temp);
			}
			else {
				while (actual_cs->siguiente_cs != NULL)
					actual_cs = actual_cs->siguiente_cs;
				cita *temp = new cita;
				actual_cs->siguiente_cs = new cita;
				arci.read(reinterpret_cast<char*>(temp), sizeof(cita));
				actual_cs->siguiente_cs->anterior_cs = actual_cs;
				actual_cs = actual_cs->siguiente_cs;
				actual_cs->siguiente_cs = NULL;
				strcpy_s(actual_cs->cliente, temp->cliente);
				strcpy(actual_cs->mecanico, temp->mecanico);
				actual_cs->hora = temp->hora;
				actual_cs->telefono = temp->telefono;
				actual_cs->num = temp->num;
				strcpy(actual_cs->foto, temp->foto);
				actual_cs->estado = temp->estado;
				strcpy(actual_cs->fecha, temp->fecha);
				strcpy(actual_cs->paquete, temp->paquete);
				num_cita = actual_cs->num;
				actual_cs = inicio_cs;
				delete reinterpret_cast<char*>(temp);
			}
		}
		arci.close();
	}
	return;
}
int part(string *arr_num, int inicio, int fin)
{
	string pivote = arr_num[fin];
	int P_ind = inicio;
	int i;
	string tempn;
	for (i = inicio; i < fin; i++)
	{
		if (arr_num[i] <= pivote)
		{
			tempn = arr_num[i];
			arr_num[i] = arr_num[P_ind];
			arr_num[P_ind] = tempn;
			P_ind++;
		}
	}
	tempn = arr_num[fin];
	arr_num[fin] = arr_num[P_ind];
	arr_num[P_ind] = tempn;
	return P_ind;
}
void Quicksort(string *arr_num, int inicio, int fin)
{
	if (inicio < fin)
	{
		int P_ind = part(arr_num, inicio, fin);
		Quicksort(arr_num, inicio, P_ind - 1);
		Quicksort(arr_num, P_ind + 1, fin);
	}
}	
void heapify(string* arr_n, int iterationSize) {
	int i, par, nodo;
	for (i = 1; i <= iterationSize; i++) {
		nodo = i;
		par = (int)nodo / 2;

		while (par >= 1) {

			if (arr_n[par] < arr_n[nodo])
				swap(arr_n[par], arr_n[nodo]);

			nodo = par;
			par = (int)nodo / 2;

		}
	}
}
void heapSort(string* arr_n, int tam) {
	for (int i = tam; i >= 1; i--) {
		heapify(arr_n, i);
		swap(arr_n[1], arr_n[i]);
	}
}
