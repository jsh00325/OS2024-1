#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DUCK_COUNT 5

// === 오리 구조체 선언 ====================================
struct duck {
	char name[20];
	double weight;
	struct duck* nxtPtr;
};
typedef struct duck Duck;
typedef Duck* DuckPtr;

// === 낚싯대 구조체 선언 ===================================
struct rod {
	int duckCount;
	DuckPtr front;
	DuckPtr rear;
};
typedef struct rod Rod;
typedef Rod* RodPtr;

// === fishing_rod.c에 정의된 함수 =========================

/**	낚싯대를 만들어 반환하는 함수
 * 	@return 낚싯대의 주소 */
RodPtr create_fishing_rod();

/**	현재 낚싯대에 잡힌 오리 정보를 출력하는 함수
 * 	@details 잡힌 오리의 수 / 잡힌 오리의 이름과 무게를 순서대로 출력 */
void print_fishing_rod_info(RodPtr curRod);

/**	생성된 낚싯대를 free하는 함수
 * 	@details 낚싯대에 잡힌 모든 오리를 free하고 낚싯대를 free함 */
void destroy_fishing_rod(RodPtr curRod);

// === duck.c에 정의된 함수 ================================

/**	새로운 오리를 만들어 반환하는 함수
 * 	@param name 새로운 오리의 이름
 * 	@param weight 새로운 오리의 무게
 * 	@return 새로운 오리의 주소 */
DuckPtr create_duck(const char* name, double weight);

/**	현재 오리의 정보를 출력하는 함수
 * 	@param curDuck 출력하고자 하는 오리의 주소 */
void print_duck_info(DuckPtr curDuck);

/**	잡은 오리를 낚싯대에 넣는 함수
 * 	@param rod 잡는 낚싯대의 주소
 * 	@param newDuck 잡은 오리의 주소
 * 	@return 해당 오리가 낚싯대에 잡히면 1, 안잡히면 0 반환 */
int catch_duck(RodPtr rod, DuckPtr newDuck);

/**	가장 먼저 잡은 오리를 놓아주는 함수
 * 	@param curRod 놓아줄 낚싯대
 * 	@return 놓아준 오리의 주소를 반환 */
DuckPtr release_duck(RodPtr curRod);

/**	생성된 오리를 free하는 함수 */
void destroy_duck(DuckPtr curDuck);