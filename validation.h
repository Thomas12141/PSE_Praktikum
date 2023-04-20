//
// Created by Matteo Illing on 20.04.23.
//

#ifndef PSE_WEBSERVER_VALIDATION_H
#define PSE_WEBSERVER_VALIDATION_H

#include <sys/syslimits.h>
#include "strlib.h"
#include "config.h"

int isResourceValid(string* resource_path);
int isFileExistent(string* filename);

#endif //PSE_WEBSERVER_VALIDATION_H
