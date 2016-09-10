#pragma once

#include "xyKrnl.h"

//重载以便构造函数和析构函数调用它们
inline void*  __cdecl operator new(UINT size,POOL_TYPE PageType=PagedPool)
{
	 return ExAllocatePool(PageType,size);
}
inline void __cdecl operator delete(void* buf)
{
	ExFreePool(buf);
}

#define printf(x)  { DbgPrint("----------");   DbgPrint x;   DbgPrint("----------");  }
#define printfrn(x)  { DbgPrint("----------");   DbgPrint x;   DbgPrint("----------");   DbgPrint("\n");}



#define INIT_ALLOCATE_LENGTH 64 //字符串缓冲区的初始分配大小

//Author: XY
//Date: 2013-4-27
//Version V1.0
/*********************Describtion***********************************************************************
//UNICODE_STRING封装类，方便操作UNICODE_STRING 操作，析构时会自动释放
//CUString 内部的缓冲附加存放了末尾的空字符，方便与PCWSTR互相转换，但Length表示的长度没有包含末尾的空字符
********************************************************************************************************/
//optimizing space:内存分配方面可以优化
class CUString: public UNICODE_STRING
{
public:
	CUString();//全部初始化为0
	CUString(SHORT InitCapacity,POOL_TYPE PoolType=PagedPool, BYTE InitFill=0x00);//初始分配构造一个空的容器
	CUString(ANSI_STRING& AnsiStr, POOL_TYPE PoolType=PagedPool);//初始为另一个现成的字符串中的值，拷贝到内部的缓冲区中
	CUString(UNICODE_STRING& ustr, POOL_TYPE PoolType=PagedPool);//分配完全相同大小的内存后拷贝源字符串内的值
	CUString(UNICODE_STRING& ustr, USHORT cch, POOL_TYPE PoolType=PagedPool);//构造一个子串
	CUString(CUString& ustr, USHORT ccb, POOL_TYPE PoolType=PagedPool);//构造一个子串
	CUString(CUString& ustr, POOL_TYPE PoolType=PagedPool);//分配完全相同大小的内存后拷贝源字符串内的值
	CUString(PCSTR sz,POOL_TYPE PoolType=PagedPool);//注意sz必须是某个以\0结尾的宽字符串
	CUString(PCWSTR wsz, POOL_TYPE PoolType=PagedPool);//注意wsz必须是某个以\0\0结尾的宽字符串
	CUString(ULONG val,ULONG ToStringJinZhiType,POOL_TYPE PoolType=PagedPool,USHORT InitAllocLength=INIT_ALLOCATE_LENGTH);//初始构造，将指定的整数值转换为一个指定类型的字符串存放在内部的缓冲区中，如12可以转换为十六进制的"C"
	CUString(WCHAR wch,POOL_TYPE PoolType=PagedPool){ Init();  AllocateBuffer(4,PoolType,0); Buffer[0]=wch; Buffer[1]=0; Length=2; }
	CUString(char ch, POOL_TYPE PoolType=PagedPool){ Init();  AllocateBuffer(4,PoolType,0); ANSI_STRING anstr={1,1,&ch}; RtlAnsiStringToUnicodeString(this,&anstr,FALSE); Buffer[1]=0;}
	virtual ~CUString();//析构时会自动释放内部的内存	

