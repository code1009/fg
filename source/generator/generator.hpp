#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class generator
{
private:
	parameter* _parameter;

public:
	generator();
	virtual ~generator();

public:
	virtual parameter* get_parameter (void);
	virtual void       set_parameter (parameter* p);

public:
	virtual cx::bool_t generate (void);
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cx::bool_t generate(std::string type);



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


