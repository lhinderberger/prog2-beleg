#ifndef PROG2_BELEG_GUI_TRANSLATIONS_H
#define PROG2_BELEG_GUI_TRANSLATIONS_H

#include <map>
#include <string>
#include <wx/wx.h>

namespace pb2 {
    /* This module provides functions to retrieve numerous common
     * translations of domain terms.
     */
    wxString mediaTypeTranslation(const std::string & type);
    wxString mediaFormatTranslation(const std::string & format);
    wxString translateMapAlgorithm(const std::map<std::string, wxString> translateMap, const std::string & source);
}

#endif
