/*
System and network security - Assignment : Implement Hill cipher in CBC mode
Group number : 4
Name 			:  		Enrollment number
Mahima Pant 				BT17CSE040
Mugdha Kolhe				BT17CSE043
Grishma Mahapurush 			BT17CSE097

Constraints:
For key : 	1. The key matrix must have a positive determinant
	  		2. There should exist inverse modulo of key matrix determinant with 127(Number of characters)
	  		3. Block size should be in multiple of dimension of key matrix

This code is of server side.
*/
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include <bits/stdc++.h> 
#include<iostream> 
#include<string.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<gmp.h>
#define PORT 8080   
#define mod 127

using namespace std;

int modInverse1(int a, int m) 
{ 
	a = a % m; 
	for (int x = 1; x < m; x++) 
		if ((a * x) % m == 1) 
			return x; 
	return 0;
} 

void  Mod1(mpz_t & ret ,mpz_t a, mpz_t m,mpz_t ans) 
{ 

	if(mpz_cmp_si(a,0))
	{
		while(mpz_cmp_si(a,0))
		{
			mpz_add(a,a,m);
		}
	}
	else
	{
		while(a>m)
		{
			mpz_sub(a,a,m);
		}
	}
	mpz_init_set(ret,a);
} 

mpz_t** allocateMatSpace1(mpz_t **mat, int rows, int columns)
{
	mat = new mpz_t*[rows];
	for(int i = 0; i<rows; i++)
		mat[i] = new mpz_t[columns];
	//cout<<"Allocation Successful\n";
	return mat;
}

void deallocateMatSpace1(mpz_t **mat, int rows, int columns)
{
	for(int i = 0; i < rows; i++)
		delete [] mat[i];

	delete [] mat;

	//cout<<"deallocation Successful\n";
}


void displayMatrix1(mpz_t **mat, int rows, int cols)
{
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
			cout<<mat[i][j]<<" ";
		cout<<"\n";
	}
}

