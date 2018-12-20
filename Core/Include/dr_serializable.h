#pragma once
#include "dr_core_prerequisites.h"

namespace driderSDK {
  
  namespace SerializableTypeID {
    enum E : UInt32  {
      AABB = 0,
      Animator,
      Camera,
      Render,
      Script,
      Sound,
      NetworkManager,
      NetworkTransform,
      Light
    };
  }

  class File;

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
		virtual void 
		serialize(File &file) = 0;
		
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