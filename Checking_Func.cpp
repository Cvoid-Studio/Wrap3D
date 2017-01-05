#include "Check.h"
unsigned comp(const void*a, const void*b)
{
	return *(unsigned*)a - *(unsigned*)b;
}

void FiberStruct::Duplicate(char Switch_)
{
	return;
}

void FiberStruct::CheckOrientation(char Switch_)
{
	unsigned badOri = 0;
	unsigned CurrentNum = 0;
	while (CurrentNum < MaxFaces)
	{
		// Opsite to keep consistently Oriented Contiguous Face //
		Work_Edge[0][0] = DataFace[CurrentNum].z;
		Work_Edge[1][0] = DataFace[CurrentNum].y;

		Work_Edge[0][1] = DataFace[CurrentNum].y;
		Work_Edge[1][1] = DataFace[CurrentNum].x;

		Work_Edge[0][2] = DataFace[CurrentNum].x;
		Work_Edge[1][2] = DataFace[CurrentNum].z;
		// Checker and filter //
		for (unsigned i = 0; i < MaxFaces; ++i)
		{
			if (Indexes[i] < 3)
			{
				Comp_Edge[0][0] = DataFace[i].x;
				Comp_Edge[1][0] = DataFace[i].y;

				Comp_Edge[0][1] = DataFace[i].y;
				Comp_Edge[1][1] = DataFace[i].z;

				Comp_Edge[0][2] = DataFace[i].z;
				Comp_Edge[1][2] = DataFace[i].x;

				Indexes[i] = Minus_Ori();
			}
		}
		
		if (Indexes[CurrentNum] == 3)
		{
			while (Indexes[CurrentNum] == 3)
				CurrentNum++;
		}
		//else
			//CurrentNum++;
		
		std::cout << CurrentNum << '\n';
	}

	// Statisics //
	for (unsigned i = 0; i < MaxFaces; ++i)
	{
		badOri += (unsigned)((Indexes[CurrentNum] < 3));
	}

	std::cout << "There are " << badOri << " bad oriented faces!\n\n";
	return;
}

void FiberStruct::GetEdges()
{
	unsigned x, y, z;
	std::cout << "Stroing the edges...";
	unsigned tempThree[3];
	
	for (unsigned i = 0; i < MaxFaces; ++i)
	{
		tempThree[0] = DataFace[i].x;
		tempThree[1] = DataFace[i].y;
		tempThree[2] = DataFace[i].z;

		//qsort(tempThree,3,sizeof(unsigned),comp);

		DataFace[i].x = tempThree[0];
		DataFace[i].y = tempThree[1];
		DataFace[i].z = tempThree[2];
	}

	for (unsigned i = 0; i < MaxVer; ++i)
	{
		x = DataFace[i].x;
		y = DataFace[i].y;
		z = DataFace[i].z;
		// Get Edges // ----------- Choose the smaller one to be pivot
		GetEdgeInfo(x, y);
		GetEdgeInfo(y, z);
		GetEdgeInfo(x, z);
	}
	std::cout << "\nDone!\n";

	return;
}

void FiberStruct::CheckTopology()
{

	int Sum_Edges = 0;
	std::cout << "Checking Holes and Topology...";
	GetEdges();

	unsigned temp1;
	unsigned temp2;
	for (int i = 0; i < MaxFaces; ++i)
	{
		for (int j = 0; j < MaxNeigh; ++j)
		{
			temp1 = Edges[i][j].End_Ver;
			temp2 = Edges[i][j].Neigh_Faces;
			if (temp2 == 0)
				break;

			if (temp2 > 2)
			{
				std::cout << i << ' ' << temp1;
				std::cout << "\t| " << temp2;
				std::cout << '\n';
				Sum_Edges++;
			}
		}
	}

	std::cout << "\nThere are total " 
			  << Sum_Edges 
			  << " Non-manifold Edges!\n";

	return;
}

