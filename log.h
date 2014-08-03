#ifndef LOG_H_H
#define LOG_H_H

#include <stdio.h>

#define DLOG(param...) if(LOG_ON) { printf("LOG(debug):"); \
							 printf(param);}
#define ELOG(param...) if(LOG_ON) {printf("LOG(error):"); \
							 printf(param);}
		 
#endif

