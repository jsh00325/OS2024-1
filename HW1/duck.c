// 오리에 대한 함수 정의
#include "duck_hunt.h"

DuckPtr create_duck(const char* name, double weight)
{
	// 동적할당을 통해서 오리 생성
	DuckPtr newDuck = (DuckPtr) malloc(sizeof(Duck));

	// 오리 할당 실패
	if (newDuck == NULL) {
		printf("No memory available to make a duck.\n");
		return NULL;
	}

	// 오리 정보 초기화
	strcpy(newDuck->name, name);
	newDuck->weight = weight;
	newDuck->nxtPtr = NULL;

	return newDuck;
}

void print_duck_info(DuckPtr curDuck)
{
	printf("[Name: %s, Weight: %.2lf]\n", curDuck->name, curDuck->weight);
}

int catch_duck(RodPtr rod, DuckPtr newDuck)
{
	// 낚싯대에 오리가 꽉찬 경우
	if (rod->duckCount == MAX_DUCK_COUNT) {
		printf("The fishing rod is full.\n");
		return 0;
	}

	// 낚싯대에 오리가 없는 경우
	if (rod->duckCount == 0)
	{
		rod->front = newDuck;
		rod->rear = newDuck;
		++(rod->duckCount);
	}
	// 낚싯대에 이미 오리가 있는 경우
	else
	{
		rod->rear->nxtPtr = newDuck;
		rod->rear = newDuck;
		++(rod->duckCount);
	}

	printf("Duck is catched. ");
	print_duck_info(newDuck);
	return 1;
}

DuckPtr release_duck(RodPtr curRod) 
{
	// 낚싯대에 오리가 없는 경우
	if (curRod->duckCount == 0) {
		printf("No ducks in fishing rod.\n");
		return NULL;
	}

	// 낚싯대에 오리가 존재하는 경우
	DuckPtr targetDuck = curRod->front;

	// targetDuck이 낚싯대에서 빠져나왔으므로, 낚싯대의 정보 수정
	curRod->front = targetDuck->nxtPtr;
	--(curRod->duckCount);

	printf("Duck is released. ");
	print_duck_info(targetDuck);

	return targetDuck;
}

void destroy_duck(DuckPtr curDuck)
{
	free(curDuck);
}