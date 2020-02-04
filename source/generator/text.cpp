/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "header.hpp"
#include <Shlobj.h>



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::string braces (std::string v)
{
	std::string s;

	s = "{'";
	s+= v;
	s+= "}'";

	return s;
}

std::string brackets (std::string v) // square brackets
{
	std::string s;

	s = "['";
	s+= v;
	s+= "]'";

	return s;
}

std::string parentheses (std::string v) // round brackets
{
	std::string s;

	s = "(";
	s+= v;
	s+= ")'";

	return s;
}

std::string squot(std::string v)  // add single quotation_marks
{
	std::string s;

	s = "\'";
	s+= v;
	s+= "\'";

	return s;
}

std::string dquot(std::string v) // add double quotation_marks
{
	std::string s;

	s = "\"";
	s+= v;
	s+= "\"";

	return s;
}

std::string eline(void) 
{
	std::string end_line = "\r\n";


	return end_line;
}

std::string itab (cx::uint_t n) // indent tab
{
	std::string s;
	cx::uint_t  i;


	for(i=0u; i<n; i++)
	{
		s+="\t";
	}

	return s;
}

std::string ispace (cx::uint_t n)
{
	std::string s;
	cx::uint_t  i;


	for(i=0u; i<n; i++)
	{
		s+=" ";
	}

	return s;
}

std::string ispace2 (cx::uint_t n)
{
	std::string s;
	cx::uint_t  i;


	for(i=0u; i<n; i++)
	{
		s+="  ";
	}

	return s;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cx::bool_t utf8_text_file_save (std::string save_file_path, std::ostringstream& oss)
{
#if 0
	OutputDebugStringA("\r\n");
	OutputDebugStringA("//===========================================================================");
	OutputDebugStringA("\r\nFILE: ");
	OutputDebugStringA(save_file_path.c_str());
	OutputDebugStringA("\r\n");
	OutputDebugStringA("//===========================================================================");
	OutputDebugStringA("\r\n");
	OutputDebugStringA(oss.str().c_str());
	OutputDebugStringA("\r\n");
	OutputDebugStringA("//===========================================================================");
	OutputDebugStringA("\r\n");
#endif


	//-----------------------------------------------------------------------
	const cx::byte_t text_file_bom_utf8 [3] = {0xEFu, 0xBBu, 0xBFu};

	std::string text;
	std::string text_utf8;


	text      = oss.str();
	text_utf8 = cx::mbcs_to_utf8(text);


	//-----------------------------------------------------------------------
	std::vector<cx::byte_t> stream;

	
	stream.reserve(
		text_utf8.size()
		+ sizeof(text_file_bom_utf8)
		);

	stream.insert(stream.end(), text_file_bom_utf8, text_file_bom_utf8+sizeof(text_file_bom_utf8));
	stream.insert(stream.end(), text_utf8.begin(), text_utf8.end());


	//-----------------------------------------------------------------------
	cx::byte_t* pointer;
	cx::size_t  size;


	size    = stream.size();
	pointer = &*stream.begin();


	CX_DEBUG_TRACEF(CX_TWA_NORMAL, "utf8_text file size = %d", size);


	//-----------------------------------------------------------------------
	std::string file_path;
	std::string directory;
	std::string splash;
	std::string backsplash;
	

	splash    = "/";
	backsplash= "\\";
	file_path = cx::replace(save_file_path, splash, backsplash);
	directory = cx::get_directory_of_file_path (file_path);


	if (SHPathPrepareForWriteA(NULL, NULL, directory.c_str(), SHPPFW_DEFAULT)!=S_OK)
	{
        return false;
	}


	//-----------------------------------------------------------------------
	FILE* fp;


	fp = fopen(file_path.c_str(), "wb");
	if (fp == CX_NULL_POINTER)
	{
		cx::debug::print_format ("fali to fopen() : %s \r\n", file_path.c_str());
		return false;
	}

	if (size>0)
	{
		fwrite(pointer, size, 1, fp);
	}
	
	fflush(fp);
	fclose(fp);

	return true;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


