#include<stdio.h>
#include<windows.h>
#include<process.h>
unsigned WINAPI Read(void*arg);
unsigned WINAPI Accu(void*arg);

static HANDLE semOne;
static HANDLE semTwo;
static int num;

int main(int argc, char*argv[])
{
	HANDLE hThread1, hThread2;
	semOne = CreateSemaphore(NULL, 0, 1, NULL);
	semTwo = CreateSemaphore(NULL, 1, 1, NULL);
	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Read, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, Accu, NULL, 0, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	CloseHandle(semOne);
	CloseHandle(semTwo);
	return 0;
}
unsigned WINAPI Read(void*arg)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		fputs("Input num: ", stdout);
		//semTwo�ź��������ֵ>0Ϊsignaled״̬��Ϊ0ʱΪnon-signaled״̬
		//����ʱ���ź���>0�Ż᷵�أ�ͬʱ���ź���ֵ��1,ͬʱ����non-signaled״̬(�ź���-1==0)
		//
		WaitForSingleObject(semTwo, INFINITE);
		scanf_s("%d", &num);
		//�ͷ��ź������ź���ֵ+1�����������ֵ����
		ReleaseSemaphore(semOne, 1, NULL);
	}
	return 0;
}

unsigned WINAPI Accu(void*arg)
{
	int sum = 0, i;
	for (i = 0; i < 5; i++)
	{
		WaitForSingleObject(semOne, INFINITE);
		sum += num;
		ReleaseSemaphore(semTwo, 1, NULL);
	}
	printf("Result: %d \n", sum);
	return 0;
}