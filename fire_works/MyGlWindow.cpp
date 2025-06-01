

#include "MyGlWindow.h"
#include "MySpring.h"
#include "Move.h"
#include "MoverConnection.h"


#include <iostream>
#include "drawUtils.h"


#include "timing.h"


static double DEFAULT_VIEW_POINT[3] = { 30, 30, 30 };
static double DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static double DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };


MyGlWindow::MyGlWindow(int x, int y, int w, int h) :
	Fl_Gl_Window(x, y, w, h)
	//==========================================================================
{

	mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);

	fieldOfView = 45;

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	mover_connection = nullptr;

	m_fireworks = new Fireworks();

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	TimingData::init();
	run = 0;
	selected = -1;
}

MyGlWindow::~MyGlWindow() {
	for (Mover* mover : m_movers) delete mover;
	m_movers.clear();
	delete m_viewer;
	delete anchor;
	delete mover_connection;
	delete m_fireworks;
}

void MyGlWindow::addBall(float x, float y, float z) {
	Mover* newBall = new Mover();
	newBall->m_particle->setPosition(x, y, z);
	newBall->m_particle->setVelocity(0, 0, 0);
	m_movers.push_back(newBall);
}

void MyGlWindow::setupLight(float x, float y, float z)
{

	// set up the lighting
	GLfloat lightPosition[] = { 500, 900.0, 500, 1.0 };
	GLfloat lightPosition2[] = { 1, 0, 0, 0 };
	GLfloat lightPosition3[] = { 0, -1, 0, 0 };

	GLfloat violetLight[] = { 0.5f, 0.1f, .5f, 1.0 };
	GLfloat whiteLight[] = { 1, 1, 1, 1.0 };
	GLfloat whiteLight2[] = { .3, .3, .3, 1.0 };
	GLfloat blueLight[] = { .1f,.1f,.3f,1.0 };


	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	lightPosition[0] = x;
	lightPosition[1] = y;
	lightPosition[2] = z;

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, whiteLight2);
	
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLight);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}




void setupObjects(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0x0, 0x0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glStencilMask(0x1);		// only deal with the 1st bit
}


void MyGlWindow::drawStuff()
{
	//glcolor4f(1, 1, 0, 0.5);
	//polygonf(4, 20., 0., -25., 20., 0., 25., -20., 30., 25., -20., 30., -25.);
	//glcolor3f(1.0f, 0.0f, 0.0f);  // rouge
	//glbegin(gl_lines);
	//glvertex3f(0, 15, 0);  // point sur le plan
	//glvertex3f(0 + 6 * 0.6f, 15 + 6 * 0.8f, 0);  // direction de la normale
	//glend();
}

void MyGlWindow::draw()
{
    glViewport(0, 0, w(), h());

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    setProjection();
    
	// ground light
	setProjection();
	setupFloor();

	glPushMatrix();
	drawFloor(200, 20);
	glPopMatrix();


	setupLight(m_viewer->getViewPoint().x, m_viewer->getViewPoint().y, m_viewer->getViewPoint().z);

	// fireworks
    if (m_fireworks) m_fireworks->draw(0);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    if (m_fireworks) m_fireworks->draw(1);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

    drawStuff();

    putText("7701615 Fievez_Stephane", 10, 10, 1.0f, 1.0f, 1.0f);

	// debug
	//if (m_fireworks) {
	//	std::string msg = "Particles: " + std::to_string(m_fireworks->getParticleCount());
	//	putText(const_cast<char*>(msg.c_str()), 10, 30, 0.9f, 0.9f, 0.9f);
	//}
}

void drawMyDebugFloor(float size, float y) {
	glColor3f(0.6f, 0.6f, 0.6f);
	glBegin(GL_QUADS);
	glVertex3f(-size, y, -size);
	glVertex3f(size, y, -size);
	glVertex3f(size, y, size);
	glVertex3f(-size, y, size);
	glEnd();
}

