/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "header.hpp"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
file_copy_generator::file_copy_generator() :
	generator()
{
}

file_copy_generator::~file_copy_generator()
{
}

//===========================================================================
cx::bool_t file_copy_generator::generate (void)
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


	//-----------------------------------------------------------------------
	std::string source_directory;
	std::string target_directory;


	source_directory = my_directory + get_parameter()->environment("$fg_type") + "/template/";
	target_directory = get_parameter()->environment("$fg_target_root_directory");


	source_directory = cx::replace(source_directory, splash, backsplash);
	target_directory = cx::replace(target_directory, splash, backsplash);


	//-----------------------------------------------------------------------
	std::string source;
	std::string target;


	source = cx::get_directory_name_of_directory( source_directory );
	target = cx::get_directory_name_of_directory( target_directory );

	source.push_back(0); // FOF_MULTIDESTFILES \0\0 필요
	target.push_back(0); // FOF_MULTIDESTFILES \0\0 필요


	//-----------------------------------------------------------------------
	SHFILEOPSTRUCTA shfo;
	int             rc;

	
	memset (&shfo, 0, sizeof(shfo));

	shfo.hwnd                      = NULL;
	shfo.wFunc                     = FO_COPY;
	shfo.pFrom                     = source.c_str();
	shfo.pTo                       = target.c_str();
	shfo.fFlags                    = FOF_MULTIDESTFILES | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
//	shfo.fFlags                    = FOF_SIMPLEPROGRESS | FOF_MULTIDESTFILES | FOF_NOCONFIRMMKDIR;
	shfo.fAnyOperationsAborted     = FALSE;
	shfo.hNameMappings             = NULL;
//	shfo.lpszProgressTitle         = "Copy";

	rc = SHFileOperationA (&shfo);


	return true;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


