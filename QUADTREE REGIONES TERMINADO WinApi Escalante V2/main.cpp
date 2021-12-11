#include <iostream>
#include <windowsx.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
using namespace std;

HINSTANCE hInst;
int valor=0,valor2=-1,valor3=-1,Ins=0;
HBRUSH brush;
HPEN greenPen;
COLORREF actual;
///QUADTREE PUNTOS
class NODOS{
    public:
    long x,y;int P,visible;
    NODOS *Prc1,*Prc2,*Prc3,*Prc4,*PaDr;
    NODOS(NODOS *Padr=NULL,int V=0,int Cx=0,int Cy=0,int P=0,NODOS *C1=NULL,NODOS *C2=NULL,NODOS *C3=NULL,NODOS *C4=NULL){
         this-> Prc1 = C1;
         this-> Prc2 = C2;
         this-> Prc3 = C3;
         this-> Prc4 = C4;
         this-> PaDr = Padr;
         this-> x    = Cx;
         this-> y    = Cy;
         this-> P    = P;
         this-> visible=V;
    }
    void IncertColor(int P){
         this-> P = P;
    }
    int RetColor(){
         return P;
    }
};
class QUADTREE{
    public:
    NODOS *raiz,*padr;
    QUADTREE(){
        raiz=NULL;
        padr=NULL;
    }
    void insertar(int x,int y,bool Vi){
         ins(raiz,padr,x,y,Vi);
    }
    void ins(NODOS *&R,NODOS *padr,int x,int y,bool Vi){
         cout<<x<<","<<y<<endl;
         if(R==NULL){
            R=new NODOS(padr,Vi,x,y);
            cout<<"intro d:"<<x<<","<<y<<endl;
         }
         if(x<R->x&&y>R->y){
            ins(R->Prc1,R,x,y,Vi);
            cout<<"cuadrante1\n";
         }
         if(x>R->x&&y>R->y){
            ins(R->Prc2,R,x,y,Vi);
            cout<<"cuadrante2\n";
         }
         if(x<R->x&&y<R->y){
            ins(R->Prc3,R,x,y,Vi);
            cout<<"cuadrante3\n";
         }
         if(x>R->x&&y<R->y){
            ins(R->Prc4,R,x,y,Vi);
            cout<<"cuadrante4\n";
         }
    }
    void Pintar(int x,int y){
         pints(raiz,x,y);
    }
    void pints(NODOS *&R,int x,int y){
         if(R!=NULL){
            if(R->Prc1==NULL){
                if(R->RetColor()==1){
                    R->IncertColor(0);
                }else{
                    R->IncertColor(1);
                }
            }
            if(x<R->x&&y>R->y){
                pints(R->Prc1,x,y);
            }
            if(x>R->x&&y>R->y){
                pints(R->Prc2,x,y);
            }
            if(x<R->x&&y<R->y){
                pints(R->Prc3,x,y);
            }
            if(x>R->x&&y<R->y){
                pints(R->Prc4,x,y);
            }
         }
    }
    void validarColor(){
         valcol(raiz);
         cout<<"\n";
    }
    int valcol(NODOS *&R){
         if(R!=NULL){int NMs[4],total;
            NMs[0]=valcol(R->Prc1);
            NMs[1]=valcol(R->Prc2);
            NMs[2]=valcol(R->Prc3);
            NMs[3]=valcol(R->Prc4);
            total=NMs[0]+NMs[1]+NMs[2]+NMs[3];
            cout<<total<<"<---\n";
            if(total==20){
                R->IncertColor(0);
            }
            if(total==4){
                R->IncertColor(1);
            }
            if(total<20&&total>4){
                R->IncertColor(2);

            }
            if(R->RetColor()==2){
                return 4;
            }
            if(R->RetColor()==1){
                return 1;
            }
            if(R->RetColor()==0){
                return 5;
            }
         }return 0;
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
            busc(R->Prc1,datx,daty);
            busc(R->Prc2,datx,daty);
            busc(R->Prc3,datx,daty);
            busc(R->Prc4,datx,daty);
         }
    }
    void Mostrar(HDC hdc,int x,int y,int xa,int ya,int a){
         Mos(raiz,hdc,x,y,xa,ya,a);
    }
    void Mos(NODOS *R,HDC hdc,int x,int y,int xa,int ya,int a){

         actual = SetTextColor(hdc, RGB(0,0,0));
         const char *cadena[7];
         cadena[0]="QUADTREE REGIONES";
         cadena[1]="Incertar";
         cadena[2]="Buscar";
         cadena[3]="Eliminar";
         cadena[4]="X =";
         cadena[5]="Y =";
         cadena[6]="0,0";
         RECT rect[7];
         rect[0]={1100,10,1340,50};
         rect[1]={10,5,80,40};
         rect[2]={10,40,80,100};
         rect[3]={10,40,80,200};
         rect[4]={1140,-40,1130,200};
         rect[5]={1200,-40,1230,200};
         rect[6]={-55,-170,1230,200};

         DrawText(hdc,cadena[0],strlen(cadena[0]) , &rect[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[1],strlen(cadena[1]) , &rect[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[2],strlen(cadena[2]) , &rect[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[3],strlen(cadena[3]) , &rect[3], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[4],strlen(cadena[4]) , &rect[4], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[5],strlen(cadena[5]) , &rect[5], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[6],strlen(cadena[6]) , &rect[6], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         SetTextColor(hdc, actual);
         if(R!=NULL){
            char CADENA[5];
            itoa(R->P,CADENA,5);

            Mos(R->Prc1,hdc,x-a+3/2,y+60,x,y,a/5);
            Mos(R->Prc2,hdc,x-a/2,y+60,x,y,a/5);
            Mos(R->Prc3,hdc,x+a+3/2,y+60,x,y,a/5);
            Mos(R->Prc4,hdc,x+a/2,y+60,x,y,a/5);
            actual = SetTextColor(hdc, RGB(255,255,255));
            if(R->RetColor()==1){
                brush=CreateSolidBrush(RGB(0, 51, 0));
                SelectObject(hdc,brush);
                MoveToEx(hdc,xa,ya,NULL);
                LineTo(hdc,x,y);
                Rectangle(hdc,x+18,y+40,x-10,y-20);
                TextOut(hdc,x,y,CADENA,1);
            }
            if(R->RetColor()==2){
                brush=CreateSolidBrush(RGB(0, 204, 102));
                SelectObject(hdc,brush);
                MoveToEx(hdc,xa,ya,NULL);
                LineTo(hdc,x,y);
                Rectangle(hdc,x+18,y+40,x-10,y-20);
                TextOut(hdc,x,y,CADENA,1);
            }
            if(R->RetColor()==3){
                brush=CreateSolidBrush(RGB(100, 204, 102));
                SelectObject(hdc,brush);
                MoveToEx(hdc,xa,ya,NULL);
                LineTo(hdc,x,y);
                Rectangle(hdc,x+18,y+40,x-10,y-20);
                TextOut(hdc,x,y,CADENA,1);
            }
            DeleteObject(brush);
            SetTextColor(hdc, actual);
         }
    }
    void MostrarCuadro(HDC hdc,int xi,int xf,int yi,int yf){
         MosCuad(raiz,hdc,xi,xf,yi,yf);
    }
    void MosCuad(NODOS *R,HDC hdc,int xi,int xf,int yi,int yf){
         if(R!=NULL){
            MosCuad(R->Prc1,hdc,xi,R->x+600,R->y+10,yf);
            MosCuad(R->Prc2,hdc,R->x+600,xf,R->y+10,yf);
            MosCuad(R->Prc3,hdc,xi,R->x+600,yi,R->y+10);
            MosCuad(R->Prc4,hdc,R->x+600,xf,yi,R->y+10);
            if(R->visible==1){
                MoveToEx(hdc,xi,R->y+10,NULL);
                LineTo(hdc,xf,R->y+10);
                MoveToEx(hdc,R->x+600,yi,NULL);
                LineTo(hdc,R->x+600,yf);
            }
         }
    }
    void PintarCuadro(HDC hdc,int xi,int xf,int yi,int yf){
         PintCuad(raiz,hdc,xi,xf,yi,yf);
    }
    void PintCuad(NODOS *R,HDC hdc,int xi,int xf,int yi,int yf){
         if(R!=NULL){
            PintCuad(R->Prc1,hdc,xi,R->x+600,R->y+10,yf);
            PintCuad(R->Prc2,hdc,R->x+600,xf,R->y+10,yf);
            PintCuad(R->Prc3,hdc,xi,R->x+600,yi,R->y+10);
            PintCuad(R->Prc4,hdc,R->x+600,xf,yi,R->y+10);

            if(R->P==1){
                brush=CreateSolidBrush(RGB(0, 51, 204));
                SelectObject(hdc,brush);
                MoveToEx(hdc,R->x+600,R->y+10,NULL);
                Rectangle(hdc,xi,yi,xf,yf);
                DeleteObject(brush);
            }
         }
    }
};

QUADTREE a;

void insT(){
    a.insertar(200,120,1);
    a.insertar(100,60 ,1);
    a.insertar(300,180,1);
    a.insertar(100,180,1);
    a.insertar(300,60 ,1);

    a.insertar(50,30,0);
    a.insertar(50,90,0);
    a.insertar(50,150,0);
    a.insertar(50,210,0);

    a.insertar(150,30,0);
    a.insertar(150,90,0);
    a.insertar(150,150,0);
    a.insertar(150,210,0);

    a.insertar(250,30,0);
    a.insertar(250,90,0);
    a.insertar(250,150,0);
    a.insertar(250,210,0);

    a.insertar(350,30,0);
    a.insertar(350,90,0);
    a.insertar(350,150,0);
    a.insertar(350,210,0);
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch(uMsg)
    {
    case WM_MOUSEMOVE:{
        long xpos=GET_X_LPARAM(lParam);
        long ypos=GET_Y_LPARAM(lParam);
        char cade[20];
        ltoa(xpos-600,cade,10);
        SetDlgItemText(hwndDlg,EDITX,cade);
        ltoa(ypos-10,cade,10);
        SetDlgItemText(hwndDlg,EDITY,cade);
        }
        return TRUE;
    case WM_PAINT:
        {   HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);
            SetBkMode(hdc, TRANSPARENT);
            greenPen=CreatePen(PS_SOLID, 1, RGB(51, 204, 255 ));
            SelectObject(hdc,greenPen);
            Rectangle(hdc,600,10,1000,250);
            Rectangle(hdc,10,260,1715,900);
            Rectangle(hdc,1100,60,1340,100);
            Rectangle(hdc,10,10,240,200);
            Rectangle(hdc,1100,10,1340,50);
            DeleteObject(greenPen);

            if(Ins==0){
                insT();
                Ins++;
            }

            a.validarColor();
            a.Mostrar(hdc,800,300,800,300,500);
            a.MostrarCuadro(hdc,600,1000,10,250);
            a.PintarCuadro(hdc,600,1000,10,250);
            SetDlgItemText(hwndDlg,EDIT3,"NO DISPONIBLE");
            EndPaint(hwndDlg,&ps);
        }
        return TRUE;
        case WM_LBUTTONDOWN:{
            HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);
            long xpos=GET_X_LPARAM(lParam);
            long ypos=GET_Y_LPARAM(lParam);
            if(xpos>600&&ypos>10&&xpos<1000&&ypos<250){
               a.Pintar(xpos-600,ypos-10);
               InvalidateRect(hwndDlg,NULL,true);
               //a.insertar(xpos-600,ypos-10);
            }

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
                    return TRUE;
                case BTN_INSERTAR:/*
                    char Cadx[20];
                    char Cady[20];
                    int Numx,Numy;
                    GetDlgItemText(hwndDlg,EDIT1,Cadx,10);//capturar evento
                    GetDlgItemText(hwndDlg,EDIT5,Cady,10);//capturar evento
                    Numx=atoi(Cadx);
                    Numy=atoi(Cady);
                    a.insertar(Numx,Numy,1);
                    if(valor!=1){
                        SetDlgItemText(hwndDlg,EDIT4,"INCERTADO");
                    }else{
                        cout<<" No se Puede Incertar"<<endl;
                        SetDlgItemText(hwndDlg,EDIT4,"YA EXISTE");
                    }
                    valor=0;valor2=-1;
                    SetDlgItemText(hwndDlg,EDIT1,"");
                    SetDlgItemText(hwndDlg,EDIT5,"");
                    InvalidateRect(hwndDlg,NULL,true);*/
                    return TRUE;
                case BTN_BUSCAR:/*
                    char Cadex[20];
                    char Cadey[20];
                    int Numex,Numey;
                    GetDlgItemText(hwndDlg,EDIT2,Cadex,10);//capturar evento
                    GetDlgItemText(hwndDlg,EDIT7,Cadey,10);//capturar evento
                    Numex=atoi(Cadex);
                    Numey=atoi(Cadey);
                    cout<<" Buscando: "<<Numex<<","<<Numy<<endl;
                    a.buscar(Numex,Numey);
                    if(valor==1){
                        SetDlgItemText(hwndDlg,EDIT4,"ENCONTRADO");
                    }else{
                        SetDlgItemText(hwndDlg,EDIT4,"NO ENCONTRADO");
                        valor2=-1;
                    }valor=0;
                    SetDlgItemText(hwndDlg,EDIT2,"");
                    SetDlgItemText(hwndDlg,EDIT7,"");
                    InvalidateRect(hwndDlg,NULL,true);*/
                    return TRUE;
                case BTN_ELIMINAR:

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
