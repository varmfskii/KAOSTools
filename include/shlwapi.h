#ifndef SHLWAPI_H
#define SHLWAPI_H
#include <sys/stat.h>
bool PathRelativePathToA(char *pszPath,
			 const char *psxFrom, const mode_t dwAttrFrom,
			 const char *pszTo, const mode_t dwAttrTo);
void _splitpath(const char *path,
		char *drive, char *dir, char *fname, char *ext);
bool PathIsRelativeA(const char *pszPath);
char *_fullpath(char *absPATH, const char *relPath, size_t maxLength);
bool CreateDirectoryA(const char *lpPathName, void *lpSecurityAttributes);
#endif