void getcofactor1(mpz_t** mat, mpz_t** cofac, int p, int q, int n)
{
	int i = 0, j = 0; 
  
    for (int row = 0; row < n; row++) 
    { 
        for (int col = 0; col < n; col++) 
        { 
             
            if (row != p && col != q) 
            { 
                //cofac[i][j++] = mat[row][col]; 

                mpz_init_set(cofac[i][j++], mat[row][col]);
               if (j == n - 1) 
                { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
}

void determinant1(mpz_t ret ,mpz_t **mat, int n)
{
	
	mpz_t det;
     mpz_init_set_str (det, "0", 10);
	if(n == 1){
        mpz_init_set(ret , mat[0][0]);
        //return  mat[0][0];
        return ;
    }
		

	mpz_t **temp;
	temp = allocateMatSpace1(temp, n-1, n-1);
	//temp = new int*[n-1];
	//for(int i = 0; i < n-1; i++)
		//temp[i] = new int[n-1];

	int sign = 1;

	for(int i = 0; i < n; i++)
	{
		getcofactor1(mat, temp, 0, i, n);

		// det += sign * mat[0][i] * determinant(temp, n-1);
        mpz_t mul,detlocal;
        mpz_init_set_str (mul, "1", 10);
        mpz_init(detlocal);
        mpz_mul_si(mul,mul,sign);
        mpz_mul(mul,mul,mat[0][i]);
        
         determinant1(detlocal,temp, n-1);

        mpz_mul(mul,mul,detlocal);

        mpz_add(det,det,mul);
 
		sign = -sign;
	}

	deallocateMatSpace1(temp, n-1, n-1);

	//for(int i = 0; i < n-1; i++ )
		//delete [] temp[i];
	//delete [] temp;

	//return det;
    mpz_init_set(ret ,det);
	
}

void adjoint1(mpz_t **mat, mpz_t **adj, int n)
{
	if(n == 1)
	{
		// adj[0][0] = 1;
         mpz_init_set_str (adj[0][0], "1", 10);
		return;
	}

	int sign = 1;

	mpz_t **temp;

	temp = allocateMatSpace1(temp, n-1, n-1);
	//temp = new int*[n-1];
	//for(int i = 0; i < n-1; i++)
		//temp[i] = new int[n-1];

	for(int i = 0; i<n; i++)
		for(int j = 0; j<n; j++)
		{
			getcofactor1(mat, temp, i, j, n);

			sign = (i+j)%2 == 0 ? 1: -1;

			//adj[j][i] = (sign) * (determinant(temp, n-1));
            mpz_t det;
            mpz_init_set_str (det, "1", 10);
            determinant1(det,temp, n-1);
            mpz_mul_si(det,det,sign);
            mpz_init_set(adj[j][i],det);

		}

	deallocateMatSpace1(temp, n-1, n-1);	
}


void InverseMat1(mpz_t **adj, int det, int n)
{
	int idet = modInverse1(det, 127);
	cout<<"Determinant: inverse "<<idet<<endl;
	if(idet == 0)
	{
		cout<<"Inverse Modulo doesn't exist!!!\n";
	}
	cout<<"Inverse modulo of "<<det<<" is "<<idet<<endl;
	for(int i = 0; i<n; i++)
	{
		for(int j = 0; j<n; j++)
			{   
                mpz_t ad;
            mpz_init(ad);
           
            mpz_mul_ui(adj[i][j],adj[i][j],idet);
				// adj[i][j] = idet*adj[i][j];

				// adj[i][j] = Mod(adj[i][j], mod);
                //inbuild  function

                mpz_t modulu;
                mpz_init_set_si(modulu,mod);
                mpz_mod(adj[i][j],adj[i][j],modulu);

			}
	}
}

void display1(vector<vector<mpz_t > > A) 
{ 
	int N=A.size();
    for (int i=0; i<N; i++) 
    { 
        for (int j=0; j<A[i].size(); j++) 
        {
            cout <<"|"<<A[i][j] << " ";
		}
		cout<<"|";
        cout << endl; 
    } 
} 


// Allocates space to store matrix
int** allocateMatSpace(int **mat, int rows, int columns)
{
	mat = new int*[rows];
	for(int i = 0; i<rows; i++)
	{
		mat[i] = new int[columns];
	}
		
	//cout<<"Allocation Successful\n";
	return mat;
}

// Free the space used to store matrix
void deallocateMatSpace(int **mat, int rows, int columns)
{
	for(int i = 0; i < rows; i++)
	{
		delete [] mat[i];
	}
		

	delete [] mat;
	//cout<<"deallocation Successful\n";
}

// Prints the matrix
void displayMatrix(int **mat, int rows, int cols)
{
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			if(mat[i][j]<0)
			{
				mat[i][j]+=mod;
			}
			cout<<mat[i][j]<<" ";
		}
			
		cout<<"\n";
	}
}

// Finds the cofactor of given matrix
void getcofactor(int** mat, int** cofac, int p, int q, int n)
{
	int i = 0, j = 0; 
  
    	for (int row = 0; row < n; row++) 
    	{ 
        	for (int col = 0; col < n; col++) 
        	{ 
             
        	    if (row != p && col != q) 
           	    { 
           	    	cofac[i][j++] = mat[row][col]; 
  	
               		if(j == n - 1) 
                	{ 
                    		j = 0; 
                    		i++; 
                	} 
            	    } 
       		 } 
    	} 
}

// Finds the determinant of given matrix
int determinant(int **mat, int n)
{
	
	int det = 0;

	if(n == 1)
	{
		return mat[0][0];
	}	

	int **temp;
	temp = allocateMatSpace(temp, n-1, n-1);

	int sign = 1;

	for(int i = 0; i < n; i++)
	{
		getcofactor(mat, temp, 0, i, n);
		det += sign * mat[0][i] * determinant(temp, n-1);

		sign = -sign;
	}

	deallocateMatSpace(temp, n-1, n-1);

	return det;
	
}

//Finds adjoint of the matrix
void adjoint(int **mat, int **adj, int n)
{
	if(n == 1)
	{
		adj[0][0] = 1;
		return;
	}

	int sign = 1;

	int **temp;

	temp = allocateMatSpace(temp, n-1, n-1);

	for(int i = 0; i<n; i++)
	{
		for(int j = 0; j<n; j++)
		{
			getcofactor(mat, temp, i, j, n);

			sign = (i+j)%2 == 0 ? 1: -1;

			adj[j][i] = (sign) * (determinant(temp, n-1));
		}
	}	

	deallocateMatSpace(temp, n-1, n-1);	
}

// Finds  modulo inverse of a with respect to m 
int modInverse(int a, int m) 
{ 
        a = a % m; 
	for (int x = 1; x < m; x++)
	{

	        if ((a * x) % m == 1)
		{
		 
	            return x;  
		}
	}
	return 0;
} 

// Finds the inverse of matrix
void InverseMat(int **adj, int det, int n)
{
	int idet = modInverse(det, 127);
	if(idet == 0)
	{
		cout<<"Inverse Modulo doesn't exist!!!\n";
	}
	cout<<"Inverse modulo of "<<det<<" is "<<idet<<endl;
	for(int i = 0; i<n; i++)
	{
		for(int j = 0; j<n; j++)
			{
				adj[i][j] = idet*adj[i][j];
				adj[i][j] = adj[i][j]%127;
			}
	}	
}

//Printing the 2-D vector
void display(vector<vector<int > >A) 
{ 
 	int N=A.size();
        for (int i=0; i<N; i++) 
        { 
        	for (int j=0; j<A[i].size(); j++) 
			{
            		cout <<"|"<<A[i][j] << " ";
			}
			cout<<"|";
	        cout << endl; 
        } 
} 

//Hill Cipher Encryption
vector<vector <int > > encryption(vector<vector<int > >pt, vector<vector<int > >key)
{
	cout<<endl<<"--------------------------------------------------------"<<endl;
	cout<<endl<<"Encryption:"<<endl;
	int i, j, k;
	vector<vector<int > >res;
	for(i=0; i<pt.size(); i++)	//initialization
	{
		vector<int>temp;
		for(j=0;j<key.size();j++)
		{
			temp.push_back(0);
		}
		res.push_back(temp);
	}	
    	for (i = 0; i < pt.size(); i++) 	//return multiplication of plain text matrix and key matrix
		{
        	for (j = 0; j < key.size(); j++) 
			{
           	 	for (k = 0; k < key[j].size(); k++) 
	  	 		{
		
                		res[i][j] += pt[i][k] * key[k][j];
				}
			}		
        }
    
    	for (i = 0; i < pt.size(); i++) 
	{
        	for (j = 0; j < key.size(); j++) 
		{
                	res[i][j]=res[i][j]%mod;	// Applying modulo operator to be able to represent the resulted matrix  	// in characters whose values are from 0 to 127
        	}
    	}
    	return res;
}
//Converting string into matrix
vector<vector<int > > tomatrix(string ptblock,int row ,int col)  	
{
	int i,j,k=0;
	vector<vector<int > > val;

	for(i=0;i<row;i++)
	{
		vector<int>temp;
		for(j=0;j<col;j++)
		{
			int x=(int)ptblock[k];
			temp.push_back(x);
			k++;
		}
		val.push_back(temp);
	}
	return val;
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
			temp.push_back(pt[i][j]^prevct[i][j]);	// Xoring initial vector block with first plain text block to get first cipher
		}						// text block, for rest of cipher text blocks xor current plaintext block with
		ctop.push_back(temp);				//previous obtained cipher text blocks
	}
	return ctop;
}

