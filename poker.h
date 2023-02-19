/* poker.h Copyright 2019, 2023 TNPLR
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

#if __WIN32 || __WINNT
#include <ctime>
#endif //__WIN32 || __WINNT

#include <ostream>
#include <string>
#include <cstdint>
#include <vector>
#include <random>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

namespace pk {
/**
 * An enum of suit
 */
enum {
	CLUB=0, /**< CLUB*/
	DIAMOND, /**< DIAMOND*/
	HEART, /**< HEART*/
	SPADE, /**< SPADE*/
};

/**
 * A poker card
 */
struct card {
	/** The constructor of a poker card
	 * @param _suit The suit of the card. You may use the enum to fill this argument. Defalut to Club.
	 * @param _number The number of the card. 0 is None. 1 - 13 is Ace to King. 14 is Joker. Default to None.
	 */
	card(unsigned short _suit = 0, unsigned short _number = 0) : suit{_suit}, number{_number} {};
	// 0 - club
	// 1 - diamond
	// 2 - heart
	// 3 - spade
	/** The suit of the card */
	unsigned short suit:2;

	// 0 - No card
	// 1 - Ace
	// 2 - two
	// ...
	// 11 - Jack
	// 12 - Queen
	// 13 - King
	// 14 - Joker
	/** The number of the card */
	unsigned short number:4;
	/** The output for card
	 * E.g.
	 *     std::cout << NAME_OF_THE_CARD;
	 * @param os The output stream
	 * @param cd The card to output
	 */
	friend std::ostream& operator<<(std::ostream& os, const card& cd);
	/** Get the suit of card (string) */
	char const* suit_sign() const;
	/** Get the rank of card (string) */
	char const* card_rank() const;
	bool operator==(card const& rop)
	{
		return number == rop.number && suit == rop.suit;
	}
#if unix || __GNUC__
	/**
	 * Graph of suits (UTF-8)
	 */
	static constexpr const char *const suit_image[4]{"\xe2\x99\xa3", "\xe2\x99\xa6", "\xe2\x99\xa5", "\xe2\x99\xa0"};
#elif __WIN32 || __WINNT
	/**
	 * Graph of suits (Code Page 437)
	 */
	static constexpr const char *const suit_image[4]{"\x5", "\x4", "\x3", "\x6"};
#endif // unix
	static constexpr char const * const cardname[15]{"", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "JOKER"};
};

char const* card::suit_sign() const
{
	return suit_image[suit];
}

char const* card::card_rank() const
{
	return cardname[number];
}


class deck {
public:
	/** Remove the card which is identical to c from the deck
	 * @param c Card to remove
	 * @return The card that is removed (i.e. c)
	*/
	card remove(card const& c);

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

