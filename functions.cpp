#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>      // For isalpha and toupper functions
#include <algorithm>   // For std::find
using namespace std;

// Function to display the current state of the word
void displayWord(const string& word, const vector<bool>& guessed) {
	for (size_t i = 0; i < word.size(); ++i) {
		if (guessed[i]) {
			cout << word[i] << " ";
		} else {
			cout << "_ ";
		}
	}
	cout << endl;
}

// Function to check if the word has been completely guessed
bool isWordGuessed(const vector<bool>& guessed) {
	for (bool isGuessed : guessed) {
		if (!isGuessed) {
			return false;
		}
	}
	return true;
}

// Function to display a simple hangman figure
void displayHangman(int incorrectGuesses) {
	const vector<string> hangmanStages = {
		"  +---+\n      |\n      |\n      |\n     ===",
		"  +---+\n  O   |\n      |\n      |\n     ===",
		"  +---+\n  O   |\n  |   |\n      |\n     ===",
		"  +---+\n  O   |\n /|   |\n      |\n     ===",
		"  +---+\n  O   |\n /|\\  |\n      |\n     ===",
		"  +---+\n  O   |\n /|\\  |\n /    |\n     ===",
		"  +---+\n  O   |\n /|\\  |\n / \\  |\n     ==="
	};
	cout << hangmanStages[incorrectGuesses] << endl;
}

// Function to set the word length range based on difficulty
pair<int, int> getWordLengthRange(const string& difficulty) {
	if (difficulty == "easy") return {3, 5};     // Short words
	if (difficulty == "medium") return {4, 7};  // Medium-length words
	if (difficulty == "hard") return {6, 10};    // Longer words
	if (difficulty == "extreme") return {8, 15}; // Very long words
	return {4, 7}; // Default to medium if unknown
}

int main() {
	// List of word categories and their hints
	vector<pair<string, vector<string>>> categories = {
		{"Body Part", {"EAR", "NOSE", "EYE", "HEART", "LEG", "ARM", "LUNG", "LIVER", "BRAIN", "KNEE", "ANKLE", "TOE", "THUMB", "FINGER", "WRIST"}},
		{"Animal", {"DOG", "CAT", "HORSE", "LION", "TIGER", "ZEBRA", "WHALE", "SHARK", "FROG", "BEAR", "ELEPHANT", "GIRAFFE", "KANGAROO", "PANDA", "SNAKE"}},
		{"Fruit", {"APPLE", "BANANA", "ORANGE", "GRAPE", "MANGO", "PEACH", "PEAR", "PLUM", "KIWI", "PAPAYA", "PINEAPPLE", "CHERRY", "WATERMELON", "STRAWBERRY", "BLACKBERRY"}},
		{"Vegetable", {"CARROT", "POTATO", "TOMATO", "BROCCOLI", "SPINACH", "CABBAGE", "LETTUCE", "ONION", "PEPPER", "GARLIC", "CUCUMBER", "ZUCCHINI", "EGGPLANT", "RADISH", "BEAN"}},
		{"Color", {"RED", "BLUE", "GREEN", "YELLOW", "PURPLE", "ORANGE", "PINK", "BROWN", "BLACK", "WHITE", "GREY", "CYAN", "MAGENTA", "VIOLET", "AQUA"}},
		{"Profession", {"DOCTOR", "TEACHER", "ENGINEER", "NURSE", "FARMER", "ARTIST", "LAWYER", "CHEF", "POLICE", "SCIENTIST", "ACCOUNTANT", "MECHANIC", "FIREFIGHTER", "PILOT", "PROGRAMMER"}},
		{"Clothes", {"SHIRT", "PANTS", "JACKET", "DRESS", "SKIRT", "SHORTS", "SOCKS", "SHOES", "HAT", "SCARF", "GLOVES", "BELT", "TIE", "SWIMSUIT", "COAT"}}
	};

	srand(time(0));
	int categoryIndex = rand() % categories.size();
	string hint = categories[categoryIndex].first;
	vector<string> words = categories[categoryIndex].second;

	// Get difficulty level from the user
	string difficulty;
	cout << "Choose a difficulty level (easy, medium, hard, extreme): ";
	cin >> difficulty;

	// Get word length range based on difficulty
	auto [minLength, maxLength] = getWordLengthRange(difficulty);

	// Filter words based on selected difficulty
	vector<string> filteredWords;
	for (const string& w : words) {
		if (w.length() >= minLength && w.length() <= maxLength) {
			filteredWords.push_back(w);
		}
	}

	// If no words match the criteria, select from all words
	if (filteredWords.empty()) {
		filteredWords = words;
	}

	string word = filteredWords[rand() % filteredWords.size()];  // Randomly choose a word from the filtered category

	// Initialize guessed positions
	vector<bool> guessed(word.size(), false);
	int incorrectGuesses = 0;

	// Initialize score
	int score = 0;
	vector<char> incorrectLetters;

	cout << "===================== HANGMAN GAME =====================" << endl;
	cout << "RULES: Guess the letters of the word and save yourself! " << endl;
	cout << "========================================================" << endl;

	while (incorrectGuesses < 6) { // Maximum of 6 incorrect guesses
		displayHangman(incorrectGuesses);
		cout << "Word: ";
		displayWord(word, guessed);
		cout << "HINT: " << hint << endl;

		char guess;
		cout << "Enter a letter: ";
		cin >> guess;
		guess = toupper(guess);  // Convert to uppercase for consistency

		// Validate input
		if (!isalpha(guess)) {
			cout << "Invalid input. Please enter a letter." << endl;
			continue;
		}

		bool found = false;
		bool alreadyGuessed = false;
		for (size_t i = 0; i < word.size(); ++i) {
			if (word[i] == guess) {
				if (guessed[i]) {
					alreadyGuessed = true;
				} else {
					guessed[i] = true;
					found = true;
				}
			}
		}

		if (alreadyGuessed) {
			cout << "You already guessed that letter correctly!\n";
		} else if (found) {
			cout << "Correct! You guessed a letter. Keep going!\n";
			score += 10; // Increment score for a correct guess
		} else {
			if (std::find(incorrectLetters.begin(), incorrectLetters.end(), guess) == incorrectLetters.end()) {
				incorrectLetters.push_back(guess);
				++incorrectGuesses;
				cout << "Incorrect guess! You have " << (6 - incorrectGuesses) << " guesses left.\n";
			} else {
				cout << "You've already guessed that letter incorrectly.\n";
			}
		}

		cout << "Incorrect guesses so far: ";
		for (char c : incorrectLetters) {
			cout << c << " ";
		}
		cout << endl;

		if (isWordGuessed(guessed)) {
			cout << "\nCongratulations! YOU ARE SAFE :)" << endl;
			cout << "Your score: " << score << endl; // Display score when the player wins
			break;
		}
	}

	if (incorrectGuesses == 6) {
		displayHangman(incorrectGuesses);
		cout << "\nYou've run out of guesses! The correct word was: " << word << endl;
		cout << "\n";
		cout << "             YOU ARE DEAD HAHAHAHA!!!!      " << endl;
		cout << "\n";
		cout << "Your score: " << score << endl; // Display score when the player loses
	}

	return 0;
}

