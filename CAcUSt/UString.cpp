#include "UString.h"
CUString::CUString()
{
	Init();
}
void CUString::Init()
{
	Buffer=NULL;
	MaximumLength=0;
	Length=0;
}
CUString::CUString(SHORT Capacity,POOL_TYPE PoolType, BYTE InitFill)
{
	Init();
	AllocateBuffer(Capacity,PoolType,InitFill);
	
}

CUString::CUString(ANSI_STRING& ansistr, POOL_TYPE PoolType)
{
	Init();
	if ( ansistr.Buffer && ansistr.Length!=0 )
	{
		int capacity=ansistr.Length*2+2;
		void* p=ExAllocatePool(PoolType,capacity);
		if (p)
		{
			Buffer=(USHORT*)p;
			Length=capacity-2;
			MaximumLength=capacity;
			RtlCopyMemory(Buffer,ansistr.Buffer,Length);
			Buffer[Length/2]=0;
		}
	}
}

CUString::CUString(UNICODE_STRING& ustr, POOL_TYPE PoolType)
{
	Init();
	if ( ustr.Buffer && ustr.Length!=0)
	{
		void* p=ExAllocatePool(PoolType,ustr.Length+2);
		if (p)
		{
			Buffer=(USHORT*)p;
			Length=ustr.Length;
			MaximumLength=Length+2;
			RtlCopyMemory(Buffer,ustr.Buffer,Length);
			Buffer[Length/2]=0;
		}
	}
}

CUString::CUString(UNICODE_STRING& ustr, USHORT cch, POOL_TYPE PoolType)
{
	Init();
	USHORT len=cch*2;
	if ( ustr.Buffer && len!=0)
	{
		int actuallen=min(len,ustr.Length);
		void* p=ExAllocatePool(PoolType,actuallen+2);
		if (p)
		{
			Buffer=(USHORT*)p;
			MaximumLength=actuallen+2;
			Length=actuallen;
			RtlCopyMemory(Buffer,ustr.Buffer,actuallen);
			Buffer[actuallen/2]=0;

		}
	}
}
CUString::CUString(CUString& ustr, USHORT cch, POOL_TYPE PoolType)
{
	Init();
	USHORT len=cch*2;
	if ( ustr.Buffer && len!=0)
	{
		int actuallen=min(len,ustr.Length);
		void* p=ExAllocatePool(PoolType,actuallen+2);
		if (p)
		{
			Buffer=(USHORT*)p;
			MaximumLength=actuallen+2;
			Length=actuallen;
			RtlCopyMemory(Buffer,ustr.Buffer,actuallen);
			Buffer[actuallen/2]=0;	
		}
	}
}
//驱动中构造函数调用构造函数是不起作用的，相当于没有调用
CUString::CUString(CUString& ustr, POOL_TYPE PoolType)
{
	Init();
	if ( ustr.Buffer && ustr.Length!=0)
	{
		AllocateBuffer(ustr.Length+2,PoolType,0);
		Length=ustr.Length;
		RtlCopyMemory(Buffer,ustr.Buffer,Length);
		Buffer[Length/2]=0;
	}

}

CUString::CUString(PCWSTR pcwstr, POOL_TYPE PoolType)
{
	Init();
	USHORT len=(wcslen(pcwstr))*2+2;
	void* p=ExAllocatePool(PoolType,len);
	if (p)
	{
		Buffer=(USHORT*)p;
		MaximumLength=len;
		Length=len-2;
		RtlCopyMemory(Buffer,pcwstr,len);

	}

}


CUString::CUString(PCSTR sz,POOL_TYPE PoolType)
{
	Init();
	ANSI_STRING AnsiStr; 
	RtlInitAnsiString(&AnsiStr,sz); 

	AllocateBuffer(strlen(sz)*2+2,PoolType,0);
	RtlAnsiStringToUnicodeString(this,&AnsiStr,FALSE);
	Buffer[Length/2]=0;
}



