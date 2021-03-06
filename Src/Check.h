#pragma once
#include <fstream>
#include <memory.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>

#ifndef FIBER_H_
#define FIBER_H_
#define MaxVer		23556
//1288176	//23556
#define MaxFaces	47128
//2576795   //47128
#define CutIndex	74
#define MaxNeigh	64

#define BoundL	250
#define BoundR	450
#define BoundU	560
#define BoundD	360
#define DeltaX	200
#define DeltaY	200

#define Max_Divide		1000
#define Delta_Length	0.2
#define Max_Layer		32

#define Max_Node	Max_Divide * Max_Divide
// ===================================== //
// ============= DataTypes ============= //
// ===================================== //

typedef struct
{
	unsigned x;
	unsigned y;
	unsigned z;
}FiberFace;

struct m_Edge
{
	unsigned End_Ver;
	unsigned Neigh_Faces;
};

typedef unsigned int FiberIndex;

typedef struct
{
	double x, y, z;
} FiberVer;

typedef struct
{
	bool Connect = 0;
	int LayerZ = 0;
}Connect_To;

typedef struct
{
	double z;
	int	Ind_Face; // Original Tri that is located on
	Connect_To Neigh[6] = { 0 }; // E W S N D U
} Grid_node;

typedef struct
{
	double x, y;
	int Layers = 0;	// Num of Layers at this grid now
	Grid_node Nodes[Max_Layer];
} Scan_Grid;

// ===================================== //
// ============= Fucntions ============= //
// ===================================== //
class FiberStruct
{
// ----------- Class Methods ----------- //
public:
	FiberStruct();
	~FiberStruct();

	FiberFace OutDir(unsigned int flag) 
			{ return DataFace[flag %= MaxFaces]; }	// ==> OutPut Function
	
	int	ImportFiberDir(const char* filename, FiberFace* Target);		// ==> Import Function
	int	ImportFiberVer(const char* filename, FiberVer* Target);

	// Use the Grids to print out
	int	WriteFiberDir(const char* filename, FiberFace* Src);			// ==> Output Function
	int	WriteFiberVer(const char* filename, Scan_Grid* Src);			// ==> Output Function

	int WritePLY(const char* filename);
// ------ Basic Data: Orientations ------//
	// For Input
	FiberFace DataFace[MaxFaces];
	FiberVer DataVer[MaxVer];
	// For writing
	FiberFace OutFace[MaxFaces];
	Scan_Grid OutVer[Max_Node];
	// For delete
	FiberFace DeleteBox[CutIndex];

private:	
	FiberIndex Indexes[MaxFaces];
	bool CutCri[MaxFaces];
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
	void DeleteFaceFrom(FiberFace*);

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
	Wrap3D wp_Algorithm;
};

class Wrap3D		// Class of Wrapping Algorithms
{
public:
	Wrap3D();
	Wrap3D(const FiberFace * SrcFace, 
		   FiberFace *const OutFace);
	~Wrap3D();
private:
	const FiberFace** DataFace;
	FiberFace *const* Res_Face;	// Final Result

public:
	void Initial_Scanner(FiberVer* PlaneGrid, const int Num_divide[2], const double Bound[4]);
	// L R D U -- RH coordinate

	void ScanModel(FiberFace* SrcFace,FiberVer* SrcVer, Scan_Grid* Target);

	bool Compare_Connect(Scan_Grid*);

	// =================================================== //
private:
	void Connect_Nodes(Grid_node Target,
		Scan_Grid* RefGridE, Scan_Grid* RefGridW, Scan_Grid* RefGridS, Scan_Grid* RefGridN,
		/*Grid_node* RefNodeU,*/ Grid_node* RefNodeD); // E W S N |D U|
	void Connect_Nodes(Grid_node Target,
		Scan_Grid* RefGridE, Scan_Grid* RefGridW, Scan_Grid* RefGridS, Scan_Grid* RefGridN,
		Grid_node* RefNodeU, Grid_node* RefNodeD); // E W S N |D U|

	bool find_Path(Grid_node TarNode, Grid_node RefNode,
				   const FiberFace** DataFace, int Switch = 1);

	bool compare_Edges(const FiberFace* Face_1, const FiberFace* Face_2);

	double Get_Z(const FiberVer Src1, const FiberVer Src2, const FiberVer Src3, const double x, const double y);

	bool Inside_Tri(const FiberVer Src1, const FiberVer Src2, const FiberVer Src3, const double x, const double y);

};

#endif