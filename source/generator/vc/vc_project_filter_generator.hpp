#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_project_filter_folder
{
public:
	std::string _name      ; // Filter: name\\name
	std::string _guid      ;
	std::string _extensions;

public:
	vc_project_filter_folder();
	virtual ~vc_project_filter_folder();
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_project_filter_generator : public generator
{
public:
	std::ostringstream _oss;

public:
	std::vector<vc_project_filter_folder*> _folder_container;
	vc_item_collection*                    _item_collection;

public:
	vc_project_filter_generator();
	virtual ~vc_project_filter_generator();

public:
	virtual cx::bool_t generate (void);

public:
	std::string get_file      (void);
	std::string get_file_path (void);

public:
	vc_project_filter_folder* add_folder (std::string name);
	vc_project_filter_folder* add_folder (std::string name, std::string extensions);
	void cleanup (void);

public:
	void build(void);

public:
	void TAG_xml         (void);
	void TAG_Project     (void);
	void TAG_Project_SUB (void);

	void TAG_ItemGroup_SUBTAG_Filter (void);
	void TAG_ItemGroup_SUBTAG_XXX    (std::string item_type);

};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


