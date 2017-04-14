#include <buff.h>

// constructor, for variable dataSize
Buff::Buff(int dataSize)
{
	double yData[dataSize];
	index=0;
}

void add(double val)
{ 
	if(index>dataSize-1){
		index=0;
	}
	yData[index]=val;
	index++;
}
