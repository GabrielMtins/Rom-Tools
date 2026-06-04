#include "Localization.hpp"

#include <cstddef>

namespace i18n {
	constexpr const char * const string_data[][NUM_LANGUAGES] = {
		#define EXPAND_AS_STRINGS(id, ...) {__VA_ARGS__},
		FOR_LANG_LIST(EXPAND_AS_STRINGS)
		#undef EXPAND_AS_STRINGS
	};

	#define EXPAND_AS_DEFINITIONS(id, en_us, ...) const char * id = NULL;
	FOR_LANG_LIST(EXPAND_AS_DEFINITIONS)
	#undef EXPAND_AS_DEFINITIONS

	void setLanguage(Language language) {
		int count = 0;

		#define EXPAND_AS_UPDATE(id, ...) id = string_data[count++][language];
		FOR_LANG_LIST(EXPAND_AS_UPDATE)
		#undef EXPAND_AS_UPDATE
	}
}
