/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../header.hpp"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static const std::string _default_Extensions_Source_Files   = "cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx";
static const std::string _default_Extensions_Header_Files   = "h;hh;hpp;hxx;hm;inl;inc;xsd";
static const std::string _default_Extensions_Resource_Files = "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms";


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_project_filter_folder::vc_project_filter_folder()
{
}

vc_project_filter_folder::~vc_project_filter_folder()
{
}


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_project_filter_generator::vc_project_filter_generator():
	generator()
{
	_item_collection = CX_NULL_POINTER;
}

vc_project_filter_generator::~vc_project_filter_generator()
{
}

//===========================================================================
cx::bool_t vc_project_filter_generator::generate (void)
{
	_oss.str("");

	build();
	
	return utf8_text_file_save(get_file_path(), _oss);
}

//===========================================================================
std::string vc_project_filter_generator::get_file (void)
{
	return get_parameter()->environment("$fg_name") + ".vcxproj.filters";
}

std::string vc_project_filter_generator::get_file_path (void)
{
	return get_parameter()->environment("$fg_target_root_directory") + get_file();
}

//===========================================================================
vc_project_filter_folder* vc_project_filter_generator::add_folder (std::string name, std::string extensions)
{
	//--------------------------------------------------------------------------
	vc_project_filter_folder* element;


	element = new vc_project_filter_folder();

	element->_name       = name;
	element->_extensions = extensions;
	
	element->_guid = make_guid_string();

	
	_folder_container.push_back(element);


	return element;
}

vc_project_filter_folder* vc_project_filter_generator::add_folder (std::string name)
{
	std::string extensions;


	return add_folder (name, extensions);
}

void vc_project_filter_generator::cleanup (void)
{
	delete_pointer_in_vector<vc_project_filter_folder*>(_folder_container);
	_folder_container.clear();
}

//===========================================================================
void vc_project_filter_generator::build (void)
{
	TAG_xml();
	TAG_Project();
}

//===========================================================================
void vc_project_filter_generator::TAG_xml (void)
{
	_oss 
		<< "<?xml "
			<< "version="  << dquot("1.0"  ) << " "
			<< "encoding=" << dquot("utf-8") << "?>" 
			<< eline();
}

void vc_project_filter_generator::TAG_Project (void)
{
	_oss 
		<< "<Project "
			<< "ToolsVersion=" << dquot("4.0" )                                                << " "
			<< "xmlns="        << dquot("http://schemas.microsoft.com/developer/msbuild/2003") << ">" 
			<< eline();

	
	TAG_Project_SUB ();

	_oss 
		<< "</Project>"
			<< eline();
}

void vc_project_filter_generator::TAG_Project_SUB (void)
{
	TAG_ItemGroup_SUBTAG_Filter ();

	TAG_ItemGroup_SUBTAG_XXX ("ClCompile"      );
	TAG_ItemGroup_SUBTAG_XXX ("ClInclude"      );
	TAG_ItemGroup_SUBTAG_XXX ("ResourceCompile");
	TAG_ItemGroup_SUBTAG_XXX ("Image"          );
	TAG_ItemGroup_SUBTAG_XXX ("Text"           );
	TAG_ItemGroup_SUBTAG_XXX ("Manifest"       );
	TAG_ItemGroup_SUBTAG_XXX ("CustomBuild"    );
	TAG_ItemGroup_SUBTAG_XXX ("None"           );
}

//===========================================================================
void vc_project_filter_generator::TAG_ItemGroup_SUBTAG_Filter (void)
{
	//--------------------------------------------------------------------------
	if (_folder_container.empty())
	{
		return;
	}


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< "<ItemGroup>"
		<< eline();


	//--------------------------------------------------------------------------
	std::vector< vc_project_filter_folder* >::iterator i;
	vc_project_filter_folder*                          element;


	for (i=_folder_container.begin(); i!=_folder_container.end(); i++)
	{
		element = *(i);


		_oss 
			<< ispace2(2) 
			<< "<Filter "
			<< "Include=" <<dquot(element->_name) << ">"
			<< eline();


		_oss 
			<< ispace2(3) 
			<< "<UniqueIdentifier>"
			<< element->_guid
			<< "</UniqueIdentifier>"
			<< eline();

		if (!element->_extensions.empty())
		{
			_oss 
				<< ispace2(3) 
				<< "<Extensions>"
				<< element->_extensions
				<< "</Extensions>"
				<< eline();
		}


		_oss 
			<< ispace2(2) 
			<< "</Filter>"
			<< eline();
	}


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< "</ItemGroup>"
		<< eline();
}

void vc_project_filter_generator::TAG_ItemGroup_SUBTAG_XXX (std::string item_type)
{
	//--------------------------------------------------------------------------
	vc_item_collection collection;


	_item_collection->get_type_collection(item_type, collection);


	if (collection._container.empty())
	{
		return;
	}


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< "<ItemGroup>"
		<< eline();


	//--------------------------------------------------------------------------
	std::vector< vc_item* >::iterator i;
	vc_item*                          element;


	for (i=collection._container.begin(); i!=collection._container.end(); i++)	
	{
		element = (*i);


		if (element->_filter.empty())
		{
			_oss 
				<< ispace2(2) 
				<< "<" << item_type << " "
				<< "Include=" <<dquot(element->_file_path) << "/>"
				<< eline();
		}
		else
		{
			_oss 
				<< ispace2(2) 
				<< "<" << item_type << " "
				<< "Include=" <<dquot(element->_file_path) << ">"
				<< eline();

			_oss 
				<< ispace2(3) 
				<< "<Filter>"
				<< element->_filter
				<< "</Filter>"
				<< eline();

			_oss 
				<< ispace2(2) 
				<< "</" << item_type << ">"
				<< eline();
		}
	}


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< "</ItemGroup>"
		<< eline();
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


