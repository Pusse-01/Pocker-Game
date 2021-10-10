/*
NAME            :P.M.N.R.PUSSADENIYA	
INDEX NO        :19/ENG/082
SUBMISSION DATE :10/06/2021
PROGRAMMING QUEST 4
*/

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <algorithm>



using namespace std;



//Defining Class to store data about card
class Card {
public:
    int val;
    char suit;
    char clr;
    string printed;
    
    //Constructor and member functions
    Card();
    void init(int v, string p, char c,char col);
    static void initDeck(vector<Card>& deck);
    void showCard(); 
    static void showDeck(vector<Card>& deck);
};



//Initializing class for Dealer
//(Here it has initialized and the member functions are defined 
//after Player class since it is derived from Player class)
class Dealer;



//Defining Class to store data about player
class Player
{
private:
    int scoreCard;
	vector<Card>hand;	
public:
	Player();
	int score(); 
	void setHand(Card c);
	void showHand();
	bool straightChecker();
	int  findHighestCard();
	int pairChecker();
	bool checkFlush();
	bool straightFlushChecker();
	void clrHand();
	int  scoreCal();
	void scoreUpdate(int val);
	bool withdrawDecider(int val,Dealer d1,int size);
	void createTxt(ofstream& fin);
	static void plyrLogs(Player players[], int size, int val, Dealer d1);
};




//Defining the class for Dealer which is inherited from Player class
class Dealer :public Player {
public:
	void shuffling(Player* player,int size ,vector<Card>&deck);

};

/***********************************************************************
 * Defining members of Card class
 * **********************************************************************/

//Initializing (Constructor)
Card::Card() {
    val = 0;
    clr = ' ';
    suit = ' ';
    printed = "";
}

//Function to assign card vals
void Card::init(int v, string p, char c,char col) {
    val = v;
    printed = p;
    suit = c;
    clr = col;
}

//Function to initialize the card deck
void Card::initDeck(vector<Card>& deck) {
    deck.clear();
    Card temp;
    char col;

    //Suits
    char suits[5] = { 'H','D','S','C' };
    for (int i = 0; i < 4; i++) {

        //Assigning respective colour
        if (suits[i] == 'H' || suits[i] == 'D') {
            col = 'R';
        }
        else {
            col = 'B';
        }

        //Assigning suitable numerical vals for Ace, J, Q, K
        for (int j = 1; j <= 13; j++) {
            if (j == 1) {
                temp.init(14, "A", suits[i],col);
                deck.push_back(temp);
            }
            else if (j <= 10) {
                temp.init(j, to_string(j), suits[i],col);
                deck.push_back(temp);
            }
            else if (j == 11) {
                temp.init(j, "J", suits[i],col);
                deck.push_back(temp);
            }
            else if (j == 12) {
                temp.init(j, "Q", suits[i],col);
                deck.push_back(temp);
            }
            else if (j == 13) {
                temp.init(j, "K", suits[i],col); 
                deck.push_back(temp);
            }
        }

    }
}

//Function to output the card
void Card::showCard() {
    cout << suit << " - " << printed<<"\t";
}

//Function to show the card deck
void Card::showDeck(vector<Card>& deck) {
    for (int i = 0; i < deck.size(); i++) {
        deck[i].showCard();
    }
}


/************************************************************************
 * Defining members of Player class
 * **********************************************************************/

//Initializing (Constructor)
Player::Player() {
	hand.clear();
	scoreCard = 0;
}

//Function clears the vector
void Player::clrHand() {
	hand.clear();
}

//Function creates the Hand
void Player::setHand(Card c) {
	hand.push_back(c);
}

//Function to check the Flush---- Flush(color): Five cards with same color.
bool Player::checkFlush() {
	for (int i = 0; i < 4; i++) {
        //returns true if all the cards are in same color
		if (hand[i].clr != hand[i + 1].clr) { 
			return false;
			break;
		}
	}
	return true;
}

//Function to check straight---Straight: five cards with valuess in sequence
bool Player::straightChecker() {
	
    //Ordering in ascending
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5 - i - 1; j++)
			if (hand[j].val > hand[j+1].val) {
				swap(hand[j+1], hand[j]);
			}
	}	
	int count = 0;
	for (int i = 0; i < 5; i++) {
		if (i!=4&&hand[i].val ==i+2 ) {
			count++;
			
		}
		if (i == 4 && hand[i].val == 14) {
			count++;
		}
	}
	if (count == 5) {
		return true;
	}
	else {
		for (int i = 0; i < 4; i++) {
			if (hand[i + 1].val - hand[i].val != 1) {
				return false;
			}
		}
		return true;
	}
}


