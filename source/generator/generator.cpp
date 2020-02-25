/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "header.hpp"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
generator::generator() :
	_parameter(CX_NULL_POINTER)
{
}

generator::~generator()
{
}

//===========================================================================
parameter* generator::get_parameter (void)
{
	return _parameter;
}

void generator::set_parameter (parameter* p)
{
	_parameter = p;
}

//===========================================================================
cx::bool_t generator::generate (void)
{
	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cx::bool_t generate(std::string type)
{
	//-----------------------------------------------------------------------
	std::string splash;
	std::string backsplash;


	splash    = "/";
	backsplash= "\\";


	//-----------------------------------------------------------------------
	std::string my_directory;


	my_directory = cx::get_directory_of_current_process();
	my_directory = cx::replace(my_directory, backsplash, splash);
#if defined(_DEBUG)
	my_directory += "../../data/";
#endif

	//-----------------------------------------------------------------------
	parameter p;


	p.set_environment("$fg_directory"            , my_directory); 
	p.set_environment("$fg_type"                 , "wxx"); 
	p.set_environment("$fg_target_root_directory", "D:/Development/Project/test/"); 
	p.set_environment("$fg_name"                 , "rs_it_editor"); 


	//-----------------------------------------------------------------------
	vc_generator g;


	g.set_parameter(&p);

	return g.generate();
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


