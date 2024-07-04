#include "stdafx.h"
#include "dir.h"
#include "util.h"
#include "aes.h"


char extstr[] = ".xxx";

void filterExt( std::vector<const std::string>fileList, const char * ext,std::vector<const std::string>&extList ) {
	char *tmp = new char[1000];
	for(int i=0;i<fileList.size();i++){
		strcpy(tmp, fileList[i].c_str());
	//	if (strstr(tmp, ext) != 0){
		if (endsWith(tmp, ext) != 0){
			extList.push_back(fileList[i]);	
		}
	}
	delete []tmp;
}

void find(char* lpPath,std::vector<const std::string> &fileList) 
{ 
	char szDir[500] ;
  char szFind[MAX_PATH]; 
  WIN32_FIND_DATA FindFileData; 
   
  strcpy(szFind,lpPath); 
  strcat(szFind,"\\*.*"); 
  
  strcpy(szDir, lpPath);
  strcat(szDir, "\\\\"); 
  HANDLE hFind=::FindFirstFile(szFind,&FindFileData); 
  if(INVALID_HANDLE_VALUE == hFind)  return; 
 
  while(true) 
  { 
    if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
    { 
      if(FindFileData.cFileName[0]!='.') 
      { 
        char szFile[MAX_PATH]; 
        strcpy(szFile,lpPath); 
        strcat(szFile, "\\"); 
        strcat(szFile,(char* )(FindFileData.cFileName)); 
        find(szFile,fileList); 
      } 
    } 
    else 
    { 
      //std::cout << FindFileData.cFileName << std::endl; 
		char szFull[1000];
		strcpy(szFull, szDir);	
		strcat(szFull, FindFileData.cFileName);
		  fileList.push_back(szFull); 
    } 
    if(!FindNextFile(hFind,&FindFileData))  break; 
  } 
  FindClose(hFind); 
} 