CUString::	CUString(ULONG val,ULONG StringJinZhiType,POOL_TYPE PoolType,USHORT InitAllocLength)//初始分配一块内存，容放转换后的整数值
{	
	Init();
	void* p=ExAllocatePool(PoolType,InitAllocLength);
	if (p)
	{
		Buffer=(USHORT*)p;
		MaximumLength=InitAllocLength;
		RtlIntegerToUnicodeString(val,StringJinZhiType,this);//转换后，实际的长度与具体的值有关
		Buffer[Length/2]=0;
	}

}
CUString& CUString::operator = (ANSI_STRING& AnsiStr)
{ 
	FreeBuffer();
	if ( AnsiStr.Buffer && AnsiStr.Length!=0)
	{
		AllocateBuffer(AnsiStr.Length*2+2,PagedPool,0);	
		RtlAnsiStringToUnicodeString(this,&AnsiStr,FALSE); 
		Buffer[Length/2]=0;
	
	}

	return *this;
}

CUString& CUString::operator = (UNICODE_STRING& ustr)
{ 
	FreeBuffer();
	if ( ustr.Buffer && ustr.Length!=0)
	{
		AllocateBuffer(ustr.Length+2,PagedPool,0);
		RtlCopyUnicodeString(this,&ustr);
		Buffer[Length/2]=0;
	}
    return *this;
}	
CUString& CUString::operator = (PCSTR sz)
{
	FreeBuffer();
	ANSI_STRING AnsiStr; 
	RtlInitAnsiString(&AnsiStr,sz); 
	RtlAnsiStringToUnicodeString(this,&AnsiStr,TRUE);
	return *this;
}	
CUString& CUString::operator = (PCWSTR pcwstr)
{
	USHORT len=(wcslen(pcwstr))*2;
	AllocateBuffer(len+2,PagedPool,0);
	RtlCopyMemory(Buffer,pcwstr,len+2);
	Length=len;
	return *this;
} 

CUString& CUString::operator = (ULONG val)
{ 
	FreeBuffer();
	void* p=ExAllocatePool(PagedPool,INIT_ALLOCATE_LENGTH);
	if (p)
	{
		Buffer=(USHORT*)p;
		MaximumLength=INIT_ALLOCATE_LENGTH;
		RtlIntegerToUnicodeString(val,10,this);//转换后，实际的长度与具体的值有关
		Buffer[Length/2]=0;
	}	
	return *this;
}

CUString::~CUString()
{
	FreeBuffer();	
}


BOOLEAN  CUString::AllocateBuffer(USHORT len,POOL_TYPE PageType,byte initval)
{
	if (MaximumLength>=len)
	{
		RtlFillMemory(Buffer,MaximumLength,initval);
		return TRUE;
	}
	FreeBuffer();//先释放先前自己分配的内存
	void* p=ExAllocatePool(PageType,len);
	if (p==NULL)
		return FALSE;
	Buffer=(PWSTR)p;
	MaximumLength=len;
	Length=0;
	RtlFillMemory(p,len,initval);
	return TRUE;
}

void CUString::FreeBuffer()
{
	if (Buffer && MaximumLength!=0)
	{
		ExFreePool(Buffer);
		Buffer=NULL;
		Length=0;
		MaximumLength=0;
	}
	
}

ULONG CUString::ConvertToInt(ULONG JinZhiType)
{
	ULONG val;
	if(FAILED(RtlUnicodeStringToInteger(this,JinZhiType,&val)))
		return 0;
	else
		return val;
}






CUString& CUString::Cat(UNICODE_STRING& o, POOL_TYPE PoolType)
{
	if (o.Buffer==NULL || o.Length==0)
		return *this;
	USHORT len=Length+o.Length+2;
	if (len > MaximumLength)
	{
		USHORT* newbuf=(USHORT*)ExAllocatePool(PoolType,len);
		if (newbuf)
		{
			RtlCopyMemory(newbuf,Buffer,Length);
			RtlCopyMemory((BYTE*)newbuf+Length,o.Buffer,o.Length);
			Buffer=newbuf;Length=len-2;MaximumLength=len;
			Buffer[Length/2]=0;
		}
	}
	else//若已有的缓冲区够大，就直接连在原串的末尾，免去内存分配操作
	{
		RtlCopyMemory((BYTE*)Buffer+Length,o.Buffer,o.Length);
		Length=len-2;
		Buffer[Length/2]=0;
	}

	return *this;

}


