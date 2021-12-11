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

void DIBUJAR_CIRCULO_R(HDC,double,double,double,int);
void DIBUJAR_PUNTOS(HDC,double,double,int);
void DIBUJAR_UNION_PUNTOS(HDC,double,double,double,double,int);
void DIBUJAR_TABLA_COORD(HDC);

double MENOR_X=0,MENOR_Y=0;
double DISTANCIA_P=999;

class NODOS{
    public:
    double x,y;
    NODOS *Prc1,*Prc2,*Prc3,*Prc4,*PaDr;
    NODOS(NODOS *Padr=NULL,double Cx=0,double Cy=0,NODOS *C1=NULL,NODOS *C2=NULL,NODOS *C3=NULL,NODOS *C4=NULL){
         this-> Prc1 = C1;
         this-> Prc2 = C2;
         this-> Prc3 = C3;
         this-> Prc4 = C4;
         this-> PaDr = Padr;
         this-> x    = Cx;
         this-> y    = Cy;
    }
};
class SADTREE{
    public:
    NODOS *raiz,*padr;
    SADTREE(){
        raiz=NULL;
        padr=NULL;
    }
    float distancia(double X, double Y, NODOS *&b)
    {
        float d;
        d = sqrt((X - b->x)*(X - b->x) + (Y - b->y)*(Y - b->y));
        return d;
    }
    void VerificDIST(double x,double y)
    {
        DISTANCIA_P=999;
        VERIF(raiz,x,y);
    }
    void VERIF(NODOS *&R,double x,double y)
    {
        if(R){
            //cout<<x<<","<<y<<" DISTANCIA: "<<distancia(x,y,R)<<" -- "<<R->x<<","<<R->y<<"\n";
            if(DISTANCIA_P>distancia(x,y,R)){
                DISTANCIA_P=distancia(x,y,R);
                MENOR_X=R->x;
                MENOR_Y=R->y;
            }
            VERIF(R->Prc1,x,y);
            VERIF(R->Prc2,x,y);
            VERIF(R->Prc3,x,y);
            VERIF(R->Prc4,x,y);
        }
    }

    void INSERT_B(double X,double Y)
    {
         if(raiz==NULL){
            raiz=new NODOS(padr,X,Y);
            //cout<<"intro d:"<<X<<",,"<<Y<<endl;
         }
         VerificDIST(X,Y);
         if(DISTANCIA_P!=999&&DISTANCIA_P!=0){
            //cout<<MENOR_X<<","<<MENOR_Y<<" Menor Distancia: "<<DISTANCIA_P<<"\n\n";
            INS_B(raiz,MENOR_X,MENOR_Y,X,Y);
         }

    }
    void INS_B(NODOS *R,double X_PADRE,double Y_PADRE,double X,double Y)
    {    bool INTRO=1;
         if(R!=NULL){
            INS_B(R->Prc1,X_PADRE,Y_PADRE,X,Y);
            INS_B(R->Prc2,X_PADRE,Y_PADRE,X,Y);
            INS_B(R->Prc3,X_PADRE,Y_PADRE,X,Y);
            INS_B(R->Prc4,X_PADRE,Y_PADRE,X,Y);

            if(R->x==X_PADRE&&R->y==Y_PADRE){
                if(R->Prc1==NULL&&INTRO){
                    R->Prc1=new NODOS(R,X,Y);
                    //cout<<" PADRE: "<<R->x<<","<<R->y<<" INTRODUJO 1: ("<<X<<","<<Y<<")\n";
                }
                else if(R->Prc2==NULL&&INTRO){
                    R->Prc2=new NODOS(R,X,Y);
                    //cout<<" PADRE: "<<R->x<<","<<R->y<<" INTRODUJO 2: ("<<X<<","<<Y<<")\n";
                }
                else if(R->Prc3==NULL&&INTRO){
                    R->Prc3=new NODOS(R,X,Y);
                    //cout<<" PADRE: "<<R->x<<","<<R->y<<" INTRODUJO 3: ("<<X<<","<<Y<<")\n";
                }
                else if(R->Prc4==NULL&&INTRO){
                    R->Prc4=new NODOS(R,X,Y);
                    //cout<<" PADRE: "<<R->x<<","<<R->y<<" INTRODUJO 4: ("<<X<<","<<Y<<")\n";
                }else{
                    //cout<<"NO SE INTRODUJO\n";
                }
            }
         }
    }

