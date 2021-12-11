#include <iostream>
#include <windowsx.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"

using namespace std;

HINSTANCE hInst;
int valor=0,valor2=-1,valor3=-1;
float MENOR_DISTANCI=9955;
float BUSQ_X=9955,BUSQ_Y=9955;
bool MOUSE_CONTROL=0;
int RADIO=80;
HBRUSH brush;
HPEN greenPen;
COLORREF actual;

///QUADTREE PUNTOS

class NODOS{
    public:
    long x,y,DR;
    long xBUSQ,yBUSQ;
    float PIVOT_DISTANCIA=9999;
    NODOS *CX,*CY,*PaDr;
    NODOS(NODOS *Padr=NULL,int Cx=0,int Cy=0,int DR=1,NODOS *CX=NULL,NODOS *CY=NULL){
        this-> PaDr = Padr;
        this-> CX   = CX;
        this-> CY   = CY;
        this-> x    = Cx;
        this-> y    = Cy;
        this-> DR   = DR;
    }
};
class QUADTREE{
    public:
    NODOS *raiz,*padr;
    QUADTREE(){
        raiz=NULL;
        padr=NULL;
    }
    void insertarBUSQ(int xB,int yB){
         MENOR_DISTANCI=9955;
         insBUSQ(raiz,xB,yB);
    }
    void insBUSQ(NODOS *&R,int xB,int yB){
         if(R!=NULL){
            R->xBUSQ=xB;
            R->yBUSQ=yB;
         }
    }
    void insertar(int x,int y){
         ins(raiz,padr,x,y,1);
    }
    void ins(NODOS *&R,NODOS *padr,int x,int y,int DR){
         if(R!=NULL){
            if(R->DR==1){
                DR=2;
            }
            if(R->DR==2){
                DR=1;
            }
         }
         if(R==NULL){
            R=new NODOS(padr,x,y,DR);
         }
         if(R->DR==1){
            if(x<R->x){
                ins(R->CX,R,x,y,DR);
            }
            if(x>R->x){
                ins(R->CY,R,x,y,DR);
            }
         }
         if(R->DR==2){
            if(y<R->y){
                ins(R->CX,R,x,y,DR);
            }
            if(y>R->y){
                ins(R->CY,R,x,y,DR);
            }
         }
    }
    void buscar(int datx,int daty){
         busc(raiz,datx,daty);
    }
    void busc(NODOS *R,int datx,int daty){
         if(R!=NULL){
            if(R->x==datx&&R->y==daty){
                valor=1;
                valor2=R->x;
                valor3=R->y;
            }
            busc(R->CX,datx,daty);
            busc(R->CY,datx,daty);
         }
    }
    void Mostrar(HDC hdc,int x,int y,int xa,int ya,int a){
         Mos(raiz,hdc,x,y,xa,ya,a);
    }
    void Mos(NODOS *R,HDC hdc,int x,int y,int xa,int ya,int a){

         actual = SetTextColor(hdc, RGB(0,0,255));
         const char *cadena[7];
         cadena[0]="KDTREE PUNTO 2D";
         cadena[1]="Incertar";
         cadena[2]="Buscar";
         cadena[3]="Radio";
         cadena[4]="X =";
         cadena[5]="Y =";
         cadena[6]="MENOR DISTANCIA:";

         RECT rect[7];
         rect[0]={1330,10,1340,50};
         rect[1]={10,5,80,40};
         rect[2]={10,40,80,100};
         rect[3]={7,40,80,200};
         rect[4]={1380,-40,1130,200};
         rect[5]={1460,-40,1230,200};
         rect[6]={1380,60,1230,200};
         ///color

         DrawText(hdc,cadena[0],strlen(cadena[0]) , &rect[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[1],strlen(cadena[1]) , &rect[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[2],strlen(cadena[2]) , &rect[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[3],strlen(cadena[3]) , &rect[3], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[4],strlen(cadena[4]) , &rect[4], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[5],strlen(cadena[5]) , &rect[5], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[6],strlen(cadena[6]) , &rect[6], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         SetTextColor(hdc, actual);
         actual = SetTextColor(hdc, RGB(0,255,255));
         if(R!=NULL){
            int cx=0,cy=0;
            char cadx[10],cady[10],*CADENA;
            itoa(R->x,cadx,10);
            itoa(R->y,cady,10);

            for(;*(cadx+cx);cx++);
            for(;*(cady+cy);cy++);
            CADENA=new char[cx+cy+4];
            if(R->DR==1){
                *(CADENA+0)='X';
            }
            if(R->DR==2){
                *(CADENA+0)='Y';
            }

            *(CADENA+1)='=';
            for(int i=2;i<cx+cy+4;i++){
                if(i-2<cx){
                   *(CADENA+i)=*(cadx+i-2);
                }
                if(i-2==cx){
                   *(CADENA+i)=',';
                }
                if(i-2>cx){
                   *(CADENA+i)=*(cady+i-cx-3);
                }
            }
            *(CADENA+cx+cy+3)='\0';

            Mos(R->CX,hdc,x-a+4/2,y+60,x,y,a/2);
            Mos(R->CY,hdc,x+a+4/2,y+60,x,y,a/2);

            MoveToEx(hdc,xa,ya,NULL);
            LineTo(hdc,x,y);
            if(R->PIVOT_DISTANCIA==MENOR_DISTANCI){

                brush=CreateSolidBrush(RGB(0,255,255));
                SelectObject(hdc,brush);
                Rectangle(hdc,x+45,y+40,x-50,y-10);

            }
            if(valor2==R->x&&valor3==R->y){

                brush=CreateSolidBrush(RGB(51, 204, 255));
                SelectObject(hdc,brush);
                Rectangle(hdc,x+45,y+40,x-50,y-20);
            }else{

                brush=CreateSolidBrush(RGB(0, 0, 0));
                SelectObject(hdc,brush);
                Rectangle(hdc,x+45,y+35,x-50,y-5);
            }

            DeleteObject(brush);
            TextOut(hdc,x-35,y,CADENA,cx+cy+3);
            if(R->PIVOT_DISTANCIA!=9999){

                char DISTANCCE[12];
                char CInt[10];
                char Cfloat[10];
                int IntD=(int)R->PIVOT_DISTANCIA;
                int FloatD=R->PIVOT_DISTANCIA*100-(int)R->PIVOT_DISTANCIA*100;
                itoa(IntD,CInt,10);
                itoa(FloatD,Cfloat,10);
                int i=0;
                for(i=0;*(CInt+i);i++){
                    *(DISTANCCE+i)=*(CInt+i);
                }*(DISTANCCE+i)='.';i++;
                for(int j=0;*(Cfloat+j);j++,i++){
                    *(DISTANCCE+i)=*(Cfloat+j);
                }*(DISTANCCE+i)='\0';

                actual = SetTextColor(hdc, RGB(255,120,100));

                TextOut(hdc,x-20,y+14,DISTANCCE,i);
                TextOut(hdc,x-35,y+14,"D: ",3);

            }
            SetTextColor(hdc, actual);

         }
    }
    void MostrarCuadro(HDC hdc,int xi,int xf,int yi,int yf){
         MosCuad(raiz,hdc,xi,xf,yi,yf);
    }
    void MosCuad(NODOS *R,HDC hdc,int xi,int xf,int yi,int yf){
         if(R!=NULL){
            if(R->DR==1){
                MosCuad(R->CX,hdc,xi,R->x+250,yi,yf);
                MosCuad(R->CY,hdc,R->x+250,xf,yi,yf);
                MoveToEx(hdc,R->x+250,yi,NULL);
                LineTo(hdc,R->x+250,yf);

            }
            if(R->DR==2){
                MosCuad(R->CX,hdc,xi,xf,yi,R->y+10);
                MosCuad(R->CY,hdc,xi,xf,R->y+10,yf);
                MoveToEx(hdc,xi,R->y+10,NULL);
                LineTo(hdc,xf,R->y+10);
            }
         }
    }
    void MostrarCuadroPUNTO(HDC hdc){
         MosCuadPUNTO(raiz,hdc);
    }
    void MosCuadPUNTO(NODOS *R,HDC hdc){
         if(R!=NULL){
            MosCuadPUNTO(R->CX,hdc);

            greenPen=CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
            SelectObject(hdc,greenPen);
            Arc(hdc, R->x-2+740, R->y-2+10, R->x+2+740, R->y+2+10, 120, 150, 110, 0);
            DeleteObject(greenPen);

            MosCuadPUNTO(R->CY,hdc);
         }
    }

    void MostrarPUNTOBUSQUEDA(HDC hdc){
         MosCuadPUNTOBUSQUEDA(raiz,hdc);
    }

    void MosCuadPUNTOBUSQUEDA(NODOS *R,HDC hdc){
        if(R!=NULL){

            if(R->xBUSQ!=0&&R->yBUSQ!=0){
                greenPen=CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
                SelectObject(hdc,greenPen);
                BUSQ_X=R->xBUSQ-740;
                BUSQ_Y=R->yBUSQ-10;
                cout<<"RANGO BUSQUEDA X= "<<R->xBUSQ-740<<"  Y= "<<R->yBUSQ-10<<endl;

                Arc(hdc, R->xBUSQ-2, R->yBUSQ-2, R->xBUSQ+2, R->yBUSQ+2, 10, 10, 10, 10);
                Arc(hdc, R->xBUSQ-RADIO, R->yBUSQ-RADIO, R->xBUSQ+RADIO, R->yBUSQ+RADIO, 10, 10, 10, 10);

                DeleteObject(greenPen);
            }

        }
    }

    void PERTENECE(HDC hdc){
         PERT_PUNTO(raiz,hdc);
    }
    void PERT_PUNTO(NODOS *R,HDC hdc){
         if(R!=NULL){

            PERT_PUNTO(R->CX,hdc);

            float RESUL,DIST;
            RESUL=(((R->x)-BUSQ_X)*((R->x)-BUSQ_X))+(((R->y)-BUSQ_Y)*((R->y)-BUSQ_Y));
            if(RESUL<RADIO*RADIO){
                DIST=sqrt(RESUL);
                cout<<RESUL<<"  -->>  "<<DIST<<"\n";

                greenPen=CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
                SelectObject(hdc,greenPen);
                MoveToEx(hdc,R->x+740,R->y+10,NULL);
                LineTo(hdc,BUSQ_X+740,BUSQ_Y+10);
                DeleteObject(greenPen);

                greenPen=CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
                SelectObject(hdc,greenPen);
                Arc(hdc, R->x-2+740, R->y-2+10, R->x+2+740, R->y+2+10, 120, 150, 110, 0);
                DeleteObject(greenPen);

                if(DIST<MENOR_DISTANCI){
                    MENOR_DISTANCI=DIST;
                }
                R->PIVOT_DISTANCIA=DIST;

            }else{
                R->PIVOT_DISTANCIA=9999;
            }

            PERT_PUNTO(R->CY,hdc);
         }
    }

}a;


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND hwnd;
float t = 0.0f;
HANDLE tickThreadHandle;
DWORD WINAPI tickThreadProc(HANDLE handle)
{
    Sleep(0);
    ShowWindow(hwnd, SW_SHOW);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT r;
    GetWindowRect(hwnd, &r);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hPen));

    int delay = 1000 / 50;

    while (true)
    {
        for (int x = 0; x<r.right; x += 5, t += 0.3f)
        {
            LineTo(hdc, x, sin(t) * 50 + 200);
        }

        t += 0.1f;
        Sleep(delay);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    EndPaint(hwnd, &ps);

}

/* Declaraciones de tipos */
typedef struct stDatos {
   char Texto[80];
} DATOS;
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hInstance;
    /* Variables para diálogo */
    static DATOS Datos;
    static HFONT hfont;
    static HBRUSH pincel;
    HWND hctrl;
    switch(uMsg)
    {

        ///
        case WM_CREATE:
           hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
           /* Insertar control Edit */
           hctrl = CreateWindowEx(
              0,
              "EDIT",          /* Nombre de la clase */
              "",              /* Texto del título, no tiene */
              ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, /* Estilo */
              36, 420,          /* Posición */
              120, 25,         /* Tamaño */
              hwnd,            /* Ventana padre */
              (HMENU)ID_TEXTO, /* Identificador del control */
              hInstance,       /* Instancia */
              NULL);           /* Sin datos de creación de ventana */
           /* Inicialización de los datos de la aplicación */
           SendMessage(hctrl, EM_SETLIMITTEXT, 10, 0L);
           strcpy(Datos.Texto, "Inicial");
           SendMessage(hctrl, WM_SETTEXT, 0, (LPARAM)Datos.Texto);
           hfont = CreateFont(20, 0, 0, 0, 300,
              FALSE, FALSE, FALSE, DEFAULT_CHARSET,
              OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
              PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN,
              "Times New Roman");
           hfont = (HFONT)GetStockObject( DEFAULT_GUI_FONT );
           SendMessage(hctrl, WM_SETFONT, (WPARAM)hfont, MAKELPARAM(TRUE, 0));
           pincel = CreateSolidBrush(RGB(0,255,0));
           SetFocus(hctrl);
           return 0;
        case WM_CTLCOLOREDIT:
           SetBkColor((HDC)wParam, RGB(0,255,0));
           SetTextColor((HDC)wParam, RGB(255,255,255));
           return (LRESULT)pincel;


           ///
    case WM_MOUSEMOVE:{
        long xpos=GET_X_LPARAM(lParam);
        long ypos=GET_Y_LPARAM(lParam);
        char cade[20];
        ltoa(xpos-250,cade,10);
        SetDlgItemText(hwndDlg,EDITX,cade);
        ltoa(ypos-10,cade,10);
        SetDlgItemText(hwndDlg,EDITY,cade);
        if(MOUSE_CONTROL){

            if(xpos>740&&ypos>10&&xpos<1220&&ypos<250){
                a.insertarBUSQ(xpos,ypos);
                InvalidateRect(hwndDlg,NULL,true);
            }
        }

        }
        return TRUE;
    case WM_PAINT:
        {
            /*Sleep(1);
            ShowWindow(hwndDlg, SW_SHOWMAXIMIZED);
            */
            HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);
            SetBkMode(hdc, TRANSPARENT);
            greenPen=CreatePen(PS_SOLID, 1, RGB(51, 204, 255));
            SelectObject(hdc,greenPen);

            brush=CreateSolidBrush(RGB(67, 68, 70));
            SelectObject(hdc,brush);
            Rectangle(hdc,-1,-1,1815,1000);
            Rectangle(hdc,10,260,1715,900);

            DeleteObject(brush);

            brush=CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc,brush);


            Rectangle(hdc,1230,60,1440,100);
            Rectangle(hdc,10,10,240,200);
            Rectangle(hdc,1230,10,1440,50);
            Rectangle(hdc,1230,110,1440,150);

            DeleteObject(brush);

            brush=CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc,brush);
            Rectangle(hdc,250,10,730,250);
            Rectangle(hdc,740,10,1220,250);
            DeleteObject(brush);
            DeleteObject(greenPen);
            ///inscerVal();

            a.MostrarCuadroPUNTO(hdc);
            a.MostrarPUNTOBUSQUEDA(hdc);
            a.PERTENECE(hdc);

            char DISTANCCE[12];
            char CInt[10];
            char Cfloat[10];
            int IntD=(int)MENOR_DISTANCI;
            int FloatD=MENOR_DISTANCI*100-(int)MENOR_DISTANCI*100;
            itoa(IntD,CInt,10);
            itoa(FloatD,Cfloat,10);
            int i=0;
            for(i=0;*(CInt+i);i++){
                *(DISTANCCE+i)=*(CInt+i);
            }*(DISTANCCE+i)='.';i++;
            for(int j=0;*(Cfloat+j);j++,i++){
                *(DISTANCCE+i)=*(Cfloat+j);
            }*(DISTANCCE+i)='\0';

            if(MENOR_DISTANCI==9955){
                SetDlgItemText(hwndDlg,EDIT23,"NA");
            }else{
                SetDlgItemText(hwndDlg,EDIT23,DISTANCCE);
            }

            greenPen=CreatePen(PS_SOLID, 1, RGB(51, 204, 255));
            SelectObject(hdc,greenPen);

            brush=CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc,brush);
            Rectangle(hdc,10,260,1715,900);
            DeleteObject(brush);

