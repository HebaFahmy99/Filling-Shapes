#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
using namespace std;

#define MAXENTRIES 600
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

struct Entry
{
    int xmin,xmax;
};

void InitEntries(Entry table[])
{
    for(int i=0; i < MAXENTRIES; i++)
        {
            table[i].xmin =  9999;
            table[i].xmax = - 9999;
        }
}
void ScanEdge(POINT v1,POINT v2,Entry table[])
{
    if(v1.y==v2.y)return;
    if(v1.y>v2.y)swap(v1,v2);
    double minv=(double)(v2.x-v1.x)/(v2.y-v1.y);
    double x=v1.x;
    int y=v1.y;
    while(y<v2.y){
            if(x<table[y].xmin)table[y].xmin=(int)ceil(x);
            if(x>table[y].xmax)table[y].xmax=(int)floor(x);
            y++;
            x+=minv;
            }
}
void DrawSanLines(HDC hdc,Entry table[],COLORREF color)
{
for(int y=0;y<MAXENTRIES;y++)
if(table[y].xmin<table[y].xmax)
for(int x=table[y].xmin;x<=table[y].xmax;x++)
SetPixel(hdc,x,y,color);
}
void ConvexFill(HDC hdc,POINT p[],int n,COLORREF color)
{
    Entry *table=new Entry[MAXENTRIES];
    InitEntries(table);
    POINT v1=p[n-1];
    for(int i=0;i<n;i++){
            POINT v2=p[i];
            ScanEdge(v1,v2,table);
            v1=p[i];
            }
    DrawSanLines(hdc,table,color);
    delete table;
}
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
int Num_of_Points=0;
POINT P[5];

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HDC hdc=GetDC(hwnd);

    switch (message)                  /* handle the messages */
    {
    case WM_LBUTTONDBLCLK:

        if(Num_of_Points==0)
        {
           P[0].x = LOWORD(lParam);
           P[0].y = HIWORD(lParam);
           Num_of_Points++;
        }
        else if(Num_of_Points==1)
        {
           P[1].x=LOWORD(lParam);
           P[1].y=HIWORD(lParam);
           Num_of_Points++;
        }
        else if(Num_of_Points==2)
        {
           P[2].x=LOWORD(lParam);
           P[2].y=HIWORD(lParam);
           Num_of_Points++;
        }
        else if(Num_of_Points==3)
        {
           P[3].x=LOWORD(lParam);
           P[3].y=HIWORD(lParam);
           Num_of_Points++;
        }
        else if(Num_of_Points==4)
        {
           P[4].x=LOWORD(lParam);
           P[4].y=HIWORD(lParam);
           Num_of_Points++;
           //change pen color//
           SelectObject(hdc, GetStockObject(DC_PEN));
           SetDCPenColor(hdc, RGB(60,239,161));

           Polygon(hdc, P, 5);
        }
        else if(Num_of_Points==5)
        {
           ConvexFill(hdc, P, 5, RGB(60,239,161) );
           Num_of_Points=0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
