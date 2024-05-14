# Huffman Coding Lab

For this lab assignment you are going to create a text file compression program.  To accomplish this us the Huffman tree as described in the Huffman Coding Handout by Owen L. Astrachan and Chapter 30.1 of your text.  

Your program should ask the user if they wish to encode or decode a file, then prompt the user for the file path of the file to open, then prompt the user for a filename to save the new file under.  It should then open the file encode or decode the content and save it to the new file path entered.  

If you code your program well you should be able to compress the larger text file examples to almost half their original size. 

## Developing and testing:
This time there are no automated tests for you to use or any starting code, it's all up to you.  I might recommend starting with a Huffman ADT class with encode and decode interface methods, but it's up to you. You can and should use STD Container libraries for Hash Tables, Unordered Lists, Priority Queues if you need them, but you ***must code the Huffman tree yourself***. I've included a few example files you can use to test your program.  

Below in this ReadMe file add a short description of how to use your program and how it is implemented.  Include some results from your compression tests, how much are you able to compress a large text file? 

## Your program needs to:

1. Ask the user if they want to compress of decompress a file.  
2. Get input file path.
3. Get a path to save the new compressed or decompressed file.
4. And then exit.  Your program should **not** remain open between compression and decompression sessions. 

### Example of running the program to compress a file:

![Compress Example](images/compressing.png)

### Example of running the program to decompress a file:

![Decompress Example](images/decompressing.png)


## Writing to a binary file. 
One of the things this assignment requires is that you be able to write a binary string as a binary file.  Since this is outside the scope of this class I've included a Storage Class for you to use or emulate.  The Storage driver will take Binary string chunks and store them to a binary file.  It will also open a binary file and return to you binary string chunks 8 bits at a time. It will also allow you to store and read header information that you'll need to stash and rebuild your huffman tree. See the StorageDriver.cpp for an example of how to use the Storage Class. 

## Huffman Node and using a priority queue
I've also included a Node.h file that can be used for the huffman tree or as an example.  When putting Node pointers in a stl priority queue you'll need to let the queue know how to compare two nodes.  The struct compareWeights in the Node.h file does exactly this.  You can find more information here: https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/

**Grading rubric 100 points for the first lab assignment**

| Points | Requirements                                                                                                                                                                                                                                                                               |
|--------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 30     | Huffman Tree: You code demonstrates the ability to create a Huffman tree.                                                                                                                                                                                                                  |
| 30     | Encoding:  Your program is able to encode a string using the Huffman tree.                                                                                                                                                                                                                 |       
| 30     | Decoding: Your program is able to decode a string using the Huffman tree.                                                                                                                                                                                                                  |       
| 15     | File IO:  You program is able to open encode/decode and save a file correctly.  Including some form of storing the a representation of the huffman tree in the encoded file.                                                                                                               |        
| 15     | Large Files:  Your program is capable of handling large file by incrementally reading in the file while creating and encoding/decoding the Huffman tree.                                                                                                                                   |       
| 10     | Short write up, appended to this readme, describing both how to use your program and how it is implemented.                                                                                                                                                                                          |        
| 20     | Good coding practices, including: self-commenting variable names, one statement per line, properly indenting and spacing, good  descriptive comments, and a lack of coding errors like memory leaks. **Create documenting comments for each method public and private**  |



## Enter you description below:
In order to use this program, there are many different functions and methods that essentially make the program function, the most important being compress and decompress.

HOW TO USE THE PROGRAM:
Main two functions are compress and decompress, Compress function prepares a text file and compresses
it into another text file, reducing the size of the file for more efficient storage. The decompression
is equally as important as it reads the bitCode and decompresses it back into the original text file 
it was before compression. For both of these we need an inputFile which is usually a text file like a
book, and an output file which in my case was just test text files.

Implementation:
For the Implementation I created a couple of functions to help with the Huffman coding process, 
all of the functions being,

void compress(const string &inputFileName, const string &outputFileName);
void decode(Node* root, string &encodedData, int &index, ofstream &outputFile);
void decompress(const string &inputFileName, const string &outputFileName);
std::unordered_map<char, int> buildFrequencyTable(const string& fileName);
void encode(ifstream &inputFile, const std::unordered_map<char, string> &huffmanCodes, ofstream &outputFile);
void HuffmanCodes(Node *node, const string &code, std::unordered_map<char, string> &huffmanCodes);
void showTree(Node *node, ofstream &outputFile);
Node *root;
Node *buildHuffmanTree(const unordered_map<char, int> &frequency);
Node *buildHuffmanTreeFromFile(ifstream &inputFile);

All of these functions listed play a very important role and I will list them below. The compress 
function that we already went over earlier takes in the two files and will perform compression. 
Same thing with the decompress function except it decompresses the file rather than compressing. 
The decode function is a helper function for the decompress function, and its purpose is to decode 
the bitCode and turn it back into the regular characters so it can form itself back into a decompressed 
text file. The buildFrequencyTable function reads from the file processing every character into a 
map, or frequency table and accounts for each time a character shows up, it is through this we build
a tree. The encode function helps process the characters in the map into bitCode so it can be 
compressed into the new file. The huffmanCodes function generates HuffmanCodes, for example 001, 
by traversing the huffman tree. The showTree function prints out the tree in the beginning of the 
compressed text file, so that the buildHuffmanTreeFromFile function can essentially create the tree
from the beginning of the file and decode it that way. And finally the buildHuffmanTree function 
creates a Huffman tree by using a minheap Priority Queue and combining nodes until only the root is
left.
