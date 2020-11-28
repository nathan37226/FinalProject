#include "Official.h"

string Official::getState()
{
	return officialState;
}

void Official::setState(string newState)
{
	officialState = newState;
}