//Function to check straight flush--- Straight Flush: Straight in which the cards have the same suite.
bool Player::straightFlushChecker() {
	if (straightChecker() == true) {
		for (int i = 0; i < 4; i++) {
			if (hand[i].suit != hand[i + 1].suit) {
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

//Function to check pairs
int	 Player::pairChecker() {
	vector<int>seek;
	seek.clear();
	int count = 1, max = 0,twos=0,fourKind=0;//Four of a kind: four cards with same value
	for (int i = 0; i < 4; i++) {
		count = 1;
		if (seek.size() == 0 || find(seek.begin(), seek.end(), hand[i].val) == seek.end()) {
			for (int j = i + 1; j < 5; j++) {
				if (hand[i].val == hand[j].val) {
					count++;
				}
			}
			seek.push_back(hand[i]. val);
		}
        //count == 2: then theres a pair
		if (count == 2) {
			twos++;
		}
		if (max < count) {
			max = count;
		}
        //count == 4: then theres a Four of a kind
		if (count == 4) {
			fourKind++;
		}		
	}

    //return 8 if there's a Four of a kind
	if (fourKind > 0) {
		return 8;
	}
    //return 7 if there's a Full house: three of a kind and one pair. (combination 2 and combination 3)
	else if (max == 3 && twos == 1) {
		return 7;
	}
    //return 4 if there's a Three of a kind: Three cards with same value
	else if (max == 3) {
		return 4;
	}
    //return 3 if there's a Two pairs: two times two cards of same value.
	else if (twos == 2) {
		return 3;
	}
    //return 2 if there's a One pair: two cards with the same value.
	else if (twos == 1) {
		return 2;
	}
    //else return 1
	else {
		return 1;
	}
}

//Function to assign a score based on the combination it can form
int Player::score() {
	if (straightFlushChecker() == true) {
		return 9;
	}
	else if (pairChecker()==8) {
		return 8;
	}
	else if (pairChecker() == 7) {
		return 7;
	}
	else if (checkFlush() == true) {
		return 6;
	}
	else if (straightChecker() == true) {
		return 5;
	}
	else if (pairChecker() == 4) {
		return 4;
	}
	else if (pairChecker() == 3) {
		return 3;
	}
	else if (pairChecker() == 2) {
		return 2;
	}
	else {
		return 1;
	}	
}

//Function to get the High Card ---- 
//High card: None of the following combinations and highest card value is considered 
int Player::findHighestCard() { 
	int max = 0;
	for (int i = 0; i < 5; i++) {
		if (hand[i].val > max) {
			max = hand[i].val;
		}
	}
	return max;
}

//Function to get the score of a player
int  Player::scoreCal() { 
	return scoreCard;
}

//Function to uupdate the score
void Player::scoreUpdate(int val) {
	scoreCard += val;
}

//Creating the log file
void Player::createTxt(ofstream& fin) {
	fin << "\n";
	for (int i = 0; i < 5; i++) {
		fin  << hand[i].suit << "-" << hand[i].printed << " ";
	}
	fin << "\n";
}

//Function to decide whether player is playing or withdrawing
bool Player::withdrawDecider(int val,Dealer d1,int size) {
	if (size > 1) { 
		if (val > score()) {
			return false;
		}
		else if (val == score()) {
			if (findHighestCard() <= d1.findHighestCard()) {
				return false;
			}
		}
		else {
			int k = rand() % 2;
			if (k == 1) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		return true;
	}
}

//Function to write logs in proper manner
void Player::plyrLogs(Player players[], int size, int val, Dealer d1) {
	ofstream log("logs_file.txt",ios ::app);	
	log << "\nDealer's cards ";
	d1.createTxt(log);
	log << "\nCards of the player\n";
	for (int i = 0; i < size; i++) {
		log << "Player " << i + 1;
		players[i].createTxt(log);
	}
	string combination[9] = { 
        "High Card",
        "One pair",
        "Two pairs",
        "Three of Kind",
        "Straight",
        "Flush",
        "Full House",
        "Four of Kind",
        "Straight Flush" 
        };
	vector<int>playedTimes, plyrI,sameCombo,findHighestCardval;
	plyrI.clear();
	playedTimes.clear();
	sameCombo.clear();
	findHighestCardval.clear();
	for (int i = 0; i < size; i++) {
		if (players[i].withdrawDecider(val,d1,size) == true) {
			cout << "Player " << i + 1 << " played with : " << combination[players[i].score() - 1] << endl;
			log << "Player " << i + 1 << " played with : " << combination[players[i].score() - 1] << endl;
			plyrI.push_back(i);//push to relavent vectors
			playedTimes.push_back(players[i].score());
			
		}
		else {
			cout << "Player " << i + 1 << " Withdrawed" << endl;//else log player wthdrawn
			log << "Player " << i + 1 << " Withdrawed" << endl;
		}
	}
	cout << endl;
	cout << endl;
	if (plyrI.size() > 0) {
		int max = *max_element(playedTimes.begin(), playedTimes.end());
		for (int i = 0; i < plyrI.size(); i++) {
			if (playedTimes[i] == max) { 
				cout << "Player " << plyrI[i]+1 << ", " << endl;;
				sameCombo.push_back(plyrI[i]);
				findHighestCardval.push_back(players[plyrI[i]].findHighestCard());
			}
		}
		int winCount = 0, winI, dealersHighCard=0, count=0;
		if (sameCombo.size() > 1) {
			cout << "All have the high combination" << endl;


			if (d1.score() == max) {
				cout << "Dealer have the same combination" << endl;
				cout << "Dealer :"; d1.showHand();
				dealersHighCard = d1.findHighestCard();
				

			}
			for (int i = 0; i < sameCombo.size(); i++) {
				cout << "Player " << sameCombo[i] + 1 << " :"; 
				players[sameCombo[i]].showHand();
			}
			for (int i = 0; i < findHighestCardval.size(); i++) {
				if (winCount < findHighestCardval[i]) {
					winCount = findHighestCardval[i];
					winI = sameCombo[i];
				}
				else if (winCount == findHighestCardval[i]) {
					count++;
				}
			}
			string wonCardval;
            //Assigning card values
			if (winCount == 14) {
				wonCardval = "A";
			}
			else if (winCount == 13) {
				wonCardval = "K";
			}
			else if (winCount == 12) {
				wonCardval = "Q";
			}
			else if (winCount == 10) {
				wonCardval = "J";
			}
			else {
				wonCardval = to_string(winCount);
			}
			log << "\n";
			if (dealersHighCard < winCount&&count==0) {
				cout << "Player " << winI + 1 << " won by High Card with value :" << wonCardval << endl;
				log << "\nPlayer " << winI + 1 << " won by High Card with value :" << wonCardval << endl;
				players[winI].scoreUpdate(100);
				for (int i = 0; i < plyrI.size(); i++) {
					if (plyrI[i] != winI) {
						players[i].scoreUpdate(-1);
					}
				}
			}
            // if dealer highest Card == player's highest Card then game is draw
			else if ((dealersHighCard == winCount)||count>0) {
				cout << "Round drawen" << endl;
			}
            //Else dealer is winnign
			else {
				cout << "Dealer won the round" << endl;
				log << "Dealer won the round" << endl;
			}
		}
		else if (sameCombo.size() == 1) {
			players[sameCombo[0]].showHand();
			if (d1.score() > players[sameCombo[0]].score()) {
				cout << "dealer wins the game" << endl;
				log << "dealer wins the game" << endl;
				players[sameCombo[0]].scoreUpdate(-1);
			}

			else if (d1.score() == players[sameCombo[0]].score()) {
				if(d1.findHighestCard() > players[sameCombo[0]].findHighestCard()) {
					cout << "dealer wins the game" << endl;
					log << "dealer wins the game" << endl;
					players[sameCombo[0]].scoreUpdate(-1);
				}
				else {
					cout << "Player "<<sameCombo[0] + 1 <<" wins with High Card :";
					log<< "Player " << sameCombo[0] + 1 << " wins with High Card :";
					if (players[sameCombo[0]].findHighestCard() == 14) {
						cout << "A" << endl;
					}
					else if (players[sameCombo[0]].findHighestCard() == 13) {
						cout << "K" << endl;
						log << "K" << endl;
					}
					else if (players[sameCombo[0]].findHighestCard() == 12) {
						cout << "Q" << endl;
						log << "Q" << endl;
					}
					else if (players[sameCombo[0]].findHighestCard() == 10) {
						cout << "J" << endl;
						log << "J" << endl;
					}
					else {
						cout << players[sameCombo[0]].findHighestCard() << endl;
						log << players[sameCombo[0]].findHighestCard() << endl;
					}
					players[sameCombo[0]].scoreUpdate(100);//update score 
				}
			}
			else {
				cout << "Player " << sameCombo[0] + 1 << " won by  " << combination[players[sameCombo[0]].score() - 1] << endl;
				log << "Player " << sameCombo[0] + 1 << " won by  " << combination[players[sameCombo[0]].score() - 1] << endl;
				players[sameCombo[0]].scoreUpdate(100);//update the score 
			}
		}
		
	}
	
	else {
		cout << "No one Played this round" << endl;
		log << "No one Played this round" << endl;
	}
	log.close();
}

//Function to print the hand 
void Player::showHand() { 
	for (int i = 0; i < hand.size(); i++) {
		hand[i].showCard();
	}
	cout << endl;
}



/************************************************************************
 * Defining members of Dealer class
 * **********************************************************************/
 
//fFunction to shuffle the deck and distribute cards among the players
void Dealer::shuffling(Player* player,int size, vector<Card>&deck) {
	//to generate new seed
	unsigned seed = chrono::system_clock::now().time_since_epoch().count(); 
	shuffle(begin(deck), end(deck), std::default_random_engine(seed));
	//removes first 2 cards
    deck.erase(deck.begin()); 
	deck.erase(deck.begin());
	clrHand();
	
	for (int i = 0; i < size; i++) {
		//distributing 5 cards per each player
        player[i].clrHand(); 
		for (int j = 0; j <5; j++) {
			player[i].setHand(deck[0]);
			deck.erase(deck.begin());
		}
	}
	for (int i = 0; i < 5; i++) {
		setHand(deck[0]);
		deck.erase(deck.begin());
	}
}





/************************************************************************
 * main function
 * **********************************************************************/

int main()
{
    //Initializing deck
    vector<Card>deck;
  
    int nuOfRounds, players, scoreVal;
    string combinations[9] = { 
        "High Card",
        "One pair",
        "Two pairs",
        "Three of Kind",
        "Straight",
        "checkFlush",
        "Full House",
        "Four of Kind",
        "Straight Flush"
         };

    Dealer d1;
    cout << "\n-------------------------------- Poker Card Game --------------------------------" << endl;
    cout << "\nEnter the number of players except Dealer(Upto 9) :";
    //taking input
    cin >> players;
    while (players < 0 || players>9) {
        cout << "Invalid Input Please enter again" << endl;
        cin >> players;
    }
    //create array of players objects
    Player* player = new Player[players];
    cout << "\nEnter the total number of rounds :";
    cin >> nuOfRounds;
    int dealerval;
    int totScore = 0;
    int init = 0;
    while (nuOfRounds > 0) {
        totScore = 100 * (init+1);
        
        system("cls");
        cout << "\n-------------------------------- Poker Card Game --------------------------------" << endl;
        cout << endl;
        cout << endl;
        Card::initDeck(deck); 
        d1.shuffling(player, players, deck);
        cout << "  Dealer's hand: " ;
        d1.showHand();
        cout<<endl;
        cout<<endl;
        cout << "Combination which dealer has :" << combinations[d1.score()-1] << endl;
        dealerval = d1.score();
        cout<<endl;
        cout<<endl;
        Player::plyrLogs(player, players,dealerval,d1);
        nuOfRounds--;
        init ++ ;
        cout << endl;
        cout << "----------Score Card----------" << endl;
        cout << endl;
        for (int i = 0; i < players; i++) {
            scoreVal = player[i].scoreCal(); 
            cout << "Player " << i + 1 << " : " << scoreVal<<"  |";
            float prob = float(player[i].scoreCal())/totScore;
            if (scoreVal > 0) {
                cout << " Winning Probability : " << prob * 100 << "%" << endl;
            }
            else {
                cout << " Winning Probability : " << 0 << "%" << endl;
            }
            cout << endl;
        }
        system("pause");
        ofstream logg("results.txt", ios::app);
        system("cls");
        cout << "\n-------------------------------- Poker Card Game --------------------------------" << endl;
        cout << endl;
        cout << "  Score Card" << endl;
        cout << endl;
        logg << "\n\nScore Card" << endl;
        for (int i = 0; i < players; i++) {
            scoreVal = player[i].scoreCal();
            
            cout << "Player " << i + 1 << " : " << scoreVal;
            logg << "Player " << i + 1 << " : " << scoreVal <<endl;
            cout << endl;
        }    
    }   
    return 0;
}
