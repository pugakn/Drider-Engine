#pragma once
#include "dr_engine_prerequisites.h"
#include "dr_codec.h"

namespace driderSDK {

	class DR_ENGINE_EXPORT CodecScript : public Codec {

	public:
		CodecScript() {}
		virtual ~CodecScript() {}

		UniqueVoidPtr
		decode(TString pathName) override;

		bool
		encode(TString pathName) override;

		bool
		isCompatible(TString resourceName) override;

		CompatibleType::E
		getType() override;

	};

}