void MyGlWindow::launchSeries() {
	if (!run || !m_fireworks) return;

	const int groups = 3;			// 3 strikes
	const int perGroup = 6;			// 10 shots per strike
	const float groupDelay = 1.5f;  // break between strikes
	const float shotSpacing = 0.1f; // delay between shots

	for (int g = 0; g < groups; ++g) {
		for (int i = 0; i < perGroup; ++i) {
			float delay = g * groupDelay + i * shotSpacing;
			Fl::add_timeout(delay, [](void* data) {
				MyGlWindow* win = static_cast<MyGlWindow*>(data);
				if (win->m_fireworks) win->m_fireworks->create();
				}, this);
		}
	}
}


void drawFloor(float size, float y) {
	glColor3f(0.6f, 0.6f, 0.6f);

	glBegin(GL_QUADS);
	glVertex3f(-size, y, -size);
	glVertex3f(size, y, -size);
	glVertex3f(size, y, size);
	glVertex3f(-size, y, size);
	glEnd();
}

void MyGlWindow::test()
{
	if (m_fireworks) m_fireworks->create();
}

void MyGlWindow::update() {
	TimingData::get().update();

	if (!run)
		return;

	float duration = (float)TimingData::get().lastFrameDuration * 0.003;
	
	if (m_fireworks) m_fireworks->update(duration);
	
	if (duration <= 0.0f) return;

	for (Mover* mover : m_movers) {
		mover->update(duration);
	}
}


void MyGlWindow::doPick()
{
	double r1x, r1y, r1z, r2x, r2y, r2z;
	getMouseLine(r1x, r1y, r1z, r2x, r2y, r2z);

	cyclone::Vector3 rayDir(r2x - r1x, r2y - r1y, r2z - r1z);
	rayDir.normalise();

	cyclone::Vector3 rayStart(r1x, r1y, r1z);
	selected = -1;

	for (size_t i = 0; i < m_movers.size(); i++) {
		cyclone::Vector3 ballPos;
		m_movers[i]->m_particle->getPosition(&ballPos);
		cyclone::Vector3 toBall = ballPos - rayStart;
		float projection = toBall.x * rayDir.x + toBall.y * rayDir.y + toBall.z * rayDir.z;

		if (projection <= 0) continue;

		cyclone::Vector3 nearestPoint = rayStart + rayDir * projection;

		float distance2 = (nearestPoint - ballPos).squareMagnitude();
		float ballRadius = 2.0f;
		if (distance2 <= ballRadius * ballRadius) {
			selected = i;
			std::cout << "Selected ball: " << selected << std::endl;
			break;
		}
	}
}


void MyGlWindow::setProjection(int clearProjection)
//==========================================================================
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, w(), h());
	if (clearProjection)
		glLoadIdentity();
	// compute the aspect ratio so we don't distort things
	double aspect = ((double)w()) / ((double)h());
	gluPerspective(fieldOfView, aspect, 1, 1000);

	// put the camera where we want it to be
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// use the transformation in the ArcBall

	gluLookAt(
		m_viewer->getViewPoint().x, m_viewer->getViewPoint().y, m_viewer->getViewPoint().z,
		m_viewer->getViewCenter().x, m_viewer->getViewCenter().y, m_viewer->getViewCenter().z,
		m_viewer->getUpVector().x, m_viewer->getUpVector().y, m_viewer->getUpVector().z
	);

	//	glDisable(GL_BLEND);
}

static int last_push;
int m_pressedMouseButton;
int m_lastMouseX;
int m_lastMouseY;

