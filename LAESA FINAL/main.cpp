#include <iostream>
#include <windowsx.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"

using namespace std;
HINSTANCE hInst;
HBRUSH brush;
HPEN greenPen;

int INTERCIONES=0;
float RADIO_CONSULTA=2,RADIO_PIVOT=0;
int XPIVOT=0,YPIVOT=0;

void DIBUJAR_CIRCULO_R(HDC,double,double,double,int);
void DIBUJAR_PUNTOS(HDC,double,double,int);
void DIBUJAR_TABLA_COORD(HDC);

class Nodo
{
public:
    int x;
    int y;
    int DESCARTE;
    Nodo *sig;
    Nodo(int _x=0,int _y=0, Nodo *_sig=NULL,int DESC=0)
    {
        x=_x;
        y=_y;
        DESCARTE=DESC;
        sig=_sig;
    }
};
class laesa
{
public:
    Nodo *inicio;
    int X_CONSULTA=0,Y_CONSULTA=0;
    float a[20][1000];

    laesa(){
        inicio=NULL;
    }
    void inconsulta(int a,int b)
    {
        X_CONSULTA=a;
        Y_CONSULTA=b;
        SELECCION_PIVOT();
        matriz();
    }
    void insertar(int x, int y)
    {
        if(inicio==NULL)
        {
            inicio=new Nodo(x,y,NULL);}
        else ins(inicio,x,y);
    }
    void ins(Nodo *&I,int x, int y)
    {
        if(I==NULL)
        {
            I=new Nodo(x,y);
            cout<<"se inserto: "<<I->x<<","<<I->y<<endl;
        }
        else ins(I->sig,x,y);
    }
    void mostmatriz(HDC hdc)
    {

    }
    float distancia(int X, int Y, Nodo *&b)
    {
        float d;
        d = sqrt((X - b->x)*(X - b->x) + (Y - b->y)*(Y - b->y));
        return d;
    }
    void SELECCION_PIVOT(){
        Nodo *q =inicio;//inicio;
        float DISTANCIA;
        for(int j=0 ;q;j++)
        {
                if(q->DESCARTE!=2){
                    if(distancia(X_CONSULTA,Y_CONSULTA,q)>RADIO_PIVOT){
                        RADIO_PIVOT=distancia(X_CONSULTA,Y_CONSULTA,q);
                        XPIVOT=q->x;
                        YPIVOT=q->y;
                    }

                }
                q=q->sig;
        }
        cout<<"\nPIVOT: ("<<XPIVOT<<","<<YPIVOT<<") Distancia: "<<RADIO_PIVOT<<" Mas Lejano\n\n";
        q =inicio;
        for(;q;)
        {
            if(q->DESCARTE!=2){
                    DISTANCIA=distancia(XPIVOT,YPIVOT,q);
                    if(DISTANCIA<RADIO_PIVOT-RADIO_CONSULTA||DISTANCIA>RADIO_PIVOT+RADIO_CONSULTA){
                        q->DESCARTE=1;
                    }
            }
            q=q->sig;
        }
    }

    void matriz()
    {
        Nodo *q =inicio;//inicio;
        for(int j=0 ;q;j++)
        {
            if(q->DESCARTE!=2){
                a[INTERCIONES][j]=distancia(XPIVOT,YPIVOT,q);
                cout<<"("<<q->x<<","<<q->y<<") Distancia Pivot: "<<a[INTERCIONES][j]<<"\n";
            }
            q=q->sig;
        }
    }
    void MostrarPUNTOS(HDC hdc){  ///GRAFICAR PUNTOS INGRESADOS
        Mos(inicio,hdc);
    }
    void Mos(Nodo *R,HDC hdc){
         if(R!=NULL){
            if(R->DESCARTE==0){DIBUJAR_PUNTOS(hdc,R->x,R->y,0);}
            if(R->DESCARTE==1){DIBUJAR_PUNTOS(hdc,R->x,R->y,4);}
            Mos(R->sig,hdc);
         }
    }
    void MC(HDC hdc)
    {
        if(X_CONSULTA!=0&&Y_CONSULTA!=0){
            DIBUJAR_PUNTOS(hdc,X_CONSULTA,Y_CONSULTA,1);
        }
    }
    void graficarconsulta(HDC hdc)
    {
        if(X_CONSULTA!=0&&Y_CONSULTA)
        {
            grafPC(inicio,hdc,1);
        }
    }
    void grafPC(Nodo *R,HDC hdc,int C)
    {
        DIBUJAR_CIRCULO_R(hdc,X_CONSULTA,Y_CONSULTA,RADIO_CONSULTA,C);
        if(XPIVOT!=0&&YPIVOT!=0){
            DIBUJAR_CIRCULO_R(hdc,XPIVOT,YPIVOT,RADIO_PIVOT-RADIO_CONSULTA,C+1);
            DIBUJAR_CIRCULO_R(hdc,XPIVOT,YPIVOT,RADIO_PIVOT+RADIO_CONSULTA,C-1);
        }
    }
    void DESCARTAR()
    {
        Nodo *p =inicio;
        for(int i=0;p;p = p->sig)
        {
            if(p->DESCARTE==1)
            {
                cout<<"\nDESCARTADO: ("<<p->x<<","<<p->y<<")";
                p->DESCARTE=2;
            }
            i++;
        }
    }
}A;

