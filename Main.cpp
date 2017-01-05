#include "Check.h"
#include <cstdio>
#include <memory.h>
#include <fstream>


int main()
{
	using namespace std;
	static FiberStruct testFiber;
	int a = 0;
	a = testFiber.ImportFiberDir("H2.face", testFiber.DataFace);
	a = testFiber.ImportFiberVer("H2.node", testFiber.DataVer);
	//testFiber.DeleteFaceFrom(testFiber.DeleteBox);
	//testFiber.GetNeigh();
	//testFiber.GarbageFace();
	//testFiber.GetEdges();
	//testFiber.CheckTopology();
	//testFiber.CheckOrientation('a');
	a = testFiber.WriteFiberDir("Out.face", testFiber.OutFace);
	a = testFiber.WriteFiberVer("Out.node", testFiber.OutVer);
	//cin.get();
	return 0;
}