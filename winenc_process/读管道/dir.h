#ifndef _DIR_H_
#define _DIR_H_

#include "stdafx.h"
#include <stdio.h> 
#include <string.h> 
#include<iostream> 
#include<vector> 
#include <Windows.h> 
#include <fstream>  
#include <iterator> 
#include <string> 


void find(char* lpPath,std::vector<const std::string> &fileList);
 
void filterExt( std::vector<const std::string>fileList, const char * ext,std::vector<const std::string>&extList ) ;
#endif