CUString CUString::operator + (UNICODE_STRING& o)
{
	CUString ustr=*this;
	ustr.Cat(o,PagedPool);
	return ustr;
}

SHORT CUString::Find(WCHAR wch,BOOLEAN bCaseSensitive)
{
	if(Buffer==NULL || Length==0)
	{
		return -1;
	}
	return Find(CUString(wch),bCaseSensitive);
}

SHORT CUString::Find(UNICODE_STRING& ustr,BOOLEAN bCaseSensitive)
{
	if (ustr.Buffer==NULL || ustr.Length==0 || ustr.Length>Length || Buffer==NULL || Length==0)
	{
		return -1;
	}
	
	UNICODE_STRING curustr={ustr.Length,ustr.Length,NULL};
	for (curustr.Buffer=Buffer; curustr.Buffer<=(USHORT*)((byte*)Buffer+Length-ustr.Length); curustr.Buffer+=1)
	{
		if (RtlCompareUnicodeString(&curustr,&ustr,!bCaseSensitive)==0)
		{   
			return (curustr.Buffer-Buffer);
		}
	}
		
	return -1;
	
}

SHORT CUString::FindOneOf(PCWSTR wszCharset,BOOLEAN bCaseSensitive)
{
	if (!bCaseSensitive)
	{
		CUString ustr=*this;
		CUString ustr2=wszCharset;
		ustr.Uppercase();
		ustr2.Uppercase();
		int idx=wcscspn(ustr.Buffer,ustr2.Buffer);
		return Buffer[idx]==0?-1:idx;	
	}
	else
	{
		int idx=wcscspn(Buffer,wszCharset);
		return Buffer[idx]==0?-1:idx;
	}
}
SHORT CUString::FindOneOf(UNICODE_STRING& ustrCharset,BOOLEAN bCaseSensitive)
{
	CUString ustr2=ustrCharset;
	if (!bCaseSensitive)
	{
		CUString ustr=*this;
		ustr.Uppercase();
		ustr2.Uppercase();
		int idx=wcscspn(ustr,ustr2);
		return Buffer[idx]==0?-1:idx;		
	}
	else
	{
		int idx=wcscspn(Buffer,ustr2);
		return Buffer[idx]==0?-1:idx;	
	}
}
SHORT CUString::FindNotOneOf(PCWSTR wszCharset,BOOLEAN bCaseSensitive)
{
	if (!bCaseSensitive)
	{
		CUString ustr=*this;
		CUString ustr2=wszCharset;
		ustr.Uppercase();
		ustr2.Uppercase();
		int idx=wcsspn(ustr.Buffer,ustr2.Buffer);
		return idx;	
	}
	else
	{
		int idx=wcsspn(Buffer,wszCharset);
		return idx;
	}
}
SHORT CUString::FindNotOneOf(UNICODE_STRING& ustrCharset,BOOLEAN bCaseSensitive)
{
	CUString ustr2=ustrCharset;
	if (!bCaseSensitive)
	{
		CUString ustr=*this;
		ustr.Uppercase();
		ustr2.Uppercase();
		int idx=wcscspn(ustr,ustr2);
		return idx;		
	}
	else
	{
		int idx=wcscspn(Buffer,ustr2);
		return idx;	
	}
}

SHORT CUString::ReverseFind(WCHAR wch,BOOLEAN bCaseSensitive)
{
	if (!bCaseSensitive)
	{
		WCHAR strdst[2];strdst[0]=wch;strdst[1]=0;
		WCHAR strsrc[2];
		for (SHORT idx=Length/2-1;idx>=0;idx--)
		{
			strsrc[0]=Buffer[idx];strsrc[1]=0;
			if (_wcsicmp(strsrc,strdst)==0)
			{
				return idx;
			}
		}

	}
	else
	{
		WCHAR* pos=wcsrchr(Buffer,wch);
		return pos?(pos-Buffer):-1;
	}
	return -1;
}