            brush=CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc,brush);
            Rectangle(hdc,250,10,730,250);
            DeleteObject(brush);

            a.Mostrar(hdc,820,300,820,300,400);
            a.MostrarCuadro(hdc,250,730,10,250);

            DeleteObject(greenPen);
            EndPaint(hwndDlg,&ps);
        }
        return TRUE;

        case WM_LBUTTONDOWN:{
            /*HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);*/
            long xpos=GET_X_LPARAM(lParam);
            long ypos=GET_Y_LPARAM(lParam);
            if(xpos>250&&ypos>10&&xpos<730&&ypos<250){
               a.insertar(xpos-250,ypos-10);
            }
            if(xpos>740&&ypos>10&&xpos<1220&&ypos<250){
               a.insertarBUSQ(xpos,ypos);
            }
            InvalidateRect(hwndDlg,NULL,true);
            }
            return TRUE;
        case WM_INITDIALOG:
        {
        }
        return TRUE;

        case WM_CLOSE:
        {   EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_COMMAND:
        {   switch(LOWORD(wParam))
            {
                case BTN_1:
                    if(MOUSE_CONTROL){
                        MOUSE_CONTROL=0;
                        SetDlgItemText(hwndDlg,EDIT22,"NO");
                    }else{
                        MOUSE_CONTROL=1;
                        SetDlgItemText(hwndDlg,EDIT22,"SI");
                    }
                    return TRUE;
                case BTN_INSERTAR:
                    char Cadx[20];
                    char Cady[20];
                    int Numx,Numy;
                    GetDlgItemText(hwndDlg,EDIT1,Cadx,10);//capturar evento
                    GetDlgItemText(hwndDlg,EDIT5,Cady,10);//capturar evento
                    Numx=atoi(Cadx);
                    Numy=atoi(Cady);
                    a.insertar(Numx,Numy);
                    if(valor!=1){
                        SetDlgItemText(hwndDlg,EDIT4,"INCERTADO");
                    }else{
                        SetDlgItemText(hwndDlg,EDIT4,"YA EXISTE");
                    }
                    valor=0;valor2=-1;
                    SetDlgItemText(hwndDlg,EDIT1,"");
                    SetDlgItemText(hwndDlg,EDIT5,"");
                    InvalidateRect(hwndDlg,NULL,true);
                    return TRUE;
                case BTN_BUSCAR:
                    char Cadex[20];
                    char Cadey[20];
                    int Numex,Numey;
                    GetDlgItemText(hwndDlg,EDIT2,Cadex,10);//capturar evento
                    GetDlgItemText(hwndDlg,EDIT7,Cadey,10);
                    Numex=atoi(Cadex);
                    Numey=atoi(Cadey);
                    a.buscar(Numex,Numey);
                    if(valor==1){
                        SetDlgItemText(hwndDlg,EDIT4,"ENCONTRADO");
                    }else{
                        SetDlgItemText(hwndDlg,EDIT4,"NO ENCONTRADO");
                        valor2=-1;
                    }valor=0;
                    SetDlgItemText(hwndDlg,EDIT2,"");
                    SetDlgItemText(hwndDlg,EDIT7,"");

                    InvalidateRect(hwndDlg,NULL,true);
                    return TRUE;
                case BTN_ELIMINAR:
                    char CadRDO[20];
                    GetDlgItemText(hwndDlg,EDIT3,CadRDO,10);
                    RADIO=atoi(CadRDO);

                    SetDlgItemText(hwndDlg,EDIT3,"");
                    InvalidateRect(hwndDlg,NULL,true);
                    return TRUE;
            }
        }
        return TRUE;



        case WM_DESTROY:
           DeleteObject(pincel);
           DeleteObject(hfont);
           PostQuitMessage(0);
        break;
        }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