    void buscar(int datx,int daty)
    {
         busc(raiz,datx,daty);
    }
    void busc(NODOS *R,int datx,int daty)
    {
         if(R!=NULL){
            if(R->x==datx&&R->y==daty){
                //valor=1;
                //valor2=R->x;
                //valor3=R->y;
            }
            busc(R->Prc1,datx,daty);
            busc(R->Prc2,datx,daty);
            busc(R->Prc3,datx,daty);
            busc(R->Prc4,datx,daty);
         }
    }
    void Mostrar(HDC hdc)
    {
         Mos(raiz,hdc);
    }
    void Mos(NODOS *R,HDC hdc)
    {
         if(R){
            if(R->PaDr){

                DIBUJAR_UNION_PUNTOS(hdc,R->PaDr->x,R->PaDr->y,R->x,R->y,1);
                DIBUJAR_PUNTOS(hdc,R->PaDr->x,R->PaDr->y,0);
                DIBUJAR_PUNTOS(hdc,R->x,R->y,0);
                //cout<<"PADRE:"<<R->PaDr->x<<","<<R->PaDr->y;
                //cout<<" DATO: "<<R->x<<" , "<<R->y<<"\n";
            }else{
                DIBUJAR_UNION_PUNTOS(hdc,R->x,R->y,R->x,R->y,1);
                DIBUJAR_PUNTOS(hdc,R->x,R->y,0);
                //cout<<" DATO: "<<R->x<<" , "<<R->y<<"\n";
            }
            Mos(R->Prc1,hdc);
            Mos(R->Prc2,hdc);
            Mos(R->Prc3,hdc);
            Mos(R->Prc4,hdc);
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

        Arc(hdc, 10+X-RADIO, 625+Y-RADIO, 10+X+RADIO, 625+Y+RADIO, 10, 10, 10, 10);

        DeleteObject(greenPen);
}

void DIBUJAR_PUNTOS(HDC hdc,double X,double Y,int COLOR){                     ///GRAFICAR PUNTOS
    if(COLOR==0){greenPen=CreatePen(PS_SOLID, 10, RGB(255, 0, 0));}
    else if(COLOR==1){greenPen=CreatePen(PS_SOLID, 10, RGB(255, 255, 0));}
    else if(COLOR==2){greenPen=CreatePen(PS_SOLID, 10, RGB(0, 0, 255));}
   // else if(color==4){greenPen=CreatePen(PS_SOLID, 15, RGB(255, 255, 255));}
    else{
       greenPen=CreatePen(PS_SOLID, 10, RGB(255, 255, 255));
    }
    /// x 250-1190
    /// y 10-625

    X*=20;
    Y*=-20;

    SelectObject(hdc,greenPen);
    MoveToEx(hdc,10+X,625+Y,NULL);
    LineTo(hdc,10+X,625+Y);
    DeleteObject(greenPen);
}

void DIBUJAR_UNION_PUNTOS(HDC hdc,double X_INI,double Y_INI,double X,double Y,int COLOR){                     ///GRAFICAR PUNTOS
    if(COLOR==0){greenPen=CreatePen(PS_SOLID, 3, RGB(255, 0, 0));}
    else if(COLOR==1){greenPen=CreatePen(PS_SOLID, 3, RGB(255, 255, 0));}
    else if(COLOR==2){greenPen=CreatePen(PS_SOLID, 3, RGB(0, 0, 255));}
   // else if(color==4){greenPen=CreatePen(PS_SOLID, 15, RGB(255, 255, 255));}
    else{
       greenPen=CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
    }
    /// x 250-1190
    /// y 10-625

    X*=20;
    Y*=-20;
    X_INI*=20;
    Y_INI*=-20;

    SelectObject(hdc,greenPen);
    MoveToEx(hdc,10+X_INI,625+Y_INI,NULL);
    LineTo(hdc,10+X,625+Y);
    DeleteObject(greenPen);
}

void DIBUJAR_TABLA_COORD(HDC hdc){                                  ///GRAFICAR LINEAS TABLA
    for(int i=30,j=0;j<46;j++,i+=20){
        MoveToEx(hdc,i,10,NULL);
        LineTo(hdc,i,625);
    }
    for(int i=605,j=0;j<30;j++,i-=20){
        MoveToEx(hdc,10,i,NULL);
        LineTo(hdc,950,i);
    }
}

///GRAFICA
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

            greenPen=CreatePen(PS_SOLID, 1, RGB(70, 70, 70));
            SelectObject(hdc,greenPen);

            brush=CreateSolidBrush(RGB(67, 68, 70));    ///FONDO
            SelectObject(hdc,brush);
            Rectangle(hdc,-1,-1,1000,1000);
            DeleteObject(brush);

            brush=CreateSolidBrush(RGB(0, 0, 0));       ///COORDENADAS
            SelectObject(hdc,brush);
            Rectangle(hdc,10,10,950,625);
            DeleteObject(brush);
            DIBUJAR_TABLA_COORD(hdc);

            A.Mostrar(hdc);

            greenPen=CreatePen(PS_SOLID, 1, RGB(51, 204, 255));
            SelectObject(hdc,greenPen);
            brush=CreateSolidBrush(RGB(255, 255, 255)); ///L IZQUIERDO
            SelectObject(hdc,brush);
            Rectangle(hdc,10,10,240,70);
            DeleteObject(brush);
            DeleteObject(greenPen);
            EndPaint(hwndDlg,&ps);
        }
    return TRUE;
    case WM_LBUTTONDOWN:
        {   double xPos = GET_X_LPARAM(lParam);
            double yPos = GET_Y_LPARAM(lParam);

            if(xPos<950&&xPos>10&&yPos<625&&yPos>10){
                //cout<<((xPos-10)/20)<<","<<31-((yPos-10)/20)<<"\n";
                A.INSERT_B(((xPos-10)/20),31-((yPos-10)/20));
                SetDlgItemText(hwndDlg,EDIT1,"INCERTADO");
                //A.Insertar(Punto((int)xPos,(int)yPos));
                InvalidateRect(hwndDlg,NULL,true);
            }

        }
    return TRUE;
    case WM_MOUSEMOVE:
        {
            /*long xPos = GET_X_LPARAM(lParam);
            long yPos = GET_Y_LPARAM(lParam);
            char Cad[20];
            sprintf(Cad,"%i",xPos);
            SetDlgItemText(hwndDlg,EDITX,Cad);
            sprintf(Cad,"%i",yPos);
            SetDlgItemText(hwndDlg,EDITY,Cad);
            if(xPos<pxf&&xPos>pxi&&yPos<pyf&&yPos>pyi)
            {   VP=A.similar(A.Raiz,Punto(int(xPos),int(yPos)),Toler);

                PQ.x=xPos;
                PQ.y=yPos;
                InvalidateRect(hwndDlg,NULL,true);
            }*/
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

                if(Numx>0&&Numx<47&&Numy>0&&Numy<31){
                    A.INSERT_B(Numx,Numy);
                }else{
                    SetDlgItemText(hwndDlg,EDIT1,"LIM EXEDIDO");
                }
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

                //A.inconsulta(Numex,Numey);

                InvalidateRect(hwndDlg,NULL,true);
            return TRUE;
            case BTN_PIVOT:          ///punto de consulta

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
