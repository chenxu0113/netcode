//***********************************************************
//Version.h用于通用版本号的构建，包含在每个工程的.rc文件中
//文件存放于\inc目录下
//Create by CHENAJ 20150129
//***********************************************************

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

//********************************************
//这一块代码是维护中需要时时更新的
#define VERSION_MAJOR					1
#define VERSION_MINOR					3
#define VERSION_REVISION				1
#define VERSION_BUILD					0
#define VER_FILE_COMMENT_STR			"V1.31.0.150711"
//********************************************

//有两种形式，一种必须逗号隔开，一种必须是字符串，都有用
#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
	"." STRINGIZE(VERSION_MINOR)    \
	"." STRINGIZE(VERSION_REVISION) \
	"." STRINGIZE(VERSION_BUILD)    \

#define VER_FILE_PLATFORM_STR			"NTS-9000"
#define VER_COMPANY_STR					"南京天溯自动化控制系统有限公司"
#define VER_COPYRIGHT_STR				"Copyright (C) 2003-2015 南京天溯自动化控制系统有限公司。保留所有权利。"

 
#ifdef _DEBUG
  #define VER_VER_DEBUG             VS_FF_DEBUG
#else
  #define VER_VER_DEBUG             0
#endif
 
#define VER_FILEOS					VOS_NT_WINDOWS32
#define VER_FILEFLAGS				VER_VER_DEBUG
#define VER_FILETYPE				VFT_APP
