
#include <iostream>
//#include <GL/glew.h> // Include the GLEW header file
#include <GL/glut.h> // Include the GLUT header file
//#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> // standard definitions
#include <vector>


#define ESC 27
#define PAWN 0
#define ROOK 1
#define KNIGHT 2
#define BISHOP 3
#define QUEEN 4
#define KING 5

#define NUMKNIGHTS 4
#define NUMPAWNS 16


using namespace std;
//----------------------------------------------------------------------
// Global variables
//
// The coordinate system is set up so that the (x,y)-coordinate plane
// is the ground, and the z-axis is directed upwards. The y-axis points
// to the north and the x-axis points to the east.
//
// The values (x,y) are the current camera position. The values (lx, ly)
// point in the direction the camera is looking. The variables angle and
// deltaAngle control the camera's angle. The variable deltaMove
// indicates the amount of incremental motion for the camera with each
// redraw cycle. The variables isDragging and xDragStart are used to
// monitor the mouse when it drags (with the left button down).
//----------------------------------------------------------------------

// Camera position
float x = 4, y = -5.0, z = 10; // initially 5 units south of origin
float dx = 0, dy = 0, dz = 0;
float deltaMove = 0.0; // initially camera doesn't move
float dist_to_center = 9.0;

// Camera direction
//float lx = -4.0, ly = 5.0, lz = -10;// camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging
float angle_around_z = -90.0;

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

// Lighting ON/OFF
bool light0 = true;
bool light1 = true;

GLint gbl_slices = 100;
GLint gbl_stacks = 100;
GLdouble gbl_radius = 0.375;
GLdouble gbl_height = 1.0;

bool fancy_mode = false;

struct move_struct
{
	int dx;
	int dy;
};

struct simple_piece
{
	int piece_type;
	float red;
	float green;
	float blue;
	GLdouble x_coord;
	GLdouble y_coord;
	int x_tile;
	int y_tile;
	GLdouble z_coord;
};

vector<move_struct> knight_moves;
vector<move_struct> pawn_moves;
vector<simple_piece> chess_pieces;
vector<vector<bool>> board_occupations;
//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------
void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

//----------------------------------------------------------------------
// Update with each idle event
//
// This incrementally moves the camera and requests that the scene be
// redrawn.
//----------------------------------------------------------------------
void update(void)
{
    glutPostRedisplay(); // redisplay everything
}

//----------------------------------------------------------------------
// Draw the entire scene
//
// We first update the camera location based on its distance from the
// origin and its direction.
//----------------------------------------------------------------------
void renderScene(void)
{

    // Clear color and depth buffers
    glClearColor(0.0, 0.0, 0.0, 0.0); // sky color is light blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt(
        x, y, z,
		4, 4, 0,        
		0, 0, 1);

    // Draw black squares 1x1
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; i < 8; i++)
    {
		for(int j = 0; j < 8; j ++)
		{
			if((i + j) % 2 == 1)
			{
				glColor3f(1.0, 1.0, 1.0);
			}
			else
			{
				glColor3f(0.0, 0.0, 0.0);
			}
        	glBegin(GL_QUADS);
        	glVertex3f(i, j, 0.0);
        	glVertex3f(i + 1, j, 0.0);
        	glVertex3f(i + 1, j + 1, 0.0);
        	glVertex3f(i, j + 1, 0.0);
        	glEnd();
		}
    }

    // Draw Chess Pieces
	

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

    for (int i = 0; i < chess_pieces.size(); i++)
    {
		//glClearColor(0, 0, 0, 0);
		glPushMatrix();
    	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		switch(chess_pieces.at(i).piece_type)
		{
			case ROOK:
					glColor3f(chess_pieces.at(i).red / 255,
								chess_pieces.at(i).green / 255,
								chess_pieces.at(i).blue / 255);
					glTranslatef(chess_pieces.at(i).x_coord,
								chess_pieces.at(i).y_coord,
								chess_pieces.at(i).z_coord);
					glutSolidCube(gbl_radius * 2);
				break;
			case KNIGHT:
					glColor3f(chess_pieces.at(i).red / 255,
								chess_pieces.at(i).green / 255,
								chess_pieces.at(i).blue / 255);
					glTranslatef(chess_pieces.at(i).x_coord,
								chess_pieces.at(i).y_coord,
								chess_pieces.at(i).z_coord);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					glutSolidTeapot(gbl_radius);
				break;
			case BISHOP:
					glColor3f(chess_pieces.at(i).red / 255,
								chess_pieces.at(i).green / 255,
								chess_pieces.at(i).blue / 255);
					glTranslatef(chess_pieces.at(i).x_coord,
								chess_pieces.at(i).y_coord,
								chess_pieces.at(i).z_coord);
					glutSolidCone(gbl_radius, gbl_height, gbl_slices, gbl_stacks);
				break;
			case QUEEN:
					glColor3f(chess_pieces.at(i).red / 255,
								chess_pieces.at(i).green / 255,
								chess_pieces.at(i).blue / 255);
					glTranslatef(chess_pieces.at(i).x_coord,
								chess_pieces.at(i).y_coord,
								chess_pieces.at(i).z_coord);
					glScalef((0.75/sqrt(3)), (0.75/sqrt(3)), (0.75/sqrt(3)));
					glutSolidTetrahedron();
				break;
			case KING:
					glTranslatef(chess_pieces.at(i).x_coord,
								chess_pieces.at(i).y_coord,
								chess_pieces.at(i).z_coord);
					glScalef(0.375, 0.375, 0.375);
					glColor3f(chess_pieces.at(i).red / 255,
								chess_pieces.at(i).green / 255,
								chess_pieces.at(i).blue / 255);
					glutSolidOctahedron();
				break;
			case PAWN:
					glColor3f(chess_pieces.at(i).red / 255,
								chess_pieces.at(i).green / 255,
								chess_pieces.at(i).blue / 255);
					glTranslatef(chess_pieces.at(i).x_coord,
								chess_pieces.at(i).y_coord,
								chess_pieces.at(i).z_coord);
					glutSolidSphere(gbl_radius, gbl_slices, gbl_stacks);
				break;
		}
		glPopMatrix();
    }
    glutSwapBuffers(); // Make it all visible
}