//CBC Encryption
vector<vector<vector<int > > > cbcencrypt(string pt,vector<vector<int > >key,int row,int col)
{
	vector<vector<int > > iv;
	vector<vector<int > >ct;
	for(int i=0;i<row;i++)
	{
		vector<int>tm;
		for(int j=0;j<col;j++)
		{
	
			tm.push_back(0);
		}
		iv.push_back(tm);		// Initializing all elements of initial vector as zero.
	}
	vector<vector<vector<int > > >chainct;
	chainct.push_back(iv);
	int pt_blksize = row*col;
	while(pt.size()%pt_blksize!=0)
	{
		pt+="Z";	// Padding the original plaintext at the end with character 'Z' , if the size of original plain text is not 					// in multiple of block size of plain text
	}
	int i=0;
	
	int iter=pt.size()/pt_blksize;
	cout<<endl;
	cout<<"Algorithm of CBC mode starts"<<endl;
	cout<<"==============================================================================="<<endl;
	for(int l=0;l<iter;l++)
	{
		vector<vector<int > >currpt;
		string substr=pt.substr(i,pt_blksize);	// Taking next substring of plain text having size of plain text block
		
		currpt=tomatrix(substr,row,col);        // Converting this substring in form of matrix

		cout<<"Plain text block[ "<<l<<" ]"<<endl;
		display(currpt);
			
	   	ct=encryption(xorop(currpt,iv),key);
	    	iv=ct;
	    	chainct.push_back(iv);
		cout<<"Cipher text block[ "<<l<<" ]"<<endl;
	    	display(iv);
	    	cout<<"============================================================================"<<endl;
		i+=pt_blksize;	
	}

	return chainct;
}

