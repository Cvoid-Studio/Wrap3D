/* Copyright @ Cvoid Studio 2016. All Right Are Reserved.*/

#pragma once
#ifndef  WRAP3D_BASE_HPP
#define  WRAP3D_BASE_HPP
/* --------------------------------------------- /
					INCLUDES
/ --------------------------------------------- */
#include <fstream>
#include <memory.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

/* --------------------------------------------- /
					 MACROS
/ --------------------------------------------- */
#define Clear(a) { delete a; a = NULL;}

/* --------------------------------------------- /
				 FILE SUPPORTED
/ --------------------------------------------- */
enum _File_Format_
{
	STL = 1,
	PLY = 2,
	OBJ = 3,
	//...
};

/* --------------------------------------------- /
				 BASIC STRUCTURE
/ --------------------------------------------- */
class wp_Vertex				// ------------- Vertex
{
public:
	wp_Vertex()
	{ Index = 0; x = 0; y = 0; z = 0; }

	wp_Vertex(unsigned SI, double Sx, double Sy, double Sz)
	{ Index = SI; x = Sx; y = Sy; z = Sz;}

	wp_Vertex(const wp_Vertex & Src)
	{ x = Src.x; y = Src.y; z = Src.z;}	// No overwriting Index

	unsigned Index;			// Index
	double	x, y, z;		// Cartesian Coordinates
};



class wp_Edge				// ------------- Edge
{
public:
	wp_Edge()
	{ memset(Vertices, NULL, sizeof(wp_Vertex) * 2);}

	wp_Edge(wp_Vertex V1, wp_Vertex V2)
	{
		Vertices[0] = V1; Vertices[0].Index = V1.Index;
		Vertices[1] = V2; Vertices[1].Index = V2.Index;
	}

public:
	wp_Vertex Vertices[2];	// Two Ends

	void Flip_Ends();

private:
	bool Flip = false;
};



typedef struct				// ------------- Tri-Face
{
	unsigned V1; 
	unsigned V2; 
	unsigned V3;			// Vertices
}wp_Triangle;

/* --------------------------------------------- /
					FUNCTIONS
/ --------------------------------------------- */
void wp_Edge::Flip_Ends()
{
	wp_Vertex _tempVer = Vertices[0];
	
	Vertices[0] = Vertices[1];
	Vertices[1] = _tempVer;

	Flip != Flip;

	return;
}
#endif // ! WRAP3D_BASE_HPP