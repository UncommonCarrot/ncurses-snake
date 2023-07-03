/*---Headers---*/
#include "headers.h"

/*---Global Variables---*/
std::vector<std::string> a_field;

/*---Predefinitions---*/
void genField();
void removeField(int x, int y);
void addField(int x, int y);
void translateField(int pos, int& x, int& y);

/*---Prints the vector a_field to a file named "field.txt"*/
void printField()
{	
	std::string filename = "field.txt";
	std::ofstream file(filename, std::ios::binary);
	for(int i = 0; i<a_field.size();i++)
	{
		file << i << "\t" << a_field.at(i) << std::endl;
	}
}
/*---Filles vector with available fields---*/
void genField(int h, int w)
{
	std::string field, y, x;
	for(int i = 0; i<=w;i++)
	{
		x = std::to_string(i);
		if(i<10)
		{
			x = "0"+x;
		}
		for(int n = 0; n<=h;n++)
		{
			y = std::to_string(n);
			if(n<10)
			{
				y = "0"+y;
			}
			field = x+y;
			a_field.push_back(field);
		}
	}
}
/*---Removes the Field from vector with given coords---*/
void removeField(int x, int y)
{
	std::string field, y_s, x_s;
	y_s = std::to_string(y);
	x_s = std::to_string(x);
	if(y < 10)
	{
		y_s = "0"+y_s;
	}
	if(x < 10)
	{
		x_s = "0"+x_s;
	}
	field = x_s+y_s;
	for (int n = 0; n<a_field.size();n++)
	{
		if(a_field[n] == field)
		{
			a_field.erase(a_field.begin()+n);
		}
	}
}

/*---add Field to vector with given coords---*/
void addField(int x, int y)
{
	std::string field, y_s, x_s;
	y_s = std::to_string(y);
	x_s = std::to_string(x);
	if(y < 10)
	{
		y_s = "0"+y_s;
	}
	if(x < 10)
	{
		x_s = "0"+x_s;
	}
	field = x_s+y_s;
	a_field.push_back(field);
}
/*---Gives out x/y coords for a field at given coords---*/
void translateField(int pos, int& x, int& y)
{
	std::string field, y_s, x_s;
	field = a_field.at(pos);
	x_s = field.substr(0,2);
	y_s = field.substr(2,2);
	x = std::stoi(x_s);
	y = std::stoi(y_s);
}
