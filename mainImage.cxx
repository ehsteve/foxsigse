
#include "mainImage.h"

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>

#include <time.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "Application.h"

#define XSTRIPS 128
#define YSTRIPS 128
#define	XBORDER 5
#define YBORDER 5
#define ZOOMNUM 5

//todo
//need to auto scale the image, add scalling types (auto, set/linear, logarithmic)
//this needs maximumValue function, add in a functions.cxx file

//each pixel is exactly 1x1 in GL coordinates to make it easier

double detImage[XSTRIPS][YSTRIPS];
double detImagemask[XSTRIPS][YSTRIPS];
double detImagealpha[XSTRIPS][YSTRIPS];
double detImagetime[XSTRIPS][YSTRIPS];
double detImagemasktime[XSTRIPS][YSTRIPS];

double ymax;
float GL_cursor[2];
int mousePixel[2];
int chosenPixel[2];

extern Gui *gui;
extern Foxsidata *data;

// preference variable
extern int mainImage_minimum;
extern int low_threshold;

mainImage::mainImage(int x,int y,int w,int h,const char *l)
: Fl_Gl_Window(x,y,w,h,l)
{	
	start_time = clock();
	
	//initialize the image
	for(int i=0;i<XSTRIPS;i++)
	{
		for(int j=0;j<YSTRIPS;j++)
	   	{
			detImage[i][j] = 0;
			detImagealpha[i][j] = 0;
			detImagetime[i][j] = 0;
			for (int detector_num = 0; detector_num < (NUM_DETECTORS+1); detector_num++) {
				detectorsImage[i][j][detector_num] = 0;
				detectorsImagealpha[i][j][detector_num] = 0;
				detectorsImagetime[i][j][detector_num] = 0;
			}
		}
	}
	detImage[15][15] = 1.0;
	detImage[75][75] = 0.5;
	show_mask = 0;
	for (int detector_num = 0; detector_num < (NUM_DETECTORS+1); detector_num++) {
		detectorsImage[25][30][detector_num] = 1.0;
		detectorsImagealpha[26][31][detector_num] = 1.0;
	}
}

