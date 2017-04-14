#ifndef BUFF_H
#define BUFF_H

class Buff
{

public:
	Buff(int dataSize);
	void add(double val);
	static int dataSize;
private:
	int index;
};

#endif // BUFF_H
