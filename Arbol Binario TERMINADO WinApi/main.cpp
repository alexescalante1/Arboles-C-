#include <iostream>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"

using namespace std;

HINSTANCE hInst;
int valor=0,valor2=-1;
HBRUSH brush;
HPEN greenPen;
COLORREF actual;
class nodo
{
    public:
    int dato;
    nodo *izq,*der,*PaDr;
    nodo(int D=0,nodo *Padr=NULL,nodo *iz=NULL,nodo *de=NULL){
         dato=D;
         izq=iz;
         der=de;
         PaDr=Padr;
    }
};

class arbol{
    public:
    nodo *raiz,*padr;
    arbol(){
        raiz=NULL;
        padr=NULL;
    }
    void insertar(int D){
         ins(D,raiz,padr);
    }
    void ins(int D,nodo *&R,nodo *padr){
         if(R==NULL)
            R=new nodo(D,padr);
         else if(D<R->dato)
            ins(D,R->izq,R);
         else
            ins(D,R->der,R);
    }
    void destruir(nodo *Ds){
         Ds->izq=NULL;
         Ds->der=NULL;
         delete Ds;
    }
    nodo *Min(nodo *m){
         if(m==NULL){
            return NULL;
         }
         if(m->izq){
            return Min(m->izq);
         }
         else{
            return m;
         }
    }
    void modificar(nodo *Re,nodo *NewN){
         if(Re->PaDr){
            if(Re->dato == Re->PaDr->izq->dato){
                Re->PaDr->izq = NewN;
            }
            else if(Re->dato == Re->PaDr->der->dato){
                Re->PaDr->der = NewN;
            }
         }
         if(NewN){
            NewN->PaDr = Re->PaDr;
         }
    }
    void eliminarN(nodo *elim){
         if(elim->izq&&elim->der){
            nodo *menor=Min(elim->der);
            elim->dato=menor->dato;
            eliminarN(menor);
         }
         else if(elim->izq){
            modificar(elim,elim->izq);
            destruir(elim);
         }
         else if(elim->der){
            modificar(elim,elim->der);
            destruir(elim);
         }
         else{
            modificar(elim,NULL);
            destruir(elim);
         }
    }
    void elimnarNodo(int n){
         eliminar(n,raiz);
    }
    void eliminar(int n,nodo *E){
         if(E==NULL){
            return;
         }
         else if(n<E->dato){
            eliminar(n,E->izq);
         }
         else if(n>E->dato){
            eliminar(n,E->der);
         }
         else{
            eliminarN(E);
         }
    }
    void Mostrar(HDC hdc,int x,int y,int xa,int ya,int a){
         Mos(raiz,hdc,x,y,xa,ya,a);
    }
    void Mos(nodo *R,HDC hdc,int x,int y,int xa,int ya,int a){
         SetBkMode(hdc, TRANSPARENT);
         actual = SetTextColor(hdc, RGB(0,0,0));
         char *cadd="Arbol Binario";
         char *cadd1="Incertar";
         char *cadd2="Buscar";
         char *cadd3="Eliminar";

         RECT rect={850,10,500,60};
         RECT rect1={10,5,80,40};
         RECT rect2={10,40,80,100};
         RECT rect3={10,40,80,200};
         const char *str=cadd;
         const char *qw=cadd1;
         const char *qe=cadd2;
         const char *qr=cadd3;

         DrawText(hdc, str, strlen(str), &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc, qw, strlen(qw)  , &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc, qe, strlen(qe)  , &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc, qr, strlen(qr)  , &rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);

         if(R!=NULL){
            char cad[40];
            itoa(R->dato,cad,10);
            int cont=0;
            for(;*(cad+cont)!=NULL;cont++);

            Mos(R->izq,hdc,x-(a+7)+3/2,y+60,x,y,a/2);
            Mos(R->der,hdc,x+(a+7)+3/2,y+60,x,y,a/2);

            MoveToEx(hdc,xa,ya,NULL);

            LineTo(hdc,x,y);
            if(valor2==R->dato){
                brush=CreateSolidBrush(RGB(102, 255, 102));
                SelectObject(hdc,brush);
                Rectangle(hdc,x+40,y+40,x-20,y-20);
            }else{
                brush=CreateSolidBrush(RGB(255, 204, 0));
                SelectObject(hdc,brush);
                Rectangle(hdc,x+40,y+20,x-20,y-5);
            }
            DeleteObject(brush);

            TextOut(hdc,x,y,cad,cont);
            SetTextColor(hdc, actual);
         }
    }
    void mostrar(){
         mos(raiz,0);
    }
    void mos(nodo *R,int C){
         if(R!=NULL){
            mos(R->der,C+1);
            for(int i=0;i<C;i++){
                cout<<"   ";
            }
            cout<<R->dato<<"\n\n";
            mos(R->izq,C+1);
         }
    }
    int buscar(int dat){
         bus(raiz,dat);
    }
    int bus(nodo *R,int val){
         if(R!=NULL){
            if(R->dato==val){
                valor=1;
                valor2=R->dato;
                return 1;
            }
            if(R->dato>val){
                bus(R->izq,val);
            }else{
                bus(R->der,val);
            }
         }
    }
};
arbol A;

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch(uMsg)
    {
    case WM_PAINT:
        {   HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);
            greenPen=CreatePen(PS_SOLID, 2, RGB(51, 204, 255));
            SelectObject(hdc,greenPen);
            Rectangle(hdc,5,5,250,200);
            Rectangle(hdc,850,10,500,60);
            DeleteObject(greenPen);

            A.Mostrar(hdc,665,100,700,100,280);
            EndPaint(hwndDlg,&ps);
        }
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
                    cout<<"\n";
                    A.mostrar();
                    cout<<"\n";
                    InvalidateRect(hwndDlg,NULL,true);
                    return TRUE;
                case BTN_INSERTAR:
                    char Cad[20];
                    int Num;
                    GetDlgItemText(hwndDlg,EDIT1,Cad,10);//capturar evento
                    Num=atoi(Cad);
                    A.buscar(Num);
                    if(valor!=1){
                        A.insertar(Num);
                        SetDlgItemText(hwndDlg,EDIT4,"INCERTADO");
                        cout<<" Incertado: "<<Num<<endl;
                    }else{
                        cout<<" No se Puede Incertar"<<endl;
                        SetDlgItemText(hwndDlg,EDIT4,"YA EXISTE");
                    }
                    valor=0;valor2=-1;
                    SetDlgItemText(hwndDlg,EDIT1,"");
                    InvalidateRect(hwndDlg,NULL,true);
                    return TRUE;
                case BTN_BUSCAR:
                    char Cad1[20];
                    int Num1;
                    GetDlgItemText(hwndDlg,EDIT2,Cad1,10);//capturar evento
                    Num1=atoi(Cad1);
                    cout<<" Buscando: "<<Num1<<endl;
                    A.buscar(Num1);
                    if(valor==1){
                        SetDlgItemText(hwndDlg,EDIT4,"ENCONTRADO");
                    }else{
                        SetDlgItemText(hwndDlg,EDIT4,"NO ENCONTRADO");
                        valor2=-1;
                    }valor=0;
                    SetDlgItemText(hwndDlg,EDIT2,"");
                    InvalidateRect(hwndDlg,NULL,true);
                    return TRUE;
                case BTN_ELIMINAR:
                    char *Y;Y = (char *) malloc(30);
                    GetDlgItemText(hwndDlg,EDIT3,Y,30);
                    int nume=0,i,NuD=1;
                    for(i=0;*(Y+i)!=NULL;i++);
                    for(int Mp=i,j=0;Mp>0;Mp--,j++){
                        for(int O=1;O<Mp;O++){
                            NuD=NuD*10;
                        }
                        nume+=(int)((int(*(Y+j))-48)*NuD);
                        NuD=1;
                    }
                    A.elimnarNodo(nume);
                    SetDlgItemText(hwndDlg,EDIT4,"ELIMINADO");
                    cout<<" Eliminado: "<<nume<<endl;
                    SetDlgItemText(hwndDlg,EDIT3,"");
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
