#pragma once
#include "AvayaExtension.h"
#include <log4cplus\logger.h>


namespace chilli{
	namespace Avaya {

		class TSAPIModule;
		class AvayaACD :public AvayaExtension {
		public:
			AvayaACD(TSAPIModule * model, const std::string &ext, const std::string &smFileName);
			virtual ~AvayaACD();
		private:
			TSAPIModule * m_model = nullptr;
		};
		typedef std::shared_ptr<AvayaACD>  AvayaACDPtr;
	}
}