	CUString& operator = (ANSI_STRING& AnsiStr);	
	CUString& operator = (UNICODE_STRING& ustr);
	CUString& operator = (CUString& ustr){ *this=(UNICODE_STRING&)ustr; return *this; }
	CUString& operator = (WCHAR wch){ if(MaximumLength<4) {AllocateBuffer(4,PagedPool,0);}   Buffer[0]=wch; Buffer[1]=0; Length=2;  return *this;}
	CUString& operator = (char ch){ if(MaximumLength<4) {AllocateBuffer(4,PagedPool,0);}  ANSI_STRING anstr={1,1,&ch}; RtlAnsiStringToUnicodeString(this,&anstr,FALSE); Buffer[1]=0; return *this;}
	CUString& operator = (PCSTR sz);//注意sz必须是某个以\0结尾的宽字符串
	CUString& operator = (PCWSTR wsz); //注意wsz必须是某个以\0\0结尾的宽字符串
	CUString& operator = (ULONG val);//将值固定转换为十进制字符串存在内部，内部会自动分配内存，实际占用的长度取决于值的大小
	CUString  operator + (UNICODE_STRING& o);
	CUString  operator + (WCHAR wch){ return *this + CUString(wch); }
	CUString  operator + (CHAR ch){ return *this + CUString(ch); }
	CUString  operator + (PCWSTR wsz){ return *this + CUString(wsz); }
	CUString  operator + (PCSTR sz){ return *this + CUString(sz); }
	CUString& operator += (UNICODE_STRING& o){ return Cat(o,PagedPool);}
	CUString& operator += (PCWSTR wsz){ return Cat(wsz,PagedPool);}
	CUString& operator += (PCSTR sz){ return Cat(CUString(sz),PagedPool);}
	CUString& operator += (WCHAR wch){ return Cat(CUString(wch),PagedPool);}
	CUString& operator += (CHAR ch){ return Cat(CUString(ch),PagedPool);}



	WCHAR& operator [] (USHORT idx){ return Buffer[idx]; }
	operator ULONG (){ return	ConvertToInt(10);	}//默认将内部的字符串当作十进制字符串，来进行转换
	operator int(){	return ConvertToInt(10);}//默认将内部的字符串当作十进制字符串，来进行转换
	operator PWSTR (){	return Buffer;	}

	
	//均为大小写不敏感的比较操作符
	operator > (CUString& other){ return Compare(other,TRUE)>0; }
	operator >= (CUString& other){ return Compare(other,TRUE)>=0; }
	operator < (CUString& other){ return Compare(other,TRUE)<0; }
	operator <= (CUString& other){ return Compare(other,TRUE)<=0; }
	operator == (CUString& other){ return Compare(other,TRUE)==0; }	
	operator != (CUString& other){ return Compare(other,TRUE)!=0; }	
	
public:
    WCHAR* GetBuffer(){return Buffer;}//缓冲区的地址
	int GetCapacity(){return MaximumLength;}//缓冲区的容量
	int GetLength(){return Length;}//当前缓冲区中的实际数据长度
	UNICODE_STRING* GetPustr(){return this;}//提供这个函数用于方便KePrint中%wZ格式化输出
public:
	void Uppercase(){ _wcsupr(Buffer); }
	void Lowercase(){ _wcslwr(Buffer); }
	LONG Compare(CUString& other, BOOLEAN CaseInsensitive=TRUE){ return RtlCompareUnicodeString(this,&other,CaseInsensitive);}
	ULONG ConvertToInt(ULONG AsJinZhiType=10);//将内部的字符串转换成整数,默认将内部的字符串当作十进制串解析
	BOOLEAN ConvertToAString(ANSI_STRING& dst,BOOLEAN bAllocMemory=TRUE){return SUCCEEDED(RtlUnicodeStringToAnsiString(&dst,this,bAllocMemory));}//目标字符串需要用RtlFreeAnsiString释放
public:
	BOOLEAN Insert(USHORT idx, WCHAR wch, POOL_TYPE pooltype=PagedPool, USHORT time=1);//在指定地方插入一个或多个字符
	BOOLEAN Insert(USHORT idx, PCWSTR wstr, POOL_TYPE pooltype=PagedPool, USHORT time=1);//在指定地方插入一个或多个子串
	BOOLEAN Insert(USHORT idx, UNICODE_STRING& ustr, POOL_TYPE pooltype=PagedPool, USHORT time=1);//在指定地方插入一个或多个子串

