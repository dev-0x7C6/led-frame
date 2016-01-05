#pragma once

namespace Interface {
	namespace Template {

		template <class Class>
		class IAttachTemplateConst {
		public:
			explicit IAttachTemplateConst() = default;
			virtual ~IAttachTemplateConst() = default;

			virtual void attach(const Class &pair) = 0;
			virtual void detach(const Class &pair) = 0;
		};


		template <class Class>
		class IAttachTemplate {
		public:
			explicit IAttachTemplate() = default;
			virtual ~IAttachTemplate() = default;

			virtual void attach(Class &pair) = 0;
			virtual void detach(Class &pair) = 0;
		};

	}
}
