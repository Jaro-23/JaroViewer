#pragma once

#include "Asset.h"
#include "Component3D.h"

namespace JaroViewer {
	class BoxComponent : public Component3D {
		public:
			BoxComponent(const AssetParameter &shader);
			void load() override;
		private:
	};
}
