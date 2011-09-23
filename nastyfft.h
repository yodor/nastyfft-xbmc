#ifndef _PLUGIN_NASTYFFT_H_
#define _PLUGIN_NASTYFFT_H_

#include "scenepreset.h"


#include <GL/gl.h>
#include <GL/glu.h>
class NastyFFT  
{

public:
    NastyFFT();
    ~NastyFFT();

	
	
	void scopeEvent(float *, int );

	void loadPreset(int pos);
	int presetIndex();
	void setSize(int width, int height);
	ScenePreset* scenePreset();
	void render();
	
private:
	void initPreset();
	
	void setupCamera();
	
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	
	GLUquadric *obj;
	
	ScenePreset priv;
	int preset_index;
	
	int width;
	int height;
	
	GLdouble fftdata[SCOPE_DEPTH][NUM_BANDS];
};

#endif // _PLUGIN_NASTYFFT_H_


