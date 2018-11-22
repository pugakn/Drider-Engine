#pragma once
#include "dr_core_prerequisites.h"

namespace driderSDK {

	/**
	*  Class containing serializable properties.
	*
	*/
	class DR_CORE_EXPORT Serializable 
	{
	public:

		/**
		* Serialize the object
		*
		* @return
		*  TString containing the serialization.
		*/
		virtual TString 
		serialize() = 0;
		
		/**
		* De-serialize the object using the given data
		*
		* @param data
		*  TString containing the object's data.
		*/
		virtual void 
		deserialize(TString& data) = 0;

	};

}