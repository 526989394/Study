#pragma once

#include "xyKrnl.h"

//�����Ա㹹�캯��������������������
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



#define INIT_ALLOCATE_LENGTH 64 //�ַ����������ĳ�ʼ�����С

//Author: XY
//Date: 2013-4-27
//Version V1.0
/*********************Describtion***********************************************************************
//UNICODE_STRING��װ�࣬�������UNICODE_STRING ����������ʱ���Զ��ͷ�
//CUString �ڲ��Ļ��帽�Ӵ����ĩβ�Ŀ��ַ���������PCWSTR����ת������Length��ʾ�ĳ���û�а���ĩβ�Ŀ��ַ�
********************************************************************************************************/
//optimizing space:�ڴ���䷽������Ż�
class CUString: public UNICODE_STRING
{
public:
	CUString();//ȫ����ʼ��Ϊ0
	CUString(SHORT InitCapacity,POOL_TYPE PoolType=PagedPool, BYTE InitFill=0x00);//��ʼ���乹��һ���յ�����
	CUString(ANSI_STRING& AnsiStr, POOL_TYPE PoolType=PagedPool);//��ʼΪ��һ���ֳɵ��ַ����е�ֵ���������ڲ��Ļ�������
	CUString(UNICODE_STRING& ustr, POOL_TYPE PoolType=PagedPool);//������ȫ��ͬ��С���ڴ�󿽱�Դ�ַ����ڵ�ֵ
	CUString(UNICODE_STRING& ustr, USHORT cch, POOL_TYPE PoolType=PagedPool);//����һ���Ӵ�
	CUString(CUString& ustr, USHORT ccb, POOL_TYPE PoolType=PagedPool);//����һ���Ӵ�
	CUString(CUString& ustr, POOL_TYPE PoolType=PagedPool);//������ȫ��ͬ��С���ڴ�󿽱�Դ�ַ����ڵ�ֵ
	CUString(PCSTR sz,POOL_TYPE PoolType=PagedPool);//ע��sz������ĳ����\0��β�Ŀ��ַ���
	CUString(PCWSTR wsz, POOL_TYPE PoolType=PagedPool);//ע��wsz������ĳ����\0\0��β�Ŀ��ַ���
	CUString(ULONG val,ULONG ToStringJinZhiType,POOL_TYPE PoolType=PagedPool,USHORT InitAllocLength=INIT_ALLOCATE_LENGTH);//��ʼ���죬��ָ��������ֵת��Ϊһ��ָ�����͵��ַ���������ڲ��Ļ������У���12����ת��Ϊʮ�����Ƶ�"C"
	CUString(WCHAR wch,POOL_TYPE PoolType=PagedPool){ Init();  AllocateBuffer(4,PoolType,0); Buffer[0]=wch; Buffer[1]=0; Length=2; }
	CUString(char ch, POOL_TYPE PoolType=PagedPool){ Init();  AllocateBuffer(4,PoolType,0); ANSI_STRING anstr={1,1,&ch}; RtlAnsiStringToUnicodeString(this,&anstr,FALSE); Buffer[1]=0;}
	virtual ~CUString();//����ʱ���Զ��ͷ��ڲ����ڴ�	

	CUString& operator = (ANSI_STRING& AnsiStr);	
	CUString& operator = (UNICODE_STRING& ustr);
	CUString& operator = (CUString& ustr){ *this=(UNICODE_STRING&)ustr; return *this; }
	CUString& operator = (WCHAR wch){ if(MaximumLength<4) {AllocateBuffer(4,PagedPool,0);}   Buffer[0]=wch; Buffer[1]=0; Length=2;  return *this;}
	CUString& operator = (char ch){ if(MaximumLength<4) {AllocateBuffer(4,PagedPool,0);}  ANSI_STRING anstr={1,1,&ch}; RtlAnsiStringToUnicodeString(this,&anstr,FALSE); Buffer[1]=0; return *this;}
	CUString& operator = (PCSTR sz);//ע��sz������ĳ����\0��β�Ŀ��ַ���
	CUString& operator = (PCWSTR wsz); //ע��wsz������ĳ����\0\0��β�Ŀ��ַ���
	CUString& operator = (ULONG val);//��ֵ�̶�ת��Ϊʮ�����ַ��������ڲ����ڲ����Զ������ڴ棬ʵ��ռ�õĳ���ȡ����ֵ�Ĵ�С
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
	operator ULONG (){ return	ConvertToInt(10);	}//Ĭ�Ͻ��ڲ����ַ�������ʮ�����ַ�����������ת��
	operator int(){	return ConvertToInt(10);}//Ĭ�Ͻ��ڲ����ַ�������ʮ�����ַ�����������ת��
	operator PWSTR (){	return Buffer;	}

	
	//��Ϊ��Сд�����еıȽϲ�����
	operator > (CUString& other){ return Compare(other,TRUE)>0; }
	operator >= (CUString& other){ return Compare(other,TRUE)>=0; }
	operator < (CUString& other){ return Compare(other,TRUE)<0; }
	operator <= (CUString& other){ return Compare(other,TRUE)<=0; }
	operator == (CUString& other){ return Compare(other,TRUE)==0; }	
	operator != (CUString& other){ return Compare(other,TRUE)!=0; }	
	
public:
    WCHAR* GetBuffer(){return Buffer;}//�������ĵ�ַ
	int GetCapacity(){return MaximumLength;}//������������
	int GetLength(){return Length;}//��ǰ�������е�ʵ�����ݳ���
	UNICODE_STRING* GetPustr(){return this;}//�ṩ����������ڷ���KePrint��%wZ��ʽ�����
public:
	void Uppercase(){ _wcsupr(Buffer); }
	void Lowercase(){ _wcslwr(Buffer); }
	LONG Compare(CUString& other, BOOLEAN CaseInsensitive=TRUE){ return RtlCompareUnicodeString(this,&other,CaseInsensitive);}
	ULONG ConvertToInt(ULONG AsJinZhiType=10);//���ڲ����ַ���ת��������,Ĭ�Ͻ��ڲ����ַ�������ʮ���ƴ�����
	BOOLEAN ConvertToAString(ANSI_STRING& dst,BOOLEAN bAllocMemory=TRUE){return SUCCEEDED(RtlUnicodeStringToAnsiString(&dst,this,bAllocMemory));}//Ŀ���ַ�����Ҫ��RtlFreeAnsiString�ͷ�
public:
	BOOLEAN Insert(USHORT idx, WCHAR wch, POOL_TYPE pooltype=PagedPool, USHORT time=1);//��ָ���ط�����һ�������ַ�
	BOOLEAN Insert(USHORT idx, PCWSTR wstr, POOL_TYPE pooltype=PagedPool, USHORT time=1);//��ָ���ط�����һ�������Ӵ�
	BOOLEAN Insert(USHORT idx, UNICODE_STRING& ustr, POOL_TYPE pooltype=PagedPool, USHORT time=1);//��ָ���ط�����һ�������Ӵ�

