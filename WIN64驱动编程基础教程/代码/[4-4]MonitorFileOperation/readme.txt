��ӡ��ʵʱ�ļ�������
ͬʱ������readme.txt����ɾ�����޸ġ���������

�ֹ���װ���������ȴ�CMD����
��װ������InfDefaultInstall INF�ļ�����·��
����������net start MFTest
�رձ�����net stop MFTest

�Զ���װ������
����WdmDrvLoader.exe

��������ش��룺
@title �����ļ�[readme.txt]����
@copy mftest.sys c:\
@copy setup.inf c:\
@InfDefaultInstall "c:\setup.inf"
@net start mftest
@echo �ļ������ɹ������ڿ��Բ����ļ�����Ч����
@echo �������ȡ���ļ�������
@pause
@del c:\mftest.sys
@del c:\setup.inf
@del c:\windows\system32\drivers\mftest.sys
@net stop mftest
@echo ��ɨ������ϡ�
@pause