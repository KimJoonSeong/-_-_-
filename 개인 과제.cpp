#define _CRT_SECURE_NO_WARNINGS

#include <bangtal>
using namespace bangtal;

#include <cstdlib>
#include <ctime>

#include <Windows.h>
#include <time.h>

ScenePtr scene;
ObjectPtr game_board[9], game_original[9];
ObjectPtr start;

TimerPtr timer;

float animationTime = 0.1f;
int mixcount = 5;


int blank;

int game_index(ObjectPtr piece) {

	for (int i = 0; i < 9; i++)
		if (game_board[i] == piece) return i;
	return -1;	//블랭크와 먼 블록
}

int index_to_x(int index) {

	return 0 + 427 * (index % 3);

}

int index_to_y(int index) {

	return 480 - 240 * (index / 3);

}


void game_move(int index) {
	//index와 blank 바꾸기

	auto piece = game_board[index];
	game_board[index] = game_board[blank];
	game_board[index]->locate(scene, index_to_x(index), index_to_y(index));
	game_board[blank] = piece;
	game_board[blank]->locate(scene, index_to_x(blank), index_to_y(blank));
	blank = index;
}

bool check_move(int index) {
	//blank왼쪽은 index == blank -1
	if (blank % 3 > 0 && blank - 1) return true;

	//index == blank +1

	if (blank % 3 < 2 && blank + 1) return true;

	//index == blank -4

	if (blank / 3 > 0 && index == blank - 3) return true;

	//index == blank +4

	if (blank / 3 < 2 && index == blank + 4) return true;

	return false;
}

int random_move() {
	int index = rand() % 9;
	while (check_move(index))
		index = rand() % 9;
	return index;

}

void start_game() {
	int mixCount = 5;

	timer->set(animationTime);
	timer->start();
	//타이머 루프를 만들어야 함.

	blank = 9;
	game_board[blank]->hide();

	start->hide();
}

bool check_end() {
	for (int i = 0; i < 9; i++) {
		if (game_board[i] != game_original[i]) return false;
	}

	return true;


}

void end_game() {
	showMessage("끝!!");
	start->show();
}

void init_game() {

	auto scene = Scene::create("원신 퍼즐", "images/원본.png");

	char path[20];

	for (int i = 0; i < 9; i++) {
		sprintf(path, "Images%d.png", i + 1);
		game_board[i] = Object::create(path, scene, index_to_x(i), index_to_y(i));
		game_board[i]->setOnMouseCallback([&](auto piece, auto x, auto y, auto action) ->bool {

			int index = game_index(piece);
			if (check_move(index)
				game_move(index);

				if (check_end())P
					end_game();
				return True;

				});

			game_original[i] = game_board[i];
	}

	//시작버튼 생성

	start = Object::create("images/start.png", scene, 590, 100);
	start->setOnMouseCallback([&](auto, auto, auto, auto) ->bool {
		start_game();
		return True;

		});


	timer = Timer::create(animationTime);
	timer->setOnTimerCallback([&](auto)->bool {
		game_move(random_move());

		mixCount--;
		if (mixCount > 0) {
			timer->set(animationTime);
			timer->start();

		}




		return true;
		});

	blank = 9;
	game_board[blank]->hide();

	startGame(scene);

}


int main() {
	srand((unsigned int)time(NULL));


	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);


	init_game();



	return 0;
}