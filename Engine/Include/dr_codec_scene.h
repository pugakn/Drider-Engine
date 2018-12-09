#pragma once
#include "dr_engine_prerequisites.h"
#include "dr_codec.h"

namespace driderSDK {

	class DR_ENGINE_EXPORT CodecScene : public Codec {

	public:

		CodecScene();

		virtual ~CodecScene();

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
