#include <iostream>
#include <windowsx.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include <math.h>
#define PI 3.1415926535

using namespace std;
HINSTANCE hInst;
HPEN pluma;
HBRUSH brush;
HPEN greenPen;
COLORREF actual;

int valor=0,valor2=-1,valor3=-1,valor4=-1,ACTMOUSE=0;

int ELEVACION=40;
int GIRO=80;

typedef struct stDatos {
   int ValorH;
   int ValorV;
}DATOS;
class Punto
{   public:
    int x,y;
    Punto(int xx=0,int yy=0)
    {   x=xx;
        y=yy;
    }
};
class Punto3D
{   public:
    int x,y,z;
    Punto3D(int xx=0,int yy=0,int zz=0)
    {  Cambiar(xx,yy,zz);
    }
    void Cambiar(int xx=0,int yy=0,int zz=0)
    {   x=xx;
        y=yy;
        z=zz;
    }
    Punto a2d(float Giro,float Elev)
    {   Punto P;
        P.x=x*cos(Giro)-z*sin(Giro);
        P.y=-x*sin(Giro)*sin(Elev)+y*cos(Elev)-z*cos(Giro)*sin(Elev);
        return P;
    }
};

///KDTREE 3D
class NODOS{
    public:
    int x,y,z,DR;
    NODOS *izq,*der,*PaDr;
    NODOS(NODOS *Padr=NULL,int Cx=0,int Cy=0,int Cz=0,int DR=1,NODOS *izq=NULL,NODOS *der=NULL){
        this-> PaDr = Padr;
        this-> izq  = izq;
        this-> der  = der;
        this-> x    = Cx;
        this-> y    = Cy;
        this-> z    = Cz;
        this-> DR   = DR;
    }
};
class KDTREE3D{
    public:
    NODOS *raiz,*padr;
    Punto3D P[15];
    double Elev,Giro;
    QUADTREE(){
        raiz=NULL;
        padr=NULL;
    }
    void insertar(int x,int y,int z){
         ins(raiz,padr,x,y,z,1);
    }
    void ins(NODOS *&R,NODOS *padr,int x,int y,int z,int DR){
         if(R!=NULL){
            if(R->DR==1){
                DR=2;
            }
            if(R->DR==2){
                DR=3;
            }
            if(R->DR==3){
                DR=1;
            }
         }
         if(R==NULL){
            R=new NODOS(padr,x,y,z,DR);
         }
         if(R->DR==1){
            if(x<R->x){
                ins(R->izq,R,x,y,z,DR);
            }
            if(x>R->x){
                ins(R->der,R,x,y,z,DR);
            }
         }
         if(R->DR==2){
            if(y<R->y){
                ins(R->izq,R,x,y,z,DR);
            }
            if(y>R->y){
                ins(R->der,R,x,y,z,DR);
            }
         }
         if(R->DR==3){
            if(z<R->z){
                ins(R->izq,R,x,y,z,DR);
            }
            if(z>R->z){
                ins(R->der,R,x,y,z,DR);
            }
         }
    }
    void buscar(int datx,int daty,int datz){
         busc(raiz,datx,daty,datz);
    }
    void busc(NODOS *R,int datx,int daty,int datz){
         if(R!=NULL){
            if(R->x==datx&&R->y==daty&&R->z==datz){
                valor=1;
                valor2=R->x;
                valor3=R->y;
                valor4=R->z;
            }
            busc(R->izq,datx,daty,datz);
            busc(R->der,datx,daty,datz);
         }
    }
    void Mostrar(HDC hdc,int x,int y,int xa,int ya,int a){
         Mos(raiz,hdc,x,y,xa,ya,a);
    }
    void Mos(NODOS *R,HDC hdc,int x,int y,int xa,int ya,int a){

         if(R!=NULL){
            int cx=0,cy=0,cz=0;
            char cadx[10],cady[10],cadz[10],CADENA[2];
            itoa(R->x,cadx,10);
            itoa(R->y,cady,10);
            itoa(R->z,cadz,10);
            for(;*(cadx+cx)!=NULL;cx++);
            for(;*(cady+cy)!=NULL;cy++);
            for(;*(cadz+cz)!=NULL;cz++);
            if(R->DR==1){*(CADENA+0)='X';}
            if(R->DR==2){*(CADENA+0)='Y';}
            if(R->DR==3){*(CADENA+0)='Z';}
            *(CADENA+1)='\0';
            Mos(R->izq,hdc,x-a+4/2,y+60,x,y,a/2);
            Mos(R->der,hdc,x+a+4/2,y+60,x,y,a/2);


            MoveToEx(hdc,xa,ya,NULL);
            LineTo(hdc,x,y);

            if(valor2==R->x&&valor3==R->y&&valor4==R->z){
                brush=CreateSolidBrush(RGB(51, 204, 255));
                SelectObject(hdc,brush);
            }else{
                brush=CreateSolidBrush(RGB(255, 255, 153));
                SelectObject(hdc,brush);
            }

            Rectangle(hdc,x+25,y+60,x-40,y-5);


            TextOut(hdc,x-35,y,CADENA,2);
            TextOut(hdc,x-15,y,cadx,cx);
            TextOut(hdc,x-15,y+20,cady,cy);
            TextOut(hdc,x-15,y+40,cadz,cz);

            DeleteObject(brush);
         }
    }
    void ingrePuntos(){
        P[0].Cambiar(-100,-100,-100);
        P[1].Cambiar(100,-100,-100);
        P[2].Cambiar(100,100,-100);
        P[3].Cambiar(-100,100,-100);

        P[4].Cambiar(-100,-100,100);
        P[5].Cambiar(100,-100,100);
        P[6].Cambiar(100,100,100);
        P[7].Cambiar(-100,100,100);

        P[10].Cambiar(-120,-120,-120);
        P[11].Cambiar(-100,-120,-120);
        P[12].Cambiar(-120,-100,-120);
        P[13].Cambiar(-120,-120,-100);
    }
    void Dibujar(HDC hdc,Punto3D P1, Punto3D P2){
        Punto Q1=P1.a2d(Giro,Elev);
        Punto Q2=P2.a2d(Giro,Elev);
        MoveToEx(hdc,700+Q1.x,180+Q1.y,NULL);
        LineTo(hdc,700+Q2.x,180+Q2.y);
    }
    void MostrarCUB(HDC hdc,double E,double G){

        Elev=E;
        Giro=G;
        pluma = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        SelectObject(hdc, pluma);
        Dibujar(hdc,P[10],P[11]);
        DeleteObject(pluma);
        pluma = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
        SelectObject(hdc, pluma);
        Dibujar(hdc,P[10],P[12]);
        DeleteObject(pluma);
        pluma = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
        SelectObject(hdc, pluma);
        Dibujar(hdc,P[10],P[13]);
        DeleteObject(pluma);


        pluma = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
        SelectObject(hdc, pluma);

        Dibujar(hdc,P[0],P[1]);
        Dibujar(hdc,P[1],P[2]);
        Dibujar(hdc,P[2],P[3]);
        Dibujar(hdc,P[3],P[0]);

        Dibujar(hdc,P[4],P[5]);
        Dibujar(hdc,P[5],P[6]);
        Dibujar(hdc,P[6],P[7]);
        Dibujar(hdc,P[7],P[4]);

        Dibujar(hdc,P[0],P[4]);
        Dibujar(hdc,P[1],P[5]);
        Dibujar(hdc,P[2],P[6]);
        Dibujar(hdc,P[3],P[7]);

        DeleteObject(pluma);
        Moscub(raiz,hdc,-100,100,-100,100,-100,100);
    }
    void Moscub(NODOS *R,HDC hdc,int xi,int xf,int yi,int yf,int zi,int zf){
         if(R!=NULL){

            if(R->DR==1){

                Moscub(R->izq,hdc,xi,R->x-100,yi,yf,zi,zf);
                Moscub(R->der,hdc,R->x-100,xf,yi,yf,zi,zf);
                Punto3D P1(R->x-100,yi,zf);
                Punto3D P2(R->x-100,yf,zf);
                Punto3D P3(R->x-100,yf,zi);
                Punto3D P4(R->x-100,yi,zi);

                pluma = CreatePen(PS_SOLID, 1, RGB(255, 51, 0));
                SelectObject(hdc, pluma);

                Dibujar(hdc,P1,P2);
                Dibujar(hdc,P2,P3);
                Dibujar(hdc,P3,P4);
                Dibujar(hdc,P1,P4);

                DeleteObject(pluma);
            }
            if(R->DR==2){

                Moscub(R->izq,hdc,xi,xf,yi,R->y-100,zi,zf);
                Moscub(R->der,hdc,xi,xf,R->y-100,yf,zi,zf);
                Punto3D P1(xi,R->y-100,zf);
                Punto3D P2(xf,R->y-100,zf);
                Punto3D P3(xf,R->y-100,zi);
                Punto3D P4(xi,R->y-100,zi);
                pluma = CreatePen(PS_SOLID, 1, RGB(0, 204, 0));
                SelectObject(hdc, pluma);

                Dibujar(hdc,P1,P2);
                Dibujar(hdc,P2,P3);
                Dibujar(hdc,P3,P4);
                Dibujar(hdc,P1,P4);

                DeleteObject(pluma);
            }
            if(R->DR==3){

                Moscub(R->izq,hdc,xi,xf,yi,yf,zi,R->z-100);
                Moscub(R->der,hdc,xi,xf,yi,yf,R->z-100,zf);
                Punto3D P1(xi,yf,R->z-100);
                Punto3D P2(xf,yf,R->z-100);
                Punto3D P3(xf,yi,R->z-100);
                Punto3D P4(xi,yi,R->z-100);
                pluma = CreatePen(PS_SOLID, 1, RGB(51, 204, 255));
                SelectObject(hdc, pluma);

                Dibujar(hdc,P1,P2);
                Dibujar(hdc,P2,P3);
                Dibujar(hdc,P3,P4);
                Dibujar(hdc,P1,P4);

                DeleteObject(pluma);

            }
         }
    }
};
void inserORACION(HDC hdc){
    actual = SetTextColor(hdc, RGB(0,0,0));
    const char *cadena[10];
    cadena[0]="KDTREE 3D NO TERMINADO";
    cadena[1]="Incertar";
    cadena[2]="Buscar";
    cadena[3]="Eliminar";
    cadena[4]="X =";
    cadena[5]="Y =";
    cadena[6]="0,0";
    cadena[7]="ELEVACION:";
    cadena[8]="GIRO:";
    cadena[9]="CONTROL MOUSE:";
    RECT rect[10];
    rect[0]={1100,10,1340,50};
    rect[1]={10,5,80,40};
    rect[2]={10,40,80,100};
    rect[3]={10,40,80,200};
    rect[4]={1140,-40,1130,200};
    rect[5]={1200,-40,1230,200};
    rect[6]={-55,-170,1230,200};
    rect[7]={40,390,80,40};
    rect[8]={0,460,80,40};
    rect[9]={80,315,80,40};

    DrawText(hdc,cadena[0],strlen(cadena[0]) , &rect[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
    DrawText(hdc,cadena[1],strlen(cadena[1]) , &rect[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
    DrawText(hdc,cadena[2],strlen(cadena[2]) , &rect[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
    DrawText(hdc,cadena[3],strlen(cadena[3]) , &rect[3], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
    DrawText(hdc,cadena[4],strlen(cadena[4]) , &rect[4], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
    DrawText(hdc,cadena[5],strlen(cadena[5]) , &rect[5], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
    DrawText(hdc,cadena[7],strlen(cadena[7]) , &rect[7], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
    DrawText(hdc,cadena[8],strlen(cadena[8]) , &rect[8], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
    DrawText(hdc,cadena[9],strlen(cadena[9]) , &rect[9], DT_CENTER | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
    SetTextColor(hdc, actual);
}

KDTREE3D a;

void INCER(){
    a.insertar(110,110,110);
    a.insertar(150,110,110);
    a.insertar(190,150,130);
    a.insertar(190,100,15);
    a.insertar(80,80,80);
    a.insertar(40,40,120);
    a.insertar(20,20,30);
    a.insertar(10,10,10);
    a.insertar(5,5,30);
    a.insertar(30,30,30);
}
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hWndScrollerH,hWndScrollerV;
    SCROLLINFO siH,siV;
    int CurPosH(0),CurPosV(0);
    hWndScrollerH = GetDlgItem(hwndDlg, SCROLLERH);
    hWndScrollerV = GetDlgItem(hwndDlg, SCROLLERV);

    ///PUNTOS INCERTADOS



       INCER();



    ///PUNTOS INCERTADOS

    switch(uMsg)
    {
    case WM_MOUSEMOVE:{
        long xpos=GET_X_LPARAM(lParam);
        long ypos=GET_Y_LPARAM(lParam);
        char cade[20];
        ltoa(xpos-320,cade,10);
        SetDlgItemText(hwndDlg,EDITX,cade);
        ltoa(ypos-10,cade,10);
        SetDlgItemText(hwndDlg,EDITY,cade);
        if(ACTMOUSE==1){
            if(xpos>=320&&ypos>=10&&xpos<1090&&ypos<355){
                GIRO=xpos-320;
                ELEVACION=ypos-10;
                InvalidateRect(hwndDlg,NULL,TRUE);
            }
        }
    }
    return TRUE;
    case WM_PAINT:
        {
            HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);
            SetBkMode(hdc, TRANSPARENT);

            greenPen=CreatePen(PS_SOLID, 1, RGB(0, 102, 255));
            SelectObject(hdc,greenPen);

            Rectangle(hdc,10,380,1715,900);
            Rectangle(hdc,1100,60,1340,100);
            Rectangle(hdc,10,10,290,375);
            Rectangle(hdc,1100,10,1340,50);

            DeleteObject(greenPen);

            brush=CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc,brush);
            Rectangle(hdc,320,10,1090,355);
            DeleteObject(brush);

            inserORACION(hdc);
            a.ingrePuntos();
            a.MostrarCUB(hdc,ELEVACION*((PI)/360),GIRO*((PI)/360));
            a.Mostrar(hdc,850,400,850,400,300);
            SetDlgItemText(hwndDlg,EDIT3,"NO DISPONIBLE");

            EndPaint(hwndDlg,&ps);
        }
        return TRUE;
        case WM_LBUTTONDOWN:{
            /*
            HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);
            long xpos=GET_X_LPARAM(lParam);
            long ypos=GET_Y_LPARAM(lParam);

            if(xpos>600&&ypos>10&&xpos<1080&&ypos<250){
               a.insertar(xpos-600,ypos-10);
            }
            InvalidateRect(hwndDlg,NULL,true);
            */
            }
            return TRUE;
        case WM_INITDIALOG:
        {
            CurPosH=0;
            ZeroMemory(&siH, sizeof(siH));
            siH.cbSize = sizeof(siH);
            siH.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
            siH.nMin   = 0;
            siH.nMax   = 1000;
            siH.nPage  = 1;
            siH.nPos   = GIRO;
            SetScrollInfo(hWndScrollerH, SB_CTL, &siH, TRUE);

            CurPosV=0;
            ZeroMemory(&siV, sizeof(siV));
            siV.cbSize = sizeof(siV);
            siV.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
            siV.nMin   = 0;
            siV.nMax   = 1000;
            siV.nPage  = 1;
            siV.nPos   = ELEVACION;
            SetScrollInfo(hWndScrollerV, SB_CTL, &siV, TRUE);
        }
        return TRUE;

        case WM_HSCROLL:
        {
            CurPosH=GetScrollPos(hWndScrollerH,SB_CTL);
            switch(LOWORD(wParam))
            {
                case SB_RIGHT:
                    CurPosH=50;
                    break;

                case SB_LINERIGHT:
                    if(CurPosH<siH.nMax)
                        CurPosH+=1;
                    break;

                case SB_THUMBPOSITION:
                    CurPosH=HIWORD(wParam);
                    break;

                case SB_THUMBTRACK:
                    CurPosH=HIWORD(wParam);
                    break;

                case SB_LINELEFT:
                    if(CurPosH>1)
                        CurPosH-=1;
                    break;

                case SB_LEFT:
                    CurPosH=1;
                    break;

                case SB_ENDSCROLL:
                    break;
            }
            SetScrollPos(hWndScrollerH,SB_CTL,CurPosH,TRUE);
            GIRO=CurPosH;
            char cadeGIRO[20];
            ltoa(GIRO,cadeGIRO,10);
            SetDlgItemText(hwndDlg,ID_EDITV,cadeGIRO);
            InvalidateRect(hwndDlg,NULL,TRUE);
        }
        return TRUE;

        case WM_VSCROLL:
        {
            CurPosV=GetScrollPos(hWndScrollerV,SB_CTL);

            switch(LOWORD(wParam))
            {
                case SB_TOP:
                    CurPosV=1;
                    break;

                case SB_LINEUP:
                    if(CurPosV>1)
                        CurPosV-=1;
                    break;

                case SB_THUMBPOSITION:
                    CurPosV=HIWORD(wParam);
                    break;

                case SB_THUMBTRACK:
                    CurPosV=HIWORD(wParam);
                    break;

                case SB_LINEDOWN:
                    if(CurPosV<siV.nMax)
                        CurPosV+=1;
                    break;

                case SB_BOTTOM:
                    CurPosV=siV.nMax;
                    break;

                case SB_ENDSCROLL:
                    break;
            }
            SetScrollPos(hWndScrollerV,SB_CTL,CurPosV,TRUE);
            ELEVACION=CurPosV;
            char cadeELEVACION[20];
            ltoa(ELEVACION,cadeELEVACION,10);
            SetDlgItemText(hwndDlg,ID_EDITH,cadeELEVACION);
            InvalidateRect(hwndDlg,NULL,TRUE);
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
                    if(ACTMOUSE==0){
                        SetDlgItemText(hwndDlg,MOUSEACT,"SI");
                        ACTMOUSE=1;
                        InvalidateRect(hwndDlg,NULL,true);
                    }else{
                        SetDlgItemText(hwndDlg,MOUSEACT,"NO");
                        ACTMOUSE=0;
                        InvalidateRect(hwndDlg,NULL,true);
                    }
                    return TRUE;
                case BTN_INSERTAR:
                    char Cadx[20],Cady[20],Cadz[20];
                    int Numx,Numy,Numz;
                    GetDlgItemText(hwndDlg,EDIT1,Cadx,10);//capturar evento
                    GetDlgItemText(hwndDlg,EDIT5,Cady,10);//capturar evento
                    GetDlgItemText(hwndDlg,EDIT6,Cadz,10);//capturar evento
                    Numx=atoi(Cadx);
                    Numy=atoi(Cady);
                    Numz=atoi(Cadz);
                    if(Numx<200&&Numy<200&&Numz<200&&Numx>0&&Numy>0&&Numz>0){
                        a.insertar(Numx,Numy,Numz);
                        if(valor!=1){
                            SetDlgItemText(hwndDlg,EDIT4,"INCERTADO");
                        }else{
                            cout<<" No se Puede Incertar"<<endl;
                            SetDlgItemText(hwndDlg,EDIT4,"YA EXISTE");
                        }
                    }else{
                       SetDlgItemText(hwndDlg,EDIT4,"FUERA DE RANGO");
                    }
                    valor=0;valor2=-1;
                    SetDlgItemText(hwndDlg,EDIT1,"");
                    SetDlgItemText(hwndDlg,EDIT5,"");
                    SetDlgItemText(hwndDlg,EDIT6,"");
                    InvalidateRect(hwndDlg,NULL,true);
                    return TRUE;
                case BTN_BUSCAR:
                    char Cadex[20];
                    char Cadey[20];
                    char Cadez[20];
                    int Numex,Numey,Numez;
                    GetDlgItemText(hwndDlg,EDIT2,Cadex,10);//capturar evento
                    GetDlgItemText(hwndDlg,EDIT7,Cadey,10);//capturar evento
                    GetDlgItemText(hwndDlg,EDIT8,Cadez,10);//capturar evento
                    Numex=atoi(Cadex);
                    Numey=atoi(Cadey);
                    Numez=atoi(Cadez);
                    cout<<" Buscando: "<<Numex<<","<<Numy<<endl;
                    a.buscar(Numex,Numey,Numez);
                    if(valor==1){
                        SetDlgItemText(hwndDlg,EDIT4,"ENCONTRADO");
                    }else{
                        SetDlgItemText(hwndDlg,EDIT4,"NO ENCONTRADO");
                        valor2=-1;
                    }valor=0;
                    SetDlgItemText(hwndDlg,EDIT2,"");
                    SetDlgItemText(hwndDlg,EDIT7,"");
                    SetDlgItemText(hwndDlg,EDIT8,"");

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
