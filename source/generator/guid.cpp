/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "header.hpp"

#include <rpc.h>
#include <midles.h>
#include <Rpcdce.h>

//#include <midles.h>
//#include <rpcasync.h>
//#include <rpcdcep.h>
//#include <rpcnsi.h>
//#include <rpcproxy.h>
//#include <rpcssl.h>




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment(lib, "rpcrt4.lib")



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace fg
{


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::string make_guid_string (void)
{
	std::string uuid_string;
	std::string guid_string;


	UUID       uuid;
	RPC_CSTR   uuid_cstr;
	RPC_STATUS status;


	UuidCreate(&uuid);

	uuid_cstr = 0;
	status = UuidToStringA(&uuid, &uuid_cstr);
	if (RPC_S_OK == status)
	{
		uuid_string = reinterpret_cast<const char*>( uuid_cstr );

		RpcStringFreeA(&uuid_cstr);
	}

	guid_string = "{";
	guid_string+= uuid_string;
	guid_string+= "}";
	
	cx::to_upper(guid_string);

	return guid_string;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


