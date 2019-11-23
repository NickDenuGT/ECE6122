/*
Author: Nicholas Denu
Class: ECE6122
Last Date Modified: 11/06/19

Description:

This file's purpose is to supplement the .cpp file associated with it
by including different .h files, housing global variable statements, 
defining functions, and defining structs.
*/

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "mpi.h"
#include <vector>

#define ACTIVE 1
#define HOVERING 2

#define COLL_DIST 0.01
#define	HOVER_DIST 10
#define SHIP_MASS 1
#define NUM_DRONES 15
#define SENDCNT 11

using namespace std;

int time_limit, max_velo = 2;
int collision_buffer[NUM_DRONES];
int docking_buffer[NUM_DRONES];
double send_buffer[SENDCNT];
double recv_buffer[(+1)*SENDCNT];
double x_rand, y_rand, z_rand;

// Camera position
float x = 4, y = -5.0, z = 10; // initially 5 units south of origin
float dx = 0, dy = 0, dz = 0;

struct drone_struct
{
	double id;
	double status;
	double x_coord;
	double y_coord;
	double z_coord;
	double x_velo;
	double y_velo;
	double z_velo;
	double x_thrust;
	double y_thrust;
	double z_thrust;
};

vector<drone_struct> drone_fleet;

void InitDroneLocation();

void CalcDroneXYZ(drone_struct*);
void CalcDroneVelo(buzz_struct *);

void PrintAllShips(double *, int);
void CheckForCollisions(double *, int *);
void CheckForDocks(double *, int *);

double CalcDistance(double, double, double, double, double, double);
double CalcMagnitude(double, double, double);
void ResetArrayToZeros(int *);

void renderScene();
void changeSize(int, int);
