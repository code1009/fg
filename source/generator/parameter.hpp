#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class parameter
{
public:
	parameter();
	virtual ~parameter();

public:
	std::map<std::string,std::string> _environment;

public:
	std::string environment (std::string name);

public:
	void        set_environment (std::string name, std::string value);
	std::string get_environment (std::string name);
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