//----------------------------------------------------------------------
// User-input callbacks
//
// processNormalKeys: ESC, q, and Q cause program to exit
// pressSpecialKey: Up arrow = forward motion, down arrow = backwards
// releaseSpecialKey: Set incremental motion to zero
//----------------------------------------------------------------------
void processNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == ESC || key == 'q' || key == 'Q')
    {
        exit(0);
    }
	else if(key == 'r' || key == 'R')
	{
		angle_around_z += 10;
		x = (dist_to_center * cos(angle_around_z * M_PI / 180.0)) + 4;
		y = (dist_to_center * sin(angle_around_z * M_PI / 180.0)) + 4;
	}
	else if(key == 'd' || key == 'D')
	{
		z -= 0.25;
	}
	else if(key == 'u' || key == 'U')
	{
		z += 0.25;
	}
	else if(key == '0')
	{
		if(light0)
		{
			glDisable(GL_LIGHT0);
			light0 = false;
		}
		else
		{
			glEnable(GL_LIGHT0);
			light0 = true;
		}
	}
	else if(key == '1')
	{
		if(light1)
		{
			glDisable(GL_LIGHT1);
			light1 = false;
		}
		else
		{
			glEnable(GL_LIGHT1);
			light1 = true;
		}
	}
	else if(key == 'k' || key == 'K')
	{
		int rand_k, k_index, target_x, target_y;
		bool success = false;
		for(int i = 0; i < 100; i++)
		{
			rand_k = rand() % 4;
			switch(rand_k)
			{
				case 0:
					k_index = 1;
					break;
				case 1:
					k_index = 6;
					break;
				case 2:
					k_index = 17;
					break;
				case 3:
					k_index = 22;
					break;
			}
			for(int j = 0; j < knight_moves.size(); j++)
			{
				target_x = chess_pieces.at(k_index).x_tile + knight_moves.at(j).dx;
				target_y = chess_pieces.at(k_index).y_tile + knight_moves.at(j).dy;
				if(target_x > 7 || target_x < 0 || target_y > 7 || target_y < 0)
				{
					success = false;
				}
				else if(! board_occupations.at(target_y).at(target_x))
				{
					board_occupations.at(target_y).at(target_x) = true;
					board_occupations.at(chess_pieces.at(k_index).y_tile).
							at(chess_pieces.at(k_index).x_tile) = false;
					chess_pieces.at(k_index).x_coord = (float)(target_x + 0.5);
					chess_pieces.at(k_index).y_coord = (float)(target_y + 0.5);
					chess_pieces.at(k_index).x_tile = target_x;
					chess_pieces.at(k_index).y_tile = target_y;
					success = true;
					break;
				}	
			}
			if(success){break;}
		}
	}
	else if(key == 'p' || key == 'P')
	{
		int rand_k, k_index, target_x, target_y;
		bool success = false;
		for(int i = 0; i < 100; i++)
		{
			rand_k = rand() % 16;
			if(rand_k < 8)
			{
				k_index = rand_k + 8;
			}
			else
			{
				k_index = rand_k + 16;
			}
			for(int j = 0; j < pawn_moves.size(); j++)
			{
				target_x = chess_pieces.at(k_index).x_tile + pawn_moves.at(j).dx;
				target_y = chess_pieces.at(k_index).y_tile + pawn_moves.at(j).dy;
				if(target_x > 7 || target_x < 0 || target_y > 7 || target_y < 0)
				{
					success = false;
				}
				else if(! board_occupations.at(target_y).at(target_x))
				{
					board_occupations.at(target_y).at(target_x) = true;
					board_occupations.at(chess_pieces.at(k_index).y_tile).
							at(chess_pieces.at(k_index).x_tile) = false;
					chess_pieces.at(k_index).x_coord = (float)(target_x + 0.5);
					chess_pieces.at(k_index).y_coord = (float)(target_y + 0.5);
					chess_pieces.at(k_index).x_tile = target_x;
					chess_pieces.at(k_index).y_tile = target_y;
					success = true;
					break;
				}	
			}
			if(success){break;}
		}
	}
}

