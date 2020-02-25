/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../header.hpp"
#include <set>
#include <cx/library/string/string_utility.hpp>




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_solution_generator::vc_solution_generator():
	generator()
{
}

vc_solution_generator::~vc_solution_generator()
{
}

//===========================================================================
cx::bool_t vc_solution_generator::generate (void)
{
	_oss.str("");

	build();
	
	return utf8_text_file_save(get_file_path(), _oss);
}

//===========================================================================
std::string vc_solution_generator::get_file (void)
{
	return get_parameter()->environment("$fg_name") + ".sln";
}

std::string vc_solution_generator::get_file_path (void)
{
	return get_parameter()->environment("$fg_target_root_directory") + get_file();
}

//===========================================================================
vc_project_generator* vc_solution_generator::add_project (vc_project_generator* project)
{
	_project_container.push_back(project);


	return project;
}

void vc_solution_generator::cleanup (void)
{
	_project_container.clear();
}

//===========================================================================
void vc_solution_generator::build (void)
{
	_guid = make_guid_string();

	BLOCK_head ();

	BLOCK_Project_GROUP ();

	BLOCK_Global ();
}

//===========================================================================
void vc_solution_generator::BLOCK_head (void)
{
	_oss << eline();

	_oss << "Microsoft Visual Studio Solution File, Format Version 12.00" << eline();
	_oss << "# Visual Studio 2013"                                        << eline();
	_oss << "VisualStudioVersion = 12.0.40629.0"                          << eline();
	_oss << "MinimumVisualStudioVersion = 10.0.40219.1"                   << eline();
}

//===========================================================================
void vc_solution_generator::BLOCK_Project_GROUP (void)
{
	std::vector<vc_project_generator*>::iterator i;

	vc_project_generator* element;


	for (i = _project_container.begin();
	     i!= _project_container.end  ();
	     i++ )
	{
		element = *i;
			
		BLOCK_Project(element);
	}
}

//===========================================================================
void vc_solution_generator::BLOCK_Project (vc_project_generator* element)
{
	std::string solution_guid;
	std::string project_name;
	std::string project_file;
	std::string project_guid;


	solution_guid = _guid;

	project_name = get_parameter()->environment("$fg_name");
	project_file = element->get_file();
	project_guid = element->_guid;

	_oss 
		<< "Project" 
		<< parentheses(dquot(solution_guid)) << " = " 
		<< dquot(project_name)               << ", " 
		<< dquot(project_file)               << ", " 
		<< dquot(project_guid)  
		<< eline();
			
	_oss << "EndProject" << eline();
}

//===========================================================================
void vc_solution_generator::BLOCK_Global (void)
{
	//-----------------------------------------------------------------------
	_oss << "Global" << eline();


	if (!_project_container.empty())
	{
		BLOCK_Global_SUB();
	}

	
	//-----------------------------------------------------------------------
	_oss << "EndGlobal" << eline();
}

void vc_solution_generator::BLOCK_Global_SUB (void)
{
	BLOCK_GlobalSection_SolutionConfigurationPlatforms();
	BLOCK_GlobalSection_ProjectConfigurationPlatforms ();
	BLOCK_GlobalSection_SolutionProperties            ();
}

//===========================================================================
void vc_solution_generator::BLOCK_GlobalSection_SolutionConfigurationPlatforms (void)
{
	_oss << itab(1u) << "GlobalSection(SolutionConfigurationPlatforms) = preSolution" << eline();

	BLOCK_GlobalSection_SolutionConfigurationPlatforms_SUB();

	_oss << itab(1u) << "EndGlobalSection" << eline();
}

void vc_solution_generator::BLOCK_GlobalSection_SolutionConfigurationPlatforms_SUB (void)
{
	std::vector<vc_project_generator*>::iterator i;
	vc_project_generator* project;


	std::vector<vc_project_configuration*>::iterator j;
	vc_project_configuration* project_configuration;


	std::set<std::string>::iterator k;
	std::set<std::string> container;

	std::string element;


	for (i = _project_container.begin();
	     i!= _project_container.end  ();
	     i++ )
	{
		project = *i;
			
		for (j = project->_configuration_container.begin();
			 j!= project->_configuration_container.end  ();
			 j++ )
		{
			project_configuration = *j;

			
			element = project_configuration->get_name();
			
			container.insert(element);
		}
	}


	for (k = container.begin();
	     k!= container.end  ();
	     k++ )
	{
		element = *k;


		_oss << itab(2u) << element << " = " << element << eline();
	}
}

//===========================================================================
void vc_solution_generator::BLOCK_GlobalSection_ProjectConfigurationPlatforms (void)
{
	_oss << itab(1u) << "GlobalSection(ProjectConfigurationPlatforms) = postSolution" << eline();

	BLOCK_GlobalSection_ProjectConfigurationPlatforms_SUB();

	_oss << itab(1u) << "EndGlobalSection" << eline();
}

void vc_solution_generator::BLOCK_GlobalSection_ProjectConfigurationPlatforms_SUB (void)
{
	std::vector<vc_project_generator*>::iterator i;
	vc_project_generator* project;


	std::vector<vc_project_configuration*>::iterator j;
	vc_project_configuration* project_configuration;

	std::string element;


	for (i = _project_container.begin();
	     i!= _project_container.end  ();
	     i++ )
	{
		project = *i;
			
		for (j = project->_configuration_container.begin();
			 j!= project->_configuration_container.end  ();
			 j++ )
		{
			project_configuration = *j;

			
			element = project_configuration->get_name();
			
			_oss << itab(2u) << project->_guid << "." << element << ".ActiveCfg = " << element << eline();
			_oss << itab(2u) << project->_guid << "." << element << ".Build.0 = "   << element << eline();
		}
	}
}

//===========================================================================
void vc_solution_generator::BLOCK_GlobalSection_SolutionProperties (void)
{
	_oss << itab(1u) << "GlobalSection(SolutionProperties) = preSolution" << eline();
	_oss << itab(2u) << "HideSolutionNode = FALSE"                        << eline();
	_oss << itab(1u) << "EndGlobalSection"                                << eline();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


