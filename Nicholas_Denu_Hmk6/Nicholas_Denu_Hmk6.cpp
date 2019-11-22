/*
Author: Nicholas Denu
Class: ECE6122
Last Date Modified: 11/21/19

Description: This program creates a 3D creation of a chess board.
		There is also the ability to move the height of the viewer,
		and rotation of the board. 
*/

// Includes
#include <iostream>
#include <GL/glut.h> // Include the GLUT header file
#include <stdio.h>
#include <math.h>
#include <stdlib.h> // standard definitions
#include <vector>

// --------------------------------------------------------------------
// Defines
//
// Values that will be assigned to the chess piece structs
// as well as a few values needed throughout 

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
float distToCenter = 9.0;

// Camera direction
//float lx = -4.0, ly = 5.0, lz = -10;// camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging
float angleAroundZ = -90.0;

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

// Lighting ON/OFF
bool light0 = true;
bool light1 = true;

GLint gblSlices = 100;
GLint gblStacks = 100;
GLdouble gblRadius = 0.375;
GLdouble gblHeight = 1.0;

bool fancyMode = false;

struct moveStruct
{
	int dx;
	int dy;
};

// Used as a struct for every chess piece
struct simplePiece
{
	int pieceType;
	float red;
	float green;
	float blue;
	GLdouble xCoord;
	GLdouble yCoord;
	int xTile;
	int yTile;
	GLdouble zCoord;
};

vector<moveStruct> knightMoves;
vector<moveStruct> pawnMoves;
vector<simplePiece> chessPieces;
vector<vector<bool>> boardOccupations;

//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
// Input: w - width
//	  h - height
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
	

	GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matShininess[] = { 50.0 };

    for (int i = 0; i < chessPieces.size(); i++)
    {
		//glClearColor(0, 0, 0, 0);
		glPushMatrix();
    	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

		switch(chessPieces.at(i).pieceType)
		{
			case ROOK:
					glColor3f(chessPieces.at(i).red / 255,
								chessPieces.at(i).green / 255,
								chessPieces.at(i).blue / 255);
					glTranslatef(chessPieces.at(i).xCoord,
								chessPieces.at(i).yCoord,
								chessPieces.at(i).zCoord);
					glutSolidCube(gblRadius * 2);
				break;
			case KNIGHT:
					glColor3f(chessPieces.at(i).red / 255,
								chessPieces.at(i).green / 255,
								chessPieces.at(i).blue / 255);
					glTranslatef(chessPieces.at(i).xCoord,
								chessPieces.at(i).yCoord,
								chessPieces.at(i).zCoord);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					glutSolidTeapot(gblRadius);
				break;
			case BISHOP:
					glColor3f(chessPieces.at(i).red / 255,
								chessPieces.at(i).green / 255,
								chessPieces.at(i).blue / 255);
					glTranslatef(chessPieces.at(i).xCoord,
								chessPieces.at(i).yCoord,
								chessPieces.at(i).zCoord);
					glutSolidCone(gblRadius, gblHeight, gblSlices, gblStacks);
				break;
			case QUEEN:
					glColor3f(chessPieces.at(i).red / 255,
								chessPieces.at(i).green / 255,
								chessPieces.at(i).blue / 255);
					glTranslatef(chessPieces.at(i).xCoord,
								chessPieces.at(i).yCoord,
								chessPieces.at(i).zCoord);
					glScalef((0.75/sqrt(3)), (0.75/sqrt(3)), (0.75/sqrt(3)));
					glutSolidTetrahedron();
				break;
			case KING:
					glTranslatef(chessPieces.at(i).xCoord,
								chessPieces.at(i).yCoord,
								chessPieces.at(i).zCoord);
					glScalef(0.375, 0.375, 0.375);
					glColor3f(chessPieces.at(i).red / 255,
								chessPieces.at(i).green / 255,
								chessPieces.at(i).blue / 255);
					glutSolidOctahedron();
				break;
			case PAWN:
					glColor3f(chessPieces.at(i).red / 255,
								chessPieces.at(i).green / 255,
								chessPieces.at(i).blue / 255);
					glTranslatef(chessPieces.at(i).xCoord,
								chessPieces.at(i).yCoord,
								chessPieces.at(i).zCoord);
					glutSolidSphere(gblRadius, gblSlices, gblStacks);
				break;
		}
		glPopMatrix();
    }
    glutSwapBuffers(); // Make it all visible
}

