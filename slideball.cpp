#include<stdio.h>
#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<typeinfo>

using namespace std;

struct Ball {
	int x; // x position
	int y; // y position
	int x_vel; // x velocity
	int y_vel; // y velocity
	bool alive; // false iff ball has been dropped
}; 

// rectangle controlled by user to keep the balls in the air
struct Rectangle {
	double x = 0; // x position of the center of the rectangle
};

// initialize positions and speeds of balls (assuming 3 balls)
struct Game {
	Ball b1 = {-300, 310, 0, -3, true};
	Ball b2 = {0, -60, 0, -3, true};
	Ball b3 = {300, 120, 0, -3, true};
	Rectangle r;
};


// Global Variables //
int ball_number; // user-specified number of balls
Game g; 
int score = 0; // how many times balls have hit rectangle

// speeds up balls based on number of starting balls
void loop() {
	if (ball_number == 1){
		g.b1.y_vel = -12;
		g.b2.alive = false;
		g.b3.alive = false;
	} else if (ball_number == 2){
		g.b1.y_vel = -6;
		g.b2.y_vel = -6;
		g.b3.alive = false;
	}
}

void init(void) {
	// Background Color
	glClearColor (1.0, 1.0, 1.0, 0.0);
	// Window size
    gluOrtho2D(-620.0, 620.0, -450.0, 340.0);
}

// moves user's rectangle to the left and right
void SpecialInput(int key, int x, int y) {
	switch(key) {
	case GLUT_KEY_LEFT:
		if (g.r.x >= -500){
			g.r.x -= 80;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (g.r.x <= 500){
			g.r.x += 80;
		}
		break;
	}
}


Ball ballMovement(Ball b){
	b.x += b.x_vel;
	b.y += b.y_vel;
	
	// bounce off side walls
	if (b.x <= -590 || b.x >= 590) { 
		b.x_vel = -b.x_vel;
	}

	// bounce off rectangle
	if (b.y >= -390 && b.y <= -380 && g.r.x - 110 <= b.x && b.x <= g.r.x + 110) {
		b.y_vel = abs(b.y_vel);
		b.x_vel = std::max(1.0, (b.x - g.r.x)/7);
		score ++;
		if (b.y_vel == 3) {
			b.y_vel += 1;
		}
	}
	
	// bounce off top wall
	if (b.y >= 310) {
		b.y_vel = -abs(b.y_vel);
	}
	
	// register that a ball is no longer in play
	if (b.y < -390) {
		b.alive = false;
	}
	return b;
}

void DrawCircle(Ball b){
	if (b.alive){
		glBegin(GL_LINE_LOOP);
			for(int i = 0; i < 1000; i++){
		    float theta = 2.0f * 3.1415926f * float(i) / float(1000); // get the current angle
		    float x = 30 * cosf(theta); // calculate x component
		    float y = 30 * sinf(theta); // calculate y component
		    glVertex2f(x + b.x, y + b.y);
		    }
		glEnd();
	}
}

// calculates the sign of an integer
int sign(int n) {
	if (n < 0) {
		return -1;
	} else if (n > 0) {
		return 1;
	} else {
		return 0;
	}
}

void updateGame(void)
{
	// moves each of the remaining balls
	g.b1 = ballMovement(g.b1);
	g.b2 = ballMovement(g.b2);
	g.b3 = ballMovement(g.b3);
	
	// prints end of game message if all balls are dead
	if (!(g.b1.alive || g.b2.alive || g.b3.alive)) {
		cout << "Game Over. Score: " << score << endl;
		exit(0);
	}
	
	// speeds up remaining ball if there is only one left
	if (!(g.b1.alive || g.b2.alive) || !(g.b1.alive || g.b3.alive) || !(g.b2.alive || g.b3.alive)) {
		g.b1.y_vel = sign(g.b1.y_vel)*12;
		g.b2.y_vel = sign(g.b2.y_vel)*12;
		g.b3.y_vel = sign(g.b3.y_vel)*12;
	} 
	
	// speeds up remaining balls if there are only two left
	else if ((!g.b1.alive) || (!g.b2.alive) || (!g.b3.alive)) {
		g.b1.y_vel = sign(g.b1.y_vel)*6;
		g.b2.y_vel = sign(g.b2.y_vel)*6;
		g.b3.y_vel = sign(g.b3.y_vel)*6;
	}
	
    glutPostRedisplay();
}


void display(void){	
	// reset black background
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);            
 		glVertex2f(-620, -450);
	    glVertex2f(-620, 340);
	    glVertex2f(620, 340);
	    glVertex2f(620, -450);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	
	// draw user's rectangle
	glBegin(GL_LINE_LOOP);
	    glVertex2i(g.r.x - 100, -425);
	    glVertex2i(g.r.x - 100, -405);
	    glVertex2i(g.r.x + 100, -405);
	    glVertex2i(g.r.x + 100, -425);
	glEnd();
	
	// draw balls
	DrawCircle(g.b1);
	DrawCircle(g.b2);
	DrawCircle(g.b3);
    
	glutSwapBuffers();	
}



// Driver Program
int main (int argc, char** argv)
{
	cout << "Enter number of balls (1-3): ";
	cin >> ball_number;	
	
	loop();
	
	glutInit(&argc, argv);
     
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    // declares window size
    glutInitWindowSize(1100, 700);
     
    // declares window position
    glutInitWindowPosition(0, 0);
 
    // names window
    glutCreateWindow("Game");
    
    glutIdleFunc(updateGame);
 
    // keyboard function
    glutSpecialFunc(SpecialInput);
    
    // Call to init()
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}

