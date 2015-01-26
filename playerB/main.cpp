
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include<cmath>
#include <algorithm>
 #include <unistd.h>
using namespace std;

#define PORT 32000   //The port on which to listen for incoming data
vector<string> deck;
vector<int> deckNo;
vector<int> aliceCard;
vector<int> BobCard;
vector<int>EncryptedDeck;
vector<int>decDeck;
vector<int>AliceBobEncryptedDeck;
vector<int>BobDecryptedDeck;
vector<int>AliceDecryptedDeck;
vector<int>AliceBobDecryptedDeck;

string suit[] = {"Diamonds", "Hearts", "Spades", "Clubs"};
string facevalue[] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

int EncryptedDeckArr[52];
string deckArr[52];
int BobCardArr[5];
int AliceBobEncryptedDeckArr[5];
int AliceBobDecryptedDeckArr[5];


// --- Generate cards
void generateCard()
{
    int i;
    int j;
    int k=1;

    for(i=0;i<13;i++)
    {
        for(j=0;j<4;j++)
        {
            string s;
            s.append(facevalue[i]);
            s.append("of");
            s.append(suit[j]);
            deck.push_back(s);
        }
    }

    for(int m=48 ; m<=99;m++)
    {
        deckNo.push_back(m);
    }

}

//-- calculate GCD
int gcd(int a , int b)
{
    int c;
      while ( a != 0 )
      {
         c = a;
         a = b%a;
         b = c;
      }
    return b;
}

//-- perform encryption and decryption of cards
int modPower(int msg,int key ,int primeNo)
{

    int y=1;
    int u = msg%primeNo;
    while(key!=0)
    {
        if(key%2==1)
        {
            y= (y*u)%primeNo;
        }
        key = floor(key/2);
        u = (u*u)%primeNo;
    }
    return y;
}


//--- Calculate Decryption key
int decKey(int a, int b)
{
    int b0 = b, t, q;
    int x0 = 0, x1 = 1;

    if (b == 1) return 1;

    while (a > 1)
    {
        q = a / b;
        t = b, b = a % b, a = t;
        t = x0, x0 = x1 - q * x0, x1 = t;
    }

    if (x1 < 0)
    {
        x1 += b0;
    }
        return x1;
}

int main(void)
{
    struct sockaddr_in si_me, si_other;
    int s, i, recv_len;
    socklen_t slen;


    int b,c,ae,be ,aD,bD;
        int primeNo=167;
        int phiNo;
       // int a[1]={rand()%(32000-100+1)+100 };
         //primeNo= a[0];
        phiNo = primeNo-1;
        int bdec[1];

        cout<< "prime no is"<<primeNo<<"\n";

        //create a UDP socket
         s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
         // zero out the structure
         bzero(&si_me,sizeof(si_me));
         si_me.sin_family = AF_INET;
         si_me.sin_addr.s_addr = htonl(INADDR_ANY);
         si_me.sin_port = htons(32000);

         //bind socket to port
          bind(s , (struct sockaddr*)&si_me, sizeof(si_me) );

slen = sizeof(si_other);

//sendto(s, a, sizeof(a), 0, (struct sockaddr*)&si_other, slen);

 int km=1;
while(km!=0)
{
    cout<< "\n enter Bob E key such that gcd=1\n";
    cin>> be;
    b=gcd(be,phiNo);
    //cout<<b;
    if(b==1)
       {
        km--;
    }
    else{
        cout<< "\n wrong input\n";
    }
}

  bdec[0]= decKey(be,phiNo);
  bD = bdec[0];
         cout<< "\n--Bob D key--\n" << bD;

        generateCard();
        cout<< "\n-------shuffle cards-----\n";
        random_shuffle(deckNo.begin(), deckNo.end());
        cout<< "\n-----encryption with Bob Key----\n";

//---Encrypt all cards with bob encrypted key
    for(int i=0 ;i<52;i++)
    {
        int no = deckNo.at(i);
        int encryptNo =modPower(no,be,primeNo);
        EncryptedDeck.push_back(encryptNo);
    }

   std::copy(EncryptedDeck.begin(), EncryptedDeck.end(), EncryptedDeckArr);

        string arr[1];


   recvfrom(s,arr , sizeof(arr), 0, (struct sockaddr *)&si_other,&slen);

  // -- Send all encrypted card to Alice

   sendto(s, EncryptedDeckArr, sizeof(EncryptedDeckArr), 0, (struct sockaddr*)&si_other, slen);     \


//--Recieved Bob's card from Alice

       recvfrom(s,BobCardArr , sizeof(BobCardArr), 0, (struct sockaddr *)&si_other,&slen);
       std::vector<int>BobCard(BobCardArr,BobCardArr +5);

  //--Recieved Alice's card from Alice

       recvfrom(s,AliceBobEncryptedDeckArr , sizeof(AliceBobEncryptedDeckArr), 0, (struct sockaddr *)&si_other,&slen);
       std::vector<int>AliceBobEncryptedDeck(AliceBobEncryptedDeckArr,AliceBobEncryptedDeckArr +5);


       cout<<"\n Recieved Bob Card \n";

       cout<<"\n Recieved Alice Card \n";

     //-- Decrypted Bob card with bob decryption key

       cout<<"\n--Decrypt Bob cards--\n";

           for(int i=0 ;i<5;i++)
           {
           int no = BobCard.at(i);
           int DecryptNo =modPower(no,bD,primeNo);
           BobDecryptedDeck.push_back(DecryptNo);
           }

           for(int i=0 ;i<5;i++)
           {
           cout<<BobDecryptedDeck.at(i)<<"\n";
           }

 //-- Decrypted Alice card with bob decryption key

       cout<<"\n---Decrypt Alice cards by Bob---\n";

          for(int i=0 ;i<5;i++)
           {
           int no = AliceBobEncryptedDeck.at(i);
           int DecryptNo =modPower(no,bD,primeNo);
           AliceBobDecryptedDeck.push_back(DecryptNo);
           }

          cout<< "\n Alice card decrypted by Bob key--\n";
           for(int i=0 ;i<5;i++)
           {
           cout<<AliceBobDecryptedDeck.at(i)<<"\n";
           }


           std::copy(AliceBobDecryptedDeck.begin(), AliceBobDecryptedDeck.end(), AliceBobDecryptedDeckArr);

           sendto(s, AliceBobDecryptedDeckArr, sizeof(AliceBobDecryptedDeckArr), 0, (struct sockaddr*)&si_other, slen);

//           std::copy(deck.begin(), deck.end(),deckArr);\

//           sendto(s, deckArr, sizeof(deckArr), 0, (struct sockaddr*)&si_other, slen);

  cout<<"\n Bob's cards \n";
           for (int i=0 ; i<5;i++)
           {
               int ab = BobDecryptedDeck.at(i)-48;
              // cout<<"\n"<< ab;
               cout<< deck.at(ab)<<"\n";
           }

//Send Bob key for verification

             sendto(s, bdec, sizeof(bdec), 0, (struct sockaddr*)&si_other, slen);



}
