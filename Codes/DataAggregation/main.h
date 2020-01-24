/*
 * Author: Yi Zhou <zhouyi1023@tju.edu.cn> 
 * Date: 2019/11
 */

#ifndef MAIN_H
#define MAIN_H

#define  THRESHOLD_LOW 4
#define  THRESHOLD_HIGH 6
#define DIFF_THRESHOLD 2 /*within this threshold, we consider the data is same*/
#define BUFFER_SIZE 12
#define ALPHABET_SIZE 3

struct distribution{
	float mean;
	float std;
};

#endif
