#pragma once
#ifndef _FSM_SENDINTERFACE_HEADER_
#define _FSM_SENDINTERFACE_HEADER_
#include "../config.h"
#include <string>

namespace fsm
{

	class FSM_EXPORT SendInterface
	{
	public:
		SendInterface(const std::string &target):_target(target){}

		// Receive messages module name
		const std::string _target;

		//get module name
		const std::string & getTarget() const { return _target ;}
		
	public:
		/// <summary>
		/// Cancel the specified send message.This feature is not yet implemented
		/// </summary>
		/// <param name="sendId"> The ID of the send message to cancel </param>
		virtual void cancel(const std::string &sendId){};

		/// <summary>
		/// Send this message to the target.
		/// </summary>
	
		virtual void fireSend(const std::string &strContent,const void * param) = 0;

	};
}
#endif //end eventdispather header