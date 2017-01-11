#include "Check.h"
#include <cstdio>
#include <memory.h>
#include <fstream>


int main()
{
	using namespace std;
	static FiberStruct testFiber;
	int a = 0;
	a = testFiber.ImportFiberFace("Face1.tst", testFiber.DataFace);
	a = testFiber.ImportFiberVer("Node1.tst", testFiber.DataVer);
	//testFiber.DeleteFaceFrom(testFiber.DeleteBox);
	//testFiber.GetNeigh();
	//testFiber.GarbageFace();
	//testFiber.GetEdges();
	//testFiber.CheckTopology();
	//testFiber.CheckOrientation('a');
	testFiber.Compute_Normal();
	testFiber.Remove_Zvol();
	cout << "Writing Data...\n";
	a = testFiber.WriteFiberFace("Out.face", testFiber.DataFace);
	a = testFiber.WriteFiberVer("Out.node", testFiber.DataVer);
	a = testFiber.WriteFiberNormal("Out.Nor", testFiber.Normal);
	cout << "Finished Writing.\n";
	//cin.get();
	return 0;
}