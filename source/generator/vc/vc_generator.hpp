#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class vc_generator : public generator
{
public:
	vc_item_collection _item_collection;

	vc_solution_generator       _solution_generator      ;
	vc_project_generator        _project_generator       ;
	vc_project_filter_generator _project_filter_generator;
	file_copy_generator         _file_copy_generator     ;

	vc_template_data _template_data;

public:
	vc_generator();
	virtual ~vc_generator();

public:
	virtual void set_parameter (parameter* p);

public:
	virtual cx::bool_t generate (void);

	//-----------------------------------------------------------------------
public:
	void initialize (void);
	void cleanup    (void);

	cx::bool_t generate_file           (void);
	cx::bool_t generate_project_filter (void);
	cx::bool_t generate_project        (void);
	cx::bool_t generate_solution       (void);
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


