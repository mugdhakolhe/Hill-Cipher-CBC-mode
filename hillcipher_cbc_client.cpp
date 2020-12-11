/*
System and network security - Assignment : Implement Hill cipher in CBC mode
Group number : 4
Name 			:  		Enrollment number
Mahima Pant 				BT17CSE040
Mugdha Kolhe				BT17CSE043
Grishma Mahapurush 			BT17CSE097

Constraints:
For key : 1. The key matrix must have a positive determinant
	  2. There should exist inverse modulo of key matrix determinant with 127(Number of characters)
	  3. Block size should be in multiple of dimension of key matrix

This code is of client side.
*/
//-----------------------------------------------------------------------------------------------------------------------------------------//
#include <bits/stdc++.h> 
#include<iostream> 
#include<string.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#define mod 127 
#define PORT 8080  
using namespace std;
vector <int >extractIntegerWords(string str) 
{ 
    stringstream ss;     
    vector <int > ty;
    /* Storing the whole string into string stream */
    ss << str; 
  
    /* Running loop till the end of the stream */
    string temp; 
    int found;
    
    while (!ss.eof()) 
    { 
  
        /* extracting word by word from stream */
        ss >> temp; 
  	
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found) 
	{
		cout<<"";
	}
        ty.push_back(found);
  
        /* To save from space at the end of string */
        temp = ""; 
    }
    return ty; 
} 
// Generic function to display the matrix.  

void display(vector<vector<int > >A) 
{ 
    int N=A.size();
    for (int i=0; i<N; i++) 
    { 
        for (int j=0; j<A[i].size(); j++) {
            cout <<"|"<<A[i][j] << " ";
		}
		cout<<"|";
        cout << endl; 
    } 
} 
//Perform XOR operation
vector<vector<int > >xorop(vector<vector<int > >pt,vector<vector<int > >prevct)
{
    vector<vector<int > >ctop;
	int i,j;
	for(i=0;i<pt.size();i++)
	{
		vector<int>temp;
		for(j=0;j<pt[i].size();j++)
		{
			temp.push_back(pt[i][j]^prevct[i][j]);	// Xoring initial vector block with first cipher text block to get first 
								// plaintext block, for rest of plain text blocks xor current ciphertext 
								// block with previous obtained plain textblocks.
		}
		ctop.push_back(temp);
	}
	return ctop;
}
//Hill Cipher Decryption
vector<vector<int > >decryption(vector<vector<int > >ct,vector<vector<int > >keyi)
{
	cout<<endl<<"-------------------------------------------------------"<<endl;
	cout<<endl<<"Decryption:"<<endl;
	int i, j, k;
	vector<vector<int > >res;
	for(i=0;i<ct.size();i++)
	{
		vector<int>temp;
		for(j=0;j<keyi.size();j++)
		{
			temp.push_back(0);
		}
		res.push_back(temp);
	}
    	for (i = 0; i < ct.size(); i++) 
		{
        	for (j = 0; j < keyi.size(); j++) 
			{
        	    	for (k = 0; k < keyi[j].size(); k++)
		    	{
	
        	        	res[i][j] += ct[i][k] * keyi[k][j];
		    	}
       		 }
   	 	}
   	 for (i = 0; i < ct.size(); i++) 
	 {
        	for (j = 0; j < keyi.size(); j++) 
		{
                	res[i][j]=res[i][j]%mod;
        	}
   	 }
    return res;
}

//CBC Decryption
vector<vector<vector<int > > >cbcdecrypt(vector<vector<vector<int > > > ct, vector<vector<int > >keyi,int row ,int col)
{
 	vector<vector<vector<int > > >chainpt;
        vector<vector<int > >decry;
    	vector<vector<int > >ivnew;
    	for(int i=0;i<row;i++)
     	{	
     		vector<int>tm;
		for(int j=0;j<col;j++)
		{
			tm.push_back(0);	// All elements of initial vector are zero
		}
		ivnew.push_back(tm);
	}
	cout<< "Decryption in CBC mode using Hill cipher"<<endl;
	cout<<"====================================================="<<endl;
	for(int i=0;i<ct.size();i++)
	{
		
		decry=decryption(ct[i],keyi);
		cout<<"\nFor given Cipher text block[ "<<i<<" ]"<<endl;
		display(ct[i]);
		cout<<"\nPlain text block[ "<<i<<" ] is :"<<endl;
		
		vector<vector<int > >exor=xorop(ivnew,decry);
		chainpt.push_back(exor);

		display(exor);
		cout<<"====================================================="<<endl;
		ivnew=ct[i];
	}
	
	
	return chainpt;
}
//Retrieving Original message from PT blocks obtained after Decryption
string to_char(vector<vector<vector<int > > > pt, int n)
{
	string res="";
	int count=0;
	
	
	count=0;
	for(int i=0;i<pt.size() && count<n;i++)
	{
	   	for(int j=0;j<pt[i].size() && count<n;j++)
	   	{
			for(int k=0;k<pt[i][j].size() && count<n;k++)
			{
	    		
	    			int x=pt[i][j][k];
	    		
				res+=char(x);
				
				count++;
			}
		}	
	}
	return res;
}
int main()
{

	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	
	char buffer[1024] = {0}; 

	// Creating socket file descriptor 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		cout<<"\n Socket creation error \n"; 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		cout<<"\nInvalid address/ Address not supported \n"; 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // Checking for connection
	{ 
		cout<<"\nConnection Failed \n"; 
		return -1; 
	} 
	
	vector<vector<vector<int > > > CT;
	
	valread = read( sock ,buffer, 1024); 	// Reading all the parameters passed from server
	vector<int>temp;
	vector<int>st;
	vector<vector<int>>st2;

        temp=extractIntegerWords(buffer); 	// Extracting inverse of key matrix and cipher text blocks and dimension of key matrix and 							// cipher text block matrix 
        int a,b,c,mt=3,u,o;
        vector<vector<int>>mykeyinv;
	int length = temp[0];			// Storing the block size of cipher text block
	int row = temp[1];			// Storing the number of rows used in cipher text block matrix
	int col = temp[2];			// Storing the number of columns used in cipher text block matrix

        for(a=0;a<col;a++)
        {
        	for(b=0;b<col;b++)
        	{
         		st.push_back(temp[mt]);
         		mt++;
        	}
        	mykeyinv.push_back(st);		// Storing the inverse of key matrix
        	st.clear();
        }
        cout<<"\n Key inverse matrix: "<<endl;
        for(b=0;b<col;b++)			// Printing key inverse matrix
        {
        	for(c=0;c<col;c++)
		{
        		cout<<mykeyinv[b][c]<<" ";
       	        }
		cout<<endl;
	}
	int pt_blksize =row*col;

	int rounds=length/pt_blksize;

	if(length%pt_blksize!=0)
	{
		 rounds+=1;
	}
        for(a=0;a<rounds;a++)
        {
        	for(b=0;b<row;b++)
        	{
        		for(c=0;c<col;c++)
			{
         			st.push_back(temp[mt]);
         			mt++;
        		}
        		st2.push_back(st);
        		st.clear();
        	}
        	CT.push_back(st2);
        	st2.clear();
        	st.clear();
        }

	vector<vector<vector<int > > > PT=cbcdecrypt(CT,mykeyinv,row,col);
	
	string origmsg=to_char(PT,length);
	cout<<"\nThe decrypted message obtained is : "<<origmsg<<endl;

	return 0;
}

