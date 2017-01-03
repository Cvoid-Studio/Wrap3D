/*  ------------ Wrap3D Version 1.00 -------------

Based on wrapping algorithm, used to repair or reconstruct
the 3D surface model with bad topology, in order to make it
useful for generating tetrahedral mesh or printable.
--------------------------------------------------------
Copyright @ Cvoid Studio 2016. All Right Are Reserved.*/

/*					 Lib Diagram						  /
	<Wrap3D_Main> <-- <File_Reader>
		 ||
	==========================================
	||
  <MODEL>
	||
	==========================================
					   ||
				  <Wrap3D_Base>
/.......................................................*/

#include "Model_Info.hpp"

// Input Format: Wrap3D	<Src_File> <Out_File>
int main(int argc, char* argv[])
{
	using namespace std;
	if (argc < 2)		// Insufficient Input	Error: 0x01
	{
		cerr << "Error: 0x01\nNo input file detected,"
			<< " choosen stl file or ply file are supported."
			<< endl;
		return -1;
	}


	

	return 0;
}