//----------------------------------------------------------------------
// 
// processNormalKeys: ESC, q, and Q cause program to exit,
//			r/R rotates the board by 10 degrees
//			d/D drops the viewer along the z-axis
//			u/P ups the viewer along the z-axis
//			0 toggles the LIGHT_0
//			1 toggles the LIGHT_1
//			k/K moves a random knight
//			p/P moves a random pawn
// 
// Input: key - the key pressed by the user
//	  xx - required input
//        yy - required input
//----------------------------------------------------------------------
void processNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == ESC || key == 'q' || key == 'Q')
    {
        exit(0);
    }
	else if(key == 'r' || key == 'R')
	{
		angleAroundZ += 10;
		x = (distToCenter * cos(angleAroundZ * M_PI / 180.0)) + 4;
		y = (distToCenter * sin(angleAroundZ * M_PI / 180.0)) + 4;
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
		int randK, kIndex, targetX, targetY;
		bool success = false;
		for(int i = 0; i < 100; i++)
		{
			randK = rand() % 4;
			switch(randK)
			{
				case 0:
					kIndex = 1;
					break;
				case 1:
					kIndex = 6;
					break;
				case 2:
					kIndex = 17;
					break;
				case 3:
					kIndex = 22;
					break;
			}
			for(int j = 0; j < knightMoves.size(); j++)
			{
				targetX = chessPieces.at(kIndex).xTile + knightMoves.at(j).dx;
				targetY = chessPieces.at(kIndex).yTile + knightMoves.at(j).dy;
				if(targetX > 7 || targetX < 0 || targetY > 7 || targetY < 0)
				{
					success = false;
				}
				else if(! boardOccupations.at(targetY).at(targetX))
				{
					boardOccupations.at(targetY).at(targetX) = true;
					boardOccupations.at(chessPieces.at(kIndex).yTile).
							at(chessPieces.at(kIndex).xTile) = false;
					chessPieces.at(kIndex).xCoord = (float)(targetX + 0.5);
					chessPieces.at(kIndex).yCoord = (float)(targetY + 0.5);
					chessPieces.at(kIndex).xTile = targetX;
					chessPieces.at(kIndex).yTile = targetY;
					success = true;
					break;
				}	
			}
			if(success){break;}
		}
	}
	else if(key == 'p' || key == 'P')
	{
		int randK, kIndex, targetX, targetY;
		bool success = false;
		for(int i = 0; i < 100; i++)
		{
			randK = rand() % 16;
			if(randK < 8)
			{
				kIndex = randK + 8;
			}
			else
			{
				kIndex = randK + 16;
			}
			for(int j = 0; j < pawnMoves.size(); j++)
			{
				targetX = chessPieces.at(kIndex).xTile + pawnMoves.at(j).dx;
				targetY = chessPieces.at(kIndex).yTile + pawnMoves.at(j).dy;
				if(targetX > 7 || targetX < 0 || targetY > 7 || targetY < 0)
				{
					success = false;
				}
				else if(! boardOccupations.at(targetY).at(targetX))
				{
					boardOccupations.at(targetY).at(targetX) = true;
					boardOccupations.at(chessPieces.at(kIndex).yTile).
							at(chessPieces.at(kIndex).xTile) = false;
					chessPieces.at(kIndex).xCoord = (float)(targetX + 0.5);
					chessPieces.at(kIndex).yCoord = (float)(targetY + 0.5);
					chessPieces.at(kIndex).xTile = targetX;
					chessPieces.at(kIndex).yTile = targetY;
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

//----------------------------------------------------------------
//
// chessPieceLocationInit()
//
// This function initializes all of the chess pieces, where they go
// their color, and the type of piece.
//
void chessPieceLocationInit()
{
	chessPieces.push_back({ROOK, 	255, 255, 240, 0.5, 0.5, 0, 0, 0.375});
	chessPieces.push_back({KNIGHT, 255, 255, 240, 1.5, 0.5, 1, 0, 0.265});
	chessPieces.push_back({BISHOP, 255, 255, 240, 2.5, 0.5, 2, 0, 0});
	chessPieces.push_back({QUEEN, 	255, 255, 240, 3.5, 0.5, 3, 0, 0.255});
	chessPieces.push_back({KING, 	255, 255, 240, 4.5, 0.5, 4, 0, 0.375});
	chessPieces.push_back({BISHOP, 255, 255, 240, 5.5, 0.5, 5, 0, 0});
	chessPieces.push_back({KNIGHT, 255, 255, 240, 6.5, 0.5, 6, 0, 0.265});
	chessPieces.push_back({ROOK, 	255, 255, 240, 7.5, 0.5, 7, 0, 0.375});
	chessPieces.push_back({PAWN, 	255, 255, 240, 0.5, 1.5, 0, 1, 0.375});
	chessPieces.push_back({PAWN, 	255, 255, 240, 1.5, 1.5, 1, 1, 0.375});
	chessPieces.push_back({PAWN, 	255, 255, 240, 2.5, 1.5, 2, 1, 0.375});
	chessPieces.push_back({PAWN, 	255, 255, 240, 3.5, 1.5, 3, 1, 0.375});
	chessPieces.push_back({PAWN, 	255, 255, 240, 4.5, 1.5, 4, 1, 0.375});
	chessPieces.push_back({PAWN, 	255, 255, 240, 5.5, 1.5, 5, 1, 0.375});
	chessPieces.push_back({PAWN, 	255, 255, 240, 6.5, 1.5, 6, 1, 0.375});
	chessPieces.push_back({PAWN, 	255, 255, 240, 7.5, 1.5, 7, 1, 0.375});

	chessPieces.push_back({ROOK, 	150, 75, 0, 0.5, 7.5, 0, 7, 0.375});
	chessPieces.push_back({KNIGHT, 150, 75, 0, 1.5, 7.5, 1, 7, 0.265});
	chessPieces.push_back({BISHOP, 150, 75, 0, 2.5, 7.5, 2, 7, 0});
	chessPieces.push_back({QUEEN, 	150, 75, 0, 3.5, 7.5, 3, 7, 0.255});
	chessPieces.push_back({KING, 	150, 75, 0, 4.5, 7.5, 4, 7, 0.375});
	chessPieces.push_back({BISHOP, 150, 75, 0, 5.5, 7.5, 5, 7, 0});
	chessPieces.push_back({KNIGHT, 150, 75, 0, 6.5, 7.5, 6, 7, 0.265});
	chessPieces.push_back({ROOK, 	150, 75, 0, 7.5, 7.5, 7, 7, 0.375});
	chessPieces.push_back({PAWN, 	150, 75, 0, 0.5, 6.5, 0, 6, 0.375});
	chessPieces.push_back({PAWN, 	150, 75, 0, 1.5, 6.5, 1, 6, 0.375});
	chessPieces.push_back({PAWN, 	150, 75, 0, 2.5, 6.5, 2, 6, 0.375});
	chessPieces.push_back({PAWN, 	150, 75, 0, 3.5, 6.5, 3, 6, 0.375});
	chessPieces.push_back({PAWN, 	150, 75, 0, 4.5, 6.5, 4, 6, 0.375});
	chessPieces.push_back({PAWN, 	150, 75, 0, 5.5, 6.5, 5, 6, 0.375});
	chessPieces.push_back({PAWN, 	150, 75, 0, 6.5, 6.5, 6, 6, 0.375});
	chessPieces.push_back({PAWN, 	150, 75, 0, 7.5, 6.5, 7, 6, 0.375});

	vector<bool> tempVect;
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(i == 0 || i == 1 || i == 6 || i ==7)
			{
				tempVect.push_back(true);
			}
			else
			{
				tempVect.push_back(false);
			}
		}
		boardOccupations.push_back(tempVect);
		tempVect.clear();
	}
	knightMoves.push_back({-2, 1});
	knightMoves.push_back({-1, 2});
	knightMoves.push_back({1, 2});
	knightMoves.push_back({2, 1});
	knightMoves.push_back({2, -1});
	knightMoves.push_back({1, -2});
	knightMoves.push_back({-1, -2});
	knightMoves.push_back({-2, -1});

	pawnMoves.push_back({0, 1});
	pawnMoves.push_back({0, -1});
}

//----------------------------------------------------------------------
// Main program
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
	GLfloat light0Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light0Specular[] = { 0.8, 0.8, 0.8, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
	glEnable(GL_LIGHT0);

	GLfloat light1Position[] = {-5, -5, -8};
	GLfloat light1Diffuse[] = {1, 1, 1, 1};
	glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
	glEnable(GL_LIGHT1);

	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0; // this is just to keep the compiler happy
}
