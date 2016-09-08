#ifndef HELPER_FUNC_H
#define HELPER_FUNC_H

#include <string.h>
#include <sstream>
#include <pwd.h>
#include <dlfcn.h>

std::string getHomeDir(void) {

   std::string homeDir;
   struct passwd *pw = getpwuid(getuid());
   const char *homedir = pw->pw_dir;
   homeDir = homedir;
   return homeDir;

}

#endif