///GRAFICA

void DIBUJAR_CIRCULO_R(HDC hdc,double X,double Y,double RADIO,int COLOR){     ///GRAFICAR RADIO
        if(COLOR==0){greenPen=CreatePen(PS_SOLID, 3, RGB(255, 0, 0));}
        else if(COLOR==1){greenPen=CreatePen(PS_SOLID, 3, RGB(255, 255, 0));}
        else if(COLOR==2){greenPen=CreatePen(PS_SOLID, 3, RGB(0, 0, 255));}
        else{
           greenPen=CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        }
        SelectObject(hdc,greenPen);

        X*=20;
        Y*=-20;
        RADIO*=20;

        Arc(hdc, 250+X-RADIO, 625+Y-RADIO, 250+X+RADIO, 625+Y+RADIO, 10, 10, 10, 10);

        DeleteObject(greenPen);
}

void DIBUJAR_PUNTOS(HDC hdc,double X,double Y,int COLOR){                     ///GRAFICAR PUNTOS
    if(COLOR==0){greenPen=CreatePen(PS_SOLID, 15, RGB(255, 0, 0));}
    else if(COLOR==1){greenPen=CreatePen(PS_SOLID, 15, RGB(255, 255, 0));}
    else if(COLOR==2){greenPen=CreatePen(PS_SOLID, 15, RGB(0, 0, 255));}
   // else if(color==4){greenPen=CreatePen(PS_SOLID, 15, RGB(255, 255, 255));}
    else{
       greenPen=CreatePen(PS_SOLID, 15, RGB(255, 255, 255));
    }
    /// x 250-1190
    /// y 10-625

    X*=20;
    Y*=-20;

    SelectObject(hdc,greenPen);
    MoveToEx(hdc,250+X,625+Y,NULL);
    LineTo(hdc,250+X,625+Y);
    DeleteObject(greenPen);
}

void DIBUJAR_TABLA_COORD(HDC hdc){                                  ///GRAFICAR LINEAS TABLA
    for(int i=270,j=0;j<46;j++,i+=20){
        MoveToEx(hdc,i,10,NULL);
        LineTo(hdc,i,625);
    }
    for(int i=605,j=0;j<30;j++,i-=20){
        MoveToEx(hdc,250,i,NULL);
        LineTo(hdc,1190,i);
    }
}

///GRAFICA
laesa B;
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
    }
    return TRUE;
    case WM_PAINT:
        {
            HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);
            SetBkMode(hdc, TRANSPARENT);

            greenPen=CreatePen(PS_SOLID, 1, RGB(51, 204, 255));
            SelectObject(hdc,greenPen);

            brush=CreateSolidBrush(RGB(67, 68, 70));    ///FONDO
            SelectObject(hdc,brush);
            Rectangle(hdc,-1,-1,1815,1000);
            DeleteObject(brush);

            brush=CreateSolidBrush(RGB(0, 0, 0));       ///COORDENADAS
            SelectObject(hdc,brush);
            Rectangle(hdc,250,10,1190,625);
            DeleteObject(brush);
            DIBUJAR_TABLA_COORD(hdc);

            A.MostrarPUNTOS(hdc);
            A.MC(hdc);
            A.graficarconsulta(hdc);

            greenPen=CreatePen(PS_SOLID, 1, RGB(51, 204, 255));
            SelectObject(hdc,greenPen);
            brush=CreateSolidBrush(RGB(255, 255, 255)); ///L IZQUIERDO
            SelectObject(hdc,brush);
            Rectangle(hdc,10,10,240,625);
            DeleteObject(brush);
            DeleteObject(greenPen);
            EndPaint(hwndDlg,&ps);
        }
        return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
            case BTN_INSERTAR:
                char Cadx[20];
                char Cady[20];
                double Numx,Numy;
                GetDlgItemText(hwndDlg,EDITX,Cadx,10);//capturar evento
                GetDlgItemText(hwndDlg,EDITY,Cady,10);
                Numx=atof(Cadx);
                Numy=atof(Cady);
                A.insertar(Numx,Numy);

                SetDlgItemText(hwndDlg,EDITX,"");
                SetDlgItemText(hwndDlg,EDITY,"");
                SetDlgItemText(hwndDlg,EDIT1,"INCERTADO");
                InvalidateRect(hwndDlg,NULL,true);
            return TRUE;
            case BTN_EJECUTAR:          ///punto de consulta
                char Cadex[20];
                char Cadey[20];
                double Numex,Numey;

                GetDlgItemText(hwndDlg,EDIT2,Cadex,10);//capturar evento
                GetDlgItemText(hwndDlg,EDIT3,Cadey,10);

                Numex=atof(Cadex);
                Numey=atof(Cadey);

                A.inconsulta(Numex,Numey);

                InvalidateRect(hwndDlg,NULL,true);
            return TRUE;
            case BTN_PIVOT:          ///punto de consulta

                A.DESCARTAR();
                INTERCIONES++;XPIVOT=0,YPIVOT=0;RADIO_PIVOT=0;
                A.SELECCION_PIVOT();

                InvalidateRect(hwndDlg,NULL,true);
            return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
