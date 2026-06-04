#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP

#define FOR_LANG_LIST(DO) \
	DO(File,    "File",    "Arquivo") \
	DO(New,     "New",     "Novo") \
	DO(Open,    "Open",    "Abrir") \
	DO(Save,    "Save",    "Salvar") \
	DO(SaveAs,  "Save as", "Salvar como") \
	DO(Quit,    "Quit",    "Sair") \
	DO(Toolbar, "Toolbar", "Ferramentas")

namespace i18n {
	enum Language {
		LANG_EN_US = 0,
		LANG_PT_BR,
		NUM_LANGUAGES
	};
	
	#define EXPAND_AS_DECLARATIONS(id, ...) extern const char * id;
	FOR_LANG_LIST(EXPAND_AS_DECLARATIONS)
	#undef EXPAND_AS_DECLARATIONS

	void setLanguage(Language language);
};
#endif
