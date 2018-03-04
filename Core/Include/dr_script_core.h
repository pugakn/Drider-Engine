#pragma once
#include "dr_core_prerequisites.h"
#include "dr_resource.h"

namespace driderSDK {

	class DR_CORE_EXPORT ScriptCore : public Resource {
	public:
		/**
		* Default constructor
		*/
		ScriptCore() {}

		/**
		* Default destructor
		*/
		virtual ~ScriptCore() {}

		virtual void
		init(void* pData) override;

		virtual void
		init(void* pData, void* extraData) override;

		/**
		* Gets the loaded script
		*
		* @return
		*	  The script
		*/
		const TString&
		getScript();

		/**
		* Gets the script's name
		*
		* @return
		*	  The name of the script
		*/
		const TString&
		getName();

	private:
		TString m_name, m_script;

	};

}