	/** @return Return true if the deck is empty. (i.e. this->size() == 0) */
	inline bool empty(void) const
	{
		return pile.size() == 0;
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

	/** This enumerator is the mode of sorting */
	enum class sort_mode {
		rank_first_descending, /**< Sort in SA HA DA CA SK HK ... D2 C2 order */
		suit_first_descending, /**< Sort in SA SK ... S2 HA HK ... C3 C2 order */
	};

	/** Sort the deck
	 * @param mode the mode of sorting
	 */
	void sort(sort_mode mode = sort_mode::rank_first_descending);

	/** This enum is the mode of printing a deck */
	enum class print_mode {
		no_sort, /**< Print without additional sorting */
		sort_by_number, /**< Sort in ascending order from A 2 3 ...*/
		sort_by_suit, /**< Sort by suit first, and then the number*/
		rank_only, /**< print only the rank */
	};


	/** Set the Printing mode
	 * @param mode The mode of printing
	 */
	inline void set_print_mode(print_mode mode = print_mode::no_sort) const
	{
		switch (mode) {
			case print_mode::sort_by_number:
				output_stream = &deck::sort_by_number_ostream;
				break;
			case print_mode::sort_by_suit:
				output_stream = &deck::sort_by_suit_ostream;
				break;
			case print_mode::no_sort:
				output_stream = &deck::no_sort_ostream;
				break;
			case print_mode::rank_only:
				output_stream = &deck::rank_only_ostream;
				break;
		}
	}

	/** Get the subset of deck of a specific suit
	 * @param suit the suit (enumerator)
	 */
	deck suit_subdeck(int suit) const;
private:
	mutable std::ostream& (deck::*output_stream)(std::ostream &os) const = &deck::no_sort_ostream;

	std::vector<struct card> pile; /**< Internal storage of cards */

	std::ostream& no_sort_ostream(std::ostream &os) const;
	std::ostream& sort_by_number_ostream(std::ostream &os) const;
	std::ostream& rank_only_ostream(std::ostream &os) const;
	std::ostream& sort_by_suit_ostream(std::ostream &os) const;
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

	/** Play a card from the player to the card pile.
	 * @param player_no The number of the player.
	 * @param card The card in the player's deck
	 */
	void play(unsigned int player_no, card const& c);

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

	/** Sort the cards of every player in rank-first ascending order*/
	void sort_player_card();

	~poker() = default;

private:
	unsigned int players;
	deck pile;
	std::vector<deck> player_card;
}; // class poker

std::ostream& operator<<(std::ostream& os, const card& cd)
{
	switch (cd.number) {
		case 14:
			os << cd.card_rank();
			return os;
		default:
			os << cd.suit_sign() << ' ' << std::setw(2) << cd.card_rank();
			return os;
	};
}

std::ostream& operator<<(std::ostream& os, poker pk)
{
	os << pk.pile;
	return os;
}

std::ostream& operator<<(std::ostream &os, const deck& dk)
{
	return (dk.*dk.output_stream)(os);
}

std::ostream& deck::no_sort_ostream(std::ostream &os) const
{
	if (empty()) {
		return os;
	}
	os << (*this)[0];
	for (unsigned int i = 1; i < size(); ++i) {
		os << "  " << (*this)[i];
	}
	return os;
}

card deck::remove(card const& c)
{
	std::vector<card>::iterator it = std::find(pile.begin(), pile.end(), c);
	if (it == pile.end()) {
		throw std::invalid_argument("He or she does not have the card");
	}
	pile.erase(it);
	return c;
}

std::ostream& deck::sort_by_number_ostream(std::ostream &os) const
{
	deck tmp = *this;
	tmp.sort();
	os << tmp;
	return os;
}

std::ostream& deck::rank_only_ostream(std::ostream &os) const
{
	std::for_each(pile.cbegin(), pile.cend(), [&os](card const& crd) -> void {
			os << crd.card_rank() << ' ';
	});
	return os;
}


std::ostream& deck::sort_by_suit_ostream(std::ostream &os) const
{
	for (int i = SPADE; i >= CLUB; --i) {
		deck tmp = suit_subdeck(i);
		tmp.set_print_mode(deck::print_mode::rank_only);
		os << card::suit_image[i] << "  " << tmp << "\n";
	}
	return os;
}

deck deck::suit_subdeck(int suit) const
{
	deck dktmp;
	std::for_each(pile.cbegin(), pile.cend(), [=, &dktmp](card const& crd) -> void {
			if (crd.suit == suit) {
				dktmp.push_back(crd);
			}
	});
	return dktmp;
}

void deck::sort(sort_mode mode)
{
	switch (mode)
	{
	case sort_mode::rank_first_descending:
		std::sort(pile.begin(), pile.end(), [](card const &a, card const &b)->bool {
			if (a.number != b.number) {
				return a.number == 1 || (b.number != 1 && a.number > b.number);
			}
			return a.suit > b.suit;
		});
		return;
	case sort_mode::suit_first_descending:
		std::sort(pile.begin(), pile.end(), [](card const &a, card const &b)->bool {
			if (a.suit == b.suit) {
				return a.number == 1 || (b.number != 1 && a.number > b.number);
			}
			return a.suit > b.suit;
		});
		break;
	default:
		return;
	}
}

void poker::sort_player_card()
{
	std::for_each(player_card.begin(), player_card.end(), [](deck& tmp) {
		tmp.sort();
	});
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
		player_card.push_back(::pk::deck{});
	}
}

void poker::shuffle(unsigned int time)
{
	unsigned int sz = pile.size();

#if unix
	std::random_device rd;
	std::mt19937_64 generator(rd());
#elif __WIN32 || __WINNT
	std::mt19937_64 generator(::time(0));
#else
	std::random_device rd;
	std::mt19937_64 generator(rd());
#endif // unix
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

void poker::play(unsigned int player_no, card const& c)
{
	if (player_no >= players) {
		throw std::out_of_range("The player number is too large");
	}
	player_card[player_no].remove(c);
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
	card_per_person *= players;
	for (unsigned int card_count = 0; card_count < card_per_person; ++card_count) {
		player_card[player_no].push_back(pile.back());
		pile.pop_back();
		if (++player_no >= players) {
			player_no = 0;
		}
	}
}

} // namespace poker
#endif // POKER_H_
