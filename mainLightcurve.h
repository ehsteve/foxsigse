#ifndef _mainLightcurve_h_     // prevent multiple includes
#define _mainLightcurve_h_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#if defined(__APPLE_CC__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "Application.h"

#define MAX_CHANNEL 1024
#define NUM_DETECTORS 7

class mainLightcurve : public Fl_Gl_Window {
public:
	mainLightcurve(int x,int y,int w,int h,const char *l=0);
	void draw();
	float binsize[MAX_CHANNEL];
	float CountRatecurveFunction[MAX_CHANNEL];
	int CountcurveDetectors[MAX_CHANNEL][NUM_DETECTORS+1];
	float CountRatecurveDetectors[MAX_CHANNEL][NUM_DETECTORS+1];	
	unsigned int current_timebin_detectors[NUM_DETECTORS+1];
	void set_xmax(int newxmax);
	void set_ymax(int newymax);
private:
	int xmax;
	int xmin;
	int ymin;
	int ymax;

};

#endif
