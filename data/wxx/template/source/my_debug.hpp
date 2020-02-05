#ifndef INCLUDED__MY_DEBUG__HPP
#define INCLUDED__MY_DEBUG__HPP

/////////////////////////////////////////////////////////////////////////////
//
// File: my_debug.hpp
//
// Created by code1009.
// Created on Nov-14th, 2013.
//
/////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define MYDEBUG_TW_NORMAL      my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_NORMAL     )
#define MYDEBUG_TW_INFORMATION my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_INFORMATION)
#define MYDEBUG_TW_WARNING     my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_WARNING    )
#define MYDEBUG_TW_ERROR       my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_ERROR      )
#define MYDEBUG_TW_CRITICAL    my_debug_get_trace_writer(cx::log::ATTRIBUTE_SEVERITY_CRITICAL   )



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cx::debug::trace_writer* my_debug_get_trace_writer (cx::uint_t severity);

cx::bool_t my_debug_initialize   (void);
void       my_debug_uninitialize (void);





#endif




