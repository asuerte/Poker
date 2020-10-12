#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>

/* handy typedefs */
typedef unsigned char card;
typedef unsigned char pairs;

/* arrays for the names of things */
static char *suits[] = {"Hearts","Diamonds","Clubs","Spades"};
static char *values[]= {"Ace","Two","Three","Four","Five","Six",\
                        "Seven","Eight","Nine","Ten","Jack",\
                        "Queen","King"};
static char *colour[]= {"Black","Red"};

/* function prototypes */
void printcard(card c); /* Displays the value of a card*/

void printdeck(card deck[52]); /* prints an entire deck of cards*/

void filldeck(card deck[52]); /* Populates a deck of cards */

void shuffle(card deck[52]); /* Randomizes the order of cards */

int compareface(const void* c1,const void *c2); 
/* compares the face value of 2 cards, suitable to pass to qsort
   as the fourth argument */

pairs findpairs(card *hand); /* finds any pairs in a hand */

int main()
{
  card deck[52],*deckp;
  card hands[5][5],handssorted[5][5];
  pairs numpairs[5],highest;
  int hand,cd,winner;
  
  srand(time(NULL));       /* seed the random number generator */

  /*populate and shuffle the deck */
  filldeck(deck);
  printdeck(deck);
  shuffle(deck);
  printdeck(deck);
  
  int i = 0;
  for(cd=0;cd<5;cd++)
  {
     for(hand=0;hand<5;hand++)
     {
     	hands[cd][hand] = deck[i++];
     	handssorted[cd][hand] = hands[cd][hand];	
        /* deal the hands here */
     }
  }
  
  int inc;
  char numOfPairsMask = 0x03;	// 0000 0011
  char valuesMask = 0xF0;	    // 1111 0000
  char resultNum = 0x00;	    // variable to get the number of pairs
  char resultValue = 0x00;
  char vals [13] = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,0x90, 0xA0, 0xB0, 0xC0};   // reversed array
 
  for(hand=0;hand<5;hand++)
  {
     qsort(handssorted[hand], 5, sizeof(char), compareface);	// hands are sorted in order using qsort
     numpairs[hand]=findpairs(handssorted[hand]);
     printf("\nHand %i:\n",hand+1);
     for (inc = 0; inc < 5; inc++)
     {
     	printcard(handssorted[hand][inc]);  // print the hands here 
	 }
	 
	 resultNum = numOfPairsMask & numpairs[hand];
	 if (resultNum == 0x00) {
		printf("Number of pairs: 0\n");
	 } else if (resultNum == 0x01) {
		printf("Number of pairs: 1\n");	 	
	 } else if (resultNum == 0x02) {
	 	printf("Number of pairs: 2\n");
	 } // print the number of pairs
	 
	 int i;
	 resultValue = valuesMask & numpairs[hand];  // resultValue = 1111 0000 & .... ....	 
     for (i=0; i<13 ; i++) {
     	if (resultValue == vals[i] && resultValue != 0x00) {	
     		 printf("Highest pair is: %s\n", values[i]);
		 }
	 } // print the highest value pair
	 resultValue = 0x00;
	 resultNum = 0x00;
  }
  
  /* Determine the winner and print it */
  char handValue[5];
  int j,k,m,n,l;
  for (j=0; j<5; j++) {
	  handValue[j] = valuesMask & numpairs[j];  // resultValue = 1111 0000 & .... ....
  }   
  int steps[5];
  for (k=0; k<5; k++) {
  	  for (m=0; m<13; m++) {
  	  	if (handValue[k] == vals[m]) {
  	  		steps[k]= m;	
		}
  	  }
  }
  int largest = steps[0];
  for (n = 1; n < 5; n++) 
  {
  	if (largest < steps[n]) {
	  largest = steps[n];
	}
  }  
  int winningHand = 0;
  int test[2] = {-1, -1}, c=0;
  for (l=0; l<5; l++) 
  {
  	if (steps[l] == largest) {
  		test[c++] = l;
  		winningHand = l;
	}
  }
  if (test[0] != -1 && test[1] != -1) {
  	printf("\nDrawn game"); 
  } else {
  	printf("\nWinner is hand %d with a pair of %s", winningHand+1, values[largest]);  
  }
  return 0;
}

