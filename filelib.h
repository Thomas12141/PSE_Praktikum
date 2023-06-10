//
// Created by Matteo Illing on 08.05.23.
//

#ifndef PSE_WEBSERVER_FILELIB_H
#define PSE_WEBSERVER_FILELIB_H

#include "strlib.h"
#include "httplib.h"
#include <sys/stat.h>

string* readFile(char* filepath);
char* getFilePath(http_request* request);
string* getFiletype (char* resource_path, int len);
string* getDocrootpath(string* hostname);
int ifFileTooBig(char * path);

#endif //PSE_WEBSERVER_FILELIB_H
