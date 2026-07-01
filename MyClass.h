
// MyClass.h
#ifndef MYCLASS_H
#define MYCLASS_H

#include <wx/string.h>  // For wxString

class MyClass {
public:
    MyClass(const wxString& path) : m_filePath(path) {}
    wxString GetPath() const;

private:
    wxString m_filePath;
};

#endif // MYCLASS_H
