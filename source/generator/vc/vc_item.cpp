/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../header.hpp"
#include <set>
#include <cx3/library/string/string_utility.hpp>



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
# 8∞≥ ≈∏¿‘
"ClInclude"
"ClCompile"
"ResourceCompile"
"Image"
"Text"
"Manifest"
"CustomBuild": ribbon.xml
"None"
*/

static std::string get_vc_item_type (std::string file_path)
{
	//-----------------------------------------------------------------------
	typedef std::map<std::string,std::string>  container_t;
	typedef std::pair<std::string,std::string> element_t;


	container_t file_extension_filter;
	container_t file_filter;

	container_t* container;

	
	container = &file_extension_filter;
	container->insert ( element_t(".cpp"     , "ClCompile"       ) );
	container->insert ( element_t(".c"       , "ClCompile"       ) );
	container->insert ( element_t(".cc"      , "ClCompile"       ) );
	container->insert ( element_t(".cxx"     , "ClCompile"       ) );
	container->insert ( element_t(".hpp"     , "ClInclude"       ) );
	container->insert ( element_t(".h"       , "ClInclude"       ) );
	container->insert ( element_t(".hh"      , "ClInclude"       ) );
	container->insert ( element_t(".hxx"     , "ClInclude"       ) );
	container->insert ( element_t(".rc"      , "ResourceCompile" ) );
	container->insert ( element_t(".jpg"     , "Image"           ) );
	container->insert ( element_t(".bmp"     , "Image"           ) );
	container->insert ( element_t(".png"     , "Image"           ) );
	container->insert ( element_t(".ico"     , "Image"           ) );
	container->insert ( element_t(".cur"     , "Image"           ) );
	container->insert ( element_t(".txt"     , "Text"            ) );
	container->insert ( element_t(".manifest", "Manifest"        ) );
	container->insert ( element_t(".cd"      , "None"            ) );

	container = &file_filter;
	container->insert( element_t("ribbon.xml", "CustomBuild"     ) );


	//-----------------------------------------------------------------------
	container_t::iterator i;

	std::string filter;
	std::string item;


	//-----------------------------------------------------------------------
	item = cx::get_file_of_file_path(file_path);
	cx::to_upper(item);


	container = &file_filter;
	for (i=container->begin(); i!=container->end(); i++)
	{
		filter = (*i).first;

		cx::to_upper(filter);

		if ( filter == item )
		{
			return (*i).second;
		}
	}


	//-----------------------------------------------------------------------
	item = cx::get_file_extension_of_file_path(file_path);
	cx::to_upper(item);


	container = &file_extension_filter;
	for (i=container->begin(); i!=container->end(); i++)
	{
		filter = (*i).first;

		cx::to_upper(filter);

		if ( filter == item )
		{
			return (*i).second;
		}
	}


	//-----------------------------------------------------------------------
	std::string None;


	None = "None";


	return None;
}

static cx::bool_t is_vc_item_ClCompile_PrecompiledHeader (std::string file_path)
{
	std::string filter;
	std::string item;


	filter = "stdafx.cpp";
	item   = cx::get_file_of_file_path(file_path);

	cx::to_upper(filter);
	cx::to_upper(item);

	if ( filter == item )
	{
		return true;
	}


	return false;
}

static cx::bool_t is_vc_item_CustomBuild_Ribbon (std::string file_path)
{
	std::string filter;
	std::string item;


	filter = "Ribbon.xml";
	item   = cx::get_file_of_file_path(file_path);

	cx::to_upper(filter);
	cx::to_upper(item);

	if ( filter == item )
	{
		return true;
	}


	return false;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_item::vc_item()
{
}

vc_item::~vc_item()
{
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_item_collection::vc_item_collection()
{
}

vc_item_collection::~vc_item_collection()
{
}

//===========================================================================
void vc_item_collection::cleanup (void)
{
	delete_pointer_in_vector<vc_item*>(_container);

	_container.clear();
}

void vc_item_collection::add (std::string file_path, std::string filter)
{
	//--------------------------------------------------------------------------
	std::string splash;
	std::string backsplash;


	splash     = "/";
	backsplash = "\\";
	filter = cx::replace(filter, splash, backsplash);


	//--------------------------------------------------------------------------
	vc_item* item;


	item = new vc_item();

	item->_file_path = file_path;
	item->_filter    = filter;
	item->_type      = get_vc_item_type(file_path);

	if ( is_vc_item_ClCompile_PrecompiledHeader(file_path) )
	{
		item->_option = "ClCompile_PCH";
	}
	if ( is_vc_item_CustomBuild_Ribbon(file_path) )
	{
		item->_option = "CustomBuild_Ribbon";
	}

	_container.push_back(item);
}

//===========================================================================
void vc_item_collection::get_project_filter(std::vector<std::string>& project_filter)
{
	std::vector<vc_item*>::iterator    i;
	std::set<std::string>::iterator    j;
	std::vector<std::string>::iterator k;

	std::set<std::string>    collection;
	std::vector<std::string> path_collection;
	std::set<std::string>    tree_collection;

	vc_item* item;

	std::string element;
	std::string delimiter;


	delimiter = "\\";


	for (i=_container.begin(); i!=_container.end(); i++)
	{
		item = (*i);

		if ( !item->_filter.empty() )
		{
			collection.insert ( (*i)->_filter );
		}
	}


	for (j=collection.begin(); j!=collection.end(); j++)
	{
		element = (*j);
		
		path_collection.clear();

		cx::split(element, delimiter, path_collection);

		if ( !path_collection.empty() )
		{
			element.clear();

			for (k=path_collection.begin(); k!=path_collection.end(); k++)
			{
				element += (*k);

				tree_collection.insert(element);

				element += delimiter;
			}
		}
	}


	for (j=tree_collection.begin(); j!=tree_collection.end(); j++)
	{
		project_filter.push_back( (*j) );
	}
}

void vc_item_collection::get_type_collection(std::string type, vc_item_collection& collection)
{
	std::vector<vc_item*>::iterator i;

	vc_item* item;


	for (i=_container.begin(); i!=_container.end(); i++)
	{
		item = (*i);

		if ( type == item->_type )
		{
			collection._container.push_back ( item );
		}
	}
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


