#include "Huffman.h"

/**
 * Compresses the input file using Huffman coding and writes the compressed data to an output file.
 *
 * @param inputFileName The name of the input file to be compressed.
 * @param outputFileName The name of the output file to store the compressed data.
 */
void Huffman::compress(const string &inputFileName, const string &outputFileName) {
    //Build frequency table to store the occurence of each character in the file
    unordered_map<char, int> frequencyTable = buildFrequencyTable(inputFileName);

    //Build Huffman tree based on the frequency table using a priority queue
    root = buildHuffmanTree(frequencyTable);

    //Generate Huffman codes for each character in the tree
    unordered_map<char, string> huffmanCodes;
    HuffmanCodes(root, "", huffmanCodes);

    //Open input and output files in binary mode
    ifstream inputFile(inputFileName, ios::binary);
    ofstream outputFile(outputFileName, ios::binary);
    //https://cplusplus.com/doc/tutorial/files/
    //Got help from the website above along with ChatGPT for binary mode

    //Write the Huffman tree to the output file with showTree function call
    showTree(root, outputFile);
    // Encode and write the input file by reading and encoding each character using Huffman codes
    //After each character is encoded, it gets output to the output file after the tree
    encode(inputFile, huffmanCodes, outputFile);
    //close the files
    inputFile.close();
    outputFile.close();
}

/**
 * Decompresses the input file using Huffman coding and writes the decompressed data to an output file.
 *
 * @param inputFileName The name of the input file to be decompressed.
 * @param outputFileName The name of the output file to store the decompressed data.
 */
void Huffman::decompress(const string &inputFileName, const string &outputFileName) {
    //Open input and output files in binary mode
    ifstream inputFile(inputFileName, ios::binary);
    ofstream outputFile(outputFileName, ios::binary);

    //Recreate the Huffman Tree from the files first line
    root = buildHuffmanTreeFromFile(inputFile);

    //Read the rest of the file to get the encoded data
    string data, line;
    while (getline(inputFile, line)) {
        data += line + '\n';
    }
    //Decode function is called to the rest of the data from the  compressed file and it uses the
    //Recreated Huffman tree to decode, then it prints it to the output file
    int index = 0;
    while (index < data.length()) {
        decode(root, data, index, outputFile);
    }
    //Close files
    inputFile.close();
    outputFile.close();
}

/**
 * Builds a Huffman tree from the given frequency table.
 *
 * @param frequency A frequency table mapping characters to their occurrence frequencies.
 * @return The root node of the constructed Huffman tree.
 */
Node* Huffman::buildHuffmanTree(const unordered_map<char, int> &frequency) {
    //Step 1: Make a priority queue of Nodes and pass in compareWeights essentially to compare the weights
    //and ensure that the queue is ordered by weight, lower weight = higher priority
    priority_queue<Node* , vector<Node*>, compareWeights> pq;

    //Step 2: Create leaf nodes and push them into the priority queue
    for (const auto &entry : frequency) {
        pq.push(new Node(entry.first, entry.second));
        //Got help from ChatGPT with this part
        //Use range based loop to push leaf nodes into priority queue
        //Iterates through the map and creates node for each character with the correct frequency
        //Then pushes the new node into the priority queue
    }
    //Step 3: Construct the Huffman tree by combining nodes until only the root is left
    while (pq.size() > 1) {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();
        //Pop the two nodes with lowest weight

        //Create a new parent node with the sum of frequencies
        Node *parent = new Node('\0', left->weight + right->weight, left, right);
        // Push the internal node back into the priority queue
        pq.push(parent);
    }
    //The remaining node is the root of the Huffman tree
    return pq.top();
}

/**
 * Builds a frequency table for characters in the given file.
 *
 * @param fileName The name of the file to read and analyze.
 * @return A frequency table mapping characters to their occurrence frequencies.
 */
std::unordered_map<char, int> Huffman::buildFrequencyTable(const string &fileName) {
    //Open file
    ifstream inputFile(fileName);
    //Create the map to hold freuencies
    std::unordered_map<char, int> frequencyTable;
    char processedCharacter;
    //
    while (inputFile.get(processedCharacter)) {
        //Process each character and update the frequency table, incrementing for each character
        frequencyTable[processedCharacter] = frequencyTable[processedCharacter] + 1;
    }
    //Close file
    inputFile.close();
    //Return frequency table
    return frequencyTable;
}

/**
 * Recursively generates Huffman codes for each character in the Huffman tree.
 *
 * @param node The current node in the Huffman tree.
 * @param code The Huffman code generated so far for the current node.
 * @param huffmanCodes A map to store the generated Huffman codes for each character.
 */
