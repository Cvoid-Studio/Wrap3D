#include "Check.h"
#include <cmath>

int comp(const void *p1, const void *p2)
{
	return ((((Nor_Ratio*)p1)->NX) > (((Nor_Ratio*)p2)->NX) ? 1 : -1);
}


void FiberStruct::Compute_Normal()
{
	unsigned tempVer1, tempVer2, tempVer3;
	double x1, x2, x3;
	double y1, y2, y3;
	double z1, z2, z3;


	for (unsigned i = 0; i < MaxFaces; ++i)
	{
		tempVer1 = DataFace[i].x;
		tempVer2 = DataFace[i].y;
		tempVer3 = DataFace[i].z;
		// ------------------------------- //
		x1 = DataVer[tempVer1].x;
		y1 = DataVer[tempVer1].y;
		z1 = DataVer[tempVer1].z;
		// ............................... //
		x2 = DataVer[tempVer2].x;
		y2 = DataVer[tempVer2].y;
		z2 = DataVer[tempVer2].z;
		// ............................... //
		x3 = DataVer[tempVer3].x;
		y3 = DataVer[tempVer3].y;
		z3 = DataVer[tempVer3].z;
		// ------------------------------- //
		double tempNorX = y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2);
		double tempNorY = x1*(z3 - z2) + x2*(z1 - z3) + x3*(z2 - z1);
		double tempNorZ = x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2);
		// Normalize
		double Norm = tempNorX*tempNorX + tempNorY*tempNorY + tempNorZ*tempNorZ;
		tempNorX = tempNorX*fabs(tempNorX) / Norm;
		tempNorY = tempNorY*fabs(tempNorY) / Norm;
		tempNorZ = tempNorZ*fabs(tempNorZ) / Norm;
		// tempNorX / tempNorX //
		Normal[i].NX = static_cast<int>(tempNorX*1000.0);
		Normal[i].NY = static_cast<int>(tempNorY*1000.0);
		Normal[i].NZ = static_cast<int>(tempNorZ*1000.0);
		Normal[i].index = i;
	}

	return;
}

void FiberStruct::Remove_Zvol()
{
	using namespace std;
	cout << fixed << setprecision(2);

	// Qsort wrt Normal_Ratio Y, sub-pivoting with Z //
	std::qsort(Normal, MaxFaces, sizeof(Normal[0]), comp);	// O(nlog2n)

	int prog = 0;
	cout << "%" << prog;

	for (unsigned i = 0; i < (MaxFaces - 1); ++i)
	{
		unsigned j = i + 1;
		while (j <= MaxFaces &&
			  (Normal[j].NX - Normal[i].NX) < tolerance_Normal) // Filt Y
		{
			if (Comp_Dist(Normal[i], Normal[j])) // Inside & Connected
			{
				DataFace[Normal[i].index].isRender = false;
				DataFace[Normal[j].index].isRender = false;
				//cout << "!\n";
			}
			j++;
		}
		if (i - prog > 499)
		{
			prog = i;
			cout << i << '\t';
			cout << (double)(i*100) / MaxFaces << '%';
			cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
		}
		
	}
	cout << '\n';
	return;
}

bool FiberStruct::CompEdge(FiberFace F1, FiberFace F2)	// True -> +Connected
{
	int Same_Sum = 0;
	if ((F1.x == F2.x && F1.y == F2.y) ||	// 1-1
		(F1.x == F2.y && F1.y == F2.z) ||	// 1-2
		(F1.x == F2.z && F1.y == F2.x))		// 1-3
		return true;

	if ((F1.y == F2.x && F1.z == F2.y) ||	// 2-1
		(F1.y == F2.y && F1.z == F2.z) ||	// 2-2
		(F1.y == F2.z && F1.z == F2.x))		// 2-3
		return true;

	if ((F1.z == F2.x && F1.x == F2.y) ||	// 3-1
		(F1.z == F2.y && F1.x == F2.z) ||	// 3-2
		(F1.z == F2.z && F1.x == F2.x))		// 3-3
		return true;

	return false;
}

bool FiberStruct::Comp_Dist(Nor_Ratio N1, Nor_Ratio N2) // True -> Inside & Connected
{
	unsigned I1 = N1.index;
	unsigned I2 = N2.index;
	FiberFace F1 = DataFace[I1];
	FiberFace F2 = DataFace[I2];

	FiberFace InvF2;
	InvF2.isRender = F2.isRender;
	InvF2.x = F2.z;
	InvF2.y = F2.y;
	InvF2.z = F2.x;


	if (CompEdge(F1, F2))		// + Connected
	{
		if ((abs(N1.NX - N2.NX) < tolerance_Normal) &&
			(abs(N1.NY - N2.NY) < tolerance_Normal) &&
			(abs(N1.NZ - N2.NZ) < tolerance_Normal))
			return true;			
	}

	if (CompEdge(F1, InvF2))	// - Connected
	{
		if ((abs(N1.NX + N2.NX) < tolerance_Normal) &&
			(abs(N1.NY + N2.NY) < tolerance_Normal) &&
			(abs(N1.NZ + N2.NZ) < tolerance_Normal))
			return true;
	}
	

	return false;
}