void pressSpecialKey(int key, int xx, int yy)
{
    switch (key) 
    {
    case GLUT_KEY_UP: deltaMove = 1.0; break;
    case GLUT_KEY_DOWN: deltaMove = -1.0; break;
    }
}

void releaseSpecialKey(int key, int x, int y)
{
    switch (key) 
    {
    case GLUT_KEY_UP: deltaMove = 0.0; break;
    case GLUT_KEY_DOWN: deltaMove = 0.0; break;
    }
}

//----------------------------------------------------------------------
// Process mouse drag events
//
// This is called when dragging motion occurs. The variable
// angle stores the camera angle at the instance when dragging
// started, and deltaAngle is a additional angle based on the
// mouse movement since dragging started.
//----------------------------------------------------------------------
/*void mouseMove(int x, int y)
{
    if (isDragging) 
    { // only when dragging
        // update the change in angle
        deltaAngle = (x - xDragStart) * 0.005;

        // camera's direction is set to angle + deltaAngle
        lx = -sin(angle + deltaAngle);
        ly = cos(angle + deltaAngle);
    }
}

void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) 
    {
        if (state == GLUT_DOWN) 
        { // left mouse button pressed
            isDragging = 1; // start dragging
            xDragStart = x; // save x where button first pressed
        }
        else 
        {  //(state = GLUT_UP) 
            angle += deltaAngle; // update camera turning angle
            isDragging = 0; // no longer dragging
        }
    }
}*/

