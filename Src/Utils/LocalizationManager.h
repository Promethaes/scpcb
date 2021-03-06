#ifndef LOCALIZATIONMANAGER_H_INCLUDED
#define LOCALIZATIONMANAGER_H_INCLUDED

#include <Misc/String.h>
#include <map>

class LocalizationManager {
    private:
        class Language {
            private:
                PGE::String name;
                std::map<PGE::String, PGE::String> map;

            public:
                PGE::String code;
                
                Language(const PGE::String& langCode);

                PGE::String getText(const PGE::String& code) const;
        };
        
        Language* currentLanguage;

    public:
        LocalizationManager(const PGE::String& langCode);
        ~LocalizationManager();
        
        void setLocalization(const PGE::String& langCode);
        PGE::String getLocalTxt(const PGE::String& key) const;
};

#endif // LOCALIZATIONMANAGER_H_INCLUDED
