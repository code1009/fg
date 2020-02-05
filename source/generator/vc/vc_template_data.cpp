/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../header.hpp"
#include "../../tdp/tdp.h"
#include <cx3/library/windows/SimpleFile.hpp>
#include <cx3/library/file/header.hpp>
#include <cx3/library/string/string_utility.hpp>



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef fg::vc_template_data data_t;

//===========================================================================
typedef struct _parsing_item_data_vc_item_collection_element_t
{
	std::string file  ;
	std::string filter;
}
parsing_item_data_vc_item_collection_element_t;

typedef struct _parsing_t
{
	data_t*        data ;
	tdp_ini_t*     ini  ;
	tdp_array_t*   array;

	parsing_item_data_vc_item_collection_element_t data_vc_item_collection_element;
	fg::vc_template_data_project_configuration*    data_vc_project_configuration_pointer;
}
parsing_t;



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static std::string tdp_string_to_string (tdp_string_t* p)
{
	std::string s;
	

	s.assign((const char*)p->begin, p->length);

	
	std::string findwhat    ="\\\\";
	std::string replacewith ="\\";

	s = cx::replace(s, findwhat, replacewith);

	return s;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void data_vc_item_collection_element_array_handler (tdp_array_t* ctx)
{
	//-----------------------------------------------------------------------
	tdp_uint_t    element_index;
	tdp_string_t* element_value;


	element_index = tdp_array_element_index(ctx);
	element_value = tdp_array_element_value(ctx);


	//-----------------------------------------------------------------------
	parsing_t* parsing;


	parsing = (parsing_t*)tdp_array_parameter(ctx);
	parsing->array = ctx;


	//-----------------------------------------------------------------------
	std::string s;


	s = tdp_string_to_string(element_value);


	//-----------------------------------------------------------------------
	switch (element_index)
	{
	case 0u:
		parsing->data_vc_item_collection_element.file = s;
		break;

	case 1u:
		parsing->data_vc_item_collection_element.filter = s;
		break;

	default:
		break;
	}
}

static void data_vc_item_collection_element (parsing_t* parsing, tdp_uint_t index, tdp_string_t* value)
{
	//-----------------------------------------------------------------------
	parsing->data_vc_item_collection_element.file  .clear();
	parsing->data_vc_item_collection_element.filter.clear();


	//-----------------------------------------------------------------------
	tdp_array_t  array;
	tdp_array_t* ctx;


	ctx = &array;

	tdp_array_initialize          (ctx);
	tdp_array_set_parameter       (ctx, parsing);
	tdp_array_set_handler_element (ctx, data_vc_item_collection_element_array_handler);
	tdp_array_parse               (ctx, value->begin, value->length);


	if (ctx->state != TDP_ARRAY_STATE_DONE)
	{
		printf("\r\n# ARRAY [ERROR LINE:%d COLUMN:%d] = %d \r\n\r\n", 
			ctx->error_line,
			ctx->error_column,
			ctx->state
			);

		tdp_ini_set_error(parsing->ini, value->begin);

		return;
	}


	//-----------------------------------------------------------------------
	parsing->data->_generator->_item_collection.add(
		parsing->data_vc_item_collection_element.file  , 
		parsing->data_vc_item_collection_element.filter
		);
}

static void data_vc_item_collection_array_handler (tdp_array_t* ctx)
{
	//-----------------------------------------------------------------------
	tdp_uint_t    element_index;
	tdp_string_t* element_value;


	element_index = tdp_array_element_index(ctx);
	element_value = tdp_array_element_value(ctx);


	//-----------------------------------------------------------------------
	parsing_t* parsing;


	parsing = (parsing_t*)tdp_array_parameter(ctx);
	parsing->array = ctx;


	//-----------------------------------------------------------------------
	tdp_string_trim_braces(element_value);


	data_vc_item_collection_element (parsing, element_index, element_value);
}

static void data_vc_item_collection (parsing_t* parsing, tdp_string_t* value)
{
	tdp_array_t  array;
	tdp_array_t* ctx;


	ctx = &array;

	tdp_array_initialize          (ctx);
	tdp_array_set_parameter       (ctx, parsing);
	tdp_array_set_handler_element (ctx, data_vc_item_collection_array_handler);
	tdp_array_parse               (ctx, value->begin, value->length);


	if (ctx->state != TDP_ARRAY_STATE_DONE)
	{
		printf("\r\n# ARRAY [ERROR LINE:%d COLUMN:%d] = %d \r\n\r\n", 
			ctx->error_line,
			ctx->error_column,
			ctx->state
			);

		tdp_ini_set_error(parsing->ini, value->begin);

		return;
	}
}

static void data_vc_item (parsing_t* parsing)
{
	//-----------------------------------------------------------------------
	tdp_string_t* value;


	value = tdp_ini_value (parsing->ini);


	//-----------------------------------------------------------------------
	if (TDP_TRUE==tdp_ini_is_variable (parsing->ini, "collection" ))
	{
		data_vc_item_collection(parsing, value);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void data_vc_configuration (parsing_t* parsing)
{
	//-----------------------------------------------------------------------
	tdp_string_t* value;


	value = tdp_ini_value (parsing->ini);



	//-----------------------------------------------------------------------
	fg::vc_template_data_project_configuration* p;
	cx::uint_t count;
	cx::uint_t index;


	p     = CX_NULL_POINTER;
	count = parsing->data->_project_configuration_container.size();
	index = 0u;
	if (0u<count)
	{
		index = count - 1u;
		p = parsing->data->_project_configuration_container.at(index);
	}


	//-----------------------------------------------------------------------
	if ( CX_NULL_POINTER==p )
	{
		return;
	}


	//-----------------------------------------------------------------------
	std::string s;


	s = tdp_string_to_string(value);


	//-----------------------------------------------------------------------
	if      (TDP_TRUE==tdp_ini_is_variable (parsing->ini, "name" ))
	{
		p->_name = s;
	}
	else if (TDP_TRUE==tdp_ini_is_variable (parsing->ini, "debug" ))
	{
		p->_debug = (s=="true") ? true : false;
	}
	else if (TDP_TRUE==tdp_ini_is_variable (parsing->ini, "configuration" ))
	{
		p->_configuration = s;
	}
	else if (TDP_TRUE==tdp_ini_is_variable (parsing->ini, "platform" ))
	{
		p->_platform = s;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void data_vc_element_handler (tdp_ini_t* ctx)
{
	//-----------------------------------------------------------------------
	parsing_t* parsing;


	parsing = (parsing_t*)tdp_ini_parameter(ctx);


	//-----------------------------------------------------------------------
	if      (TDP_TRUE==tdp_ini_is_section(ctx, "item"))
	{
		data_vc_item(parsing);
		return;
	}
	else if (TDP_TRUE==tdp_ini_is_section(ctx, "configuration"))
	{
		data_vc_configuration(parsing);
		return;
	}


	//-----------------------------------------------------------------------
	fg::vc_template_data_project_configuration* p;


	p = parsing->data_vc_project_configuration_pointer;
	if (CX_NULL_POINTER==p)
	{
		return;
	}


	//-----------------------------------------------------------------------
	tdp_string_t* section;
	tdp_string_t* variable;
	tdp_string_t* value;


	section  = tdp_ini_section  (parsing->ini);
	variable = tdp_ini_variable (parsing->ini);
	value    = tdp_ini_value    (parsing->ini);


	//-----------------------------------------------------------------------
	std::string svariable;
	std::string svalue;


	svariable = tdp_string_to_string(variable);
	svalue    = tdp_string_to_string(value);


	if      (TDP_TRUE==tdp_string_prefix_suffix_compare(section, "configuration:", "/property/configuration", TDP_FALSE))
	{
		p->add_entry("/property/configuration", svariable, svalue);
	}
	else if (TDP_TRUE==tdp_string_prefix_suffix_compare(section, "configuration:", "/property", TDP_FALSE))
	{
		p->add_entry("/property", svariable, svalue);
	}
	else if (TDP_TRUE==tdp_string_prefix_suffix_compare(section, "configuration:", "/compile", TDP_FALSE))
	{
		p->add_entry("/compile", svariable, svalue);
	}
	else if (TDP_TRUE==tdp_string_prefix_suffix_compare(section, "configuration:", "/link", TDP_FALSE))
	{
		p->add_entry("/link", svariable, svalue);
	}
}

//===========================================================================
static fg::vc_template_data_project_configuration* find_project_configuration(
	parsing_t*    parsing,
	tdp_char_t*   section_prefix,
	tdp_char_t*   section_suffix
	)
{
	//-----------------------------------------------------------------------
	fg::vc_template_data_project_configuration* e;


	e = CX_NULL_POINTER;


	//-----------------------------------------------------------------------
	tdp_string_t* section;


	section = tdp_ini_section (parsing->ini);
	if (TDP_FALSE==tdp_string_prefix_suffix_compare(section, section_prefix, section_suffix, TDP_FALSE))
	{
		return CX_NULL_POINTER;
	}


	//-----------------------------------------------------------------------
	tdp_string_t section_part;


	tdp_string_part_without_prefix_suffix(section, section_prefix, section_suffix, TDP_FALSE, &section_part);
	tdp_string_trim_dquotes(&section_part);
	

	//-----------------------------------------------------------------------
	std::string s;


	s = tdp_string_to_string(&section_part);


	//-----------------------------------------------------------------------
	std::vector<fg::vc_template_data_project_configuration*>::iterator i;


	for (i =parsing->data->_project_configuration_container.begin();
	     i!=parsing->data->_project_configuration_container.end();
	     i++)
	{
		e = *i;

		if (e->_name == s)
		{
			return e;
		}
	}


	return CX_NULL_POINTER;
}

static void data_vc_section_handler (tdp_ini_t* ctx)
{
	//-----------------------------------------------------------------------
	parsing_t* parsing;


	parsing = (parsing_t*)tdp_ini_parameter(ctx);


	//-----------------------------------------------------------------------
	if (TDP_TRUE==tdp_ini_is_section(ctx, "configuration"))
	{
		fg::vc_template_data_project_configuration* p;

		
		p = new fg::vc_template_data_project_configuration();

		parsing->data->_project_configuration_container.push_back(p);
	}


	//-----------------------------------------------------------------------
	fg::vc_template_data_project_configuration* e;
	fg::vc_template_data_project_configuration* p;

	
	p = CX_NULL_POINTER;
	e = find_project_configuration(parsing, "configuration:", "/property/configuration");if (CX_NULL_POINTER!=e) {p = e;}
	e = find_project_configuration(parsing, "configuration:", "/property"              );if (CX_NULL_POINTER!=e) {p = e;}
	e = find_project_configuration(parsing, "configuration:", "/compile"               );if (CX_NULL_POINTER!=e) {p = e;}
	e = find_project_configuration(parsing, "configuration:", "/link"                  );if (CX_NULL_POINTER!=e) {p = e;}

	parsing->data_vc_project_configuration_pointer = p;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void data_generater (parsing_t* parsing)
{
	//-----------------------------------------------------------------------
	tdp_string_t* value;


	value = tdp_ini_value (parsing->ini);


	//-----------------------------------------------------------------------
	if (TDP_TRUE==tdp_ini_is_variable(parsing->ini, "type"))
	{
		if (TDP_TRUE==tdp_string_compare(value, "vc", TDP_FALSE))
		{
			tdp_ini_set_handler_element (parsing->ini, data_vc_element_handler);
			tdp_ini_set_handler_section (parsing->ini, data_vc_section_handler);
		}
		else
		{
			tdp_ini_set_error(parsing->ini, value->begin);
		}
	}
}

static void data_element_handler (tdp_ini_t* ctx)
{
	//-----------------------------------------------------------------------
	parsing_t* parsing;


	parsing = (parsing_t*)tdp_ini_parameter(ctx);
	parsing->ini = ctx;


	//-----------------------------------------------------------------------
	if (TDP_TRUE==tdp_ini_is_section(ctx, "generator"))
	{
		data_generater(parsing);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static cx::bool_t load_data(data_t* data, tdp_char_t* spointer, tdp_uint_t slength)
{
	//-----------------------------------------------------------------------
	parsing_t parsing;

	
	parsing.data  = data;
	parsing.ini   = CX_NULL_POINTER;
	parsing.array = CX_NULL_POINTER;
	
	parsing.data_vc_project_configuration_pointer = CX_NULL_POINTER;


	//-----------------------------------------------------------------------
	tdp_ini_t  ini;
	tdp_ini_t* ctx;


	ctx = &ini;

	tdp_ini_initialize          (ctx);
	tdp_ini_set_parameter       (ctx, &parsing);
	tdp_ini_set_handler_element (ctx, data_element_handler);
	tdp_ini_parse               (ctx, spointer, slength);
 	if (ctx->state != TDP_INI_STATE_DONE)
	{
		printf("\r\n# INI [ERROR LINE:%d COLUMN:%d] = %d \r\n\r\n", 
			ctx->error_line,
			ctx->error_column,
			ctx->state
			);

		return false;
	}


	return true;
}






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_template_data_entry::vc_template_data_entry()
{
}

vc_template_data_entry::~vc_template_data_entry()
{
	cleanup();
}


//===========================================================================
void vc_template_data_entry::cleanup (void)
{
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_template_data_entry_collection::vc_template_data_entry_collection()
{
}

vc_template_data_entry_collection::~vc_template_data_entry_collection()
{
	cleanup();
}


//===========================================================================
void vc_template_data_entry_collection::cleanup (void)
{
	delete_pointer_in_vector<vc_template_data_entry*>(_contanier);
	_contanier.clear();
}

void vc_template_data_entry_collection::add(std::string variable, std::string value)
{
	vc_template_data_entry* e;

	
	e = new vc_template_data_entry();

	e->_variable = variable;
	e->_value    = value;

	_contanier.push_back(e);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_template_data_project_configuration::vc_template_data_project_configuration()
{
}

vc_template_data_project_configuration::~vc_template_data_project_configuration()
{
	cleanup();
}

//===========================================================================
void vc_template_data_project_configuration::cleanup (void)
{
	delete_map_second_pointer_in_map<std::string, vc_template_data_entry_collection*>(_entry_collection_container);
	_entry_collection_container.clear();
}

void vc_template_data_project_configuration::add_entry (std::string name, std::string variable, std::string value)
{
	vc_template_data_entry_collection* c;
	

	c = _entry_collection_container[name];
	if (CX_NULL_POINTER==c)
	{
		c = new vc_template_data_entry_collection();
		c->_name = name;

		_entry_collection_container[name] = c;
	}

	c->add(variable, value);
}

vc_template_data_entry_collection* vc_template_data_project_configuration::find_entry (std::string name)
{
	std::map<std::string, vc_template_data_entry_collection*>::iterator i;
	
	vc_template_data_entry_collection* e;


	i = _entry_collection_container.find(name);
	if (i==_entry_collection_container.end())
	{
		return CX_NULL_POINTER;
	}

	e = (*i).second;

	return e;
}


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
vc_template_data::vc_template_data()
{
	_generator = CX_NULL_POINTER;
}

vc_template_data::~vc_template_data()
{
	cleanup();
}


//===========================================================================
void vc_template_data::cleanup (void)
{
	delete_pointer_in_vector<vc_template_data_project_configuration*>(_project_configuration_container);
	_project_configuration_container.clear();
}

vc_template_data_project_configuration* vc_template_data::find_project_configuration (std::string name)
{
	std::vector<vc_template_data_project_configuration*>::iterator i;
	
	vc_template_data_project_configuration* e;


	for(i =_project_configuration_container.begin();
	    i!=_project_configuration_container.end();
	    i++)
	{
		e = *i;

		if (e->_name == name)
		{
			return e;
		}
	}


	return CX_NULL_POINTER;
}

//===========================================================================
cx::bool_t vc_template_data::load (std::string file_path)
{
	//-----------------------------------------------------------------------
	cx::bool_t result;

	
	result = true;


	//-----------------------------------------------------------------------
	LPBYTE buffer_pointer = CX_NULL_POINTER;
	DWORD  buffer_size    = 0u;


	if (FALSE==cx::LoadFileToBuffer(file_path.c_str(), buffer_pointer, buffer_size))
	{
		if (buffer_pointer)
		{
			cx::ReleaseFileBuffer(buffer_pointer);
		}

		return false;
	}


	//-----------------------------------------------------------------------
	cx::text_file_bom_t tfb;


	cx::get_text_file_bom(buffer_pointer, buffer_size, tfb);
	if (cx::TEXT_FILE_BOM_UTF8!=tfb)
	{
		if (buffer_pointer)
		{
			cx::ReleaseFileBuffer(buffer_pointer);
		}

		return false;
	}


	//-----------------------------------------------------------------------
	result = load_data (this, reinterpret_cast<tdp_char_t*>(buffer_pointer+3), buffer_size-3);


	//-----------------------------------------------------------------------
	cx::ReleaseFileBuffer(buffer_pointer);


	return result;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


