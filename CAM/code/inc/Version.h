//***********************************************************
//Version.h����ͨ�ð汾�ŵĹ�����������ÿ�����̵�.rc�ļ���
//�ļ������\incĿ¼��
//Create by CHENAJ 20150129
//***********************************************************

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

//********************************************
//��һ�������ά������Ҫʱʱ���µ�
#define VERSION_MAJOR					1
#define VERSION_MINOR					3
#define VERSION_REVISION				1
#define VERSION_BUILD					0
#define VER_FILE_COMMENT_STR			"V1.31.0.150711"
//********************************************

//��������ʽ��һ�ֱ��붺�Ÿ�����һ�ֱ������ַ�����������
#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
	"." STRINGIZE(VERSION_MINOR)    \
	"." STRINGIZE(VERSION_REVISION) \
	"." STRINGIZE(VERSION_BUILD)    \

#define VER_FILE_PLATFORM_STR			"NTS-9000"
#define VER_COMPANY_STR					"�Ͼ������Զ�������ϵͳ���޹�˾"
#define VER_COPYRIGHT_STR				"Copyright (C) 2003-2015 �Ͼ������Զ�������ϵͳ���޹�˾����������Ȩ����"

 
#ifdef _DEBUG
  #define VER_VER_DEBUG             VS_FF_DEBUG
#else
  #define VER_VER_DEBUG             0
#endif
 
#define VER_FILEOS					VOS_NT_WINDOWS32
#define VER_FILEFLAGS				VER_VER_DEBUG
#define VER_FILETYPE				VFT_APP