// the drawing method: draws the histFunc into the window
void mainImage::draw() 
{
	double grey, vert, bleu, alpha;
	clock_t current_time;
	double duration;
	current_time = clock();
	
	ymax = maximumValue(*detImage);
	duration = ( double ) ( current_time ) / CLOCKS_PER_SEC;
	
	//int cursorBox = 2;
	if (!valid()) {
		make_current();
	}
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0,0,w(),h());	
   	glOrtho(0,XSTRIPS+2*XBORDER,0,YSTRIPS+2*YBORDER,0,-1);
   	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
   	glPushMatrix();
   	glLoadIdentity();
   	glClearColor(0.0,0.0,0.0,0.0);
   	glClear(GL_COLOR_BUFFER_BIT);  
	
	if (gui->mainHistogramWindow->get_detector_display(0) == 1) {
		for(int j = 0; j < XSTRIPS; j++)
		{
			for(int i = 0; i < YSTRIPS; i++)
			{
				grey = detImage[i][j]/ymax;
				
				if (grey != 0)
				{
					if (gui->app->get_pixel_half_life() != 0){
						alpha = exp(-(current_time - detImagetime[i][j])/(CLOCKS_PER_SEC * gui->app->get_pixel_half_life() ));
					} else {
						alpha = 1.0;
					}

					detImagealpha[i][j] = alpha;
					glColor4f(grey, grey, grey, alpha);
					glBegin(GL_QUADS);
					glVertex2f(i+XBORDER, j+YBORDER); glVertex2f(i+1+XBORDER, j+YBORDER); 
					glVertex2f(i+1+XBORDER, j+1+YBORDER); glVertex2f(i+XBORDER, j+1+YBORDER);
					glEnd();
				}
				//now draw the mask
				if (show_mask == 1)
				{
					vert = detImagemask[i][j];
					
					if (bleu == 1){
						
						glColor4f(0, vert, 0, alpha * 0.5);
						glBegin(GL_QUADS);
						glVertex2f(i+XBORDER, j+YBORDER); glVertex2f(i+1+XBORDER, j+YBORDER); 
						glVertex2f(i+1+XBORDER, j+1+YBORDER); glVertex2f(i+XBORDER, j+1+YBORDER);
						glEnd();
					}
				}
			}
		}
	}
	
	//ymax = maximumValue(*detectorsImage);
	
	// draw individual detectors if needed
	for(int j = 0; j < XSTRIPS; j++)
	{
		for(int i = 0; i < YSTRIPS; i++)
		{
			for (int detector_num = 1; detector_num < NUM_DETECTORS + 1; detector_num++) {
				if (gui->mainHistogramWindow->get_detector_display(detector_num) == 1) {
					bleu = 7*detectorsImage[i][j][detector_num]/ymax;
					if (bleu != 0)
					{
						if (gui->app->get_pixel_half_life() != 0){
							alpha = exp(-(current_time - detectorsImagetime[i][j][detector_num])/(CLOCKS_PER_SEC * gui->app->get_pixel_half_life()));
						} else {
							alpha = 1.0;
						}
						detectorsImagealpha[i][j][detector_num] = alpha;
						glColor4f(0, 0, bleu, alpha);
						glBegin(GL_QUADS);
						glVertex2f(i+XBORDER, j+YBORDER); glVertex2f(i+1+XBORDER, j+YBORDER); 
						glVertex2f(i+1+XBORDER, j+1+YBORDER); glVertex2f(i+XBORDER, j+1+YBORDER);
						glEnd();
					}
				}
			}
		}
	}

	//draw a border around the detector
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(XBORDER, YBORDER); glVertex2f(XBORDER+XSTRIPS, YBORDER); 
	glVertex2f(XBORDER+XSTRIPS, YBORDER+YSTRIPS); glVertex2f(XBORDER, YBORDER+YSTRIPS);
	glEnd();
	
	// draw a cross under the cursor selection
	if(gui->Lockbut->value() == 0)
	{
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
			glVertex2f(mousePixel[0] + XBORDER - 3.5, mousePixel[1] + YBORDER);
			glVertex2f(mousePixel[0] + XBORDER + 4.5, mousePixel[1] + YBORDER);
		glEnd();
		
		glBegin(GL_LINES);
		glVertex2f(mousePixel[0] + XBORDER - 3.5, mousePixel[1] + YBORDER + 1);
		glVertex2f(mousePixel[0] + XBORDER + 4.5, mousePixel[1] + YBORDER + 1);
		glEnd();	
		
		glBegin(GL_LINES);
			glVertex2f(mousePixel[0] + XBORDER, mousePixel[1] + YBORDER - 3.5); 
			glVertex2f(mousePixel[0] + XBORDER, mousePixel[1] + YBORDER + 4.5); 
		glEnd();
		
		glBegin(GL_LINES);
		glVertex2f(mousePixel[0] + XBORDER + 1, mousePixel[1] + YBORDER - 3.5); 
		glVertex2f(mousePixel[0] + XBORDER + 1, mousePixel[1] + YBORDER + 4.5); 
		glEnd();
	} else {
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(chosenPixel[0] + XBORDER - 3.5, chosenPixel[1] + YBORDER);
		glVertex2f(chosenPixel[0] + XBORDER + 4.5, chosenPixel[1] + YBORDER);
		glEnd();
		
		glBegin(GL_LINES);
		glVertex2f(chosenPixel[0] + XBORDER - 3.5, chosenPixel[1] + YBORDER + 1);
		glVertex2f(chosenPixel[0] + XBORDER + 4.5, chosenPixel[1] + YBORDER + 1);
		glEnd();	
		
		glBegin(GL_LINES);
		glVertex2f(chosenPixel[0] + XBORDER, chosenPixel[1] + YBORDER - 3.5); 
		glVertex2f(chosenPixel[0] + XBORDER, chosenPixel[1] + YBORDER + 4.5); 
		glEnd();
		
		glBegin(GL_LINES);
		glVertex2f(chosenPixel[0] + XBORDER + 1, chosenPixel[1] + YBORDER - 3.5); 
		glVertex2f(chosenPixel[0] + XBORDER + 1, chosenPixel[1] + YBORDER + 4.5); 
		glEnd();
		
		//now update the text box with the current chosen pixel
		Fl::lock();
		gui->pixelCounts->value(detImage[chosenPixel[0]][chosenPixel[1]]);
		Fl::unlock();
		
	}

	glPopMatrix();	
}

int mainImage::handle(int eventType)
{
	int button;
	char text[8];
	
	button=Fl::event_button();
	
	//convert between fltk coordinates and openGL coordinates
	GL_cursor[0]=Fl::event_x()*(XSTRIPS + 2.0*XBORDER)/w();
	GL_cursor[1]=(h()-Fl::event_y())*(YSTRIPS + 2.0*YBORDER)/h();
	
	//translate to pixel number but keep within bounds 
	mousePixel[0] = (GL_cursor[0] - XBORDER) > 1 ? GL_cursor[0] - XBORDER : 1;
	mousePixel[0] = mousePixel[0] < XSTRIPS ? mousePixel[0]: XSTRIPS;
	
	mousePixel[1] = (GL_cursor[1] - YBORDER) > 1 ? GL_cursor[1] - YBORDER : 1;
	mousePixel[1] = mousePixel[1] < YSTRIPS ? mousePixel[1]: YSTRIPS;
		
	if((eventType==FL_PUSH)&&(button==1))
	{
		//set the view lock button to ON
		gui->Lockbut->set();
		//save the location
		chosenPixel[0] = mousePixel[0];	chosenPixel[1] = mousePixel[1];
	}
	
	//now update the text box with the current chosen pixel
	Fl::lock();
	if (gui->Lockbut->value() == 0) {
		gui->pixelCounts->value(detImage[mousePixel[0]][mousePixel[1]]);
		sprintf( text, "%d,%d", mousePixel[0], mousePixel[1]);
		gui->pixelNum->value(text);
	}
	Fl::unlock();
	redraw();
	
	return(1);
}

double mainImage::maximumValue(double *array)
{
	//float length = sizeof(array)/sizeof(array[0]);  // establish size of array
	//cout << "size of array " << length << endl;
	//do not consider below xmin
	int max = array[0];       // start with max = first element
	for(int i = 1; i<XSTRIPS*YSTRIPS; i++)
	{
		if(array[i] > max){
			max = array[i];}
	}
	
	return max;                // return highest value in array
}

