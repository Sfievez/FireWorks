#include <FL/Fl_Gl_Window.h>
#include <Fl/Fl.h>
#include <Fl/Fl_Value_Slider.H>
#include <Fl/Fl_Light_Button.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <Fl/Fl_Double_Window.h>

#include "stdio.h"
#include "math.h"
#include "3DUtils.h"

#include "Vec3f.h"
#include "Viewer.h"
#include <vector>

#include "core.h"
#include "particle.h"
#include "pfgen.h"
#include "MySpring.h"
#include "Move.h"
#include "MoverConnection.h"
#include "Fireworks.h"
#include "Fire.h"

#ifndef MY_GL_WINDOW_H
#define MY_GL_WINDOW_H


class MyGlWindow : public Fl_Gl_Window {
public:
    MyGlWindow(int x, int y, int w, int h);
    ~MyGlWindow() {
        for (Mover* mover : m_movers) delete mover;
        m_movers.clear();
        delete m_viewer;
        delete anchor;
        delete mover_connection;
        delete m_fireworks;
    }
    Fl_Slider* time;
    Fl_Light_Button* ui;
    int run;
    void update();
    void drawStuff();
    void doPick();
    void test();
    void launchSeries();
    void step();
    int selected;

    void addBall(float x, float y, float z);


private:
    void draw();                    // standard FlTk
    int handle(int);                // standard FlTk
    float fieldOfView;
    Viewer* m_viewer;
    Fireworks* m_fireworks;


    std::vector<Mover*> m_movers;
    MoverConnection* mover_connection;
    Vector3* anchor;
	cyclone::ParticleGravity* m_gravity;
	


    void setProjection(int clearProjection = 1);
    void getMouseNDC(float& x, float& y);
    void setupLight(float x, float y, float z);
    void putText(char* string, int x, int y, float r, float g, float b);

    cyclone::Vector3 p1, p2;
    clock_t t1, t2;
};

#endif
