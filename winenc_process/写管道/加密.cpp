// ����.cpp : �������̨Ӧ�ó������ڵ㡣

#include "stdafx.h"
#include<stdio.h>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include "aes.h"
#include "err.h"
#include "file.h"
#include "dir.h"
#include "th.h"

#define MYTHNUM 5

#define BUF_SIZE 4096
// ����ܵ��� , ����ǿ�����ͨ�� , ����Բ�㴦ָ���������˳������ڵ�������
#define EXAMP_PIPE   "\\\\.\\PIPE\\EB3F2E4B_52E2_40F9_A17D_B4A2588F23AB"

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	//��ȡĿ¼�����ļ�
	vector<const string> fileList;
	vector<const string> fileExtList;
	find(".", fileList);
	strcpy(extstr, ".png");
	filterExt(fileList, extstr, fileExtList);


	HANDLE hPipe = NULL;
    char  szBuffer[BUF_SIZE] = { 0 };
    DWORD dwReturn = 0;
 
    // �ж��Ƿ��п������õ������ܵ�  
    if (!WaitNamedPipe(EXAMP_PIPE, NMPWAIT_USE_DEFAULT_WAIT))
    {
        cout << "No Read Pipe Accessible" << endl;
        return 0;
    }
 
    // �򿪿��õ������ܵ� , ����������˽��̽���ͨ��  
    hPipe = CreateFile(EXAMP_PIPE, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);
 
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        cout << "Open Read Pipe Error" << endl;
        return 0;
    }

    // �����˷�������
	memset(szBuffer, 0, BUF_SIZE);

	char filePath[1000];
	

	while(1){
		if(fileExtList.size()>0) {
			strcpy(filePath, fileExtList.back().c_str());
			fileExtList.pop_back();
		}else {
			break;
			//continue;
		}
//		cin >> szBuffer;
		if (!WriteFile(hPipe, filePath, strlen(filePath), &dwReturn, NULL))
		{
			cout << "Write Failed" << endl;
		}
	}


    CloseHandle(hPipe);

/*
	if(argc != 2) {
		cout << "����.exe xxx.png";
		exit(1);
	}
	if(EncFile(argv[1]) ==FALSE) {
		exit(1);
	}
*/

	/*
	BYTE readbs[ENCLEN] = {0};
	DWORD lold = 16, l = 16;

	BinaryReadByte(argv[1], l, readbs);
	
	printf("read count[%d]\n", l);
	
	for(int i =0;i<l;i++) {
		printf("%x ",  readbs[i]);
	}
	printf("\n");
	

	Encbs(readbs, l);
	printf("wbs[%d]\n" ,l );

	for(int i =0;i<l;i++) {
		printf("%x ",  readbs[i]);
	}
	printf("\n");
	
	
	//WriteEncbs2Encfile(argv[1],lold, l, readbs);
	exit(0);
*/

/*	
	cout <<"line: "<<__LINE__ << " " << extstr << endl;
	if(argc != 4 ) {
		cout << "����.exe \"·��\" enc/dec .png" << endl;
		exit(1);
	}
	strcpy(extstr, argv[3]);
	if(strcmp(argv[2], "dec")==0 && strcmp(extstr ,".enc")!=0){
		cout << "����.exe \"·��\" dec .enc" << endl;
		strcpy(extstr, ".enc");
	}
	vector<const string> fileList;
	vector<const string> fileExtList;
	find(argv[1], fileList);
	filterExt(fileList, extstr, fileExtList);
	if(strcmp(argv[2], "enc") == NULL){
		ctAes(MYTHNUM,EncList, &fileExtList);
	}
	if(strcmp(argv[2], "dec")==NULL){
		ctAes(MYTHNUM,DecList, &fileExtList);
	}
	exit(0);
*/

//	ct(10, ThreadFun)	;




	/*
	char *src = "Hello, World!";
	printf("%d\n", strlen(src));
    char dest[2];
    strncpy(dest, src, 10); // ����src \0 ֮ǰ��ǰn���ַ�  �������\0���
	printf("%d %s %x %x %x\n",__LINE__ ,dest, dest[6], dest[7],dest[8]);
	printf("%c %c %x %x\n", dest[0], dest[1], dest[2], dest[3]);	
	system("pause");
	*/

