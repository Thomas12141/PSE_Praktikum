//
// Created by Matteo Illing on 08.05.23.
//

#ifndef PSE_WEBSERVER_FILELIB_H
#define PSE_WEBSERVER_FILELIB_H

#include "strlib.h"
#include "httplib.h"

string* readFile(char* filepath);
char* getFilePath(http_request* request);
string* getFiletype (char* resource_path, int len);
string* getDocrootpath(string* hostname);

#endif //PSE_WEBSERVER_FILELIB_H
