#include <chrono>
#include <thread>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cmath>
using namespace std;
#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds

#define NIL (0)       // A name for the void pointer
#define FPS 45
#define PI 3.14
#define AL 100000
#define INDEX(x,y) (x*y)
int WIDTH=1024;
int HEIGHT=768;
float rain[AL][6]={};
int raincount=0;
float oldTime=0;
float deltaTime=0;

void shiftf(int j){
	for (int i=j+1;i<raincount; i++){
		for (int k=0;k<6;k++){
			rain[i-1][k]=rain[i][k];
		}
	}

}
void fall(){
	deltaTime = (clock() - oldTime)/100000;
	double fps = (1.0 / deltaTime) * 1000.0;

	for(int i=0;i<raincount;i++){
		rain[i][0]=rain[i][0]+(deltaTime*5.0*((float)rain[i][2]*abs(cos((float)rain[i][3] / (2*PI))))+rain[i][4]);
		rain[i][1]=rain[i][1]+(deltaTime*5.0*((float)rain[i][2]*abs(sin((float)rain[i][3] / (2*PI))))+rain[i][5]);
		rain[i][5]+=(deltaTime*0.01*((float)rain[i][2]*abs(cos((float)rain[i][3] / (2*PI)))));
		rain[i][4]+=(deltaTime*0.01*((float)rain[i][2]*abs(sin((float)rain[i][3] / (2*PI)))));
	}
	int i=0;
	while (i< raincount){
		if (rain[i][1]>HEIGHT){
			shiftf(i);
			raincount-=1;
			i-=1;
		}
		i+=1;
	}


	oldTime = clock();

}
void plotLineHigh(int x0,int y0,int x1,int y1){
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;
	if (dx < 0){
		xi = -1;
		dx = -dx;
	}
	int D = (2 * dx) - dy;
	int x = x0;

	for (int y=y0;y<y1+1;y++){
		if (x<=0 || x>=WIDTH || y<=0 || y>=HEIGHT){return;}
//		buf[INDEX(x,y)]=1;
		//oled.drawPixel(x,y,WHITE);
		if (D > 0){
			x = x + xi;
			D = D + (2 * (dx - dy));
		}else{
			D = D + 2*dx;
		};
	};
};
void plotLineLow(int x0,int y0,int x1,int y1){
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;
	if (dy < 0){
		yi = -1;
		dy = -dy;
	}
	int D = (2 * dy) - dx;
	int y = y0;

	for (int x=x0;x<x1+1;x++){
		if (x<=0 || x>=WIDTH || y<=0 || y>=HEIGHT){return;}
//		buf[INDEX(x,y)]=1;
		//oled.drawPixel(x,y,WHITE);
		if (D > 0){
			y = y + yi;
			D = D + (2 * (dy - dx));
		}else{
			D = D + 2*dy;
		}
	}
}


void line(int x0,int y0,int x1,int y1){
	if (abs(y1 - y0) < abs(x1 - x0)){
		if (x0 > x1){
			plotLineLow(x1, y1, x0, y0);
		}else{
			plotLineLow(x0, y0, x1, y1);
		};
	}else{
		if (y0 > y1){
			plotLineHigh(x1, y1, x0, y0);
		}else{
			plotLineHigh(x0, y0, x1, y1);
		};
	};
};




















Display *dpy = XOpenDisplay(NIL);
int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));



int main()
{
	assert (dpy);
	// sizeX, sizeY, FileName, BackgroundColor
	Window w =DefaultRootWindow(dpy); // XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, WIDTH, HEIGHT, 0, blackColor, blackColor);
	//XSelectInput(dpy, w, StructureNotifyMask);
//	XMapWindow(dpy, w);
	GC gc = XCreateGC(dpy, w, 0, NIL);
	//for(;;) {
	//	XEvent e;
	//	XNextEvent(dpy, &e);
	//	if (e.type == MapNotify)
	//		break;
	//}
	int prev=clock();
	while (true){
		XWindowAttributes attr;
		XGetWindowAttributes(dpy,w,&attr);
		WIDTH=attr.width;
		HEIGHT=attr.height;
		while (((float)(clock() -prev)/(float)(CLOCKS_PER_SEC))*1000.00<(1000/FPS)){
		}
		//cout<<"fps: "<<CLOCKS_PER_SEC/(clock()-prev);
		prev=clock();
		for(int i=0; i< rand()%(5000/FPS);i++){
			if ( (raincount<AL)){
				raincount+=1;
				rain[raincount-1][0]=(rand()%(2*WIDTH))-(WIDTH/2);
				rain[raincount-1][1]=-(rand()%(HEIGHT/2));
				rain[raincount-1][2]=30+rand()%10;
				rain[raincount-1][3]=PI*500.0;
				rain[raincount-1][4]=0;
				rain[raincount-1][5]=0;

			}}
		fall();

//		XSetForeground(dpy, gc, blackColor);
//		for(int i=0; i<raincount;i++){
//			XDrawLine(dpy, w, gc, rain[i][0],\
//					rain[i][1],\
//					rain[i][0]+(((float)rain[i][2]*abs(cos((float)rain[i][3] / (2*PI))))),\
//					rain[i][1]+(((float)rain[i][2]*abs(sin((float)rain[i][3] / (2*PI)))))
//				 );
//		}
//		XFlush(dpy);
		XClearWindow(dpy,w);
		XSetForeground(dpy, gc, blackColor);
		for(int i=0; i<raincount;i++){
			XDrawLine(dpy, w, gc, rain[i][0],\
					rain[i][1],\
					rain[i][0]+(((float)rain[i][2]*abs(cos((float)rain[i][3] / (2*PI))))+rain[i][4]),\
					rain[i][1]+(((float)rain[i][2]*abs(sin((float)rain[i][3] / (2*PI))))+rain[i][5])
				 );

		} 	
		XFlush(dpy);


	}  

	return 0;
}
