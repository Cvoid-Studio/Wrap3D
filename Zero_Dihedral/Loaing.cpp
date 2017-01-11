#include "Check.h"


FiberStruct::FiberStruct()
{
	memset(DataFace, 0, MaxFaces * sizeof(FiberFace));
	memset(DataVer, 0, MaxVer * sizeof(FiberVer));
	memset(Normal, 0, MaxFaces * sizeof(Nor_Ratio));
	for (unsigned i = 0; i < MaxFaces; ++i)
	{
		Face_Index[i] = i;
		DataFace[i].isRender = true;
	}


	//memset(OutFace, 0, MaxFaces * sizeof(FiberFace));
	//memset(OutVer, 0, Max_Node * sizeof(FiberVer));

	//memset(DeleteBox, 0, CutIndex * sizeof(FiberFace));

	/*for (int i = 0; i < MaxVer; ++i)
	{
		for (int j = 0; j < MaxNeigh; ++j)
		{
			Edges[i][j].End_Ver = -1;
			Edges[i][j].Neigh_Faces = 0;
		}
	}*/

	//wp_Algorithm = Wrap3D(DataFace);

	tempIndex1 = 0;
	tempIndex2 = 0;
	Look_up = 0;
}

FiberStruct::~FiberStruct()
{
	memset(DataFace, 0, MaxFaces * sizeof(FiberFace));
}

// =============================================================== //
int FiberStruct::ImportFiberFace(const char* filename, FiberFace* Target)
{
	using namespace std;
	cout << "reading faces...";

	//===========================================================//
	ios_base::openmode ifs = ios_base::in;
	//-----------------------------------------------------------//
	ifstream inf(filename, ifs);
	//-----------------------------------------------------------//
	if (inf.fail())
	{
		printf("Error ix01: open file failed!\n");
		return -1; //Fail to exit				--> Error ix01
	}
	//-----------------------------------------------------------//
	inf.seekg(0, inf.end);
	unsigned int length = inf.tellg();
	inf.seekg(0, inf.beg);

	unsigned int Index_Mem = 0;
	unsigned int Index_Dir = 0;

	while (inf.tellg() >= 0)
	{
		
		switch (Index_Dir)
		{
		case 0:
			break;
		case 1:
			inf >> (Target[Index_Mem]).x;
			//cout << (DataFace[Index_Mem]).x << '\n';
			break;
		case 2:
			inf >> (Target[Index_Mem]).y;
			//cout << (DataFace[Index_Mem]).y << ' ';
			break;
		case 3:
			inf >> (Target[Index_Mem]).z;
			//cout << (DataFace[Index_Mem]).z << '\n';
			inf.seekg(2, inf.cur);
			inf.tellg();
			Index_Mem++;
			break;
		}
		inf.seekg(1, inf.cur);
		Index_Dir++;
		Index_Dir %= 4;
	}
	cout << "\nfinish reading faces.\n\n";
	inf.close();
	//===========================================================//
	return 0;
}

int FiberStruct::ImportFiberVer(const char * filename, FiberVer * Target)
{
	using namespace std;
	cout << "reading Vertices...";

	//===========================================================//
	ios_base::openmode ifs = ios_base::in;
	//-----------------------------------------------------------//
	ifstream inf(filename, ifs);
	//-----------------------------------------------------------//
	if (inf.fail())
	{
		printf("Error ix01: open file failed!\n");
		return -1; //Fail to exit				--> Error ix01
	}
	//-----------------------------------------------------------//
	inf.seekg(0, inf.end);
	unsigned int length = inf.tellg();
	inf.seekg(0, inf.beg);

	unsigned int Index_Mem = 0;
	unsigned int Index_Dir = 0;

	while (inf.tellg() >= 0)
	{

		switch (Index_Dir)
		{
		case 0:
			inf >> (Target[Index_Mem]).x;
			//cout << (DataVer[Index_Mem]).x << '\n';
			break;
		case 1:
			inf >> (Target[Index_Mem]).y;
			//cout << (DataFace[Index_Mem]).y << ' ';
			break;
		case 2:
			inf >> (Target[Index_Mem]).z;
			//cout << (DataFace[Index_Mem]).z << '\n';
			inf.seekg(1, inf.cur);
			inf.tellg();
			Index_Mem++;
			break;
		}
		inf.seekg(1, inf.cur);
		Index_Dir++;
		Index_Dir %= 3;
	}
	cout << "\nfinish reading Vertices.\n\n";

	inf.close();
	//===========================================================//
	return 0;
}

