#include <iostream>
#include <windowsx.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"

using namespace std;

HINSTANCE hInst;
int valor=0,valor2=-1,valor3=-1;
HBRUSH brush;
HPEN greenPen;
COLORREF actual;
///QUADTREE PUNTOS
class NODOS{
    public:
    long x,y;
    NODOS *Prc1,*Prc2,*Prc3,*Prc4,*PaDr;
    NODOS(NODOS *Padr=NULL,int Cx=0,int Cy=0,NODOS *C1=NULL,NODOS *C2=NULL,NODOS *C3=NULL,NODOS *C4=NULL){
         this-> Prc1 = C1;
         this-> Prc2 = C2;
         this-> Prc3 = C3;
         this-> Prc4 = C4;
         this-> PaDr = Padr;
         this-> x    = Cx;
         this-> y    = Cy;
    }
};
class QUADTREE{
    public:
    NODOS *raiz,*padr;
    QUADTREE(){
        raiz=NULL;
        padr=NULL;
    }
    void insertar(int x,int y){
         ins(raiz,padr,x,y);
    }
    void ins(NODOS *&R,NODOS *padr,int x,int y){
         cout<<x<<","<<y<<endl;
         if(R==NULL){
            R=new NODOS(padr,x,y);
            cout<<"intro d:"<<x<<","<<y<<endl;
         }
         if(x<R->x&&y>R->y){
            ins(R->Prc1,R,x,y);
            cout<<"cuadrante1\n";
         }
         if(x>R->x&&y>R->y){
            ins(R->Prc2,R,x,y);
            cout<<"cuadrante2\n";
         }
         if(x<R->x&&y<R->y){
            ins(R->Prc3,R,x,y);
            cout<<"cuadrante3\n";
         }
         if(x>R->x&&y<R->y){
            ins(R->Prc4,R,x,y);
            cout<<"cuadrante4\n";
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
         SetBkMode(hdc, TRANSPARENT);
         actual = SetTextColor(hdc, RGB(0,0,0));
         const char *cadena[7];
         cadena[0]="QUADTREE PUNTO";
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
         ///color
         /*
         HPEN greenPen=CreatePen(PS_SOLID, 1, RGB(51, 204, 255 ));
         SelectObject(hdc,greenPen);
         */

         DrawText(hdc,cadena[0],strlen(cadena[0]) , &rect[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[1],strlen(cadena[1]) , &rect[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[2],strlen(cadena[2]) , &rect[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[3],strlen(cadena[3]) , &rect[3], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[4],strlen(cadena[4]) , &rect[4], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[5],strlen(cadena[5]) , &rect[5], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         DrawText(hdc,cadena[6],strlen(cadena[6]) , &rect[6], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
         if(R!=NULL){
            int cx=0,cy=0;
            char cadx[10],cady[10],*CADENA;
            itoa(R->x,cadx,10);
            itoa(R->y,cady,10);
            for(;*(cadx+cx)!=NULL;cx++);
            for(;*(cady+cy)!=NULL;cy++);
            CADENA=new char[cx+cy+2];
            for(int i=0;i<cx+cy+2;i++){
                if(i<cx){
                   *(CADENA+i)=*(cadx+i);
                }
                if(i==cx){
                   *(CADENA+i)=',';
                }
                if(i>cx){
                   *(CADENA+i)=*(cady+i-cx-1);
                }
            }
            *(CADENA+cx+cy+1)='\0';
            Mos(R->Prc1,hdc,x-a+3/2,y+60,x,y,a/2);
            Mos(R->Prc2,hdc,x-a/2,y+60,x,y,a/2);
            Mos(R->Prc3,hdc,x+a/2,y+60,x,y,a/2);
            Mos(R->Prc4,hdc,x+a+3/2,y+60,x,y,a/2);

            MoveToEx(hdc,xa+20,ya,NULL);
            LineTo(hdc,x,y);

            if(valor2==R->x&&valor3==R->y){

                brush=CreateSolidBrush(RGB(255, 255, 153));
                SelectObject(hdc,brush);
                Rectangle(hdc,x+60,y+40,x-10,y-20);
            }else{

                brush=CreateSolidBrush(RGB(51, 153, 255));
                SelectObject(hdc,brush);
                Rectangle(hdc,x+60,y+20,x-10,y-5);
            }
            DeleteObject(brush);
            SetTextColor(hdc, actual);
            actual = SetTextColor(hdc, RGB(255,255,255));
            TextOut(hdc,x,y,CADENA,cx+cy+1);
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
            MoveToEx(hdc,xi,R->y+10,NULL);
            LineTo(hdc,xf,R->y+10);
            MoveToEx(hdc,R->x+600,yi,NULL);
            LineTo(hdc,R->x+600,yf);
         }
    }
};

QUADTREE a;

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
            greenPen=CreatePen(PS_SOLID, 1, RGB(51, 204, 255));
            SelectObject(hdc,greenPen);

            Rectangle(hdc,10,260,1715,900);
            Rectangle(hdc,1100,60,1340,100);
            Rectangle(hdc,10,10,240,200);
            Rectangle(hdc,1100,10,1340,50);
            DeleteObject(greenPen);

            brush=CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc,brush);

            Rectangle(hdc,600,10,1080,250);
            DeleteObject(brush);
/*
            a.insertar(3,4);
            a.insertar(5,6);
            a.insertar(1,2);
            a.insertar(7,2);
            a.insertar(5,1);
            a.insertar(6,5);
            a.insertar(2,5);
*/
            a.Mostrar(hdc,820,300,850,300,500);
            a.MostrarCuadro(hdc,600,1080,10,250);
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
            if(xpos>600&&ypos>10&&xpos<1080&&ypos<250){
               a.insertar(xpos-600,ypos-10);
               InvalidateRect(hwndDlg,NULL,true);
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
                        cout<<" No se Puede Incertar"<<endl;
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
                    InvalidateRect(hwndDlg,NULL,true);
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
