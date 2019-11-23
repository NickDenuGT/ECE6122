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

#define ACTIVE 1
#define DOCKED 2
#define DESTROYED 0

#define COLL_DIST 250
#define	DOCK_DIST 50
#define SHIP_MASS 10000
#define NUM_YELLJACKS 7
#define SENDCNT 11

using namespace std;

int time_limit, max_thrust;
int collision_buffer[NUM_YELLJACKS];
int docking_buffer[NUM_YELLJACKS];
double send_buffer[SENDCNT];
double recv_buffer[(NUM_YELLJACKS+1)*SENDCNT];
double x_rand, y_rand, z_rand;
string input_file = "in.dat";

struct buzz_struct
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



buzz_struct buzzy;
buzz_struct buzz_fleet[NUM_YELLJACKS];

void LoadInputFile();

void InitShipVals(buzz_struct*, int);

void CalculateBuzzyXYZ(buzz_struct*);
void CalculateYellowJacketXYZ(buzz_struct*);
void CalculateYellowJacketVelocity(buzz_struct *);

void PrintAllShips(double *, int);
void CheckForCollisions(double *, int *);
void CheckForDocks(double *, int *);

double CalcDistance(double, double, double, double, double, double);
double CalcMagnitude(double, double, double);
void ResetArrayToZeros(int *);

int FindClosestToBuzzy(double *);
