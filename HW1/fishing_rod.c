// 낚싯대를 위한 함수를 정의
#include "duck_hunt.h"

RodPtr create_fishing_rod()
{
    // 동적 할당으로 낚싯대 생성
    RodPtr newRod = (RodPtr) malloc(sizeof(Rod));

    // 낚싯대 할당 실패
    if (newRod == NULL) {
        printf("No memory available to make fishing rod.\n");
        return NULL;
    }

	// 낚싯대 초기화
	newRod->duckCount = 0;
	newRod->front = newRod->rear = NULL;	
	
    return newRod;
}

void print_fishing_rod_info(RodPtr curRod)
{
	// 현재 낚싯대에 오리가 안 잡힌 경우
	if (curRod->duckCount == 0) {
		printf("No ducks are caught.\n");
		return;
	}

	// 잡힌 오리의 수 출력
	printf("%d ducks are caught.\n", curRod->duckCount);

	// 순서대로 잡힌 오리의 이름과 무게 출력
	DuckPtr curDuck = curRod->front;
	for (int duckNum = 1; curDuck != NULL; ++duckNum) {
		printf("Duck %d: ", duckNum);
		print_duck_info(curDuck);
		curDuck = curDuck->nxtPtr;
	}
}

void destroy_fishing_rod(RodPtr curRod)
{
	// 연결리스트를 순회하면서 모든 오리를 destroy
	DuckPtr curDuck = curRod->front;
	while (curDuck != NULL) {
		// curDuck을 바로 free하면 다음 오리에 접근할 수 없어 nxtDuck을 정의
		DuckPtr nxtDuck = curDuck->nxtPtr;
		destroy_duck(curDuck);
		curDuck = nxtDuck;
	}

	// 남은 낚싯대를 free
	free(curRod);
}