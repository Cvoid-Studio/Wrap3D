#include "Check.h"
#include <cmath>

Wrap3D::Wrap3D()
{
}

Wrap3D::Wrap3D(const FiberFace * SrcFace, FiberFace *const OutFace)
{
	DataFace = &SrcFace;
	Res_Face = &OutFace;
}

Wrap3D::~Wrap3D()
{
}

// View xy-plane
void Wrap3D::Initial_Scanner(FiberVer * PlaneGrid, const int Num_divide[2], const double Bound[4])
{
	// Initailize the Grids
	for (int i = 0; i < Num_divide[0]; ++i)
		PlaneGrid[i].x = i*((Bound[1] - Bound[0]) / Num_divide[0]);

	for (int i = 0; i < Num_divide[1]; ++i)
		PlaneGrid[i].y = i*((Bound[3] - Bound[2]) / Num_divide[1]);

	return;
}


void Wrap3D::ScanModel(FiberFace* SrcFace, FiberVer* SrcVer, Scan_Grid* Target)
{
	for (unsigned i = 0; i < MaxFaces; ++i) // Original Data -> Cover Grid
	{
		unsigned tPt1 = SrcFace[i].x;
		unsigned tPt2 = SrcFace[i].y;
		unsigned tPt3 = SrcFace[i].z;
		double L, R, U, D;
		// --------- X Bound ----------- //
		if (SrcVer[tPt1].x > SrcVer[tPt2].x)
		{
			L = SrcVer[tPt2].x;
			R = SrcVer[tPt1].x;
		}
		else
		{
			L = SrcVer[tPt1].x;
			R = SrcVer[tPt2].x;
		}

		if (SrcVer[tPt3].x > R)
			R = SrcVer[tPt3].x;
		else if (SrcVer[tPt3].x < L)
			L = SrcVer[tPt3].x;
		// --------- Y Bound ----------- //
		if (SrcVer[tPt1].y > SrcVer[tPt2].y)
		{
			D = SrcVer[tPt2].y;
			U = SrcVer[tPt1].y;
		}
		else
		{
			D = SrcVer[tPt1].y;
			U = SrcVer[tPt2].y;
		}

		if (SrcVer[tPt3].y > U)
			U = SrcVer[tPt3].y;
		else if (SrcVer[tPt3].y < D)
			D = SrcVer[tPt3].y;

		int Bd_L, Bd_R, Bd_U, Bd_D;
		Bd_L = static_cast<int>((L - BoundL) / (Delta_Length)) + 1;
		Bd_R = static_cast<int>((R - BoundL) / (Delta_Length));
		Bd_D = static_cast<int>((D - BoundD) / (Delta_Length)) + 1;
		Bd_U = static_cast<int>((U - BoundD) / (Delta_Length));

		// ------- Fill the Grid ------- //
		for (int m = Bd_L; m <= Bd_R; ++m)
		{
			for (int n = Bd_D; n <= Bd_U; ++n)
			{
				if (Inside_Tri(SrcVer[tPt1], SrcVer[tPt2], SrcVer[tPt3],
					(double)m, (double)n))
				{
					Scan_Grid* ptGrid = 
						&Target[n*Max_Divide + m];
					Grid_node* ptNode = 
						&Target[n*Max_Divide + m].Nodes[Target[n*Max_Divide + m].Layers];
					// -- Node -- //
					ptNode->z = Get_Z(SrcVer[tPt1], SrcVer[tPt2], SrcVer[tPt3],
									  ptGrid->x, ptGrid->y);
					ptNode->Ind_Face = i;

					// -- Grid -- //
					ptGrid->Layers++;
				}
				else break;
			}
		}
	}

	return;
}

bool Wrap3D::Compare_Connect(Scan_Grid* Tar_Grid)
{
	// Fill the info <bool>Neigh[6] of each node //
	for (unsigned m = 0; m < Max_Divide; ++m)
	{
		for (unsigned n = 0; n < Max_Divide; ++n)
		{
			Scan_Grid* Target = &Tar_Grid[n*Max_Divide + m];
			if (Target->Layers >= Max_Layer)
			{
				std::cerr << "Need more layers!\n\n";
				return false;
			}
			else
			{
				Scan_Grid* Grid_E = &Tar_Grid[n*Max_Divide + m + 1];
				Scan_Grid* Grid_W = &Tar_Grid[n*Max_Divide + m - 1];
				Scan_Grid* Grid_S = &Tar_Grid[(n + 1)*Max_Divide + m];
				Scan_Grid* Grid_N = &Tar_Grid[(n - 1)*Max_Divide + m];

				int Lay_in;
				for (Lay_in = 0; Lay_in < (Target->Layers - 1); ++Lay_in)
				{
					Grid_node* Node_U = &Target->Nodes[Lay_in + 1];
					Grid_node* Node_D = &Target->Nodes[Lay_in - 1];
					Connect_Nodes(Target->Nodes[Lay_in], 
								  Grid_E, Grid_W, Grid_S, Grid_N,
								  Node_U, Node_D);
				}
				Lay_in++; // Top
				{
					// Grid_node* Node_U = &Target->Nodes[Lay_in]; // Useless
					Grid_node* Node_D = &Target->Nodes[Lay_in - 1];
					Connect_Nodes(Target->Nodes[Lay_in],
								  Grid_E, Grid_W, Grid_S, Grid_N,
								  Node_D);
				}
			}
			
		}
	}
	return true;
}

