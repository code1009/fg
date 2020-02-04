#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::string parentheses (std::string v);
std::string brackets (std::string v);
std::string braces (std::string v);

std::string dquot(std::string v);
std::string squot(std::string v);

std::string eline(void);

std::string itab (cx::uint_t n);
std::string ispace (cx::uint_t n);
std::string ispace2 (cx::uint_t n);



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cx::bool_t utf8_text_file_save (std::string file_path, std::ostringstream& oss);



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


