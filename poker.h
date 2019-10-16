/* poker.h Copyright 2019 TNPLR
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef POKER_H_
#define POKER_H_

#include <vector>
#include <random>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

namespace poker {
/**
 * An enum of suit
 */
enum {
	CLUB=0, /**< CLUB*/
	DIAMOND, /**< DIAMOND*/
	HEART, /**< HEART*/
	SPADE, /**< SPADE*/
};

#if unix
/**
 * Graph of suits (UTF-8)
 */
constexpr const char *suit[4] = {"\xe2\x99\xa3", "\xe2\x99\xa6", "\xe2\x99\xa5", "\xe2\x99\xa0"};
#elif __WIN32 || __WINNT
/**
 * Graph of suits (Code Page 437)
 */
constexpr const char *suit[4] = {"\x5", "\x4", "\x3", "\x6"};
#endif // unix

/**
 * A poker card
 */
struct card {
	/** The constructor of a poker card
	 * @param _suit The suit of the card. You may use the enum to fill this argument. Defalut to Club.
	 * @param _number The number of the card. 0 is None. 1 - 13 is Ace to King. 14 is Joker. Default to None.
	 */
	card(unsigned char _suit = 0, unsigned char _number = 0) : suit{_suit}, number{_number} {};
	// 0 - club
	// 1 - diamond
	// 2 - heart
	// 3 - spade
	/** The suit of the card */
	unsigned char suit:2;

	// 0 - No card
	// 1 - Ace
	// 2 - two
	// ...
	// 11 - Jack
	// 12 - Queen
	// 13 - King
	// 14 - Joker
	/** The number of the card */
	unsigned char number:4;
	/** The output for card
	 * E.g.
	 *     std::cout << NAME_OF_THE_CARD;
	 * @param os The output stream
	 * @param cd The card to output
	 */
	friend std::ostream& operator<<(std::ostream& os, card cd);
};

class deck {
public:
	/** @return The vector of this deck of cards*/
	std::vector<struct card>& get_vec(void)
	{
		return pile;
	}

	/** @return The const vector of this deck of cards*/
	const std::vector<struct card>& get_vec(void) const
	{
		return pile;
	}

	/** @return The const vector of this deck of cards*/
	const std::vector<struct card>& get_vec_const(void) const
	{
		return pile;
	}

	/** Put a card into the deck
	 * @param c Card to put into
	 */
	inline void push_back(struct card c)
	{
		pile.push_back(c);
	}

	/** Remove the last card in the deck*/
	inline void pop_back(void)
	{
		pile.pop_back();
	}

	/** @return The size of the deck */
	inline size_t size(void) const
	{
		return pile.size();
	}

	/** @return The last card in the deck*/
	inline struct card& back()
	{
		return pile.back();
	}

	/** Output the deck
	 * E.g.
	 *     std::cout << NAME_OF_THE_DECK;
	 * @param os The output stream
	 * @param dk The deck to output
	 */
	friend std::ostream& operator<<(std::ostream &os, const deck& dk);
	/** The indexth element of the deck
	 * @return A reference of the card
	 */
	inline struct card& operator[](unsigned int index)
	{
		return pile[index];
	}

	/** The indexth element of the deck (for const deck)
	 * @return A const reference of the card
	 */
	inline const struct card& operator[](unsigned int index) const
	{
		return pile[index];
	}
private:
	std::vector<struct card> pile;
};

class poker {
public:
	// Default 1 Deck (52 cards and 2 persons)
	/** A constructor of poker game
	 * @param deck The number of decks to put into the card pile. Default to 1.
	 * @param player The player of the poker game. Default to 2.
	 * @param joker Whether the jokers should be put into the pile. Default to false.
	 */
	explicit poker(unsigned int deck = 1, unsigned int player = 2, bool joker = false);

	/** Shuffle the card pile
	 * @param time The time to shuffle the card. Default to 1000
	 */
	void shuffle(unsigned int time = 1000);

