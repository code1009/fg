#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_project_configuration
{
public:
	std::string _configuration;
	std::string _platform;

	cx::bool_t _debug;

public:
	vc_project_configuration();
	vc_project_configuration(std::string c, std::string p, cx::bool_t d);

public:
	std::string get_name(void);
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_project_generator : public generator
{
public:
	std::ostringstream _oss;

public:
	std::vector<vc_project_configuration*> _configuration_container;
	std::string                            _guid;
	vc_item_collection*                    _item_collection;

public:
	vc_template_data* _template_data;

public:
	vc_project_generator();
	virtual ~vc_project_generator();

public:
	virtual cx::bool_t generate (void);

public:
	std::string get_file      (void);
	std::string get_file_path (void);

public:
	vc_project_configuration* add_configuration (std::string c, std::string p, cx::bool_t d);
	void cleanup (void);

public:
	void build(void);

public:
	void TAG_xml (void);

	void TAG_Project     (void);
	void TAG_Project_SUB (void);

	void TAG_ItemGroup_ATTR_Label_ProjectConfigurations                             (void);
	void TAG_ItemGroup_ATTR_Label_ProjectConfigurations_SUB                         (void);
	void TAG_ItemGroup_ATTR_Label_ProjectConfigurations_SUBTAG_ProjectConfiguration (vc_project_configuration* e);

	void TAG_PropertyGroup_ATTR_Label_Globals (void);

	void TAG_Import_ATTR_Project_VCTargetsPath_Microsoft_Cpp_Default_props (void);

	void TAG_PropertyGroup_ATTR_Label_Configuration_GROUP  (void);
	void TAG_PropertyGroup_ATTR_Label_Configuration        (vc_project_configuration* e);
	void TAG_PropertyGroup_ATTR_Label_Configuration_SUB    (vc_project_configuration* e);
	void TAG_PropertyGroup_ATTR_Label_Configuration_SUBTAG (std::string name, std::string value);

	void TAG_Import_ATTR_Project_VCTargetsPath_Microsoft_Cpp_props (void);

	void TAG_ImportGroup_ATTR_Label_ExtensionSettings (void);

	void TAG_ImportGroup_ATTR_Label_PropertySheets_GROUP (void);
	void TAG_ImportGroup_ATTR_Label_PropertySheets       (vc_project_configuration* e);

	void TAG_PropertyGroup_ATTR_Label_UserMacros (void);

	void TAG_PropertyGroup_GROUP  (void);
	void TAG_PropertyGroup        (vc_project_configuration* e);
	void TAG_PropertyGroup_SUB    (vc_project_configuration* e);
	void TAG_PropertyGroup_SUBTAG (std::string name, std::string value);

	void TAG_ItemDefinitionGroup_GROUP                  (void);
	void TAG_ItemDefinitionGroup                        (vc_project_configuration* e);
	void TAG_ItemDefinitionGroup_SUB                    (vc_project_configuration* e);
	void TAG_ItemDefinitionGroup_SUBTAG                 (vc_project_configuration* e, std::string group);
	void TAG_ItemDefinitionGroup_SUBTAG_XXX_SUBTAG      (std::string name, std::string value);
	void TAG_ItemDefinitionGroup_SUBTAG_ClCompile       (vc_project_configuration* e);
	void TAG_ItemDefinitionGroup_SUBTAG_Link            (vc_project_configuration* e);
	void TAG_ItemDefinitionGroup_SUBTAG_ResourceCompile (vc_project_configuration* e);
	void TAG_ItemDefinitionGroup_SUBTAG_Midl            (vc_project_configuration* e);

	void TAG_Import_ATTR_Project_VCTargetsPath_Microsoft_Cpp_targets(void);

	void TAG_ImportGroup_ATTR_Label_ExtensionTargets(void);

	void TAG_ItemGroup_GROUP                    (void);
	void TAG_ItemGroup                          (std::string item_type);
	void TAG_ItemGroup_SUBTAG                   (vc_item* item);
	void TAG_ItemGroup_SUBTAG_ClCompile_PCH     (vc_item* item, std::string value);
	void TAG_ItemGroup_SUBTAG_CustomBuild_Ribbon(vc_item* item);
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