void Wrap3D::Connect_Nodes(Grid_node Target, Scan_Grid * RefGridE, Scan_Grid * RefGridW, Scan_Grid * RefGridS, Scan_Grid * RefGridN, Grid_node * RefNodeD)
{
	// -------------- East -------------- // Neigh[0]
	for (int i = 0; i < RefGridE->Layers; ++i)
	{
		if (!Target.Neigh[0].Connect)	 // Skip the one've been refered
										 /*static_cast<bool>*/
		if (Target.Neigh[0].Connect = find_Path(Target, RefGridE->Nodes[i], DataFace))
		{
			Target.Neigh[0].LayerZ = i;
			break;
		}	
	}

	// -------------- West -------------- // Neigh[1]
	for (int i = 0; i < RefGridW->Layers; ++i)
	{
		if (!Target.Neigh[1].Connect)   // Skip the one've been refered
										/*static_cast<bool>*/
		if (Target.Neigh[1].Connect = find_Path(Target, RefGridW->Nodes[i], DataFace))
		{
			Target.Neigh[1].LayerZ = i;
			break;
		}
	}

	// -------------- South ------------- // Neigh[2]
	for (int i = 0; i < RefGridS->Layers; ++i)
	{
		if (!Target.Neigh[2].Connect)   // Skip the one've been refered
										/*static_cast<bool>*/
		if (Target.Neigh[2].Connect = find_Path(Target, RefGridS->Nodes[i], DataFace))
		{
			Target.Neigh[2].LayerZ = i;
			break;
		}
	}

	// -------------- North ------------- // Neigh[3]
	for (int i = 0; i < RefGridE->Layers; ++i)
	{
		if (!Target.Neigh[3].Connect) // Skip the one've been refered
									  /*static_cast<bool>*/
		if (Target.Neigh[3].Connect = find_Path(Target, RefGridN->Nodes[i], DataFace))
		{
			Target.Neigh[3].LayerZ = i;
			break;
		}
	}

	// -------------- Down -------------- // Neigh[4]
	if (!Target.Neigh[4].Connect) // Skip the one've been refered
									/*static_cast<bool>*/
		Target.Neigh[4].Connect = find_Path(Target, *RefNodeD, DataFace);

		//Target.Neigh[4].LayerZ = Target.Layer + 1; // No need

	return;
}

void Wrap3D::Connect_Nodes(Grid_node Target,
	Scan_Grid* RefGridE, Scan_Grid* RefGridW, Scan_Grid* RefGridS, Scan_Grid* RefGridN,
	Grid_node* RefNodeU, Grid_node* RefNodeD)
{
	// -------------- Others ------------ // Neigh[0 - 4]
	Connect_Nodes(Target, RefGridE, RefGridW, RefGridS, RefGridN, RefNodeD);

	// -------------- Up ---------------- // Neigh[5]
	Target.Neigh[5].Connect = find_Path(Target, *RefNodeU, DataFace);

	return;
}

bool Wrap3D::find_Path(Grid_node TarNode, Grid_node RefNode,
					   const FiberFace** DataFace, int Switch = 1)
{
	switch (Switch)
	{
	case 1:	// Defauted mode
	{
		const FiberFace* Face_Start = DataFace[TarNode.Ind_Face];
		const FiberFace* Face_End = DataFace[RefNode.Ind_Face];
		return compare_Edges(Face_Start, Face_End);
	}
	default:
		break;
	}
	return false;
}

// Geometric Algorithms //
inline bool Wrap3D::compare_Edges(const FiberFace * Face_1, const FiberFace * Face_2)
{
	unsigned TarNodes[3] =
	{
		Face_1->x, Face_1->y, Face_1->z
	};
	unsigned RefNodes[3] =
	{
		Face_2->x, Face_2->y, Face_2->z
	};
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			if (TarNodes[i] == TarNodes[j])
				return true;
	}
	return false;
}

inline double Wrap3D::Get_Z(const FiberVer Src1, const FiberVer Src2, const FiberVer Src3, const double x, const double y)
{
	double Res_Z = 0.0;
	double Dx1, Dx2, Dx3;
	double Dy1, Dy2, Dy3;

	Dx1 = Src1.x - x; Dx2 = Src2.x - x; Dx3 = Src3.x - x;
	Dy1 = Src1.y - y; Dy2 = Src2.y - y; Dy3 = Src3.y - y;

	double Det_M = Dx1*Dy2 - Dx2*Dy1;

	double F1 = Dy2*Dx3 - Dx2*Dy3;
	double F2 = Dx1*Dy3 - Dy1*Dx3;

	Res_Z = (F1*Src1.z + F2*Src2.z - Det_M) / (F1 + F2 - Det_M);

	return Res_Z;

	// 22 Alg + 8 Prog
}

bool Wrap3D::Inside_Tri(const FiberVer Src1, const FiberVer Src2, const FiberVer Src3, const double x, const double y)
{
	const double x1 = Src1.x; const double y1 = Src1.y;
	const double x2 = Src2.x; const double y2 = Src2.y;
	const double x3 = Src3.x; const double y3 = Src3.y;

	const double Dx1 = Src1.x - x; const double Dy1 = Src1.y - y;
	const double Dx2 = Src2.x - x; const double Dy2 = Src2.y - y;
	const double Dx3 = Src3.x - x; const double Dy3 = Src3.y - y;

	const double ReferS = fabs((x3 - x1)*(x2 - x1) + (y3 - y1)*(y2 - y1));
	const double S1 = fabs(Dx1*Dx2 + Dy1*Dy2);
	const double S2 = fabs(Dx1*Dx3 + Dy1*Dy3);
	const double S3 = fabs(Dx3*Dx2 + Dy3*Dy2);

	if (S1 + S2 > ReferS)	return false;
	if (S1 + S3 > ReferS)	return false;
	if (S3 + S2 > ReferS)	return false;

	return true;
}