/*
	const char *src = "Hello, W%s%sorld!";
    char dest[20];
	printf("%x\n", &dest[14]);// ֱ�Ӵ�ӡ�ַ���ʾ δ��ʼ��
    strncpy(dest, src, sizeof(dest) - 1); // ����һ��λ�ø����ַ�
	printf("17 %d\n", sizeof(dest));// ֱ�Ӵ�ӡ�ַ���ʾ δ��ʼ��
	printf("18 %d\n", strlen(dest));// ֱ�Ӵ�ӡ�ַ���ʾ δ��ʼ��
    dest[sizeof(dest) - 1] = '\0'; // ȷ���ַ����Կ��ַ���β
    dest[sizeof(dest) - 2] = 'a'; // ȷ���ַ����Կ��ַ���β
	printf("21 %d\n", sizeof(dest));// ֱ�Ӵ�ӡ�ַ���ʾ δ��ʼ��
	printf("22 %d\n", strlen(dest));// ֱ�Ӵ�ӡ�ַ���ʾ δ��ʼ��
    printf("Copied string: %s\n", dest);
	printf("strlen dest %d\n", strlen(dest));
	printf("%d %c %x %x %x %x\n", sizeof(dest), dest[12], dest[13], dest[14], dest[19], dest[20]);
*/	

	/*
	char *a1054 = new char[100];
	char a1057[]={0, 1};
	printf("%d\n", strlen(a1054));
	printf("%d\n", strlen(a1057));
	system("pause");
	*/

/*	
	char a2019[] = {0};
	a2019[1] = 'a';
	a2019[2] = 'b';
	printf("%x\n", a2019[200]);
	printf("%d\n", sizeof(a2019));
	system("pause");
*/	

/*
	BYTE ccc[] = {0};
	printf("%d %x\n",__LINE__, readbs[10]);
	printf("%d %x\n",__LINE__, ccc[0]);
	*/



/*	
	BYTE readbs[ENCLEN] = {0};
	DWORD lold = 32, l = 32;

	BinaryReadByte(argv[1], l, readbs);
	
	printf("read count[%d]\n", l);
	for(int i =0;i<l;i++) {
		printf("%x ",  readbs[i]);
	}
	printf("\n");
	system("pause");

	Decbs(readbs ,l);
	printf("wbs[%d]\n" ,l );
	for(int i =0;i<l;i++) {
		printf("%x ",  readbs[i]);
	}
	printf("\n");
	WriteDecbs2Decfile(argv[1],lold, l, readbs);

*/


	/*
	BYTE *writebs = new BYTE[l];//����������ָ�볣��

	for(int i =0;i<l ;i++) {
		writebs[i] = readbs[i];
	}
	
	strncpy((char*)writebs, (char*)readbs, l);

	printf("wbs[%d]\n" ,l );
	for(int i =0;i<l;i++) {
		printf("%x ",  writebs[i]);
	}
	printf("\n");
*/	

/*

	if (strncmp(argv[1], "enc", 3 ) == 0){
		if(EncFile(argv[2])==FALSE) {
			char pszErr[ENCNUM] = {0};
			_snprintf(pszErr,100,"encfile %s err", argv[2] );
			ShowError(pszErr);
			return -1;
		}
	}else if (strncmp(argv[1], "dec", 3 ) == 0) {
		if(DecFile(argv[2]) == FALSE) {
			char pszErr[ENCNUM] = {0};
			_snprintf(pszErr,100,"decfile %s err", argv[2] );
			ShowError(pszErr);
			return -1;
		}
	} else {
		ShowError("type");
		return -1;
	}
	
*/

/*
	memset((void*)writebs,0, l);
	Decbs(readbs, lold);
*/

	/*
	printf("dec[%d]\n" ,lold );
	for(int i =0;i<l;i++) {
		printf("%x ",  readbs[i]);
	}
	printf("\n");
	*/


	//system("pause");
	return 0;
}

