/* MasterMind.cpp
   Author: Sharon Diskin
   21/04/2019
   This program is the game "Master mind".
   The user select a code, 
   than the computer tries to guess the code using elimination method according
   to the data (amount of bulls&cows) the user enters.
*/

#include <iostream>
using namespace std;

//Declaration of struct
typedef struct dig4 { //struct that represents the 4-digit code
	int pos1;
	int pos2;
	int pos3;
	int pos4;
} DIG4;

typedef struct bull_cows { // struct to hold the number of bulls and cows
	int same_pos;
	int diff_pos;
} RESLT;

//Daclration of const
int const NUM_DIGS = 4;
int const MAX_CODE = 10000;

void init_canidates(bool array[], int size) { //Function that initialize boolean array
	for (int i = 0; i < size; i++)
		array[i] = 1;
}

bool is_valid_input(int bulls, int cows) { //Function that make sure the input entered is valid
	int total = bulls + cows;
	return (0 <= bulls && bulls < 5 &&
		0 <= cows && cows < 5 &&
		0 <= total && total < 5);
}

RESLT get_bulls_cows() { //Function that gets input from user
	RESLT res;
	int bulls = 0, cows = 0;
	cout << "Bulls? ";
	cin >> bulls;
	cout << "Cows? ";
	cin >> cows;
	if (!is_valid_input(bulls, cows)) {
		do {
			cout << "You entered invalid input, please try again\n";
			cout << "Bulls? ";
			cin >> bulls;
			cout << "Cows? ";
			cin >> cows;
		} while (!is_valid_input(bulls, cows));
	}
	res.same_pos = bulls;
	res.diff_pos = cows;
	return res;
}

DIG4 i_to_dig4(int i) { //Function that converts an int to DIG4
	DIG4 d4 = { i / 1000 ,(i / 100) % 10,(i / 10) % 10, i % 10 };
	return d4;
}

DIG4 select_Guess(bool candidates[], int size) { //Function that pick the computer's guess
	DIG4 newGuess;
	for (int i = 0; i < size; i++) {
		if (candidates[i]) {
			newGuess = i_to_dig4(i);
			return newGuess;
		}
	}
}

RESLT compr_guess(DIG4 guess, DIG4 code) { //Function that compares the given guess with the given code
	RESLT res;
	int bulls = 0;
	int cows = 0;

	//Bulls check
	if (guess.pos1 == code.pos1) {
		bulls++; guess.pos1 = 10; code.pos1 = 11;
	}
	if (guess.pos2 == code.pos2) {
		bulls++; guess.pos2 = 10; code.pos2 = 11;
	}
	if (guess.pos3 == code.pos3) {
		bulls++; guess.pos3 = 10; code.pos3 = 11;
	}
	if (guess.pos4 == code.pos4) {
		bulls++; guess.pos4 = 10; code.pos4 = 11;
	}

	//Cows check
	if (guess.pos1 == code.pos2) {
		cows++;
		code.pos1 = 11;
	}
	else if (guess.pos1 == code.pos3) {
		cows++;
		code.pos3 = 11;
	}
	else if (guess.pos1 == code.pos4) {
		cows++;
		code.pos4 = 11;
	}
	//---------------------------------//
	if (guess.pos2 == code.pos1) {
		cows++;
		code.pos1 = 11;
	}
	else if (guess.pos2 == code.pos3) {
		cows++;
		code.pos3 = 11;
	}
	else if (guess.pos2 == code.pos4) {
		cows++;
		code.pos4 = 11;
	}
	//---------------------------------//
	if (guess.pos3 == code.pos1) {
		cows++;
		code.pos1 = 11;
	}
	else if (guess.pos3 == code.pos2) {
		cows++;
		code.pos2 = 11;
	}
	else if (guess.pos3 == code.pos4) {
		cows++;
		code.pos4 = 11;
	}
	//---------------------------------//
	if (guess.pos4 == code.pos1) {
		cows++;
		code.pos1 = 11;
	}
	else if (guess.pos4 == code.pos2) {
		cows++;
		code.pos2 = 11;
	}
	else if (guess.pos4 == code.pos3) {
		cows++;
		code.pos3 = 11;
	}

	res.same_pos = bulls;
	res.diff_pos = cows;
	return res;
}

bool is_eq_score(RESLT scor1, RESLT scor2) { //Function that determine if two "bulls-cows" result are equal
	return (scor1.same_pos == scor2.same_pos && scor1.diff_pos == scor2.diff_pos);
}

void turn_off_impossible_code(bool candidates[], int size, DIG4 guess, RESLT res) { // Function that sieve impossible codes
	for (int i = 0; i < size; i++)
		if (candidates[i])
			candidates[i] = is_eq_score(res, compr_guess(i_to_dig4(i), guess));
}

bool is_all_turned_off(bool candidates[], int size) { //Function that checks if all the boolean array is turned off
	for (int i = 0; i < MAX_CODE; i++) 
		if (candidates[i])
			return 0;
		return 1;
}

void prn_guess(DIG4 guess) { //Function that prints guess
	cout << "My guess: " << guess.pos1 << " " << guess.pos2
		<< " " << guess.pos3 << " " << guess.pos4 << endl;
}

void prn_res(RESLT res) { //Function that prints code comparassion result
	cout << "Bulls? " << res.same_pos << endl;
	cout << "Cows? " << res.diff_pos << endl << endl;
}

void prn_round(int round) { //Function that prints evrey round
	cout << "Round " << round << endl;
}

bool is_win(RESLT res) { //Function that checks if there is a win
	return (res.same_pos == NUM_DIGS);
}

void prn_win(int round) { //function that prints win
	cout << "\nI guessed in " << round << " rounds." << endl;
}

void prn_wrong_data() { //Function that prints a massage of wrong data entered by user
	cout << "You entered wrong data."
		<< " Computer ran out of options and can't guess your code\n";
}

int main() {
	DIG4 guess; // the guess provided by the computer
	RESLT res; // result of comparrasion the provided by user
	bool candidates[MAX_CODE] = { 0 };
	bool endGame = false;
	int round = 1;

	init_canidates(candidates, MAX_CODE);

	while (!endGame) {
		prn_round(round);
		guess = select_Guess(candidates, MAX_CODE);
		prn_guess(guess);
		res = get_bulls_cows();
		if (is_win(res)) {
			prn_win(round);
			endGame = true;
		}
		else {
			turn_off_impossible_code(candidates, MAX_CODE, guess, res);
			round++;
		}
		cout << endl;
		if (is_all_turned_off(candidates, MAX_CODE)) {
			prn_wrong_data();
			endGame = true;
		}
	}
	system("pause");
}

