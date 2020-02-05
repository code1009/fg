#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_generator;



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_template_data_entry
{
public:
	std::string _variable;
	std::string _value;

public:
	vc_template_data_entry();
	~vc_template_data_entry();

public:
	void cleanup(void);
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_template_data_entry_collection
{
public:
	std::string                          _name;
	std::vector<vc_template_data_entry*> _contanier;

public:
	vc_template_data_entry_collection();
	~vc_template_data_entry_collection();

public:
	void cleanup(void);

public:
	void add (std::string variable, std::string value);
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_template_data_project_configuration
{
public:
	std::string _name;
	cx::bool_t  _debug;
	std::string _configuration;
	std::string _platform;

	std::map<std::string, vc_template_data_entry_collection*> _entry_collection_container;

public:
	vc_template_data_project_configuration();
	~vc_template_data_project_configuration();

public:
	void add_entry (std::string name, std::string variable, std::string value);
	vc_template_data_entry_collection* find_entry (std::string name);

public:
	void cleanup(void);
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_template_data
{
public:
	std::vector<vc_template_data_project_configuration*> _project_configuration_container;

public:
	vc_generator* _generator;

public:
	vc_template_data();
	~vc_template_data();

public:
	void cleanup (void);

public:
	vc_template_data_project_configuration* find_project_configuration (std::string name);

public:
	cx::bool_t load (std::string file_path);
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


