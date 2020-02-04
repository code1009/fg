#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_solution_generator : public generator
{
public:
	std::ostringstream _oss;

public:
	std::string _guid;

public:
	std::vector<vc_project_generator*> _project_container;

public:
	vc_solution_generator();
	virtual ~vc_solution_generator();

public:
	virtual cx::bool_t generate (void);

public:
	std::string get_file      (void);
	std::string get_file_path (void);

public:
	vc_project_generator* add_project (vc_project_generator* project);
	void cleanup (void);

public:
	void build (void);

public:
	void BLOCK_head (void);

	void BLOCK_Project_GROUP (void);
	void BLOCK_Project       (vc_project_generator* element);

	void BLOCK_Global     (void);
	void BLOCK_Global_SUB (void);

public:
	void BLOCK_GlobalSection_SolutionConfigurationPlatforms    (void);
	void BLOCK_GlobalSection_SolutionConfigurationPlatforms_SUB(void);

	void BLOCK_GlobalSection_ProjectConfigurationPlatforms    (void);
	void BLOCK_GlobalSection_ProjectConfigurationPlatforms_SUB(void);

	void BLOCK_GlobalSection_SolutionProperties (void);
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


