/*
 * Author: Yi Zhou <zhouyi1023@tju.edu.cn> 
 * Date: 2019/11
 */

#ifndef SAX_H
#define SAX_H

#include <stdio.h>
#include "main.h"

/*get sax representation*/
void sax(float *data, uint8_t size, struct distribution *dist, char *sax_data);

#endif
