#include "gui/translations.h"

using namespace std;

wxString pb2::translateMapAlgorithm(
        const map<string, wxString> translateMap, const string & source) {
    /* Try to find translation, otherwise return source string */
    wxString result = source;
    auto it = translateMap.find(source);
    if (it != translateMap.end())
        result = it->second;
    return result;
}

wxString pb2::mediaTypeTranslation(const std::string & type) {
    static auto translateMap = map<string,wxString>();

    /* Build translation map, if necessary */
    if (translateMap.empty()) {
        translateMap = {
                {"book",     _("Buch")},
                {"audio",    _("Audio")},
                {"software", _("Software")},
                {"video",    _("Video")}
        };
    }

    /* Pass on control */
    return translateMapAlgorithm(translateMap, type);
}

wxString pb2::mediaFormatTranslation(const std::string & format) {
    static auto translateMap = map<string,wxString>();

    /* Build translation map, if necessary */
    if (translateMap.empty()) {
        translateMap = {
                {"hardcover",     _("Hardcover")},
                {"softcover",     _("Softcover")},
                {"vinyl",         _("Schallplatte")},
                {"cassette",      _("Kassette")},
                {"floppy",        _("Diskette")},
                {"vhs",           _("VHS")},
                {"cd",            _("CD")},
                {"dvd",           _("DVD")},
                {"memory-card",   _("Speicherkarte")},
                {"usb-stick",     _("USB-Stick")},
                {"download",      _("Download")},
                {"online-access", _("Online-Medium")},
                {"other",         _("Andere")},
        };
    }

    /* Pass on control */
    return translateMapAlgorithm(translateMap, format);
}