void chessPieceLocationInit()
{
	chess_pieces.push_back({ROOK, 	255, 255, 240, 0.5, 0.5, 0, 0, 0.375});
	chess_pieces.push_back({KNIGHT, 255, 255, 240, 1.5, 0.5, 1, 0, 0.265});
	chess_pieces.push_back({BISHOP, 255, 255, 240, 2.5, 0.5, 2, 0, 0});
	chess_pieces.push_back({QUEEN, 	255, 255, 240, 3.5, 0.5, 3, 0, 0.255});
	chess_pieces.push_back({KING, 	255, 255, 240, 4.5, 0.5, 4, 0, 0.375});
	chess_pieces.push_back({BISHOP, 255, 255, 240, 5.5, 0.5, 5, 0, 0});
	chess_pieces.push_back({KNIGHT, 255, 255, 240, 6.5, 0.5, 6, 0, 0.265});
	chess_pieces.push_back({ROOK, 	255, 255, 240, 7.5, 0.5, 7, 0, 0.375});
	chess_pieces.push_back({PAWN, 	255, 255, 240, 0.5, 1.5, 0, 1, 0.375});
	chess_pieces.push_back({PAWN, 	255, 255, 240, 1.5, 1.5, 1, 1, 0.375});
	chess_pieces.push_back({PAWN, 	255, 255, 240, 2.5, 1.5, 2, 1, 0.375});
	chess_pieces.push_back({PAWN, 	255, 255, 240, 3.5, 1.5, 3, 1, 0.375});
	chess_pieces.push_back({PAWN, 	255, 255, 240, 4.5, 1.5, 4, 1, 0.375});
	chess_pieces.push_back({PAWN, 	255, 255, 240, 5.5, 1.5, 5, 1, 0.375});
	chess_pieces.push_back({PAWN, 	255, 255, 240, 6.5, 1.5, 6, 1, 0.375});
	chess_pieces.push_back({PAWN, 	255, 255, 240, 7.5, 1.5, 7, 1, 0.375});

	chess_pieces.push_back({ROOK, 	150, 75, 0, 0.5, 7.5, 0, 7, 0.375});
	chess_pieces.push_back({KNIGHT, 150, 75, 0, 1.5, 7.5, 1, 7, 0.265});
	chess_pieces.push_back({BISHOP, 150, 75, 0, 2.5, 7.5, 2, 7, 0});
	chess_pieces.push_back({QUEEN, 	150, 75, 0, 3.5, 7.5, 3, 7, 0.255});
	chess_pieces.push_back({KING, 	150, 75, 0, 4.5, 7.5, 4, 7, 0.375});
	chess_pieces.push_back({BISHOP, 150, 75, 0, 5.5, 7.5, 5, 7, 0});
	chess_pieces.push_back({KNIGHT, 150, 75, 0, 6.5, 7.5, 6, 7, 0.265});
	chess_pieces.push_back({ROOK, 	150, 75, 0, 7.5, 7.5, 7, 7, 0.375});
	chess_pieces.push_back({PAWN, 	150, 75, 0, 0.5, 6.5, 0, 6, 0.375});
	chess_pieces.push_back({PAWN, 	150, 75, 0, 1.5, 6.5, 1, 6, 0.375});
	chess_pieces.push_back({PAWN, 	150, 75, 0, 2.5, 6.5, 2, 6, 0.375});
	chess_pieces.push_back({PAWN, 	150, 75, 0, 3.5, 6.5, 3, 6, 0.375});
	chess_pieces.push_back({PAWN, 	150, 75, 0, 4.5, 6.5, 4, 6, 0.375});
	chess_pieces.push_back({PAWN, 	150, 75, 0, 5.5, 6.5, 5, 6, 0.375});
	chess_pieces.push_back({PAWN, 	150, 75, 0, 6.5, 6.5, 6, 6, 0.375});
	chess_pieces.push_back({PAWN, 	150, 75, 0, 7.5, 6.5, 7, 6, 0.375});

	vector<bool> temp_vect;
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(i == 0 || i == 1 || i == 6 || i ==7)
			{
				temp_vect.push_back(true);
			}
			else
			{
				temp_vect.push_back(false);
			}
		}
		board_occupations.push_back(temp_vect);
		temp_vect.clear();
	}
	knight_moves.push_back({-2, 1});
	knight_moves.push_back({-1, 2});
	knight_moves.push_back({1, 2});
	knight_moves.push_back({2, 1});
	knight_moves.push_back({2, -1});
	knight_moves.push_back({1, -2});
	knight_moves.push_back({-1, -2});
	knight_moves.push_back({-2, -1});

	pawn_moves.push_back({0, 1});
	pawn_moves.push_back({0, -1});
}

//----------------------------------------------------------------------
// Main program  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
int main(int argc, char **argv)
{
    // general initializations
	srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Chess Set");

	chessPieceLocationInit();

    // register callbacks
    glutReshapeFunc(changeSize); // window reshape callback
    glutDisplayFunc(renderScene); // (re)display callback
    glutIdleFunc(update); // incremental update
    glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
    //glutMouseFunc(mouseButton); // process mouse button push/release
    //glutMotionFunc(mouseMove); // process mouse dragging motion
    glutKeyboardFunc(processNormalKeys); // process standard key clicks
    glutSpecialFunc(pressSpecialKey); // process special key pressed
                        // Warning: Nonstandard function! Delete if desired.
    glutSpecialUpFunc(releaseSpecialKey); // process special key release

    // OpenGL init
    glEnable(GL_DEPTH_TEST);

	// Lighting Initialization
	glEnable(GL_LIGHTING);
	GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light0_specular[] = { 0.8, 0.8, 0.8, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glEnable(GL_LIGHT0);

	GLfloat light1_position[] = {-5, -5, -8};
	GLfloat light1_diffuse[] = {1, 1, 1, 1};
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glEnable(GL_LIGHT1);

	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0; // this is just to keep the compiler happy
}
