#include "duck_hunt.h"

void print_menu();
void simul_catch_duck(RodPtr rod);
void simul_release_duck(RodPtr rod);

int main() 
{	
	RodPtr rod = create_fishing_rod();

	// 낚싯대 할당 실패 시 프로그램 종료
	if (rod == NULL) return 0;

	// 오리 낚시 시뮬레이션
	while (1) {
		print_menu();
		char menu_input;
		scanf("%c", &menu_input);
		// getchar();	// 입력 버퍼에서 개행문자 제거
		
		// 'q' 입력 시 시뮬레이션 종료
		if (menu_input == 'q') break;

		switch (menu_input)
		{
		case '1':	// 오리 잡기
			simul_catch_duck(rod);
			break;

		case '2':	// 오리 놓아주기
			simul_release_duck(rod);
			break;

		case '3':	// 현재 낚싯대 정보
			print_fishing_rod_info(rod);
			break;

		default:
			printf("Wrong Input\n");
			break;
		}
		printf("\n\n");
	}

	// 시뮬레이션 이후 할당된 메모리 해제
	destroy_fishing_rod(rod);

    return 0;
}

void print_menu()
{
	printf("=== Duck Hunt ============\n");
	printf("1. Catch duck\n");
	printf("2. Release duck\n");
	printf("3. View information about current fishing rod.\n");
	printf("q. Exit program\n");
	printf(">>> ");
}

void simul_catch_duck(RodPtr rod)
{	
	// 새로운 오리의 이름과 무게 입력받기
	char name[20];
	double weight;

	printf("Duck name: ");
	scanf("%s", name);
	// getchar();	// 입력 버퍼에서 개행문자 제거

	printf("Duck weight: ");
	scanf("%lf", &weight);
	// getchar();	// 입력 버퍼에서 개행문자 제거

	// 오리 할당 이후 낚싯대에 넣기
	DuckPtr newDuck = create_duck(name, weight);
	if (newDuck == NULL) return;
	
	int catch_result = catch_duck(rod, newDuck);
	
	// 낚싯대가 꽉 차서 잡지 못했을 때 -> 오리 메모리 반환
	if (!catch_result) destroy_duck(newDuck);
}

void simul_release_duck(RodPtr rod)
{
	DuckPtr removedDuck = release_duck(rod);
	
	// 낚싯대에 오리가 없는 경우
	if (removedDuck == NULL) return;

	// 낚싯대에 오리가 존재해서, 오리가 반환된 경우 -> destroy
	destroy_duck(removedDuck);
}