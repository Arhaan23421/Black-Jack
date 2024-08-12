#include <iostream>
#include <stdexcept>
#include <vector>
#include <random>

using namespace std;

class Card
{
public:
  Card(int s, int r){
    if(s>0 && s<=4){suit = s;}
    else{throw invalid_argument("Invalid Suit");}
    if(r>=2 && r<=14){rank = r;}
    else{throw invalid_argument("Invalid Rank");}
  }

  int getRank()
  {
    return rank;
  }

  string getSuit()
  {
    switch (suit) {
      case 1: return "spades";
      case 2: return "clubs";
      case 3: return "diamonds";
      case 4: return "hearts";
      case 5: return "joker";
      default: throw domain_error("Invalid Suit");
    }
  }

  string getSymbol()
  {
    if(suit == 1)
    {
      return newName("♠");
    }
    else if(suit == 2)
    {
      return newName("♣");
    }
    else if(suit == 3)
    { 
      return newName("♦");
    }
    else if( suit == 4)
    {
      return newName("♥");
    }
    else
    {
      return "joker";
    }
  }

  int calculateNum()
  {
    if(rank<10)
    {
      return rank;
    }
    else if(rank == 14)
    {
      return 11;
    }
    else
    {
      return 10;
    }
  }

private:
  int suit;
  int rank;
  string newName(string symbol)
  {
    if (rank == 10)
    {
      return "T" + symbol;
    }
    else if(rank == 11)
    {
      return "J" + symbol;
    }
    else if(rank == 12)
    {
      return "Q" + symbol;
    }
    else if(rank == 13)
    {
      return "K" + symbol;
    }
    else if(rank == 14)
    {
      return "A" + symbol;
    }
    else
    {
      return to_string(rank) + symbol;
    }
  }
};

/*
  Access via [] operator (same as arrays)
  Add an element to the vector via `push_back` method
  Remove the element at the end via `pop_back`
  Access the size via `size` method
*/

class Deck
{
public:

  int totalCards()
  {
    return cards.size();
  }

  Card* drawACard()
  {
    Card* temp = cards[cards.size()-1];
    cards.pop_back();
    return temp;
  }

  void fillDeck()
  {
    cards.clear();
    for(int i=1;i<= 4;i++)
    {
      for(int c=2;c<=14;c++)
      {
        cards.push_back(new Card(i,c));
      }
    }
  }

  string stringDeck()
  {
    string result = "";
    for(int i=0;i<cards.size();i++)
    {
      result += ", " + cards[i]->getSymbol();
    }
    return result;
  }

  void shuffle()
  {
    random_device rd;
    mt19937 gen{rd()};
    uniform_int_distribution<> dist{0, (int)(cards.size())-1};
    for(int i=0;i<cards.size();i++)
    {
      int rand = dist(gen);
      Card* temp;
      temp = cards[i];
      cards[i] = cards[rand];
      cards[rand] = temp;
    }
  }


private:
  vector<Card *> cards;
};


void displayCards(vector<Card *> playerCards, vector<Card *> dealerCards, bool showDealer)
{
  string s = "";
  for(int i=0; i<dealerCards.size(); i++)
  {
    s += "┌──┐";
  }
  s += "\n";
  for(int i=0; i<dealerCards.size(); i++)
  {
    if(showDealer)
    {
      s += "│" + dealerCards[i]->getSymbol() +"│" ;
    }
    else
    {
      s+= "│? │" ;
      showDealer =!showDealer;
    }
  }
  s += "\n";
  for(int i=0; i<dealerCards.size(); i++)
  {
    s += "└──┘";
  }
  cout << "These are the dealer's cards:\n" + s << endl;


  string dc = "";
  for(int i=0; i<playerCards.size(); i++)
  {
    dc+= "┌──┐";
  }
  dc+= "\n";
  for(int i=0; i<playerCards.size(); i++)
  {
    dc+= "│" + playerCards[i]->getSymbol() +"│" ;
  }
  dc+= "\n";
  for(int i=0; i<playerCards.size(); i++)
  {
    dc+= "└──┘";
  }
  cout << "These are the player cards:\n" + dc << endl;
}



int calculateSum(vector <Card*> sample)
{
  int sum = 0;
  for(int i = 0; i < sample.size(); i++)
    {
      sum += sample[i]->calculateNum();
    }
  return sum;
}

int main() {
  Deck d;
  vector<Card*> playerCards;
  vector<Card*> dealerCards;
  bool playAgain = true;

  while(playAgain == true)
  {
    d.fillDeck();
    d.shuffle();
    playerCards.clear();
    playerCards.push_back(d.drawACard());
    playerCards.push_back(d.drawACard());
    dealerCards.clear();
    dealerCards.push_back(d.drawACard());
    dealerCards.push_back(d.drawACard());

    string stor;
    // Player Turn
    displayCards(playerCards, dealerCards, false);
    if (calculateSum(playerCards) < 21)
    {
      cout << " do you want another card?" << endl;
      getline(cin, stor);

      while(stor.compare("yes") == 0)
      {
        playerCards.push_back(d.drawACard());
        displayCards(playerCards, dealerCards, false);
        if(calculateSum(playerCards) > 21)
        {
          cout << " your sum is over 21, you bust" << endl;
          break;
        }
        if(calculateSum(playerCards) == 21)
          {
            cout << " your sum is 21!" << endl;
            break;
          }
        cout << " do you want another card?" << endl;
        getline(cin, stor);
      }
    }
    else
    {
      cout << "Blackjack!" << endl;
    }

    cout << "Press Enter to continue...";
    getline(cin, stor);

    // Dealer Turn
    displayCards(playerCards, dealerCards, true);
    while (calculateSum(dealerCards) <= 16)
    {
      dealerCards.push_back(d.drawACard());
      cout<< " dealer has drawn another card"<<endl;
      displayCards(playerCards, dealerCards, true);
      cout << "Press Enter to continue...";
      getline(cin, stor);
    }
    if( calculateSum(dealerCards)>21)
    {
      cout<< "dealer has busted"<< endl;
    }
    else if( calculateSum(dealerCards)==21)
    {
      cout<< "dealer got blackjack!"<< endl;
    }
    else
    {
      cout<< " dealer stays" << endl;
    }

    cout << "Press Enter to continue...";
    getline(cin, stor);


    // End of game
    if(calculateSum(dealerCards)>21 && calculateSum(playerCards)<=21)
    {
      cout << " you win!" << endl;
    }
    else if(calculateSum(dealerCards)<=21 && calculateSum(playerCards)>21)
    {
      cout<< " you loose!"<< endl;
    }
    else if(calculateSum(dealerCards)>21 && calculateSum(playerCards)>21)
    {
      cout<< " you tie!" << endl;
    }
    else if(calculateSum(dealerCards)==21 && calculateSum(playerCards)==21)
    {
      cout << " you both got blackjack!" << endl;
    }
    else if(calculateSum(dealerCards) > calculateSum(playerCards))
    {
      cout << " you loose" << endl;
    }
    else if(calculateSum(playerCards) > calculateSum(dealerCards))
    {
      cout << " you win" << endl;
    }
    else
    {
      cout << " you tied" << endl;
    }

    string ss = "";
    cout << " do you want to play again?";
    getline(cin, ss);
    if( ss.compare("no") == 0)
    {
      cout<< " thanks for playing!" << endl;
      playAgain=false;
    }
  }
}

/*
  Future Features:
    * Add betting
    * Add record (wins/loses)
    * Improve user interface
*/
