/////////////////////////////////////////////////////////////////////////////
//
// File: my_debug.cpp
//
// Created by code1009.
// Created on Nov-14th, 2013.
//
/////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
//
// Headers
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

//===========================================================================
#include <cx/library/header.hpp>
#include <cx/library/datetime/header.hpp>
#include <cx/library/platform/header.hpp>
#include <cx/library/filesystem/header.hpp>
#include <cx/library/filesystem/file_path.hpp>
#include <cx/library/log/header.hpp>

#include "my_debug.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//#define MY_DEBUG_LOG_FILE_USE
//#define MY_DEBUG_LOG_FILE_PER_HOUR
#define MY_DEBUG_LOG_FILE_EXPIRATION_TIME_SECOND 1u
#define MY_DEBUG_LOG_FILE_EXPIRATION_TIME_MINUTE 60u
#define MY_DEBUG_LOG_FILE_EXPIRATION_TIME_HOUR   3600u
#define MY_DEBUG_LOG_FILE_EXPIRATION_TIME_DAY    86400u
#define MY_DEBUG_LOG_FILE_EXPIRATION_TIME        MY_DEBUG_LOG_FILE_EXPIRATION_TIME_SECOND

//===========================================================================
#define MY_DEBUG_LOG_ASYNC

//===========================================================================
#define MY_DEBUG_DEFAULT_USE





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class my_debug_recorder : public cx::log::text_file_recorder
{
private:
	std::string _base_directory;
	cx::uint_t  _severity_level;

public:
	my_debug_recorder();
	virtual ~my_debug_recorder();

public:
	virtual cx::bool_t create (void);
	virtual void       destroy(void);

public:
	virtual std::string get_base_directory (void);
	virtual std::string get_sub_directory  (const cx::log::record_t& r);
	virtual std::string get_file_name      (const cx::log::record_t& r);

	virtual cx::time_t  get_expiration (void);

	virtual std::string get_file_find_filter  (void);
	virtual cx::time_t  file_path_to_datetime (std::string& file_path);

public:
	virtual cx::bool_t filter (const cx::log::record_t& r);
	virtual cx::bool_t render (const cx::log::record_t& r, std::string& s);
	virtual cx::bool_t write  (const cx::log::record_t& r, std::string& s);

public:
	void set_base_directory (std::string base_directory);
	void set_severity_level (cx::uint_t level);
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
my_debug_recorder::my_debug_recorder()
{
	_severity_level = cx::log::ATTRIBUTE_SEVERITY_NORMAL;
}

my_debug_recorder::~my_debug_recorder()
{
}

//===========================================================================
cx::bool_t my_debug_recorder::create (void)
{
	std::string base_directory;


	base_directory = cx::get_directory_of_current_process();
	base_directory+= "log\\";

	set_base_directory (base_directory);
	
	
//	set_severity_level (cx::log::ATTRIBUTE_SEVERITY_ERROR);


	return true;
}

void my_debug_recorder::destroy (void)
{
}

//===========================================================================
std::string my_debug_recorder::get_base_directory (void)
{
	return _base_directory;
}

std::string my_debug_recorder::get_sub_directory (const cx::log::record_t& r)
{
#if defined(MY_DEBUG_LOG_FILE_PER_HOUR)
	//-----------------------------------------------------------------------
	cx::int_t year       ;
	cx::int_t month      ;
	cx::int_t day        ;
	cx::int_t day_of_week;
	cx::int_t hour  ;
	cx::int_t minute;
	cx::int_t second;


	cx::unix_time_to_datetime(r.attribute.datetime_second, year, month, day, day_of_week, hour, minute, second);


	//-----------------------------------------------------------------------
	cx::char_t directory[MAX_PATH];


	minute = 0;
	second = 0;
	sprintf(directory, "%04d_%02d_%02d\\",
		year, month, day, hour, minute, second);


	//-----------------------------------------------------------------------
	std::string sub_directory;


	sub_directory = directory;
	sub_directory+= "\\";

	return sub_directory;
#else
	std::string sub_directory;


	return sub_directory;
#endif
}

std::string my_debug_recorder::get_file_name (const cx::log::record_t& r)
{
	//-----------------------------------------------------------------------
	cx::int_t year       ;
	cx::int_t month      ;
	cx::int_t day        ;
	cx::int_t day_of_week;
	cx::int_t hour  ;
	cx::int_t minute;
	cx::int_t second;


	cx::unix_time_to_datetime(r.attribute.datetime_second, year, month, day, day_of_week, hour, minute, second);


	//-----------------------------------------------------------------------
	cx::char_t file[MAX_PATH];

#if defined(MY_DEBUG_LOG_FILE_PER_HOUR)
//	hour   = 0;
	minute = 0;
	second = 0;
	sprintf(file, "%04d_%02d_%02d_%02d_%02d_%02d.log",
		year, month, day, hour, minute, second);
#else
	sprintf(file, "%04d-%02d-%02d.log",
		year, month, day);
#endif

	//-----------------------------------------------------------------------
	std::string file_name;


	file_name = file;

	return file_name;
}

cx::time_t my_debug_recorder::get_expiration (void)
{
	const cx::time_t a_second = 1;
	const cx::time_t a_minute = a_second * 60;
	const cx::time_t a_hour   = a_minute * 60;
	const cx::time_t a_day    = a_hour   * 24;

	cx::time_t expiration = MY_DEBUG_LOG_FILE_EXPIRATION_TIME; // 7 * a_day;

	
	return expiration;
}

std::string my_debug_recorder::get_file_find_filter (void)
{
	/*
	하위폴더의 와일드카드 사용 금지(예:"debug_??_??_??\\??_??\\app_????_??_??_??_??_??.log")
	하위폴더 포함 금지
	베이스폴더에서 파일만 검색하도록 필터 설정
	파일명에 대한 와일드 키워드만 사용
	*/

	std::string file_find_filter;


	file_find_filter += get_base_directory();
#if defined(MY_DEBUG_LOG_FILE_PER_HOUR)
	file_find_filter += "????_??_??_??_??_??.log";
#else
	file_find_filter += "????-??-??.log";
#endif

	return file_find_filter;
}

cx::time_t my_debug_recorder::file_path_to_datetime (std::string& file_path)
{
	//-----------------------------------------------------------------------
	// file_path = { directory/ }{ file{ {file_name}{.file_extension} } }


	/*
	//-----------------------------------------------------------------------
	std::string base_directory;


	base_directory = get_base_directory();

	
	//-----------------------------------------------------------------------
	std::string sub_directory_file_path;


	if (!base_directory.empty())
	{
		sub_directory_file_path = file_path.substr(base_directory.size());
	}
	else
	{
		sub_directory_file_path = file_path;
	}


	//--------------------------------------------------------------------------
	std::string file;


	file = cx::get_file_of_file_path (sub_directory_file_path);
	*/


	//--------------------------------------------------------------------------
	std::string file;


	file = cx::get_file_of_file_path (file_path);

	
	//--------------------------------------------------------------------------
#if defined(CX_DEBUG)
#if defined(MY_DEBUG_LOG_FILE_PER_HOUR)
	cx::uint_t length = strlen("????_??_??_??_??_??.log");
#else
	cx::uint_t length = strlen("????-??-??.log");
#endif

	CX_DEBUG_ASSERT (file.size() == length);
#endif

	
	//--------------------------------------------------------------------------
	cx::int_t year   = 0;
	cx::int_t month  = 0;
	cx::int_t day    = 0;
	cx::int_t hour   = 0;
	cx::int_t minute = 0;
	cx::int_t second = 0;


#if defined(MY_DEBUG_LOG_FILE_PER_HOUR)
	sscanf (file.c_str(), "%04d_%02d_%02d_%02d_%02d_%02d.log",
		&year  ,
		&month ,
		&day   ,
		&hour  ,
		&minute,
		&second);
#else
	sscanf (file.c_str(), "%04d-%02d-%02d.log",
		&year  ,
		&month ,
		&day   );
#endif


	//--------------------------------------------------------------------------
	cx::time_t datetime_second;
	

	cx::datetime_to_unix_time(
		year  ,
		month ,
		day   ,
		hour  ,
		minute,
		second,
		datetime_second
		);

	return datetime_second;
}

//===========================================================================
cx::bool_t my_debug_recorder::filter (const cx::log::record_t& r)
{
	if (r.attribute.severity < _severity_level)
	{
		return false;
	}

	return true;
}

cx::bool_t my_debug_recorder::render (const cx::log::record_t& r, std::string& s)
{
	return cx::log::text_file_recorder::render(r, s);
}

cx::bool_t my_debug_recorder::write (const cx::log::record_t& r, std::string& s)
{
	cx::debug::print (s.c_str());

	return cx::log::text_file_recorder::write(r, s);
}

//===========================================================================
void my_debug_recorder::set_severity_level (cx::uint_t level)
{
	_severity_level = level;
}

void my_debug_recorder::set_base_directory (std::string base_directory)
{
	_base_directory = base_directory;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined MY_DEBUG_LOG_FILE_USE
static my_debug_recorder _my_debug_recorder;
#else
static cx::log::recorder _my_debug_recorder;
#endif



//===========================================================================
#if defined MY_DEBUG_LOG_ASYNC
static cx::log::mt_logger _my_debug_logger (&_my_debug_recorder);
#else
static cx::log::logger    _my_debug_logger (&_my_debug_recorder);
#endif



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static cx::log::debug_trace_writer _my_debug_trace_writer_normal      (&_my_debug_logger, cx::log::ATTRIBUTE_SEVERITY_NORMAL     , 0u);
static cx::log::debug_trace_writer _my_debug_trace_writer_information (&_my_debug_logger, cx::log::ATTRIBUTE_SEVERITY_INFORMATION, 0u);
static cx::log::debug_trace_writer _my_debug_trace_writer_warning     (&_my_debug_logger, cx::log::ATTRIBUTE_SEVERITY_WARNING    , 0u);
static cx::log::debug_trace_writer _my_debug_trace_writer_error       (&_my_debug_logger, cx::log::ATTRIBUTE_SEVERITY_ERROR      , 0u);
static cx::log::debug_trace_writer _my_debug_trace_writer_critical    (&_my_debug_logger, cx::log::ATTRIBUTE_SEVERITY_CRITICAL   , 0u);



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void my_deubug_set_lib_trace_writer_to_cx_null(void)
{
	cx::debug::set_lib_normal_trace_writer     (cx::debug::get_null_trace_writer());
	cx::debug::set_lib_information_trace_writer(cx::debug::get_null_trace_writer());
	cx::debug::set_lib_warning_trace_writer    (cx::debug::get_null_trace_writer());
	cx::debug::set_lib_error_trace_writer      (cx::debug::get_null_trace_writer());
	cx::debug::set_lib_critical_trace_writer   (cx::debug::get_null_trace_writer());
}

static void my_deubug_set_app_trace_writer_to_cx_null(void)
{
	cx::debug::set_app_normal_trace_writer     (cx::debug::get_null_trace_writer());
	cx::debug::set_app_information_trace_writer(cx::debug::get_null_trace_writer());
	cx::debug::set_app_warning_trace_writer    (cx::debug::get_null_trace_writer());
	cx::debug::set_app_error_trace_writer      (cx::debug::get_null_trace_writer());
	cx::debug::set_app_critical_trace_writer   (cx::debug::get_null_trace_writer());
}

static void my_deubug_set_app_trace_writer_to_cx_default(void)
{
	cx::debug::set_app_normal_trace_writer     (cx::debug::get_normal_rt_trace_writer     ());
	cx::debug::set_app_information_trace_writer(cx::debug::get_information_rt_trace_writer());
	cx::debug::set_app_warning_trace_writer    (cx::debug::get_warning_rt_trace_writer    ());
	cx::debug::set_app_error_trace_writer      (cx::debug::get_error_rt_trace_writer      ());
	cx::debug::set_app_critical_trace_writer   (cx::debug::get_critical_rt_trace_writer   ());
}

static void my_deubug_set_lib_trace_writer_to_cx_default(void)
{
	cx::debug::set_lib_normal_trace_writer     (cx::debug::get_normal_rt_trace_writer     ());
	cx::debug::set_lib_information_trace_writer(cx::debug::get_information_rt_trace_writer());
	cx::debug::set_lib_warning_trace_writer    (cx::debug::get_warning_rt_trace_writer    ());
	cx::debug::set_lib_error_trace_writer      (cx::debug::get_error_rt_trace_writer      ());
	cx::debug::set_lib_critical_trace_writer   (cx::debug::get_critical_rt_trace_writer   ());
}

static void my_deubug_set_lib_trace_writer(void)
{
	cx::debug::set_lib_normal_trace_writer     (my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_NORMAL     ));
	cx::debug::set_lib_information_trace_writer(my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_INFORMATION));
	cx::debug::set_lib_warning_trace_writer    (my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_WARNING    ));
	cx::debug::set_lib_error_trace_writer      (my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_ERROR      ));
	cx::debug::set_lib_critical_trace_writer   (my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_CRITICAL   ));
}