void Huffman::HuffmanCodes(Node *node, const string &code, std::unordered_map<char, string> &huffmanCodes) {
    //Base case that returns if the node is null
    if (node == nullptr) {
        return;
    }
    //Checks if the node is a leaf node, and if it is then it stores the letter it essentially stopped at and returns
    if (node->zero == nullptr && node->one == nullptr) {
        huffmanCodes[node->letter] = code;
        return;
    }
    //Recursive cases calls the function adding either a 0, or a 1 based on if it goes left or right
    HuffmanCodes(node->zero, code + "0", huffmanCodes);
    HuffmanCodes(node->one, code + "1", huffmanCodes);
}

/**
 * Encodes the input file using Huffman codes and writes the encoded data to an output file.
 *
 * @param inputFile The input file stream to be encoded.
 * @param huffmanCodes A map of characters to their corresponding Huffman codes.
 * @param outputFile The output file stream to store the encoded data.
 */
void Huffman::encode(ifstream &inputFile, const std::unordered_map<char, string> &huffmanCodes, ofstream &outputFile) {
    char processedCharacter;
    string data;
    //While loop to iterate through the characters in the file
    while (inputFile.get(processedCharacter)) {
        //Retrieves the Huffman Code for the current character from the map
        data += huffmanCodes.at(processedCharacter);
    }

    //Consulted ChatGPT for help on this part
    for (char bit : data) {
        outputFile.put(bit);
    }
    //What this does is creates a range-based loop and then iterates through each bit in the
    //string 'data', and then it converts it to a single character 'bit' and prints it out to the file
}

/**
 * Builds a Huffman tree from the encoded file representation.
 *
 * @param inputFile The input file stream containing the encoded representation of the Huffman tree.
 * @return The root node of the reconstructed Huffman tree.
 */
Node *Huffman::buildHuffmanTreeFromFile(ifstream &inputFile) {
    //create a variable that holds the bitCode from the file
    char bitCode;
    inputFile.get(bitCode);
    //read in the bitCode from the file and then store it with bitCode


    if (bitCode == '0') {
        // If 0 is encountered, it means a node with children
        Node *left = buildHuffmanTreeFromFile(inputFile);
        Node *right = buildHuffmanTreeFromFile(inputFile);
        //Since the node has children, it recursively calls the function and builds the left and right children
        return new Node('\0', 0, left, right);
        //then creates a new node with placeholder /0 since it is a parent node and does not have a
        //character, the weight is 0 because it is the sum of its children, which are also 0
    } else {
        // If 1 is encountered, it means a leaf node
        char character;
        inputFile.get(character);
        //creates character to read the character associated with the leaf node and then creates
        //and returns a new leaf node with the character that was just read
        return new Node(character, 0);
    }
}

/**
 * Decodes the Huffman-encoded data using the provided Huffman tree and writes the original data to an output file.
 *
 * @param root The root node of the Huffman tree.
 * @param data The Huffman-encoded data to be decoded.
 * @param index The current index in the encoded data.
 * @param outputFile The output file stream to store the decoded data.
 */
void Huffman::decode(Node *root, string &data, int &index, ofstream &outputFile) {
    //Base case if root is null then return
    if (root == nullptr) {
        return;
    }

    //Checks if the current node is a leaf node, and if it is then it prints it to the file
    if (root->zero == nullptr && root->one == nullptr) {
        outputFile.put(root->letter);
        return;
    }
    //Otherwise if it is not a leaf node, it keeps traversing the tree, if the bitCode is 0 then it
    //traverses to the left, and if it is 1 it traverses to the right
    if (data[index] == '0') {
        decode(root->zero, data, ++index, outputFile);
        //Recursive call that either goes left or right based on the number, and passes through data,
        //pre incrementing index and outputFile
    } else {
        decode(root->one, data, ++index, outputFile);
    }
}

/**
 * Writes the structure of the Huffman tree to an output file.
 *
 * The tree is represented using a binary format:
 * @param node The current node in the Huffman tree.
 * @param outputFile The output file stream to store the tree structure.
 */
void Huffman::showTree(Node *node, ofstream &outputFile) {
    //base case if the node is nullptr then return
    if (node == nullptr) {
        return;
    }
    //Checks if the current node is a leaf node, if it is then it prints 1 along with the character
    if (node->zero == nullptr && node->one == nullptr) {
        outputFile.put('1');
        outputFile.put(node->letter);
    } else {
        //Otherwise if the node has children then it prints 0
        outputFile.put('0');
        //Recursively calls function for left and right child
        showTree(node->zero, outputFile);
        showTree(node->one, outputFile);
    }
}