CUString CUString::Left(USHORT cch,POOL_TYPE pooltype)
{
	return CUString(*this,cch,pooltype);
}

CUString CUString::Right(USHORT cch, POOL_TYPE pooltype)
{
	CUString ustr((SHORT)cch*2+2,pooltype,0);
	RtlCopyMemory(ustr.Buffer,Buffer+Length/2-cch,cch*2);
	ustr.Length=cch*2;
	ustr[cch]=0;
	return ustr;	
}

CUString CUString::Mid(USHORT idx, USHORT cch, POOL_TYPE pooltype)
{
	if (idx>=Length/2)
		return CUString();
	int len;//最终要拷贝的长度
	if (cch==0  || idx+cch>Length/2)
		len=Length-idx*2;
	else
		len=cch*2;
		
	CUString ustr((SHORT)len+2,pooltype,0);
	RtlCopyMemory(ustr.Buffer,Buffer+idx,len);
	ustr.Length=len;
	ustr[USHORT(Length/2)]=0;
	return ustr;
	
}
int CUString::GetSubStrCnt(PCWSTR wstr,BOOLEAN bCaseSensitive)
{
	int dstlen=wcslen(wstr);
	if (dstlen==0 || dstlen>Length/2)
		return 0;
	UNICODE_STRING dst;
	RtlInitUnicodeString(&dst,wstr);
	UNICODE_STRING tmp={dstlen*2,dstlen*2,NULL};
	int cnt=0;
	for (tmp.Buffer=Buffer; tmp.Buffer<=Buffer+Length/2-dstlen; )
	{
		if (RtlCompareUnicodeString(&tmp,&dst,bCaseSensitive)==0)
		{
			cnt++;
			tmp.Buffer+=dstlen;
		}
		else
		{
			tmp.Buffer++;
		}
	}

	return cnt;

}

int CUString::GetSubStrCnt(UNICODE_STRING &dst,BOOLEAN bCaseSensitive)
{
	int dstlen=dst.Length/2;
	if (dstlen==0 || dstlen>Length/2)
		return 0;
	UNICODE_STRING tmp={dstlen*2,dstlen*2,NULL};
	int cnt=0;
	for (tmp.Buffer=Buffer; tmp.Buffer<=Buffer+Length/2-dstlen; )
	{
		if (RtlCompareUnicodeString(&tmp,&dst,bCaseSensitive)==0)
		{
			cnt++;
			tmp.Buffer+=dstlen;
		}
		else
		{
			tmp.Buffer++;
		}
	}
	
	return cnt;
}

int CUString::Replace(PCWSTR wstrSrc, PCWSTR wstrDst, POOL_TYPE pooltype,BOOLEAN bCaseSensitive)
{
	int srclen=wcslen(wstrSrc);
	if (srclen>Length/2)
		return 0;
	int dstlen=wcslen(wstrDst);
	CUString ustrSrc=wstrSrc;
	CUString ustrReplaced((short)1024,pooltype,0);//初始的目标字符串的长度为1024，防止频繁发生重分配降低效率
	
	UNICODE_STRING dst={dstlen*2,dstlen*2,(USHORT*)wstrDst};
	UNICODE_STRING tmp={srclen*2,srclen*2,NULL};
	UNICODE_STRING stage={0,0,Buffer};//每次阶段拷贝的源字符串
	int cnt=0;
	for (tmp.Buffer=Buffer; tmp.Buffer<=Buffer+Length/2-srclen; )
	{
		if (RtlCompareUnicodeString(&tmp,&ustrSrc,!bCaseSensitive)==0)
		{	//找到一个子串后
			stage.MaximumLength=stage.Length=(tmp.Buffer-stage.Buffer)*2;
			ustrReplaced.Cat(stage,pooltype);//将这个阶段的字符串拷贝到目标地
			ustrReplaced.Cat(dst,pooltype);//再连接目标字符串


			tmp.Buffer+=srclen;//从这个地方开始继续查找后面的子串
			stage.Buffer=tmp.Buffer;//阶段串的位置也跟着后移
			cnt++;
		}
		else
		{
			tmp.Buffer++;
		}
	}

	if (stage.Buffer<Buffer+Length/2)//若阶段字符串没达到末尾
	{
		stage.MaximumLength=stage.Length=Length-(stage.Buffer-Buffer)*2;
		ustrReplaced.Cat(stage,pooltype);
	}

	
	Buffer=ustrReplaced.Buffer;
	MaximumLength=ustrReplaced.MaximumLength;
	Length=ustrReplaced.Length;
	ustrReplaced.Buffer=NULL;
	ustrReplaced.MaximumLength=ustrReplaced.Length=0;//防止析构时被自动释放

	return cnt;

}

