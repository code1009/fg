/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../header.hpp"
#include "../../tdp/tdp.h"
#include <cx3/library/windows/SimpleFile.hpp>
#include <cx3/library/file/header.hpp>



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
}
parsing_t;



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void data_vc_item_collection_element_handler (tdp_array_t* ctx)
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


	s.assign((const char*)element_value->begin, element_value->length);


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
	tdp_array_set_handler_element (ctx, data_vc_item_collection_element_handler);
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

static void data_vc_item_collection_handler (tdp_array_t* ctx)
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
	tdp_array_set_handler_element (ctx, data_vc_item_collection_handler);
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
static void data_vc_handler (tdp_ini_t* ctx)
{
	//-----------------------------------------------------------------------
	parsing_t* parsing;


	parsing = (parsing_t*)tdp_ini_parameter(ctx);


	//-----------------------------------------------------------------------
	if (TDP_TRUE==tdp_ini_is_section(ctx, "item"))
	{
		data_vc_item(parsing);
	}
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
		if (TDP_TRUE==tdp_string_compare(value, "vc", TDP_TRUE))
		{
			tdp_ini_set_handler_element (parsing->ini, data_vc_handler);
		}
		else
		{
			tdp_ini_set_error(parsing->ini, value->begin);
		}
	}
}

static void data_handler (tdp_ini_t* ctx)
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


	//-----------------------------------------------------------------------
	tdp_ini_t  ini;
	tdp_ini_t* ctx;


	ctx = &ini;

	tdp_ini_initialize          (ctx);
	tdp_ini_set_parameter       (ctx, &parsing);
	tdp_ini_set_handler_element (ctx, data_handler);
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
vc_template_data::vc_template_data()
{
	_generator = CX_NULL_POINTER;
}

vc_template_data::~vc_template_data()
{
}


//===========================================================================
void vc_template_data::cleanup (void)
{
}

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


	return result;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


