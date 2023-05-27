//
// Created by Matteo Illing on 20.04.23.
//

#ifdef linux
#include <linux/limits.h>
#else
#include <sys/syslimits.h>
#endif

#ifndef PSE_WEBSERVER_VALIDATION_H
#define PSE_WEBSERVER_VALIDATION_H
#include <unistd.h>
#include "strlib.h"
#include "config.h"
#include "filelib.h"

int isFileExistent(char* filepath);
int isFileInsideDocroot(char* filepath, string* hostname);
int isFileAccessible(char* filepath);
int isMethodValid(string* method);
int isProtocolValid(string* protocol);
int isAuthenticationRequired(string* hostname);
int isPasswordUsernameRight(http_request * request);
#endif //PSE_WEBSERVER_VALIDATION_H