int MyGlWindow::handle(int e) {
	switch (e) {
	case FL_SHOW:
		show();
		return 1;

	case FL_PUSH:
		m_pressedMouseButton = Fl::event_button();
		m_lastMouseX = Fl::event_x();
		m_lastMouseY = Fl::event_y();

		if (m_pressedMouseButton == 1) {
			doPick();

			if (selected >= 0) {
				// run = 0;
				m_movers[selected]->m_particle->getPosition(&p1);
				t1 = clock();
				std::cout << "picked" << std::endl;
				ui->value(0);
			}
		}
		damage(1);
		return 1;

	case FL_DRAG:
		if (selected >= 0 && m_pressedMouseButton == 1) {
			// We are dragging the particle
			double r1x, r1y, r1z, r2x, r2y, r2z;
			getMouseLine(r1x, r1y, r1z, r2x, r2y, r2z);

			double rx, ry, rz;
			cyclone::Vector3 position;
			m_movers[selected]->m_particle->getPosition(&position);

			// Get new position based on mouse movement
			mousePoleGo(r1x, r1y, r1z, r2x, r2y, r2z,
				position.x, position.y, position.z,
				rx, ry, rz,
				(Fl::event_state() & FL_CTRL) != 0);

			// Update particle position and stop its movement
			m_movers[selected]->m_particle->setPosition(rx, ry, rz);
			m_movers[selected]->m_particle->setVelocity(0, 0, 0);
		}
		else {
			double fractionChangeX = static_cast<double>(Fl::event_x() - m_lastMouseX) / static_cast<double>(this->w());
			double fractionChangeY = static_cast<double>(m_lastMouseY - Fl::event_y()) / static_cast<double>(this->h());

			if (m_pressedMouseButton == 1) {
				m_viewer->rotate(fractionChangeX, fractionChangeY);
			}
			else if (m_pressedMouseButton == 2) {
				m_viewer->zoom(fractionChangeY);
			}
			else if (m_pressedMouseButton == 3) {
				m_viewer->translate(-fractionChangeX, -fractionChangeY, (Fl::event_key(FL_Shift_L) == 0) || (Fl::event_key(FL_Shift_R) == 0));
			}
		}

		m_lastMouseX = Fl::event_x();
		m_lastMouseY = Fl::event_y();

		damage(1);
		return 1;

	case FL_RELEASE:
		if (selected >= 0 && m_pressedMouseButton == 1) {
			m_movers[selected]->m_particle->getPosition(&p2);
			t2 = clock();

			float timeDiff = (t2 - t1) / (float)CLOCKS_PER_SEC;

			if (timeDiff > 0) {
				cyclone::Vector3 velocity = (p2 - p1) / timeDiff;

				m_movers[selected]->m_particle->setVelocity(velocity);
				std::cout << "Velocity set to: " << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;
			}

			// Automatic enable the simulation
			selected = -1;
			run = 1;
			ui->value(1);
		}

		m_pressedMouseButton = -1;
		damage(1);
		return 1;

	case FL_KEYBOARD:
		return 0;

	default:
		return 0;
	}
}


//
// get the mouse in NDC
//==========================================================================
void MyGlWindow::getMouseNDC(float& x, float& y)
//==========================================================================
{
	// notice, we put everything into doubles so we can do the math
	float mx = (float)Fl::event_x();	// remeber where the mouse went down
	float my = (float)Fl::event_y();

	// we will assume that the viewport is the same as the window size
	float wd = (float)w();
	float hd = (float)h();

	// remember that FlTk has Y going the wrong way!
	my = hd - my;

	x = (mx / wd) * 2.0f - 1.f;
	y = (my / hd) * 2.0f - 1.f;
}

void drawStrokeText(char* string, int x, int y, int z)
{
	char* c;
	glPushMatrix();
	glTranslated(x, y + 8, z);
	glScaled(0.2, 0.2, 0.2);

	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}

	glPopMatrix();
}

void MyGlWindow::step()
{
	TimingData::get().update();
	float duration = 0.03; // or 0.06
	for (Mover* mover : m_movers) {
		mover->update(duration);
	}
	std::cout << "step" << std::endl;
}

void MyGlWindow::putText(char* string, int x, int y, float r, float g, float b)
{
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ortho();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glColor3f(r, g, b);
	drawStrokeText(string, x, y, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
