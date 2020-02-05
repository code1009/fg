/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../header.hpp"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_generator::vc_generator() :
	generator()
{
}

vc_generator::~vc_generator()
{
}


//===========================================================================
void vc_generator::set_parameter (parameter* p)
{
	generator::set_parameter(p);
	
	_solution_generator      .set_parameter(p);
	_project_generator       .set_parameter(p);
	_project_filter_generator.set_parameter(p);
	_file_copy_generator     .set_parameter(p);
}

//===========================================================================
cx::bool_t vc_generator::generate (void)
{
	//-----------------------------------------------------------------------
	cx::bool_t result;

	
	result = true;


	//-----------------------------------------------------------------------
	initialize();

	result = generate_file           ();
	result = generate_project_filter ();
	result = generate_project        ();
	result = generate_solution       ();

	cleanup();

	return result;
}

void vc_generator::initialize (void)
{
	//-----------------------------------------------------------------------
	std::string my_directory;


	my_directory = get_parameter()->environment("$fg_directory");


	//-----------------------------------------------------------------------
	std::string source_file_path;


	source_file_path = my_directory + get_parameter()->environment("$fg_type") + "/template.ini";

	_template_data._generator = this;

	_template_data.load(source_file_path);

	//-----------------------------------------------------------------------
/*
	_item_collection.add("source/res/Resource.h"           , "source/res"      );
	_item_collection.add("source/res/Application.rc"       , "source/res"      );
	_item_collection.add("source/res/Application.manifest" , "source/res"      );
	_item_collection.add("source/res/Application.ico"      , "source/res"      );
	_item_collection.add("source/res/Toolbar.bmp"          , "source/res"      );
	_item_collection.add("source/res/Docker.ico"           , "source/res"      );
	_item_collection.add("source/res/View.ico"             , "source/res"      );
	_item_collection.add("source/res/wxx/wxx_resource.h"   , "source/res/wxx"  );
	_item_collection.add("source/res/wxx/SDbottom.bmp"     , "source/res/wxx"  );
	_item_collection.add("source/res/wxx/SDcenter.bmp"     , "source/res/wxx"  );
	_item_collection.add("source/res/wxx/SDleft.bmp"       , "source/res/wxx"  );
	_item_collection.add("source/res/wxx/SDmiddle.bmp"     , "source/res/wxx"  );
	_item_collection.add("source/res/wxx/SDright.bmp"      , "source/res/wxx"  );
	_item_collection.add("source/res/wxx/SDtop.bmp"        , "source/res/wxx"  );
	_item_collection.add("source/res/wxx/SplitH.cur"       , "source/res/wxx"  );
	_item_collection.add("source/res/wxx/SplitV.cur"       , "source/res/wxx"  );
	_item_collection.add("source/ReadMe.txt"                                   );
	_item_collection.add("source/stdafx.h"                 , "source"          );
	_item_collection.add("source/stdafx.cpp"               , "source"          );
	_item_collection.add("source/targetver.h"              , "source"          );
	_item_collection.add("source/wxx.hpp"                  , "source"          );
	_item_collection.add("source/Application.hpp"          , "source"          );
	_item_collection.add("source/Application.cpp"          , "source"          );
	_item_collection.add("source/my_debug.hpp"             , "source"          );
	_item_collection.add("source/my_debug.cpp"             , "source"          );
	_item_collection.add("source/Declaration.hpp"          , "source"          );
	_item_collection.add("source/MyWinApp.hpp"             , "source"          );
	_item_collection.add("source/MyWinApp.cpp"             , "source"          );
	_item_collection.add("source/MainFrame.hpp"            , "source"          );
	_item_collection.add("source/MainFrame.cpp"            , "source"          );
	_item_collection.add("source/MyTabbedMDI.hpp"          , "source"          );
	_item_collection.add("source/MyTabbedMDI.cpp"          , "source"          );
	_item_collection.add("source/MyDocker.hpp"             , "source"          );
	_item_collection.add("source/MyDocker.cpp"             , "source"          );
	_item_collection.add("source/EventMessageDocker.hpp"   , "source"          );
	_item_collection.add("source/EventMessageDocker.cpp"   , "source"          );
	_item_collection.add("source/MyView.hpp"               , "source"          );
	_item_collection.add("source/MyView.cpp"               , "source"          );
*/
	_project_generator       ._template_data   = &_template_data;

	_project_generator       ._item_collection = &_item_collection;
	_project_filter_generator._item_collection = &_item_collection;
}

void vc_generator::cleanup (void)
{
	_project_generator       .cleanup();
	_project_filter_generator.cleanup();
	_solution_generator      .cleanup();

	_item_collection.cleanup();

	_template_data.cleanup();
}

cx::bool_t vc_generator::generate_file (void)
{
	//-----------------------------------------------------------------------
	cx::bool_t result;

	
	result = true;


	//-----------------------------------------------------------------------
	result = _file_copy_generator.generate();

	CX_DEBUG_TRACEF(CX_TWA_NORMAL, "copy file");


	//-----------------------------------------------------------------------


	return result;
}

cx::bool_t vc_generator::generate_project_filter (void)
{
	//-----------------------------------------------------------------------
	cx::bool_t result;

	
	result = true;


	//-----------------------------------------------------------------------
	std::vector<std::string>           project_filter;
	std::vector<std::string>::iterator i;

	
	_item_collection.get_project_filter(project_filter);
	
	for (i=project_filter.begin(); i!=project_filter.end(); i++)
	{
		_project_filter_generator.add_folder( (*i) );
	}


	//-----------------------------------------------------------------------
	result = _project_filter_generator.generate ();

	CX_DEBUG_TRACEF(CX_TWA_NORMAL, "generate .vcxproj.filters file");


	//-----------------------------------------------------------------------
	

	return result;
}


cx::bool_t vc_generator::generate_project (void)
{
	//-----------------------------------------------------------------------
	cx::bool_t result;

	
	result = true;


	//-----------------------------------------------------------------------
/*
	_project_generator.add_configuration ("Debug"  , "Win32", true );
	_project_generator.add_configuration ("Release", "Win32", false);
*/
	std::vector<vc_template_data_project_configuration*>::iterator i;
	
	vc_template_data_project_configuration* e;

	
	for (i =_template_data._project_configuration_container.begin();
	     i!=_template_data._project_configuration_container.end();
	     i++
	    )
	{
		e = *i;

		_project_generator.add_configuration (e->_configuration, e->_platform, e->_debug);
	}


	//-----------------------------------------------------------------------
	result = _project_generator.generate ();

	CX_DEBUG_TRACEF(CX_TWA_NORMAL, "generate .vcxproj file");


	//-----------------------------------------------------------------------
	if (result)
	{
		_solution_generator.add_project(&_project_generator);
	}


	return result;
}
cx::bool_t vc_generator::generate_solution (void)
{
	//-----------------------------------------------------------------------
	cx::bool_t result;

	
	result = true;


	//-----------------------------------------------------------------------
	result = _solution_generator.generate ();

	CX_DEBUG_TRACEF(CX_TWA_NORMAL, "generate .sln file");


	//-----------------------------------------------------------------------


	return result;
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


