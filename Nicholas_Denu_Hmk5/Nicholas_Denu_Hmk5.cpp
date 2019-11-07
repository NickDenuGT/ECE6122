/*
Author: Nicholas Denu
Class: ECE6122
Last Date Modified: 11/06/19

Description:

This file's purpose is to simulate a spaceship scenario. The scenario is thus, Battlestar Buzzy 
has just finished defending GaTech from the evil space bulldogs. It was a fierce
battle and only seven yellow jackets are still operational and need to dock as soon as possible.
The yellow jackets are all damaged, flying blind, and having propulsion issues. This program
uses MPI programming to communicate between multiple ships and attempts to guide the
yellow jackets safety back to the Buzzy so that they can dock.
*/

#include "Nicholas_Denu_Hmk5.h"

/*
	Main Function
	Purpose:	The main function begins the program and is later split
				into the several different MPI nodes, which act as ships.
				The main function will call multiple other functions to 
				do most of the logic required for the program.

	Inputs: argc - The number of command line inputs
			argv - The command line input array

	Outputs: int - Returns 0 in this case to indicate a successful run
*/
int main(int argc, char**argv)
{
    int  numtasks, rank, rc;
    rc = MPI_Init(&argc, &argv);
	
	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

    if (rc != MPI_SUCCESS)
    {
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Seed the random number generator to get different results each time
    srand(rank);
    if (rank == 0)
    {
        LoadInputFile();
    }

    // Broadcast all loaded values to yellowjackets
    MPI_Bcast(&buzz_fleet, NUM_YELLJACKS * 12, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&time_limit, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max_thrust, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	int closest_yellow_jacket_index = 0;
	double x_mult = 0, y_mult = 0, z_mult = 0, max_velo;
	double x_thrust, y_thrust, z_thrust;

    // Loop through the number of time steps
    for (int round = 0; round < time_limit; ++round)
    {	
		if (rank == 0)
		{
			send_buffer[0] = 0;
			send_buffer[1] = 1;
			send_buffer[2] = buzzy.x_coord;
			send_buffer[3] = buzzy.y_coord;
			send_buffer[4] = buzzy.z_coord;
			send_buffer[5] = buzzy.x_thrust;
			send_buffer[6] = buzzy.y_thrust;
			send_buffer[7] = buzzy.z_thrust;
			send_buffer[8] = buzzy.x_velo;
			send_buffer[9] = buzzy.y_velo;
			send_buffer[10] = buzzy.z_velo;

			//Update Location of Buzzy
			CalculateBuzzyXYZ(&buzzy);		
		 }
		 else
		 {
					send_buffer[0] = buzz_fleet[rank - 1].id;
		            send_buffer[1] = buzz_fleet[rank - 1].status;
		            send_buffer[2] = buzz_fleet[rank - 1].x_coord;
		            send_buffer[3] = buzz_fleet[rank - 1].y_coord;
		            send_buffer[4] = buzz_fleet[rank - 1].z_coord;
		            send_buffer[5] = buzz_fleet[rank - 1].x_thrust;
		            send_buffer[6] = buzz_fleet[rank - 1].y_thrust;
		            send_buffer[7] = buzz_fleet[rank - 1].z_thrust;
					send_buffer[8] = buzz_fleet[rank - 1].x_velo;
		            send_buffer[9] = buzz_fleet[rank - 1].y_velo;
		            send_buffer[10] = buzz_fleet[rank - 1].z_velo;

			x_rand = ((double)rand()) / ((double)RAND_MAX) * 0.4 + 0.8;
			y_rand = ((double)rand()) / ((double)RAND_MAX) * 0.4 + 0.8;
			z_rand = ((double)rand()) / ((double)RAND_MAX) * 0.4 + 0.8;


			// Update location of buzz_fleet
			CalculateYellowJacketXYZ(&(buzz_fleet[rank - 1]));
			
			// Update velocity of buzz_fleet		
			CalculateYellowJacketVelocity(&(buzz_fleet[rank - 1]));
			
		}
			// Update every MPI node on every other MPI's information
			MPI_Allgather(&send_buffer, SENDCNT, MPI_DOUBLE,
				&recv_buffer, SENDCNT, MPI_DOUBLE, MPI_COMM_WORLD);
		if (rank == 0)
		{
			//Go through the steps of checking for docks or collisions
			// Finding the closest yellow jacket allows us to activate 
			// that ship to try and dock on the Buzzy
			closest_yellow_jacket_index = FindClosestToBuzzy(recv_buffer);
			ResetArrayToZeros(collision_buffer);
			ResetArrayToZeros(docking_buffer);
			CheckForCollisions(recv_buffer, collision_buffer);
			CheckForDocks(recv_buffer, docking_buffer);
			PrintAllShips(recv_buffer, round);
		}
		MPI_Bcast(&closest_yellow_jacket_index, 1, MPI_INT, 0, MPI_COMM_WORLD);
		// Using the closest yellow jacket to Buzzy, we begin our
		// voyage to try and dock
		if(rank == closest_yellow_jacket_index + 1)
		{
			// thrust in direction of buzzy
			max_velo = CalcMagnitude(recv_buffer[8],
							recv_buffer[9],
							recv_buffer[10]) * 1.1;

			x_mult = (-1) * (buzz_fleet[rank - 1].x_coord - recv_buffer[2]) / 
									CalcDistance(recv_buffer[2],
												recv_buffer[3],
												recv_buffer[4],
												buzz_fleet[rank - 1].x_coord,
												buzz_fleet[rank - 1].y_coord,
												buzz_fleet[rank - 1].z_coord);
			y_mult = (-1) * (buzz_fleet[rank - 1].y_coord - recv_buffer[3]) / 
									CalcDistance(recv_buffer[2],
												recv_buffer[3],
												recv_buffer[4],
												buzz_fleet[rank - 1].x_coord,
												buzz_fleet[rank - 1].y_coord,
												buzz_fleet[rank - 1].z_coord);
			z_mult = (-1) * (buzz_fleet[rank - 1].z_coord - recv_buffer[4]) / 
									CalcDistance(recv_buffer[2],
												recv_buffer[3],
												recv_buffer[4],
												buzz_fleet[rank - 1].x_coord,
												buzz_fleet[rank - 1].y_coord,
												buzz_fleet[rank - 1].z_coord);
			x_thrust = max_thrust * x_mult;
			y_thrust = max_thrust * y_mult;
			z_thrust = max_thrust * z_mult;	

			// To assure we are not going to thrust past the maximum velocity that is acceptable
			//	we try several values for our thrust and decide upon one that is reasonable.
			for(int i = 1; i <= 100; i+=10)
			{
				if((buzz_fleet[rank - 1].x_velo + (x_thrust / i) / SHIP_MASS >= max_velo)	||
					(buzz_fleet[rank - 1].y_velo + (y_thrust / i) / SHIP_MASS >= max_velo)	||
					(buzz_fleet[rank - 1].z_velo + (z_thrust / i) / SHIP_MASS >= max_velo))
				{	
					buzz_fleet[rank - 1].x_thrust = 0;
					buzz_fleet[rank - 1].y_thrust = 0;
					buzz_fleet[rank - 1].z_thrust = 0;
				}
				else
				{
					// Found a reasonable thrust, let's now use it to fly to the Buzzy
					buzz_fleet[rank - 1].x_thrust = x_thrust / i;
					buzz_fleet[rank - 1].y_thrust = y_thrust / i;
					buzz_fleet[rank - 1].z_thrust = z_thrust / i;
					break;
				}
			}
		}
		
		// Update Yellow Jacket collision and docking information using the buffers
		// that have been returned from the Buzzy node
		MPI_Bcast(collision_buffer, NUM_YELLJACKS, MPI_INT, 0, MPI_COMM_WORLD);
		if(rank != 0)
		{
			if(collision_buffer[rank - 1] == 1)
			{
				buzz_fleet[rank - 1].status = DOCKED;
			}
		}
		MPI_Bcast(docking_buffer, NUM_YELLJACKS, MPI_INT, 0, MPI_COMM_WORLD);
		if(rank != 0)
		{
			if(docking_buffer[rank - 1] == 1)
			{
				buzz_fleet[rank - 1].status = DOCKED;
			}
			else if(docking_buffer[rank - 1] == 2)
			{
				buzz_fleet[rank - 1].status = DESTROYED;
			}
		}
    }


    MPI_Finalize();
}

/*
	LoadInputFile
	Purpose:	This function loads the input file: in.dat into the program.
				This function goes through each value in the .dat file and
				assigns it to it's proper location on various ships or in 
				global variables that have been set in the header file.

	Inputs: None

	Outputs: None
*/
void LoadInputFile()
{
	ifstream inFile;
	inFile.open("in.dat");
	if(!inFile)
	{
		cout << "Unable to open in.dat\n";
		exit(1);
	}
	double speed, x_vect, y_vect, z_vect;
	inFile >> time_limit;
	inFile >> max_thrust;
	inFile >> buzzy.x_coord >> buzzy.y_coord >> buzzy.z_coord;
	inFile >> speed;
	inFile >> x_vect >> y_vect >> z_vect;
	buzzy.x_velo = speed * x_vect;
	buzzy.y_velo = speed * y_vect;
	buzzy.z_velo = speed * z_vect;
	InitShipVals(&buzzy, 0);
	for(int i = 0; i < NUM_YELLJACKS; i++)
	{
        	inFile >> buzz_fleet[i].x_coord >> buzz_fleet[i].y_coord >> buzz_fleet[i].z_coord;
        	inFile >> speed;
        	inFile >> x_vect >> y_vect >> z_vect;
		buzz_fleet[i].x_velo = speed * x_vect;
        	buzz_fleet[i].y_velo = speed * y_vect;
        	buzz_fleet[i].z_velo = speed * z_vect;
		InitShipVals(&buzz_fleet[i], i+1);
	}
}

/*
	InitShipVals
	Purpose:	All values that aren't set from the in.dat file
				are set here to zero or the proper values if 
				they are known.

	Inputs: ship_ptr - A pointer to the ship that is being 	
						initialized.
			id - The id number for the ship

	Outputs: None
*/
void InitShipVals(buzz_struct *ship_ptr, int id)
{
	ship_ptr->id = id;
	ship_ptr->status = ACTIVE;
	ship_ptr->x_thrust = 0;
	ship_ptr->y_thrust = 0;
	ship_ptr->z_thrust = 0;
}

/*
	CalculateBuzzyXYZ
	Purpose:	The function takes the buzzy ship as long as it is active
				and will update the x, y, and z coordinates based on 
				their current location and the current velocity in each
				direction.

	Inputs: buzz_struct_ptr - A pointer to the Buzzy ship struct

	Outputs: None
*/
void CalculateBuzzyXYZ(buzz_struct *buzz_struct_ptr)
{
	if(buzz_struct_ptr->status != ACTIVE)
	{
		return;	
	}
	buzz_struct_ptr->x_coord = buzz_struct_ptr->x_coord + buzz_struct_ptr->x_velo;
	buzz_struct_ptr->y_coord = buzz_struct_ptr->y_coord + buzz_struct_ptr->y_velo;
	buzz_struct_ptr->z_coord = buzz_struct_ptr->z_coord + buzz_struct_ptr->z_velo;
}

/*
	CalculateYellowJacketXYZ
	Purpose:	The function takes the yellow jacket ship as long as it
			 	is active and will update the x, y, and z coordinates 
				based on their current location and the current velocity
				in each direction.

	Inputs: buzz_struct_ptr - A pointer to the Buzzy ship struct

	Outputs: None
*/
void CalculateYellowJacketXYZ(buzz_struct *buzz_struct_ptr)
{
	if(buzz_struct_ptr->status != ACTIVE)
	{
		return;	
	}
	buzz_struct_ptr->x_coord = buzz_struct_ptr->x_coord + buzz_struct_ptr->x_velo + (0.5) * buzz_struct_ptr->x_thrust * x_rand / SHIP_MASS;
		buzz_struct_ptr->y_coord = buzz_struct_ptr->y_coord + buzz_struct_ptr->y_velo + (0.5) * buzz_struct_ptr->y_thrust * y_rand / SHIP_MASS;
		buzz_struct_ptr->z_coord = buzz_struct_ptr->z_coord + buzz_struct_ptr->z_velo + (0.5) * buzz_struct_ptr->z_thrust * z_rand / SHIP_MASS;
}

/*
	CalculateYellowJacketVelocity
	Purpose:	The function takes the yellow jacket ship as long as it
			 	is active and will update the x, y, and z velocities 
				based on their current velocity and the current acceleration
				in each direction.

	Inputs: buzz_struct_ptr - A pointer to the Buzzy ship struct

	Outputs: None
*/
void CalculateYellowJacketVelocity(buzz_struct *buzz_struct_ptr)
{
	if(buzz_struct_ptr->status != ACTIVE)
	{
		return;	
	}
		buzz_struct_ptr->x_velo = buzz_struct_ptr->x_velo + (buzz_struct_ptr->x_thrust * x_rand / SHIP_MASS);
		buzz_struct_ptr->y_velo = buzz_struct_ptr->y_velo + (buzz_struct_ptr->y_thrust * y_rand / SHIP_MASS);
		buzz_struct_ptr->z_velo = buzz_struct_ptr->z_velo + (buzz_struct_ptr->z_thrust * z_rand / SHIP_MASS);
}

/*
	PrintAllShips
	Purpose:	This function prints the updated coordinates and thrust for 
				each ship as well as the current time or round.

	Inputs: recv_buffer - The received buffer from the MPI_Allgather call
							This has all the information for all of the
							ships in it.
			time - The current time or round

	Outputs: None
*/
void PrintAllShips(double *recv_buffer, int time)
{
	cout << "Time: " << time << "\n";
	for(int i = 0; i < NUM_YELLJACKS; i++)
	{
		// The indices in the following print correspond to the proper 
		// values that are required to be printed out for each ship
		printf("%d, %d, %8.6e, %8.6e, %8.6e, %8.6e, %8.6e, %8.6e\n",
			(int)recv_buffer[SENDCNT * (i + 1)],
                        (int)recv_buffer[SENDCNT * (i + 1) + 1],
                        recv_buffer[SENDCNT * (i + 1) + 2],
                        recv_buffer[SENDCNT * (i + 1) + 3],
                        recv_buffer[SENDCNT * (i + 1) + 4],
                        recv_buffer[SENDCNT * (i + 1) + 5],
                        recv_buffer[SENDCNT * (i + 1) + 6],
                        recv_buffer[SENDCNT * (i + 1) + 7]);
	}	
	cout << "\n\n"; 
}

/*
	CheckForCollisions
	Purpose:	This function checks to make sure no two ships
				have collided into eachother based on their 
				distances

	Inputs: recv_buffer - The received buffer from the MPI_Allgather call
							This has all the information for all of the
							ships in it.
			collision_buffer - The buffer that will contain all of 
								the collision occurences as ones
								in the index that corresponds to 
								the ship index 

	Outputs: None
*/
void CheckForCollisions(double *recv_buffer, int *collision_buffer)
{
	for(int i = 0; i < NUM_YELLJACKS; i++)
	{
		for(int j = i + 1; j < NUM_YELLJACKS; j++)
		{
			if(CalcDistance(recv_buffer[SENDCNT * (i + 1) + 2],
							recv_buffer[SENDCNT * (i + 1) + 3],
							recv_buffer[SENDCNT * (i + 1) + 4],
							recv_buffer[SENDCNT * (j + 1) + 2],
							recv_buffer[SENDCNT * (j + 1) + 3],
							recv_buffer[SENDCNT * (j + 1) + 4]) < COLL_DIST)
			{
				collision_buffer[i] = 1;
				collision_buffer[j] = 1;
			}
		}
	}
}

/*
	CheckForDocks
	Purpose:	This function checks for any ship that has the proper docking
				parameters met and tells the main function, through the 
				docking buffer, that the ship is docked or destroyed based
				on the situation.

	Inputs: recv_buffer - The received buffer from the MPI_Allgather call
							This has all the information for all of the
							ships in it.
			docking_buffer - The buffer that will contain all of 
								the docking occurences as ones
								in the index that corresponds to 
								the ship index 

	Outputs: None
*/
void CheckForDocks(double *recv_buffer, int *docking_buffer)
{
	for(int i = 0; i < NUM_YELLJACKS; i++)
	{
		if(CalcDistance(recv_buffer[SENDCNT * (i + 1) + 2],
						recv_buffer[SENDCNT * (i + 1) + 3],
						recv_buffer[SENDCNT * (i + 1) + 4],
						recv_buffer[2],
						recv_buffer[3],
						recv_buffer[4]) < DOCK_DIST)
		{
			if(cos((buzzy.x_velo * recv_buffer[SENDCNT * (i + 1) + 8] + 
					buzzy.y_velo * recv_buffer[SENDCNT * (i + 1) + 9] + 
					buzzy.z_velo * recv_buffer[SENDCNT * (i + 1) + 10]) /
					(CalcMagnitude(buzzy.x_velo, buzzy.y_velo, buzzy.z_velo) * 
					CalcMagnitude(recv_buffer[SENDCNT * (i + 1) + 8],
									recv_buffer[SENDCNT * (i + 1) + 9],
									recv_buffer[SENDCNT * (i + 1) + 10]))) < 0.8)
			{
				
				if(CalcMagnitude(buzzy.x_velo, buzzy.y_velo, buzzy.z_velo) / 
				CalcMagnitude(recv_buffer[SENDCNT * (i + 1) + 8],
									recv_buffer[SENDCNT * (i + 1) + 9],
									recv_buffer[SENDCNT * (i + 1) + 10]) < 1.1)
				{
					docking_buffer[i] = 1;
				}
				else
				{
					docking_buffer[i] = 2;
				}
			}
		}
	}
}

/*
	FindClosestToBuzzy
	Purpose:	This function uses the distances between Buzzy
				and each yellow jacket to determine which 
				yellow jacket is the closest to Buzzy and will
				get the first go at getting to buzzy.

	Inputs: recv_buffer - The received buffer from the MPI_Allgather call
							This has all the information for all of the
							ships in it.

	Outputs: int - Returns the yellow jacket index for the closest 
					yellow jacket.
*/
int FindClosestToBuzzy(double *recv_buffer)
{
	int low_index = -1;
	double low_val = 100000000000000;
	for(int i = 0; i < NUM_YELLJACKS; i++)
	{
		if(recv_buffer[SENDCNT * (i + 1) + 1] != 1)
		{
			// do nothing
		}
		else if(CalcDistance(recv_buffer[SENDCNT * (i + 1) + 2],
						recv_buffer[SENDCNT * (i + 1) + 3],
						recv_buffer[SENDCNT * (i + 1) + 4],
						recv_buffer[2],
						recv_buffer[3],
						recv_buffer[4]) < low_val)
		{
			low_val = CalcDistance(recv_buffer[SENDCNT * (i + 1) + 2],
						recv_buffer[SENDCNT * (i + 1) + 3],
						recv_buffer[SENDCNT * (i + 1) + 4],
						recv_buffer[2],
						recv_buffer[3],
						recv_buffer[4]);
			low_index = i;
		}
	}
	return low_index;
}

/*
	CalcDistance
	Purpose:	This function takes two 3D vectors and finds the distance
				between them using the distance formula

	Inputs: x1 - The first ship's x coordinate
			y1 - The first ship's y coordinate
			z1 - The first ship's z coordinate
			x2 - The second ship's x coordinate
			y2 - The second ship's y coordinate
			z2 - The second ship's z coordinate
			
	Outputs: double - The distance calculated
*/
double CalcDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}

/*
	CalcMagnitude
	Purpose:	This function calculates the magnitude of a 3D vector
				such as the velocity vectors of a ship to find the 
				total speed

	Inputs: x1 - The ship's x velocity
			y1 - The ship's y velocity
			z1 - The ship's z velocity

	Outputs: double - The magnitude calculated
*/
double CalcMagnitude(double x1, double y1, double z1)
{
	return sqrt(pow(x1, 2) + pow(y1, 2) + pow(z1, 2));
}

/*
	ResetArrayToZeros
	Purpose:	This function returns all of the values in an array of 
				size NUM_YELLJACKS to 0

	Inputs: array - The array that needs the values to be returned to
					zero

	Outputs: None
*/
void ResetArrayToZeros(int *array)
{
	for(int i = 0; i < NUM_YELLJACKS; i++)
	{
		array[i] = 0;
	}
}