int main()
{

	string str;
	int flag, num;

	cout<<"Enter your string: ";
	getline(cin,str);
	int n=str.length();
	int row, col;
	int block_size;
	string length = to_string(n)+" ";
	vector<vector<int > > key;
	vector<vector<int > > keyinv;
	cout<<"Enter the dimension of the key matrix : ";

		cout<<endl;
		cin>>num;
		cout<<"Enter the block size of the plain text(which should be used in CBC mode and should be multiple of dimension) : ";
		cin>>block_size;
		cout<<endl;
		row = block_size/num;			// The total number of rows in plain text matrix
		col = num;					// The total number of columns in plain text matrix

		length+=to_string(row)+" "+to_string(col)+" ";

		mpz_t **key1;
    	key1 = allocateMatSpace1(key1, num, num);
    	cout<<"Enter the key matrix\n";

	    for(int i = 0; i < num; i++)
	       for(int j = 0; j < num; j++)
	           cin>>key1[i][j];

	    	displayMatrix1(key1, num, num);
	    	cout<<"\n";

	    	// int det = determinant(key1, num);
	    	mpz_t det ;
	        mpz_init_set_str (det, "1", 10);
	    	determinant1(det,key1, num);
	        cout<<"The determinant of the matrix is : "<< det<<endl;

        	mpz_t modulu;
        	mpz_init_set_si(modulu,mod);
              

    		if(mpz_cmp_si(det, 0) <= 0)
    		{
    	    	cout<<"Inverse of "<<det<<" doesn't exist\n";
    	   		 exit(-1);
    	    	return 0;
        
    		}
    		else
    		{
    		    cout<<"The key works, you are good to go!!!\n";
    		}

        	mpz_t **adj;

        	adj = allocateMatSpace1(adj, num, num);
        	adjoint1(key1, adj, num);
        	//cout<< det <<"---";
        	displayMatrix1(adj, num, num);
        	int x = mpz_get_si(det);
        	cout<< x <<"---";
        	InverseMat1(adj, x, num);
        	//displayMatrix1(adj, num, num);
        	vector<int> keystore;
        	
			
			for(int i = 0; i<num; i++)
			{
				for(int j = 0; j<num; j++)
				{
					int x= mpz_get_si(key1[i][j]);
					keystore.push_back(x);
				}
				key.push_back(keystore);
				keystore.clear();
			}	
			//inverse stored
			for(int i = 0; i<num; i++)
			{
				for(int j = 0; j<num; j++)
				{
					int x= mpz_get_si(adj[i][j]);
					keystore.push_back(x);
					}
				keyinv.push_back(keystore);
				keystore.clear();
			}
	 	
	vector<vector<vector<int > > > CT=cbcencrypt(str,key,row,col);

	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 	
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 	
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	//Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0)  // Wait for the client to approach it(server)
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 

	// Creates a new connected socket and return a new file descriptor referring to that socket
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)  
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

	string pass="";
	for(int a=0;a<keyinv.size();a++)
	{
	   for(int b=0;b<keyinv[a].size();b++)
	   {
	   	if(keyinv[a][b]<0)
	   	{
	   		keyinv[a][b]+=127;
	   	}
	  	pass+=to_string(keyinv[a][b]);			//Converting all elements of key matrix in one string
	    	pass+=" ";
	   }

	}
		
	
	for(int a=1;a<CT.size();a++)
	{
		for(int b=0;b<CT[a].size();b++)
	  	{
			for(int c=0;c<CT[a][b].size();c++)
			{
	    			pass+=to_string(CT[a][b][c]);		//Converting all elements of matrix in one string
	    			pass+=" ";
			}
		}
		
	}
	char* parameters_length = const_cast<char*>(length.c_str());	
	send(new_socket,parameters_length,strlen(parameters_length) , 0 );  // Passing dimension of key matrix and cipher text matrix
	char *hello = const_cast<char*>(pass.c_str()); 
 
	send(new_socket,hello,strlen(hello) , 0 ); 		// Passing chain of cipher texts (C1,C2.....) to client side
	cout<<"The message is sent\n"; 


	//deallocateMatSpace(adj, num, num);			// Freeing space of key inverse matrix
	//deallocateMatSpace(key1, num, num);			// Freeing space of key matrix
	
	return 0;
}

