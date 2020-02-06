/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../header.hpp"





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_project_configuration::vc_project_configuration()
{
	_configuration = "Debug";
	_platform      = "Win32";

	_debug = true;
}

vc_project_configuration::vc_project_configuration(std::string c, std::string p, cx::bool_t d)
{
	_configuration = c;
	_platform      = p;

	_debug = d;
}

//===========================================================================
std::string vc_project_configuration::get_name(void)
{
	std::string s;


	s = _configuration;
	s+= "|";
	s+= _platform;

	return s;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_project_generator::vc_project_generator():
	generator()
{
	_item_collection = CX_NULL_POINTER;
	
	_template_data = CX_NULL_POINTER;
}

vc_project_generator::~vc_project_generator()
{
}

//===========================================================================
cx::bool_t vc_project_generator::generate (void)
{
	_oss.str("");

	build();
	
	return utf8_text_file_save(get_file_path(), _oss);
}

//===========================================================================
std::string vc_project_generator::get_file (void)
{
	return get_parameter()->environment("$fg_name") + ".vcxproj";
}

std::string vc_project_generator::get_file_path (void)
{
	return get_parameter()->environment("$fg_target_root_directory") + get_file();
}

//===========================================================================
vc_project_configuration* vc_project_generator::add_configuration (std::string c, std::string p, cx::bool_t d)
{
	vc_project_configuration* element;


	element = new vc_project_configuration(c, p, d);

	
	_configuration_container.push_back(element);


	return element;
}

void vc_project_generator::cleanup (void)
{
	delete_pointer_in_vector<vc_project_configuration*>(_configuration_container);
	_configuration_container.clear();
}

//===========================================================================
void vc_project_generator::build (void)
{
	_guid = make_guid_string();

	TAG_xml();
	TAG_Project();
}

//===========================================================================
void vc_project_generator::TAG_xml (void)
{
	_oss 
		<< "<?xml "
			<< "version="  << dquot("1.0"  ) << " "
			<< "encoding=" << dquot("utf-8") << "?>" 
			<< eline();
}

//===========================================================================
void vc_project_generator::TAG_Project (void)
{
	_oss 
		<< "<Project "
			<< "DefaultTargets=" << dquot("Build" )                                              << " "
			<< "ToolsVersion="   << dquot("12.0" )                                               << " "
			<< "xmlns="          << dquot("http://schemas.microsoft.com/developer/msbuild/2003") << ">" 
			<< eline();


	TAG_Project_SUB();


	_oss 
		<< "</Project>"
			<< eline();
}

void vc_project_generator::TAG_Project_SUB (void)
{
	TAG_ItemGroup_ATTR_Label_ProjectConfigurations                    ();
	TAG_PropertyGroup_ATTR_Label_Globals                              ();
	TAG_Import_ATTR_Project_VCTargetsPath_Microsoft_Cpp_Default_props ();
	TAG_PropertyGroup_ATTR_Label_Configuration_GROUP                  ();
	TAG_Import_ATTR_Project_VCTargetsPath_Microsoft_Cpp_props         ();
	TAG_ImportGroup_ATTR_Label_ExtensionSettings                      ();
	TAG_ImportGroup_ATTR_Label_PropertySheets_GROUP                   ();
	TAG_PropertyGroup_ATTR_Label_UserMacros                           ();
	TAG_PropertyGroup_GROUP                                           ();
	TAG_ItemDefinitionGroup_GROUP                                     ();

	TAG_ItemGroup_GROUP                                               ();

	TAG_Import_ATTR_Project_VCTargetsPath_Microsoft_Cpp_targets       ();
	TAG_ImportGroup_ATTR_Label_ExtensionTargets                       ();
}

//===========================================================================
void vc_project_generator::TAG_ItemGroup_ATTR_Label_ProjectConfigurations (void)
{
	_oss 
		<< ispace2(1) 
		<< "<ItemGroup"
		<< " Label=" << dquot("ProjectConfigurations") 
		<< ">" 
		<< eline();


	TAG_ItemGroup_ATTR_Label_ProjectConfigurations_SUB();


	_oss 
		<< ispace2(1) 
		<< "</ItemGroup>"  
		<< eline();
}

void vc_project_generator::TAG_ItemGroup_ATTR_Label_ProjectConfigurations_SUB (void)
{
	std::vector<vc_project_configuration*>::iterator i;

	vc_project_configuration* e;


	for (i =_configuration_container.begin();
	     i!=_configuration_container.end();
	     i++)
	{
		e = *i;

		TAG_ItemGroup_ATTR_Label_ProjectConfigurations_SUBTAG_ProjectConfiguration(e);
	}
}

void vc_project_generator::TAG_ItemGroup_ATTR_Label_ProjectConfigurations_SUBTAG_ProjectConfiguration (vc_project_configuration* e)
{
	_oss 
		<< ispace2(2) 
		<< "<ProjectConfiguration"
		<< " Include=" << dquot(e->get_name()) 
		<< ">" 
		<< eline();

	_oss << ispace2(3) << "<Configuration>" << e->_configuration << "</Configuration>" << eline();
	_oss << ispace2(3) << "<Platform>"      << e->_platform      << "</Platform>"      << eline();

	_oss << ispace2(2) << "</ProjectConfiguration>"<< eline();
}

//===========================================================================
void vc_project_generator::TAG_PropertyGroup_ATTR_Label_Globals (void)
{
	_oss 
		<< ispace2(1) 
		<< "<PropertyGroup"
		<< " Label=" << dquot("Globals") 
		<< ">" 
		<< eline();

	_oss << ispace2(2) << "<ProjectGuid>"   << _guid       << "</ProjectGuid>"   << eline();
//	_oss << ispace2(2) << "<Keyword>"       << "Win32Proj" << "</Keyword>"       << eline();
//	_oss << ispace2(2) << "<RootNamespace>" << _name       << "</RootNamespace>" << eline();

	_oss 
		<< ispace2(1) 
		<< "</PropertyGroup>"  
		<< eline();
}

//===========================================================================
void vc_project_generator::TAG_Import_ATTR_Project_VCTargetsPath_Microsoft_Cpp_Default_props (void)
{
	_oss 
		<< ispace2(1) 
		<< "<Import"
		<< " Project=" << dquot("$(VCTargetsPath)\\Microsoft.Cpp.Default.props") 
		<< " />" 
		<< eline();
}

//===========================================================================
void vc_project_generator::TAG_PropertyGroup_ATTR_Label_Configuration_GROUP (void)
{
	std::vector<vc_project_configuration*>::iterator i;

	vc_project_configuration* e;


	for (i =_configuration_container.begin();
	     i!=_configuration_container.end();
	     i++)
	{
		e = *i;

		TAG_PropertyGroup_ATTR_Label_Configuration(e);
	}
}

void vc_project_generator::TAG_PropertyGroup_ATTR_Label_Configuration (vc_project_configuration* e)
{
	//--------------------------------------------------------------------------
	std::string Condition="\'$(Configuration)|$(Platform)\'";


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< "<PropertyGroup"
		<< " Condition="  << "\"" << Condition << "==" << squot(e->get_name()) << "\"" 
		<< " Label=" << dquot("Configuration") 
		<< ">" 
		<< eline();


	//--------------------------------------------------------------------------
	TAG_PropertyGroup_ATTR_Label_Configuration_SUB(e);


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< "</PropertyGroup>"  
		<< eline();
}

// TODO:
void vc_project_generator::TAG_PropertyGroup_ATTR_Label_Configuration_SUB (vc_project_configuration* e)
{
	//--------------------------------------------------------------------------
	vc_template_data_project_configuration* p;
	vc_template_data_entry_collection*      c;


	p = _template_data->find_project_configuration(e->get_name());
	if (CX_NULL_POINTER!=p)
	{
		c = p->find_entry("/property/configuration");

		if (CX_NULL_POINTER!=c)
		{
			if (!c->_contanier.empty())
			{
				std::vector<vc_template_data_entry*>::iterator i;
			
				vc_template_data_entry* e;


				for (i =c->_contanier.begin();
					 i!=c->_contanier.end();
					 i++)
				{
					e = *i;

					TAG_PropertyGroup_ATTR_Label_Configuration_SUBTAG(e->_variable, e->_value);
				}

				return;
			}
		}
	}


	//--------------------------------------------------------------------------
	std::string ConfigurationType       ;
	std::string UseDebugLibraries       ;
	std::string PlatformToolset         ;
	std::string WholeProgramOptimization;
	std::string CharacterSet            ;


	ConfigurationType        = "Application"               ;
	UseDebugLibraries        = e->_debug ? "true" : "false";
	PlatformToolset          = "v120"                      ; // "v120" or "v120_xp"
	WholeProgramOptimization = "true"                      ;
	CharacterSet             = "MultiByte"                 ; // "MultiByte" or "Unicode";

	TAG_PropertyGroup_ATTR_Label_Configuration_SUBTAG("ConfigurationType", ConfigurationType);
	TAG_PropertyGroup_ATTR_Label_Configuration_SUBTAG("UseDebugLibraries", UseDebugLibraries);
	TAG_PropertyGroup_ATTR_Label_Configuration_SUBTAG("PlatformToolset"  , PlatformToolset  );
	if (!e->_debug)
	{
		TAG_PropertyGroup_ATTR_Label_Configuration_SUBTAG("WholeProgramOptimization", WholeProgramOptimization);
	}
	TAG_PropertyGroup_ATTR_Label_Configuration_SUBTAG("CharacterSet"     , CharacterSet    );
}

void vc_project_generator::TAG_PropertyGroup_ATTR_Label_Configuration_SUBTAG (std::string name, std::string value)
{
	if (value.empty())
	{
		_oss << ispace2(2) << "<" << name << "/>" << eline();
	}
	else
	{
		_oss << ispace2(2) << "<" << name << ">" << value << "</" << name << ">" << eline();
	}
}

//===========================================================================
void vc_project_generator::TAG_Import_ATTR_Project_VCTargetsPath_Microsoft_Cpp_props (void)
{
	_oss 
		<< ispace2(1) 
		<< "<Import"
		<< " Project=" << dquot("$(VCTargetsPath)\\Microsoft.Cpp.props") 
		<< " />" 
		<< eline();
}

//===========================================================================
void vc_project_generator::TAG_ImportGroup_ATTR_Label_ExtensionSettings (void)
{
	_oss 
		<< ispace2(1) 
		<< "<ImportGroup"
		<< " Label=" << dquot("ExtensionSettings") 
		<< ">" 
		<< eline();

	_oss 
		<< ispace2(1) 
		<< "</ImportGroup>"  
		<< eline();
}

//===========================================================================
void vc_project_generator::TAG_ImportGroup_ATTR_Label_PropertySheets_GROUP (void)
{
	std::vector<vc_project_configuration*>::iterator i;

	vc_project_configuration* e;


	for (i =_configuration_container.begin();
	     i!=_configuration_container.end();
	     i++)
	{
		e = *i;

		TAG_ImportGroup_ATTR_Label_PropertySheets(e);
	}
}

void vc_project_generator::TAG_ImportGroup_ATTR_Label_PropertySheets (vc_project_configuration* e)
{
	std::string Condition = "\'$(Configuration)|$(Platform)\'";
	std::string Condition_LocalAppDataPlatform = "exists(\'$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\')";


	_oss 
		<< ispace2(1) 
		<< "<ImportGroup"
		<< " Label=" << dquot("PropertySheets") 
		<< " Condition="  << "\"" << Condition << "==" << squot(e->get_name()) << "\"" 
		<< ">" 
		<< eline();

	
	_oss 
		<< ispace2(2) 
		<< "<Import"
		<< " Project="   << dquot("$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props") 
		<< " Condition=" << dquot(Condition_LocalAppDataPlatform) 
		<< " Label="     << dquot("LocalAppDataPlatform") 
		<< " />" 
		<< eline();


	_oss 
		<< ispace2(1) 
		<< "</ImportGroup>"  
		<< eline();
}

//===========================================================================
void vc_project_generator::TAG_PropertyGroup_ATTR_Label_UserMacros (void)
{
	_oss 
		<< ispace2(1) 
		<< "<PropertyGroup"
		<< " Label=" << dquot("UserMacros") 
		<< " />" 
		<< eline();
}

//===========================================================================
void vc_project_generator::TAG_PropertyGroup_GROUP (void)
{
	std::vector<vc_project_configuration*>::iterator i;

	vc_project_configuration* e;


	for (i =_configuration_container.begin();
	     i!=_configuration_container.end();
	     i++)
	{
		e = *i;

		TAG_PropertyGroup(e);
	}
}

void vc_project_generator::TAG_PropertyGroup (vc_project_configuration* e)
{
	std::string Condition="\'$(Configuration)|$(Platform)\'";


	_oss << ispace2(1) 
		<< "<PropertyGroup"
		<< " Condition="  << "\"" << Condition << "==" << squot(e->get_name()) << "\"" 
		<< ">" 
		<< eline();

	
	TAG_PropertyGroup_SUB(e);


	_oss << ispace2(1) 
		<< "</PropertyGroup>"  
		<< eline();
}

// TODO:
void vc_project_generator::TAG_PropertyGroup_SUB (vc_project_configuration* e)
{
	//--------------------------------------------------------------------------
	vc_template_data_project_configuration* p;
	vc_template_data_entry_collection*      c;


	p = _template_data->find_project_configuration(e->get_name());
	if (CX_NULL_POINTER!=p)
	{
		c = p->find_entry("/property");

		if (CX_NULL_POINTER!=c)
		{
			if (!c->_contanier.empty())
			{
				std::vector<vc_template_data_entry*>::iterator i;
			
				vc_template_data_entry* e;


				for (i =c->_contanier.begin();
					 i!=c->_contanier.end();
					 i++)
				{
					e = *i;

					TAG_PropertyGroup_SUBTAG(e->_variable, e->_value);
				}

				return;
			}
		}
	}


	//--------------------------------------------------------------------------
	std::string IntDir;
	std::string OutDir;
	std::string LinkIncremental;


	IntDir          ="$(SolutionDir)output\\object\\$(Configuration)\\$(ProjectName)\\";
	OutDir          ="$(SolutionDir)output\\$(Configuration)\\";
	LinkIncremental = e->_debug ? "true" : "false";

	TAG_PropertyGroup_SUBTAG("IntDir"         , IntDir);
	TAG_PropertyGroup_SUBTAG("OutDir"         , OutDir);
	TAG_PropertyGroup_SUBTAG("LinkIncremental", LinkIncremental);
}

void vc_project_generator::TAG_PropertyGroup_SUBTAG (std::string name, std::string value)
{
	if (value.empty())
	{
		_oss << ispace2(2) << "<" << name << "/>" << eline();
	}
	else
	{
		_oss << ispace2(2) << "<" << name << ">" << value << "</" << name << ">" << eline();
	}
}

//===========================================================================
void vc_project_generator::TAG_ItemDefinitionGroup_GROUP(void)
{
	std::vector<vc_project_configuration*>::iterator i;

	vc_project_configuration* e;


	for (i =_configuration_container.begin();
	     i!=_configuration_container.end();
	     i++)
	{
		e = *i;

		TAG_ItemDefinitionGroup(e);
	}
}

void vc_project_generator::TAG_ItemDefinitionGroup (vc_project_configuration* e)
{
	std::string Condition="\'$(Configuration)|$(Platform)\'";


	_oss << ispace2(1) 
		<< "<ItemDefinitionGroup"
		<< " Condition="  << "\"" << Condition << "==" << squot(e->get_name()) << "\"" 
		<< ">" 
		<< eline();


	TAG_ItemDefinitionGroup_SUB (e);


	_oss << ispace2(1) 
		<< "</ItemDefinitionGroup>"  
		<< eline();
}

void vc_project_generator::TAG_ItemDefinitionGroup_SUB (vc_project_configuration* e)
{
	TAG_ItemDefinitionGroup_SUBTAG (e, "ClCompile"      );
	TAG_ItemDefinitionGroup_SUBTAG (e, "Link"           );
//	TAG_ItemDefinitionGroup_SUBTAG (e, "ResourceCompile");
//	TAG_ItemDefinitionGroup_SUBTAG (e, "Midl"           );
}

void vc_project_generator::TAG_ItemDefinitionGroup_SUBTAG (vc_project_configuration* e, std::string group)
{
	_oss << ispace2(2) << "<"  << group << ">" << eline();

	
	if      (group=="ClCompile")
	{
		TAG_ItemDefinitionGroup_SUBTAG_ClCompile(e);
	}
	else if (group=="Link")
	{
		TAG_ItemDefinitionGroup_SUBTAG_Link(e);
	}
	else if (group=="ResourceCompile")
	{
		TAG_ItemDefinitionGroup_SUBTAG_ResourceCompile(e);
	}
	else if (group=="Midl")
	{
		TAG_ItemDefinitionGroup_SUBTAG_Midl(e);
	}


	_oss << ispace2(2) << "</" << group << ">" << eline();
}

void vc_project_generator::TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG (std::string name, std::string value)
{
	if (value.empty())
	{
		_oss << ispace2(3) << "<" << name << "/>" << eline();
	}
	else
	{
		_oss << ispace2(3) << "<" << name << ">" << value << "</" << name << ">" << eline();
	}
}

// TODO:
void vc_project_generator::TAG_ItemDefinitionGroup_SUBTAG_ClCompile (vc_project_configuration* e)
{
	//--------------------------------------------------------------------------
	vc_template_data_project_configuration* p;
	vc_template_data_entry_collection*      c;


	p = _template_data->find_project_configuration(e->get_name());
	if (CX_NULL_POINTER!=p)
	{
		c = p->find_entry("/compile");

		if (CX_NULL_POINTER!=c)
		{
			if (!c->_contanier.empty())
			{
				std::vector<vc_template_data_entry*>::iterator i;
			
				vc_template_data_entry* e;


				for (i =c->_contanier.begin();
					 i!=c->_contanier.end();
					 i++)
				{
					e = *i;

					TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG(e->_variable, e->_value);
				}

				return;
			}
		}
	}


	//-----------------------------------------------------------------------
	std::string WarningLevel           ;
	std::string PrecompiledHeader      ;
	std::string Optimization           ;
	std::string FunctionLevelLinking   ;
	std::string IntrinsicFunctions     ;
	std::string PreprocessorDefinitions;
	std::string SDLCheck               ;


	if (e->_debug)
	{
		PrecompiledHeader      ="Use";
		WarningLevel           ="Level3";
		Optimization           ="Disabled";
		PreprocessorDefinitions="_CRT_SECURE_NO_WARNINGS;WIN32;_DEBUG;_WINDOWS;%(PreprocessorDefinitions)";
		SDLCheck               ="true";

		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("PrecompiledHeader"      , PrecompiledHeader      );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("WarningLevel"           , WarningLevel           );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("Optimization"           , Optimization           );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("PreprocessorDefinitions", PreprocessorDefinitions);
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("SDLCheck"               , SDLCheck               );
	}
	else
	{
		WarningLevel           ="Level3";                                          
		PrecompiledHeader      ="Use";                                             
		Optimization           ="MaxSpeed";                                        
		FunctionLevelLinking   ="true";                                            
		IntrinsicFunctions     ="true";                                            
		PreprocessorDefinitions="_CRT_SECURE_NO_WARNINGS;WIN32;NDEBUG;_WINDOWS;%(PreprocessorDefinitions)";
		SDLCheck               ="true";                                            

		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("WarningLevel"           , WarningLevel           );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("PrecompiledHeader"      , PrecompiledHeader      );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("Optimization"           , Optimization           );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("FunctionLevelLinking"   , FunctionLevelLinking   );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("IntrinsicFunctions"     , IntrinsicFunctions     );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("PreprocessorDefinitions", PreprocessorDefinitions);
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("SDLCheck"               , SDLCheck               );
	}


	//-----------------------------------------------------------------------
	std::string AdditionalIncludeDirectories;


	AdditionalIncludeDirectories = "D:/Development/SDK/cxLibrary;D:/Development/SDK/cxLibrary/Win32xx/870/include";

	TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("AdditionalIncludeDirectories", AdditionalIncludeDirectories);



	//-----------------------------------------------------------------------
	std::string RuntimeLibrary;


	if (e->_debug)
	{
		RuntimeLibrary = "MultiThreadedDebug";
	}
	else
	{
		RuntimeLibrary = "MultiThreaded";
	}

	TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("RuntimeLibrary", RuntimeLibrary);


	//-----------------------------------------------------------------------
	std::string ObjectFileName;


	ObjectFileName = "$(IntDir)%(RelativeDir)\\";

	TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("ObjectFileName", ObjectFileName);
}

void vc_project_generator::TAG_ItemDefinitionGroup_SUBTAG_Link (vc_project_configuration* e)
{
	//--------------------------------------------------------------------------
	vc_template_data_project_configuration* p;
	vc_template_data_entry_collection*      c;


	p = _template_data->find_project_configuration(e->get_name());
	if (CX_NULL_POINTER!=p)
	{
		c = p->find_entry("/link");

		if (CX_NULL_POINTER!=c)
		{
			if (!c->_contanier.empty())
			{
				std::vector<vc_template_data_entry*>::iterator i;
			
				vc_template_data_entry* e;


				for (i =c->_contanier.begin();
					 i!=c->_contanier.end();
					 i++)
				{
					e = *i;

					TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG(e->_variable, e->_value);
				}

				return;
			}
		}
	}


	//-----------------------------------------------------------------------
	std::string AdditionalLibraryDirectories;


	if (e->_debug)
	{
		AdditionalLibraryDirectories = "D:/Development/SDK/cxLibrary/cx3/build_msvc2013_static_library/Debug;D:/Development/SDK/cxLibrary/Visual Leak Detector/2.5.1/lib/Win32";

		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("SubSystem"                    , "Windows" );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("GenerateDebugInformation"     , "true"    );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("AdditionalLibraryDirectories" , AdditionalLibraryDirectories);
	}
	else
	{
		AdditionalLibraryDirectories = "D:/Development/SDK/cxLibrary/cx3/build_msvc2013_static_library/Release";

		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("SubSystem"                    , "Windows");
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("GenerateDebugInformation"     , "true"   );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("EnableCOMDATFolding"          , "true"   );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("OptimizeReferences"           , "true"   );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("AdditionalLibraryDirectories" , AdditionalLibraryDirectories);
	}
}

void vc_project_generator::TAG_ItemDefinitionGroup_SUBTAG_ResourceCompile (vc_project_configuration* e)
{
	//--------------------------------------------------------------------------
	vc_template_data_project_configuration* p;
	vc_template_data_entry_collection*      c;


	p = _template_data->find_project_configuration(e->get_name());
	if (CX_NULL_POINTER!=p)
	{
		c = p->find_entry("/compile-res");

		if (CX_NULL_POINTER!=c)
		{
			if (!c->_contanier.empty())
			{
				std::vector<vc_template_data_entry*>::iterator i;
			
				vc_template_data_entry* e;


				for (i =c->_contanier.begin();
					 i!=c->_contanier.end();
					 i++)
				{
					e = *i;

					TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG(e->_variable, e->_value);
				}

				return;
			}
		}
	}


	//-----------------------------------------------------------------------
	std::string Culture                      ;
	std::string AdditionalIncludeDirectories ;
	std::string PreprocessorDefinitions      ;
	std::string AdditionalOptions            ;


	Culture                     ="0x0409";
	AdditionalIncludeDirectories="$(IntDir);../../sdk/WTL/10_8280/Include;%(AdditionalIncludeDirectories)";
	AdditionalOptions           ="";

	if (e->_debug)
	{
		PreprocessorDefinitions     ="_DEBUG;%(PreprocessorDefinitions)";

		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("Culture"                     , Culture                     );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("AdditionalIncludeDirectories", AdditionalIncludeDirectories);
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("PreprocessorDefinitions"     , PreprocessorDefinitions     );
//		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("AdditionalOptions"           , AdditionalOptions           );
	}
	else
	{
		PreprocessorDefinitions     ="NDEBUG;%(PreprocessorDefinitions)";

		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("Culture"                     , Culture                     );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("AdditionalIncludeDirectories", AdditionalIncludeDirectories);
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("PreprocessorDefinitions"     , PreprocessorDefinitions     );
//		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("AdditionalOptions"           , AdditionalOptions           );
	}
}

void vc_project_generator::TAG_ItemDefinitionGroup_SUBTAG_Midl (vc_project_configuration* e)
{
	//--------------------------------------------------------------------------
	vc_template_data_project_configuration* p;
	vc_template_data_entry_collection*      c;


	p = _template_data->find_project_configuration(e->get_name());
	if (CX_NULL_POINTER!=p)
	{
		c = p->find_entry("/compile-midl");

		if (CX_NULL_POINTER!=c)
		{
			if (!c->_contanier.empty())
			{
				std::vector<vc_template_data_entry*>::iterator i;
			
				vc_template_data_entry* e;


				for (i =c->_contanier.begin();
					 i!=c->_contanier.end();
					 i++)
				{
					e = *i;

					TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG(e->_variable, e->_value);
				}

				return;
			}
		}
	}


	//-----------------------------------------------------------------------
	std::string MkTypLibCompatible     ;
	std::string TargetEnvironment      ;
	std::string PreprocessorDefinitions;


	if (e->_debug)
	{
		MkTypLibCompatible     ="false";
		TargetEnvironment      ="Win32";
		PreprocessorDefinitions="NDEBUG;%(PreprocessorDefinitions)";

		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("MkTypLibCompatible"      , MkTypLibCompatible     );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("TargetEnvironment"       , TargetEnvironment      );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("PreprocessorDefinitions" , PreprocessorDefinitions);
	}
	else
	{
		MkTypLibCompatible     ="false";
		TargetEnvironment      ="Win32";
		PreprocessorDefinitions="NDEBUG;%(PreprocessorDefinitions)";

		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("MkTypLibCompatible"      , MkTypLibCompatible     );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("TargetEnvironment"       , TargetEnvironment      );
		TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG("PreprocessorDefinitions" , PreprocessorDefinitions);
	}
}

//===========================================================================
void vc_project_generator::TAG_ImportGroup_ATTR_Label_ExtensionTargets(void)
{
	_oss 
		<< ispace2(1) 
		<< "<ImportGroup"
		<< " Label=" << dquot("ExtensionTargets") 
		<< ">" 
		<< eline();

	_oss 
		<< ispace2(1) 
		<< "</ImportGroup>" 
		<< eline();
}

//===========================================================================
void vc_project_generator::TAG_Import_ATTR_Project_VCTargetsPath_Microsoft_Cpp_targets(void)
{
	_oss 
		<< ispace2(1) 
		<< "<Import"
		<< " Project=" << dquot("$(VCTargetsPath)\\Microsoft.Cpp.targets") 
		<< " />" 
		<< eline();
}

//===========================================================================
void vc_project_generator::TAG_ItemGroup_GROUP(void)
{
	TAG_ItemGroup ("Text"           );
	TAG_ItemGroup ("ClInclude"      );
	TAG_ItemGroup ("ClCompile"      );
	TAG_ItemGroup ("ResourceCompile");
	TAG_ItemGroup ("Image"          );
	TAG_ItemGroup ("Manifest"       );
	TAG_ItemGroup ("CustomBuild"    );
	TAG_ItemGroup ("None"           );
}

void vc_project_generator::TAG_ItemGroup(std::string item_type)
{
	vc_item_collection collection;


	_item_collection->get_type_collection(item_type, collection);


	if (collection._container.empty())
	{
		return;
	}


	_oss << ispace2(1) << "<ItemGroup>" << eline();


	std::vector<vc_item*>::iterator i;

	vc_item* element;


	for(i=collection._container.begin(); i!=collection._container.end(); i++)
	{
		element = (*i);

		TAG_ItemGroup_SUBTAG(element);
	}


	_oss << ispace2(1) << "</ItemGroup>" << eline();
}

void vc_project_generator::TAG_ItemGroup_SUBTAG(vc_item* item)
{
	if (item->_option.empty())
	{
		_oss 
			<< ispace2(2) 
			<< "<" << item->_type
			<< " Include=" << dquot(item->_file_path)
			<< " />" 
			<< eline();
	}
	else
	{
		//	"ClCompile_PrecompiledHeader_XXX"     
		//	"CustomBuild_Ribbon"

		if      (item->_type=="ClCompile")
		{
			if (item->_option=="ClCompile_PrecompiledHeader_Create")
			{
				TAG_ItemGroup_SUBTAG_ClCompile_PrecompiledHeader(item, "Create");
			}
			if (item->_option=="ClCompile_PrecompiledHeader_NotUsing")
			{
				TAG_ItemGroup_SUBTAG_ClCompile_PrecompiledHeader(item, "NotUsing");
			}
		}
		else if (item->_type=="CustomBuild")
		{
			if (item->_option=="CustomBuild_Ribbon")
			{
				TAG_ItemGroup_SUBTAG_CustomBuild_Ribbon(item);
			}
		}
	}
}

void vc_project_generator::TAG_ItemGroup_SUBTAG_ClCompile_PrecompiledHeader(vc_item* item, std::string value)
{
	//--------------------------------------------------------------------------
	_oss << ispace2(2) 
		<< "<" << "ClCompile"
		<< " Include=" << dquot(item->_file_path)
		<< ">" 
		<< eline();


	//--------------------------------------------------------------------------
	std::string Condition="\'$(Configuration)|$(Platform)\'";
	std::string ProjectConfiguration;


	//--------------------------------------------------------------------------
	std::vector<vc_project_configuration*>::iterator i;

	vc_project_configuration* e;


	for (i =_configuration_container.begin();
	     i!=_configuration_container.end();
	     i++)
	{
		e = *i;


		ProjectConfiguration = e->get_name();

		_oss 
			<< ispace2(3) 
			<< "<PrecompiledHeader" 
			<< " Condition="  << "\"" << Condition << "==" << squot(ProjectConfiguration) << "\"" 
			<< ">" 
			<< value
			<< "</PrecompiledHeader>"
			<< eline();
	}


	//--------------------------------------------------------------------------
	_oss << ispace2(2) << "</ClCompile>" << eline();
}

void vc_project_generator::TAG_ItemGroup_SUBTAG_CustomBuild_Ribbon(vc_item* item)
{
	//--------------------------------------------------------------------------
#if 0
    <CustomBuild Include="..\src\Ribbon.xml">
      <Command Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'"  >uicc.exe ..\src\Ribbon.xml ..\src\Ribbon.bml /header:..\src\RibbonUI.h /res:..\src\RibbonUI.rc</Command>
      <Outputs Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'"  >Ribbon.bml;RibbonUI.rc;RibbonUI.h</Outputs>
      <Command Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">uicc.exe ..\src\Ribbon.xml ..\src\Ribbon.bml /header:..\src\RibbonUI.h /res:..\src\RibbonUI.rc</Command>
      <Outputs Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">Ribbon.bml;RibbonUI.rc;RibbonUI.h</Outputs>
    </CustomBuild>
#endif
	/*
	1. Ribbon.xml 파일로 Ribbon.bml RibbonUI.h RibbonUI.rc 생성
	2. Resouce.rc에서
		//////////////////////////////////////////////////////
		// Ribbon
		#include "RibbonUI.rc"
	3. RibbonUI.rc에서
		...
		#include ".\RibbonUI.h"
		...
		APPLICATION_RIBBON    UIFILE    "..\\src\\Ribbon.bml"
	*/
	

	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(2) 
		<< "<CustomBuild" 
		<< " Include=" << dquot(item->_file_path)
		<< ">" 
		<< eline();


	//--------------------------------------------------------------------------
	std::string Condition="\'$(Configuration)|$(Platform)\'";
	std::string ProjectConfiguration;
	std::string Command;
	std::string Outputs;
  

	Command = "uicc.exe ..\\src\\Ribbon.xml ..\\src\\Ribbon.bml /header:..\\src\\RibbonUI.h /res:..\\src\\RibbonUI.rc";
	Outputs = "Ribbon.bml;RibbonUI.rc;RibbonUI.h";


	//--------------------------------------------------------------------------
	std::vector<vc_project_configuration*>::iterator i;

	vc_project_configuration* e;


	for (i =_configuration_container.begin();
	     i!=_configuration_container.end();
	     i++)
	{
		e = *i;


		ProjectConfiguration = e->get_name();

		_oss << ispace2(3) 
			<< "<Command" 
			<< " Condition="  << "\"" << Condition << "==" << squot(ProjectConfiguration) << "\"" 
			<< ">" 
			<< Command
			<< "</Command>"
			<< eline();

		_oss << ispace2(3) 
			<< "<Outputs" 
			<< " Condition="  << "\"" << Condition << "==" << squot(ProjectConfiguration) << "\"" 
			<< ">" 
			<< Outputs
			<< "</Outputs>"
			<< eline();
	}


	//--------------------------------------------------------------------------
	_oss << ispace2(2) << "</CustomBuild>" << eline();
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


