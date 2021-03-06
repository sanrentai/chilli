#pragma once
#include "..\Extension\ExtensionImp.h"
#include <log4cplus\logger.h>


namespace chilli{
namespace FreeSwitch {

	class FreeSwitchModule;
	class FreeSwitchExtension :public Extension::ExtensionImp {
	public:
		FreeSwitchExtension(FreeSwitchModule * model, const std::string &ext, const std::string &smFileName);
		virtual ~FreeSwitchExtension();

		//inherit from SendInterface
		virtual void fireSend(const std::string &strContent, const void * param) override;

	protected:
		void processSend(const std::string &strContent, const void * param, bool & bHandled);
	private:
		FreeSwitchModule * m_model;

	};
	typedef std::shared_ptr<FreeSwitchExtension>  FreeSwitchExtensionPtr;
}
}