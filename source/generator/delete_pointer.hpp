#pragma once



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename POINTER_TYPE>
static void delete_pointer (POINTER_TYPE p)
{
	delete p;
}

template <typename POINTER_TYPE>
static void delete_array_pointer (POINTER_TYPE p)
{
	delete []p;
}

template <typename FIRST, typename SECOND_POINTER_TYPE>
static void delete_pair_second_pointer(std::pair<typename FIRST, typename SECOND_POINTER_TYPE> p)
{
	delete p.second;
}

template <typename TYPE>
static void delete_pointer_in_vector(std::vector<typename TYPE>& c)
{
	std::for_each(c.begin(), c.end(), delete_pointer < typename TYPE >);
}

template <typename KEY, typename POINTER_TYPE>
static void delete_map_second_pointer_in_map (std::map<typename KEY, typename POINTER_TYPE>& c)
{
	std::for_each(c.begin(), c.end(), delete_pair_second_pointer < typename KEY, typename POINTER_TYPE >);
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