int CUString::Replace(UNICODE_STRING& Src, UNICODE_STRING& Dst, POOL_TYPE pooltype,BOOLEAN bCaseSensitive)
{
	CUString cusrc(Src);
	CUString cudst(Dst);
	PCWSTR wstrSrc=cusrc.Buffer;
	PCWSTR wstrDst=cudst.Buffer;

	return Replace(wstrSrc,wstrDst,pooltype,bCaseSensitive);
}

int CUString::Remove(PCWSTR substr, POOL_TYPE pooltype,BOOLEAN bCaseSensitive)
{
	return Replace(substr,L"",pooltype,bCaseSensitive);
}
int CUString::Remove(UNICODE_STRING& substr,  POOL_TYPE pooltype, BOOLEAN bCaseSensitive)
{
	return Replace(substr,CUString(L""),pooltype,bCaseSensitive);
}
int CUString::Remove(WCHAR wch,POOL_TYPE pooltype,BOOLEAN bCaseSensitive)
{
	return Replace(CUString(wch).Buffer,L"",pooltype,bCaseSensitive);
}
void CUString::Delete(USHORT idx, USHORT cch)
{
	if (idx>=Length/2)
		return;
	if (idx+cch>=Length/2)
	{
		Length=idx*2;
		Buffer[idx]=0;
		return;
	}
	else
	{
		RtlMoveMemory(Buffer+idx, Buffer+idx+cch, Length-2*(idx+cch));
		Length-=cch*2;
		Buffer[Length/2]=0;
		return;
	}
}

BOOLEAN CUString::Insert(USHORT idx, PCWSTR wstr, POOL_TYPE pooltype, USHORT time)
{
	if (time==0 || wstr==L"" || idx>=Length/2)
		return TRUE;
	int dstlen=wcslen(wstr);
	USHORT finallen=Length+dstlen*2*time;
	if (finallen+2>MaximumLength)
	{
		CUString newustr((SHORT)(finallen+2),pooltype);
		RtlCopyMemory(newustr.Buffer,Buffer,idx*2);
		newustr.Length=idx*2;
		for (USHORT u=0;u<time;u++)
			newustr.Cat(wstr,pooltype);
		RtlCopyMemory(newustr.Buffer+idx+dstlen*time, Buffer+idx,Length-idx*2);	
		newustr.Length=finallen;
		newustr.Buffer[finallen/2]=0;
		
		Buffer=newustr.Buffer;
		MaximumLength=newustr.MaximumLength;
		Length=newustr.Length;

		newustr.Buffer=NULL;
		newustr.MaximumLength=newustr.Length=0;//防止析构时被释放
		return TRUE;
	}
	else
	{	

		RtlMoveMemory(Buffer+idx+dstlen*time, Buffer+idx, Length-idx*2);
		WCHAR* pos=Buffer+idx;
		for (USHORT u=0; u<time; u++,pos+=dstlen)
			RtlCopyMemory(pos,wstr,dstlen*2);
		Length+=dstlen*2*time;
		Buffer[Length/2]=0;
		return TRUE;

	}
}
BOOLEAN CUString::Insert(USHORT idx, WCHAR wch, POOL_TYPE pooltype, USHORT time)
{
	return Insert(idx,CUString(wch).Buffer,pooltype,time);
}
BOOLEAN CUString::Insert(USHORT idx, UNICODE_STRING& ustr, POOL_TYPE pooltype, USHORT time)
{
	return Insert(idx,CUString(ustr).Buffer,pooltype,time);
}