	//���ڲ����е�src�ַ����滻Ϊdst����ע��Ŀ���Ӵ�����Դ��ʱ���ܻᷢ���ڴ����·��䣬���������滻���ַ�������
	int Replace(PCWSTR wstrSrc, PCWSTR wstrDst, POOL_TYPE pooltype=PagedPool, BOOLEAN bCaseSensitive=FALSE);
	int Replace(UNICODE_STRING& wstrSrc, UNICODE_STRING& wstrDst, POOL_TYPE pooltype=PagedPool, BOOLEAN bCaseSensitive=FALSE);

	int Remove(WCHAR wch,POOL_TYPE pooltype=PagedPool,BOOLEAN bCaseSensitive=FALSE);//�Ƴ�����ָ�����ַ��������ַ�����
	int Remove(PCWSTR substr,POOL_TYPE pooltype=PagedPool,BOOLEAN bCaseSensitive=FALSE);//�Ƴ�����ָ�����Ӵ��������ַ�������
	int Remove(UNICODE_STRING& substr,  POOL_TYPE pooltype=PagedPool, BOOLEAN bCaseSensitive=FALSE);//�Ƴ������Ӵ��������ַ�������
	void Delete(USHORT idx, USHORT cch=1);//��ָ���������ĵط�ɾ��ָ���������ַ�

	CUString Mid(USHORT idx,USHORT cch=0, POOL_TYPE pooltype=PagedPool);//��cchΪ0����ʾ��ȡ��ĩβ
	CUString Left(USHORT cch,POOL_TYPE PoolType=PagedPool);
	CUString Right(USHORT cch, POOL_TYPE pooltype=PagedPool);
	////////////////////////////////////��������//////////////////////////////////////////////////////////////////////////////
	SHORT Find(UNICODE_STRING& ustr,BOOLEAN bCaseSensitive=FALSE);//�����Ӵ����ȳ��ֵ�λ�ã���δ�ҵ�������-1
	SHORT Find(WCHAR wch,BOOLEAN bCaseSensitive=FALSE);//�����ַ����ȳ��ֵ�λ�ã���δ�ҵ�������-1
	SHORT Find(PCWSTR wsz,BOOLEAN bCaseSensitive=FALSE){return Find(CUString(wsz),bCaseSensitive);}//����ʱ��������ĩβ�Ŀ��ַ�
	SHORT FindOneOf(PCWSTR wszCharset,BOOLEAN bCaseSensitive=FALSE);//�������ȳ�����ָ���ַ������ַ���λ��	
	SHORT FindOneOf(UNICODE_STRING& ustrCharset,BOOLEAN bCaseSensitive=FALSE);//�������ȳ�����ָ���ַ������ַ���λ��
	SHORT FindNotOneOf(PCWSTR wszCharset,BOOLEAN bCaseSensitive=FALSE);//���ҵ�һ��û�г�����ָ���ַ������ַ���λ��	
	SHORT FindNotOneOf(UNICODE_STRING& ustrCharset,BOOLEAN bCaseSensitive=FALSE);//���ҵ�һ��û�г�����ָ���ַ������ַ���λ��
	SHORT ReverseFind(WCHAR wch,BOOLEAN bCaseSensitive=FALSE);//��������ַ����ȳ��ֵ�λ�ã���δ�ҵ�������-1
	int GetSubStrCnt(PCWSTR SubStr,BOOLEAN bCaseSensitive=FALSE);//��ȡ�ڲ������ж��ٸ�ָ�����Ӵ�
	int GetSubStrCnt(UNICODE_STRING& SubStr, BOOLEAN bCaseSensitive=FALSE);//��ȡ�ڲ������ж��ٸ�ָ�����Ӵ�
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CUString& Cat(PCWSTR wstr, POOL_TYPE PoolType=PagedPool){ UNICODE_STRING ustr;RtlInitUnicodeString(&ustr,wstr); return Cat(ustr,PoolType); }
	CUString& Cat(UNICODE_STRING& o, POOL_TYPE PoolType=PagedPool);//��������һ���ַ���
	BOOLEAN AllocateBuffer(USHORT length,POOL_TYPE PoolType=PagedPool,BYTE InitFill=0x00);//ע������ڴ��ʱ�򣬻��Զ��ͷ��������е��ڴ�
	void FreeBuffer();//��ȫ�ͷ��ڲ��Ļ���


protected:
	void Init(); 
};





