  
#ifndef __TREEDEF_H   //确保该头文件只被包含一次
#define __TREEDEF_H


typedef struct _TABLE_NAME_RECORDSET {
	    WORD TableNo;
		char TableName[TABLEOFDB_NAME_LENGTH];
		char ccTableName[TABLEOFDB_CCNAME_LENGTH];
		BYTE TableType;
		BYTE IndexFieldNo;
		BYTE NameFieldNo;
		BYTE SortFieldNo;
		BYTE FilterFieldNo;
	} TABLE_NAME_RECORDSET;


typedef struct _TABLE_NAME_TREE {
		WORD wMapNo;
		HTREEITEM hTreeItem;
		TABLE_NAME_RECORDSET NameRecord;
		CPtrArray arrayField;
	} TABLE_NAME_TREE;

typedef struct _TABLE_FIELD_SET {
	    WORD    FieldDataType;
		WORD	MemLen;
		char	DataTypeName[32];
		int		ShowLen;
		char	FieldName[FIELDOFTB_NAME_LENGTH];
	} TABLE_FIELD_SET;


#endif