	//将内部所有的src字符串替换为dst串，注意目标子串长于源串时可能会发生内存重新分配，返回最终替换的字符串个数
	int Replace(PCWSTR wstrSrc, PCWSTR wstrDst, POOL_TYPE pooltype=PagedPool, BOOLEAN bCaseSensitive=FALSE);
	int Replace(UNICODE_STRING& wstrSrc, UNICODE_STRING& wstrDst, POOL_TYPE pooltype=PagedPool, BOOLEAN bCaseSensitive=FALSE);

	int Remove(WCHAR wch,POOL_TYPE pooltype=PagedPool,BOOLEAN bCaseSensitive=FALSE);//移除所有指定的字符，返回字符个数
	int Remove(PCWSTR substr,POOL_TYPE pooltype=PagedPool,BOOLEAN bCaseSensitive=FALSE);//移除所有指定的子串，返回字符串个数
	int Remove(UNICODE_STRING& substr,  POOL_TYPE pooltype=PagedPool, BOOLEAN bCaseSensitive=FALSE);//移除所有子串，返回字符串个数
	void Delete(USHORT idx, USHORT cch=1);//从指定索引出的地方删除指定个数的字符

	CUString Mid(USHORT idx,USHORT cch=0, POOL_TYPE pooltype=PagedPool);//若cch为0，表示截取到末尾
	CUString Left(USHORT cch,POOL_TYPE PoolType=PagedPool);
	CUString Right(USHORT cch, POOL_TYPE pooltype=PagedPool);
	////////////////////////////////////返回索引//////////////////////////////////////////////////////////////////////////////
	SHORT Find(UNICODE_STRING& ustr,BOOLEAN bCaseSensitive=FALSE);//查找子串最先出现的位置，若未找到，返回-1
	SHORT Find(WCHAR wch,BOOLEAN bCaseSensitive=FALSE);//查找字符最先出现的位置，若未找到，返回-1
	SHORT Find(PCWSTR wsz,BOOLEAN bCaseSensitive=FALSE){return Find(CUString(wsz),bCaseSensitive);}//查找时，不包含末尾的空字符
	SHORT FindOneOf(PCWSTR wszCharset,BOOLEAN bCaseSensitive=FALSE);//查找最先出现在指定字符集中字符的位置	
	SHORT FindOneOf(UNICODE_STRING& ustrCharset,BOOLEAN bCaseSensitive=FALSE);//查找最先出现在指定字符集中字符的位置
	SHORT FindNotOneOf(PCWSTR wszCharset,BOOLEAN bCaseSensitive=FALSE);//查找第一个没有出现在指定字符集中字符的位置	
	SHORT FindNotOneOf(UNICODE_STRING& ustrCharset,BOOLEAN bCaseSensitive=FALSE);//查找第一个没有出现在指定字符集中字符的位置
	SHORT ReverseFind(WCHAR wch,BOOLEAN bCaseSensitive=FALSE);//反向查找字符最先出现的位置，若未找到，返回-1
	int GetSubStrCnt(PCWSTR SubStr,BOOLEAN bCaseSensitive=FALSE);//获取内部包含有多少个指定的子串
	int GetSubStrCnt(UNICODE_STRING& SubStr, BOOLEAN bCaseSensitive=FALSE);//获取内部包含有多少个指定的子串
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CUString& Cat(PCWSTR wstr, POOL_TYPE PoolType=PagedPool){ UNICODE_STRING ustr;RtlInitUnicodeString(&ustr,wstr); return Cat(ustr,PoolType); }
	CUString& Cat(UNICODE_STRING& o, POOL_TYPE PoolType=PagedPool);//连接另外一个字符串
	BOOLEAN AllocateBuffer(USHORT length,POOL_TYPE PoolType=PagedPool,BYTE InitFill=0x00);//注意分配内存的时候，会自动释放现有已有的内存
	void FreeBuffer();//完全释放内部的缓冲


protected:
	void Init(); 
};





