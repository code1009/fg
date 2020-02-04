/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "header.hpp"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
parameter::parameter()
{
}

parameter::~parameter()
{
}

//===========================================================================
std::string parameter::environment(std::string name)
{
	return get_environment(name);
}

std::string parameter::get_environment(std::string name)
{
	return _environment[name];
}

void parameter::set_environment (std::string name, std::string value)
{
	_environment[name] = value;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


