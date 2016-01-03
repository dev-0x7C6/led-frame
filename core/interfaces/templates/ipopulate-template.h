#pragma once

namespace Interface {
	namespace Template {

		template <class Class>
		class IPopulateTemplate {
		public:
			explicit IPopulateTemplate() = default;
			virtual ~IPopulateTemplate() = default;

			virtual void populate(Class &object) = 0;
		};

	}
}