void FiberStruct::GarbageFace()
{
	int sum_Gar = 0;
	int sum_Boundary = 0;
	std::cout << "\nProcessing Checking the Garbage faces...";
	for (unsigned i = 0; i < MaxVer; ++i)
	{
		if (Vertices[i][1] < 2)
		{
			//std::cout << i << '\t' << Vertices[i][1]
			//	<< '\n';
			sum_Gar++;
		}
		else if (Vertices[i][1] == 2)
			sum_Boundary++;
	}
	std::cout << "\n\nThere are totally "
			  << sum_Gar
			  << " Garbage Faces!\n";
	std::cout << "There are at least "
			  << sum_Boundary
			  << " Boundary Faces!\n";
	std::cout << "\nFinished!\n";
}

unsigned int FiberStruct::Minus_Ori()
{
	int m_result = 0;
	if  (((Work_Edge[0][0] == Comp_Edge[0][0])
					 && (Work_Edge[1][0] == Comp_Edge[1][0])) ||
		((Work_Edge[0][0] == Comp_Edge[0][1])
					 && (Work_Edge[1][0] == Comp_Edge[1][1])) ||
		((Work_Edge[0][0] == Comp_Edge[0][2])
					 && (Work_Edge[1][0] == Comp_Edge[1][2])))
		m_result++;

	if  (((Work_Edge[0][1] == Comp_Edge[0][0])
					 && (Work_Edge[1][1] == Comp_Edge[1][0])) ||
		((Work_Edge[0][1] == Comp_Edge[0][1])
					 && (Work_Edge[1][1] == Comp_Edge[1][1])) ||
		((Work_Edge[0][1] == Comp_Edge[0][2])
					 && (Work_Edge[1][1] == Comp_Edge[1][2])))
		m_result++;

	if  (((Work_Edge[0][2] == Comp_Edge[0][0])
					 && (Work_Edge[1][2] == Comp_Edge[1][0])) ||
		((Work_Edge[0][2] == Comp_Edge[0][1])
					 && (Work_Edge[1][2] == Comp_Edge[1][1])) ||
		((Work_Edge[0][2] == Comp_Edge[0][2])
					 && (Work_Edge[1][2] == Comp_Edge[1][2])))
		m_result++;

	return m_result;
}

void FiberStruct::GetEdgeInfo(unsigned x, unsigned y)
{
	tempIndex1 = 0;
	tempIndex2 = 0;
	Look_up = 0;

	tempIndex1 = x;	// Pivot
	tempIndex2 = y;	// End

	for (int k = 0; k < MaxNeigh; ++k)
	{
		if (Edges[tempIndex1][k].End_Ver == tempIndex2)
		{
			Look_up = k;
			break;
		}
		else if (Edges[tempIndex1][k].Neigh_Faces == 0)
		{
			Look_up = k;
			break;
		}
	}
	if (Look_up >= MaxNeigh)
	{
		std::cout << tempIndex1 << '\t';
		std::cout << "Need a Larger MaxNeigh!!\n";
		return;
	}

	Edges[tempIndex1][Look_up].End_Ver = tempIndex2;
	Edges[tempIndex1][Look_up].Neigh_Faces++;

}

void FiberStruct::GetNeigh()
{
	unsigned temp1, temp2, temp3;
	
	for (int j = 0; j < MaxFaces; ++j)
	{
		//std::cout << DataFace[j].x;
		temp1 = (DataFace[j].x);	Vertices[temp1][1] ++;
		temp2 = (DataFace[j].y);	Vertices[temp2][1] ++;
		temp3 = (DataFace[j].z);	Vertices[temp3][1] ++;
	}
}

void FiberStruct::DeleteFaceFrom(FiberFace *Source)
{
	for (int i = 0; i < CutIndex; ++i)
	{
		for (int j = 0; j < MaxFaces; ++j)
		{
			if (DataFace[j].x == Source[i].x ||
				DataFace[j].x == Source[i].y ||
				DataFace[j].x == Source[i].z)
			{
				if (DataFace[j].y == Source[i].x ||
					DataFace[j].y == Source[i].y ||
					DataFace[j].y == Source[i].z)
				{
					if (DataFace[j].z == Source[i].x ||
						DataFace[j].z == Source[i].y ||
						DataFace[j].z == Source[i].z)
					{
						CutCri[j] = true;
						break;
					}
				}
			}
		}
	}
}
