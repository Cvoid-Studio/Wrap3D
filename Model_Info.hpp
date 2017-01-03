/* Copyright @ Cvoid Studio 2016. All Right Are Reserved.*/
#pragma once
#ifndef  MODEL_INFO_
#define  MODEL_INFO_
/* --------------------------------------------- /
					INCLUDES
/ --------------------------------------------- */
#include "Wrap3D_base.hpp"

/* --------------------------------------------- /
					 MACROS
/ --------------------------------------------- */

using std::string;
/* --------------------------------------------- /
					 CLASS
/ --------------------------------------------- */
class wp_Model
{
	// ----------- Class Methods ----------- //
public:
	wp_Model();
	wp_Model(string infile, string outfile);
	~wp_Model();

	int	ImportFiberDir(char* filename, wp_Model* Target);		// ==> Import Function
	int	WriteFiberDir(char* filename, int lines);				// ==> Output Function
																// ------ Basic Data: Orientations ------//
	wp_Model* f_Triangles;
	wp_Model DeleteBox[CutIndex];
private:
	wp_Model Indexes[MaxIndex];
	bool CutCri[MaxIndex];
	unsigned Work_Edge[2][3];
	unsigned Comp_Edge[2][3];

	m_Edge Edges[MaxVer][MaxNeigh];		// RCF [Pivot][Info]

	unsigned Vertices[MaxVer][2];
	//char tempstr[16];
	// ------------------------------------- //
public:
	void Duplicate(char Switch_);
	void CheckOrientation(char Switch_);
	void CheckTopology();
	void GarbageFace();
	void GetNeigh();
	void DeleteFaceFrom(FiberDir*);

	// ------------------------------------- //
private:
	inline unsigned int Minus_Ori();
	void GetEdges();
	void GetEdgeInfo(unsigned x, unsigned y);


	// -------------------------------------- //
private:
	//-------------------------------------//
	unsigned tempIndex1, tempIndex2;
	unsigned Look_up;

	//-------------------------------------//
private:
	_File_Format_	Format_In, Format_Out;
};

// Constructer & Destructer
// =============================================== //
wp_Model::wp_Model()
{
	wp_Model* f_Triangles = new wp_Model;
}

wp_Model::wp_Model(string infile, string outfile)
{

	wp_Model* f_Triangles = new wp_Model;
}

wp_Model::~wp_Model()
{
	Clear(f_Triangles);
}


// File I/O
// =============================================== //
int wp_Model::ImportFiberDir(char * filename, wp_Model * Target)
{

	return 0;
}
#endif // ! MODEL_INFO_