static void my_deubug_set_app_trace_writer(void)
{
	cx::debug::set_app_normal_trace_writer     (my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_NORMAL     ));
	cx::debug::set_app_information_trace_writer(my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_INFORMATION));
	cx::debug::set_app_warning_trace_writer    (my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_WARNING    ));
	cx::debug::set_app_error_trace_writer      (my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_ERROR      ));
	cx::debug::set_app_critical_trace_writer   (my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_CRITICAL   ));
}

//===========================================================================
cx::debug::trace_writer* my_debug_get_trace_writer(cx::uint_t severity)
{
	switch(severity)
	{
	case cx::log::ATTRIBUTE_SEVERITY_NORMAL     : return &_my_debug_trace_writer_normal      ; break;
	case cx::log::ATTRIBUTE_SEVERITY_INFORMATION: return &_my_debug_trace_writer_information ; break;
	case cx::log::ATTRIBUTE_SEVERITY_WARNING    : return &_my_debug_trace_writer_warning     ; break;
	case cx::log::ATTRIBUTE_SEVERITY_ERROR      : return &_my_debug_trace_writer_error       ; break;
	case cx::log::ATTRIBUTE_SEVERITY_CRITICAL   : return &_my_debug_trace_writer_critical    ; break;

	default:
		break;
	}

	return CX_NULL_POINTER;
}

cx::bool_t my_debug_initialize (void)
{
	//--------------------------------------------------------------------------
	if (false==_my_debug_recorder.create())
	{
		return false;
	}


	//--------------------------------------------------------------------------
	if (false==_my_debug_logger.create())
	{
		return false;
	}


	//--------------------------------------------------------------------------
#if defined MY_DEBUG_DEFAULT_USE
	my_deubug_set_lib_trace_writer_to_cx_null();
//	my_deubug_set_app_trace_writer_to_cx_null();
	my_deubug_set_app_trace_writer();
#endif


	return true;
}

void my_debug_uninitialize (void)
{
	//--------------------------------------------------------------------------
#if defined MY_DEBUG_DEFAULT_USE
	my_deubug_set_lib_trace_writer_to_cx_default();
	my_deubug_set_app_trace_writer_to_cx_default();
#endif


	//--------------------------------------------------------------------------
	_my_debug_logger  .destroy();
	_my_debug_recorder.destroy();
}