int FiberStruct::WriteFiberFace(const char* filename, FiberFace* Src)
{
	using namespace std;
	ios_base::openmode ofs = ios_base::out;
	//-----------------------------------------------------------//
	fstream iof(filename, ofs | ios_base::trunc);
	//-----------------------------------------------------------//
	if (iof.fail())
	{
		printf("Error ox01: open file failed!\n");
		return -1; //Fail to exit				--> Error ox01
	}
	//-----------------------------------------------------------//
	unsigned int Index_Dir = 0;
	unsigned int Index_Mem = 0;
	char tempstr[16];
	unsigned Keep = 0;
	for (unsigned i = 0; i < MaxFaces; ++i)
	{
		if (Src[i].isRender)
			Keep++;
	}
	cout << "Total " << Keep << " Faces Kept.\n";
	cout << "And " << MaxFaces - Keep << " Faces Connected to Zero Vol Tetrahedral are culted.\n\n";

	while (Index_Mem < MaxFaces)
	{
		if (Src[Index_Mem].isRender)
		{
			switch (Index_Dir)
			{
			case 0:
				sprintf(tempstr, "3 %i ", (Src[Index_Mem]).x);
				break;
			case 1:
				sprintf(tempstr, "%i ", (Src[Index_Mem]).y);
				break;
			case 2:
				sprintf(tempstr, "%i\n", (Src[Index_Mem]).z);
				Index_Mem++;
				break;
			}
			iof.write(tempstr, strlen(tempstr));
			Index_Dir++;

			Index_Dir %= 3;
		}
		else
			Index_Mem++;
	}
	

	iof.close();
	//delete tempstr;
	return 0;
}

int FiberStruct::WriteFiberVer(const char * filename, FiberVer* Src)
{
	using namespace std;
	ios_base::openmode ofs = ios_base::out;
	//-----------------------------------------------------------//
	fstream iof(filename, ofs | ios_base::trunc);
	//-----------------------------------------------------------//
	if (iof.fail())
	{
		printf("Error ox01: open file failed!\n");
		return -1; //Fail to exit				--> Error ox01
	}
	//-----------------------------------------------------------//
	unsigned int Index_Dir = 0;
	unsigned int Index_Mem = 0;
	char tempstr[16];
	while (Index_Mem < MaxVer)
	{
		switch (Index_Dir)
		{
		case 0:
			sprintf(tempstr, "%7.3f ", (Src[Index_Mem]).x);
			break;
		case 1:
			sprintf(tempstr, "%7.3f ", (Src[Index_Mem]).y);
			break;
		case 2:
			sprintf(tempstr, "%7.3f\n", (Src[Index_Mem]).z);
			Index_Mem++;
			break;
		}
		
		
		iof.write(tempstr, strlen(tempstr));
		Index_Dir++;

		Index_Dir %= 3;
	}


	iof.close();
	//delete tempstr;
	return 0;
}

int FiberStruct::WritePLY(const char * filename)
{
	return 0;
}


int FiberStruct::WriteFiberNormal(const char* filename, Nor_Ratio* Src)
{
	using namespace std;
	ios_base::openmode ofs = ios_base::out;
	//-----------------------------------------------------------//
	fstream iof(filename, ofs | ios_base::trunc);
	//-----------------------------------------------------------//
	if (iof.fail())
	{
		printf("Error ox01: open file failed!\n");
		return -1; //Fail to exit				--> Error ox01
	}
	//-----------------------------------------------------------//
	unsigned int Index_Dir = 0;
	unsigned int Index_Mem = 0;
	char tempstr[16];
	unsigned Keep = 0;

	while (Index_Mem < MaxFaces)
	{
		switch (Index_Dir)
		{
		case 0:
			sprintf(tempstr, "%i %i ", (Src[Index_Mem]).index,(Src[Index_Mem]).NX);
			break;
		case 1:
			sprintf(tempstr, "%i ", (Src[Index_Mem]).NY);
			break;
		case 2:
			sprintf(tempstr, "%i\n", (Src[Index_Mem]).NZ);
			Index_Mem++;
			break;
		}
		iof.write(tempstr, strlen(tempstr));
		Index_Dir++;

		Index_Dir %= 3;
	}


	iof.close();
	//delete tempstr;
	return 0;
}