pairs findpairs(card *hand)
{
   pairs numpairs=0;  
   char valueMask = 0x78; //0111 1000
   char vals [13] = {0x00, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08};
   char numOfPairsMask[2] = {0x01, 0x02};
   char eachCardVal[5];	// contains each of the 5 cards value in each hand
   int i,j,k, countPairs = 0;
   
   for (i=0; i<5; i++) {
		eachCardVal[i] = hand[i] & valueMask; // ...... & 0111 1000	
		eachCardVal[i] <<= 1;	// 0111 1000 <<= 1111 0000	
   }
   
   int inc = 0;
   card compareCards[2];
   for (j=0; j<5; j++) {
	   for (k=0; k<5; k++) {
   	   		if (eachCardVal[j] == eachCardVal[k] && j != k && k < j) {
				countPairs++;
				compareCards[inc++] = eachCardVal[j];
			}
     	}
   }  // find the number of pairs, and when finding a pair store the value into an array

   if (countPairs == 0) {
   		numpairs = 0x00;
   } else if (countPairs == 1) {
		numpairs |= numOfPairsMask[0];
   } else if (countPairs == 2) {
		numpairs |= numOfPairsMask[1];
   } // store the number of pairs in numpairs

   if (countPairs == 0) {
   		numpairs = 0x00;
   } else if (countPairs == 1) {
   		numpairs |= compareCards[0];
   } else if (countPairs == 2) {
   		qsort(compareCards, 2, sizeof(char), compareface);	
   		numpairs |= compareCards[1];
   } // finding the highest pair
   return numpairs;
}

void filldeck(card deck[52])
{
	int i,j, inc=0;
	char hCard = 0x00, dCard = 0x00, cCard = 0x00, sCard = 0x00;
	char hearts= 0x01, diamonds = 0x03, clubs = 0x04, spades = 0x06; 
	char vals [13] = {0x00, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08};

	for (j=0; j<4; j++)
	{
		for (i=0; i<13; i++)
		{
			if (inc < 13) {
				hCard |= hearts;
				hCard |= vals[i];
				deck[inc] = hCard;
				hCard &= hearts;	
				inc++;
			}
			else if (inc > 12 && inc < 26) {
				dCard |= diamonds;
				dCard |= vals[i];
				deck[inc] = dCard;
				dCard &= diamonds;	
				inc++;
			}
			else if (inc > 25 && inc < 39) {
				cCard |= clubs;
				cCard |= vals[i];
				deck[inc] = cCard;
				cCard &= clubs;	
				inc++;
			}	
			else if (inc > 38 && inc < 52) {
				sCard |= spades;
				sCard |= vals[i];
				deck[inc] = sCard;
				sCard &= spades;	
				inc++;
			}		
		}
	}
   return;
}

void printdeck(card deck[52])
{
   int i;
   for(i=0;i<52;i++)
      printcard(deck[i]);
   return;
}

void printcard(card c)
{
	char colourMask = 0x01; //0000 0001
	char suitMask = 0x06; //0000 0110
	char valueMask = 0x78; //0111 1000
	
	char colourTest = c & colourMask; 	     //  x & 0000 0001 = ....
	char suitTest = c & suitMask;            //  x & 0000 0110 = ....		
	char valueTest = (c & valueMask) >> 3;   //  x & 0111 1000 = ....
	
	char vals [13] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C};
	char suit[4] = {0x00, 0x02, 0x04, 0x06};	
	char col[2] = {0x00, 0x01};
	char printingCard[50];
	int i,j,k;
	
	for (i=0; i<13; i++)
	{
		if (valueTest == vals[i])
		{
			strcpy(printingCard, "\t");	
			strcat(printingCard, values[i]);	
			strcat(printingCard, " of ");	
		}
	}
	for (j=0; j<4; j++)
	{
		if (suitTest == suit[j])
		{	
			strcat(printingCard, suits[j]);	
			strcat(printingCard, ", is ");	
		}
	}	
	for (k=0; k<2; k++)
	{
		if (colourTest == col[k])
		{	
			strcat(printingCard, colour[k]);	
			strcat(printingCard, ".");	
		}
	}
	printf("%s\n", printingCard);
									
   return;
}

void shuffle(card deck[52])
{
   printf("\nBELOW IS THE SHUFFLED DECK\n");
   int i,j,d,h,rnd;
   card c;
   int used[52];
   card shuffledDeck[52];
   for(j=0;j<52;j++)
   {
   		used[j] = -1;
   }
   
   int dup = 1;
   for(i=0;i<52;i++)
   {
      do {
	    rnd=rand() * 52.0 / RAND_MAX;
		dup = 0; 
	    for (d=0; d<52; d++)
		{
			if (used[d] == rnd) {
				dup = 1;
			}
		}
	  } while (dup == 1);
	  shuffledDeck[i] = deck[rnd];
	  used[i] = rnd;
   }

   for(h=0;h<52;h++)
   {
	  deck[h] = shuffledDeck[h];
   }
   return;
}

int compareface(const void* c1, const void *c2)
{
/* This function extracts the two cards face values
   and returns 1 if cd1 > cd2, 0 if cd1 == cd2, and
   -1 otherwise. The weird argument types are for
   compatibility with qsort(), the first two lines
   decode the arguments back into "card".
   */
    card cd1,cd2;
    
    cd1=*((card*) c1);
    cd2=*((card*) c2);
    
    cd1= (cd1&0x78)>>3;
    cd2= (cd2&0x78)>>3;
    
    if(cd1>cd2)
      return 1;
    if(cd1==cd2)
      return 0;
      
    return -1;
}
/* Left the out the part where 3 same cards in each hand get considered as 1 pair and 4 same cards gets counted at 2 pairs */
/* Left out the part where the highest pair is aces in each hand doesn't get printed out*/
