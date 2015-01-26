
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>
#include <iostream>
#include<vector>
#include<cmath>

#define PORT 32000   //The port on which to send data

using namespace std;


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


int main(void)
{
    struct sockaddr_in si_other;
    socklen_t slen;
    int s, i;
    int st,st1,st2;

    int b,c,ae,be ,aD,bD;
    int primeNo=167;
    int phiNo;
    int encryptedArr[52];
    int BobCardArr[5];
    int AliceBobEncryptedDeckArr[5];
    int AliceBobDecryptedDeckArr[5];
    string deckArr[52];
    int a[1];
    int bdec[1];

    generateCard();


   //---socket connection

    s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bzero(&si_other,sizeof(si_other));
    si_other.sin_family = AF_INET;
  //  si_other.sin_addr.s_addr=INADDR_ANY;
   si_other.sin_port = htons(32000);
    slen=sizeof(si_other);

\


string arr[1] ={"connected"};

sendto(s, arr, sizeof(arr), 0, (struct sockaddr*)&si_other, slen);

//recvfrom(s, a, sizeof(a), 0, (struct sockaddr *)&si_other,&slen);

//primeNo =a[0];
//cout<< "prime no is" << primeNo <<"\n";

phiNo = primeNo-1;


    int km=1;
    while(km!=0)
    {
        cout<< "\n enter Alice Encrypted key such that gcd=1\n";
        cin>> ae;
        b=gcd(ae,phiNo);
        //cout<<b;
        if(b==1)
           {
            km--;
        }
        else{
            cout<< "\n wrong input\n";
        }
    }



    //---Alice Decryption key


    aD= decKey(ae,phiNo);
     cout<< "\n--Alice D key--\n" << aD << "\n";


       st= recvfrom(s, encryptedArr, sizeof(encryptedArr), 0, (struct sockaddr *)&si_other,&slen);



std::vector<int>EncryptedDeck(encryptedArr,encryptedArr +52);

for(int i=0 ;i<52;i++)
{
 cout << EncryptedDeck.at(i)<<"\n";
}

//--Card Selection
int pick;
cout<< "\n--------Enter position for pick up 5 cards for Alice------ \n";
cin>> pick;
int j = pick;

    for (int i=j; i<j+5;i++)
    {
        int a = EncryptedDeck.at(i);
//        cout<<"\n inside"<<a;
       aliceCard.push_back(a);
    }

  cout << "\n Alice Cards \n";

    for(int i=0 ;i<5;i++)
    {
        cout<<aliceCard.at(i)<<"\n";
    }

 /*   for(int i=0 ;i<5;i++)
    {
        int no = aliceCard.at(i);
        int encryptNo =modPower(no,bD,primeNo);
        int ab=encryptNo-48;
        cout<<"\n"<< deck.at(ab);
    }
*/
int k=j+5;

cout<<"\n------pick up 5 cards for bob------\n";
    for (int i=k; i<k+5;i++)
    {
        int b = EncryptedDeck.at(i);
       BobCard.push_back(b);
    }

    cout<<"\n Bob cards \n";

    for(int i=0 ;i<5;i++)
    {
        cout<<BobCard.at(i)<<"\n";
    }

    std::copy(BobCard.begin(), BobCard.end(), BobCardArr);

  /*  for(int i=0 ;i<5;i++)
    {
        int no = BobCard.at(i);
        int encryptNo =modPower(no,bD,primeNo);
        //AliceBobEncryptedDeck.push_back(encryptNo);
        int ab=encryptNo-48;
        cout<<"\n"<< deck.at(ab);
    }

*/

 //--Encrypt Alice card with Alice encrypted key

cout<<"\n----encrypt with Alice  key-------\n";

for(int i=0 ;i<5;i++)
{
    int no = aliceCard.at(i);
    int encryptNo =modPower(no,ae,primeNo);
    AliceBobEncryptedDeck.push_back(encryptNo);
}


cout<<"\n-----display Alice cards Encrypted with Alice and Bob Key----\n";

    for(int i=0 ;i<5;i++)
    {
    cout<<AliceBobEncryptedDeck.at(i)<<"\n";
    }


//-- Send Bob cards -//

    sendto(s, BobCardArr, sizeof(BobCardArr), 0, (struct sockaddr*)&si_other, slen);


//---Send Alice cards--//
    std::copy(AliceBobEncryptedDeck.begin(), AliceBobEncryptedDeck.end(), AliceBobEncryptedDeckArr);


 sendto(s, AliceBobEncryptedDeckArr, sizeof(AliceBobEncryptedDeckArr), 0, (struct sockaddr*)&si_other, slen);

//--- Recevied Alice card decrypted by Bob decryption key

 cout<<"\n Recieved cards from Bob \n";

 st1= recvfrom(s, AliceBobDecryptedDeckArr, sizeof(AliceBobDecryptedDeckArr), 0, (struct sockaddr *)&si_other,&slen);



std::vector<int>AliceBobDecryptedDeck(AliceBobDecryptedDeckArr,AliceBobDecryptedDeckArr +5);

//-- Decrypt Alice card with Alice decryption key

 cout<<"\n----Decrypt Alice cards by Alice--- \n";


    for(int i=0 ;i<5;i++)
    {
    int no = AliceBobDecryptedDeck.at(i);
    int DecryptNo =modPower(no,aD,primeNo);
    AliceDecryptedDeck.push_back(DecryptNo);
    }

  for(int i=0 ;i<5;i++)
    {
    cout<<AliceDecryptedDeck.at(i)<<"\n";
    }

  //recvfrom(s, deckArr, sizeof(deckArr), 0, (struct sockaddr *)&si_other,&slen);

   // std::vector<string>deck(deckArr,deckArr +52);

//    for(int i=0 ; i<52; i++)
//       {
//        cout<<"\n"<< deck.at(i);
//    }


  cout << "\n Alice Cards";
    for (int i=0 ; i<5;i++)
    {
        int ab = AliceDecryptedDeck.at(i)-48;
       // cout<<"\n"<< ab;
        cout<<"\n"<< deck.at(ab);
    }

//--- Recieved decryption key from Bob to verify cheating
    int st3;
st3= recvfrom(s, bdec, sizeof(bdec), 0, (struct sockaddr *)&si_other,&slen);

cout<<"\n recieved Bob decrypted key to verify cheating \n" <<bdec[0];
bD=bdec[0];


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


cout<<"\n Varified Bob's cards \n";
         for (int i=0 ; i<5;i++)
         {
             int ab = BobDecryptedDeck.at(i)-48;
            // cout<<"\n"<< ab;
             cout<< deck.at(ab)<<"\n";
         }

}
