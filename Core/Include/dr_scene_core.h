#pragma once
#include "dr_core_prerequisites.h"
#include "dr_resource.h"

namespace driderSDK {

class File;

	class DR_CORE_EXPORT SceneCore : public Resource {

	public:
		/**
		* Default constructor
		*/
		SceneCore();

		/**
		* Default destructor
		*/
		~SceneCore();

		virtual void
		init(void* pData) override;

		virtual void
		init(void* pData, void* extraData) override;

	private:
    TString m_name;
		File *m_dataFile;

	};
}
