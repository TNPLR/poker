#include "poker.h"

#include <iostream>

using pk::poker;
int main()
{
	poker game(1, 4, false);
	std::cout << game << '\n';
	game.shuffle();
	std::cout << game << '\n';
	game.deal(13);
	std::cout << "Player 0 " << game[0] << '\n';
	std::cout << "Player 1 " << game[1] << '\n';
	std::cout << "Player 2 " << game[2] << '\n';
	std::cout << "Player 3 " << game[3] << '\n';
	std::cout << "Sort\n";
	game.sort_player_card();
	std::cout << "Player 0 " << game[0] << '\n';
	std::cout << "Player 1 " << game[1] << '\n';
	std::cout << "Player 2 " << game[2] << '\n';
	std::cout << "Player 3 " << game[3] << '\n';
	std::cout << "Player 2, card 3: " << game[2][3] << " Number:" << game[2][3].number << " suit:" << game[2][3].suit << '\n';
	game.play(2, 3);
	std::cout << "Play 2, 3\nPlayer 2 " << game[2] << '\n';

	return 0;
}
