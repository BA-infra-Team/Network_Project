#include <stdio.h>


// 메뉴를 출력하는 UI 함수
int PrintUI()
{
	int nInput = 0;
	// system("cls");
	printf("===================================================\n");
	printf("서버 Start\n");
	printf("---------------------------------------------------\n");
	printf("[1] 출력\t [2] 연결종료\t [3] 로그기록보기\t [0] 종료\n");
	printf("===================================================\n");

	// 사용자가 선택한 메뉴의 값을 반환한다.
	scanf("%d", &nInput);
	getchar();
	// getchar();//버퍼에 남은 엔터 제거용
	return nInput;
}

int main (void)
{
	while(1)
	{
		PrintUI();
	}
}