	/** Draw a card from the pile to the player.
	 * @param player_no The number of the player.
	 * @return The card drawed by the function.
	 */
	struct card draw(unsigned int player_no);

	/** Deal some card to each person.
	 * @param card_per_person How many card should the function deal to each player.
	 */
	void deal(unsigned int card_per_person);

	/** Output the card pile
	 * E.g.
	 *     std::cout << NAME_OF_THE_GAME;
	 * @param os The output stream
	 * @param pk The poker game card pile to output
	 */
	friend std::ostream& operator<<(std::ostream& os, poker pk);

	/** Card of number *th player.
	 * @return A reference of the player's deck
	 */
	deck& operator[](unsigned int player_no)
	{
		return player_card[player_no];
	}

	/** Sort the cards of every player. */
	void sort_player_card();
	~poker() = default;

private:
	unsigned int players;
	deck pile;
	std::vector<deck> player_card;
}; // class poker

std::ostream& operator<<(std::ostream& os, card cd)
{
	if (cd.number != 14) {
		os << suit[cd.suit] << ' ' << std::setw(2) << static_cast<unsigned>(cd.number);
	} else {
		os << "JOKER";
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, poker pk)
{
	os << pk.pile;
	return os;
}

std::ostream& operator<<(std::ostream &os, const deck& dk)
{
	os << dk[0];
	for (unsigned int i = 1; i < dk.size(); ++i) {
		os << "  " << dk[i];
	}
	return os;
}

void poker::sort_player_card()
{
	for (auto &vec : player_card) {
		std::sort(vec.get_vec().begin(), vec.get_vec().end(), [](struct card a, struct card b)->bool {
			if (a.number < b.number) {
				return true;
			} else if (a.number == b.number) {
				if (a.suit < b.suit) {
					return true;
				}
			}
			return false;
		});
	}
}

poker::poker(unsigned int deck, unsigned int player, bool joker) : players{player}
{
	if (player == 0) {
		throw std::invalid_argument("Argument \'player\' cannot be zero");
	}

	for (unsigned int k = 0; k < deck; ++k) {
		for (unsigned char i = 0; i < 4; ++i) {
			for (unsigned char j = 1; j <= 13; ++j) {
				pile.push_back(card{i, j});
			}
		}
		if (joker) {
			pile.push_back(card{1,14});
			pile.push_back(card{1,14});
		}
	}

	for (unsigned int i = 0; i < players; ++i) {
		player_card.push_back(::poker::deck{});
	}
}

void poker::shuffle(unsigned int time)
{
	unsigned int sz = pile.size();

	std::random_device rd;
	std::mt19937_64 generator(rd());
	std::uniform_real_distribution<double> unif(0.0, static_cast<double>(sz));
	for (unsigned int i = 0; i < time; ++i) {
		unsigned int a = static_cast<unsigned int>(floor(unif(generator)));
		unsigned int b;
		do {
			b = static_cast<unsigned int>(floor(unif(generator)));
		} while (a == b);
		std::swap(pile[a], pile[b]);
	}
}

struct card poker::draw(unsigned int player_no)
{
	if (player_no >= players) {
		throw std::out_of_range("The player number is too large");
	}
	player_card[player_no].push_back(pile.back());
	pile.pop_back();
	return player_card[player_no].back();
}

void poker::deal(unsigned int card_per_person)
{
	unsigned int player_no{0};
	unsigned int card_count{0};
	card_per_person *= players;
	for (auto it = pile.get_vec_const().crbegin(); it < pile.get_vec_const().crend(); ++it) {
		if (card_count < card_per_person) {
			++card_count;
		} else {
			break;
		}
		player_card[player_no].push_back(*it);
		pile.pop_back();
		if (++player_no >= players) {
			player_no = 0;
		}
	}
}

}; // namespace poker
#endif // POKER_H_
