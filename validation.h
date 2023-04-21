//
// Created by Matteo Illing on 20.04.23.
//

#ifndef PSE_WEBSERVER_VALIDATION_H
#define PSE_WEBSERVER_VALIDATION_H

#include <sys/syslimits.h>
#include <unistd.h>
#include "strlib.h"
#include "config.h"

int isFileExistent(char* filepath);
int isFileInsideDocroot(char* filepath);
int isFileAccessible(char* filepath);
int isMethodValid(string* method);

#endif //PSE_WEBSERVER_VALIDATION_H
