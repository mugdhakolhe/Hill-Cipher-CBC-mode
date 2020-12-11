# Hill-Cipher-CBC-mode

System and Network Security:

*### Problem Statement: Implement Hill Cipher in CBC mode with client and server programming and ability to handle large integers(gmp lib) and ASCII values*

Inside this code we are having server and client, server(sender) is sending the encrypted message and key to the client(receiver). 
At the client-side, the key matrix ( i.e, the inverse of the original key matrix) will be built and ciphertext blocks will be retrieved. 
After the Decryption, decrypted message will get printed so that user can verify if the message has got decrypted correctly. 
We have implemented CBC mode wherein, Encryption and Decryption have been done as per the conventional Hill Cipher method.

#### Constraints:

##### For key : 
1. The key matrix must have a positive determinant.
2. There should exist inverse modulo of key matrix determinant with 127(Number of ASCII characters).
3. Block size should be in multiple of dimension of key matrix.


##### Supports:
1. Alphanumeric inputs(For input message).
2. Variable dimension of key matrix and block size of Plain Text and Cipher Text.
3. GMP library(For supporting large integer values inside Key matrix).
4. Socket programming between server and client.

#### Steps:

##### Installation:
* sudo apt install g++
* sudo apt install build-essential
* sudo apt-get install libgmp3-dev


##### On terminal window type:
* g++ hillcipher_cbc_server.cpp -o server -lgmp -lgmpxx
* ./server

(Input the string, dimension of the key matrix, block size)

##### On another terminal window:
* g++ hillcipher_cbc_client.cpp -o client -lgmp(client side)
* ./client

#### Example input:

String: Hello1234user
Size of  Matrix : 3
Matrix elements:
6 21 1
13 16 10
20 17 15

Output: Hello1234user
