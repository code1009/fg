#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_item
{
public:
	std::string _file_path;
	std::string _filter;
	std::string _type;
	std::string _option;

public:
	vc_item();
	virtual ~vc_item();
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_item_collection
{
public:
	std::vector<vc_item*> _container;

public:
	vc_item_collection();
	virtual ~vc_item_collection();

public:
	void get_project_filter  (std::vector<std::string>& project_filter);
	void get_type_collection (std::string type, vc_item_collection& collection);

public:
	void cleanup (void);
	void add        (std::string file_path, std::string filter=std::string());
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


