
#include "GDUtilities.hpp"

namespace GDUtility
{
	
	float smoothInput(float in, float max)
	{
		//proportion -> 0 in 0 out, max in max out, -max in -max out
		float prop = in/max;
		//guess this'll do for now..........
		float out = (float) exp(abs(in));
		return prop * out;
	}
	
	//input comes in as...
	//+XX|-YY|+XX|-YY|fire1|fire2|spec1|pedal
	void ParseInput(char* msg, Input* I)
	{
		
		//cout<<msg<<std::endl;
		/*if(strlen(msg) == 0 || strlen(msg) >  28)
		{
			cout<<"nothing and/or received!"<<std::endl;
			return;
		}*/
		//probably don't need this input at this point, just directly change the values since
		//we have a pointer now
		Input* out = new Input;
		
		string delimiter = "|";
		string token;
		int pos = 0;
		
		//convert char* to string
		//or just cast it to a string
		string s(msg);
		
		//use an int array to store the info
		float vals[8];
		int index = 0;
		
		//loop to get that sweet data
		while((pos = s.find(delimiter)) != string::npos)
		{
			//get the component
			token = s.substr(0, pos);
			std::stringstream tmp(token);
			vals[index] = 0;
			tmp >> vals[index];
			
			//erase it from the string
			s.erase(0, pos + delimiter.length());
			index++;
		}
		
		//presumably there should be one more value within msg
		//but index should still be 5
		std::stringstream tmp(token);
		vals[index] = 0;
		tmp >> vals[index];
		
		//dump if it doesn't all come through
		/*if(index < 8)
		{
			cout<<"Too few"<<std::endl;
			return;
		}*/
		
		//get those values up in there
		(*I).dx1 = vals[0];
		(*I).dy1 = vals[1];
		(*I).dx2 = vals[2];
		(*I).dy2 = vals[3];
		
		(*I).prevFire1 = (*I).fire1;
		(*I).prevFire2 = (*I).fire2;
		
		(*I).fire1 = int(vals[4]);
		I->fire1 = !I->fire1;
		(*I).fire2 = int(vals[5]);
		I->fire2 = !I->fire2;
		
		if(I->fire1 && !I->prevFire1)
			I->cooldown1 = 1;
		else if (I->cooldown1 > 0)
			I->cooldown1 -= 0.05;
		
		if(I->fire2 && !I->prevFire2)
			I->cooldown2 = 1;
		else if (I->cooldown2 > 0)
			I->cooldown2 -= 0.05;
			
		
		(*I).prevSpec1 = (*I).spec1;
		(*I).prevSpec2 = (*I).spec2;
		
		(*I).spec1 = int(vals[6]);
		I->spec1 = !I->spec1;
		//(*I).spec2 = int(vals[7]);
		(*I).pedal = int(vals[7]);
		I->spec2 = !I->spec2;
	}
	
	void DefaultInput(Input* I)
	{
		//get those values up in there
		(*I).dx1 = 0;
		(*I).dy1 = 0;
		(*I).dx2 = 0;
		(*I).dy2 = 0;
		
		(*I).prevFire1 = false;
		(*I).prevFire2 = false;
		
		(*I).fire1 = false;
		(*I).fire2 = false;
		
		(*I).prevSpec1 = false;
		(*I).prevSpec2 = false;
		
		I->cooldown1 = 0;
		I->cooldown2 = 0;
		
		(*I).spec1 = false;
		I->spec2 = false;
		//(*I).spec2 = int(vals[7]);
		(*I).pedal = 0;
	}
}
