#include <string>
#include <queue>
#include <fstream>
#include "Node.h"
#include "Storage/Storage.h"
#include <unordered_map>
using namespace std;
#ifndef HUFFMAN_H
#define HUFFMAN_H

class Huffman {
public:
    void compress(const string &inputFileName, const string &outputFileName); //compress function
    void decode(Node* root, string &encodedData, int &index, ofstream &outputFile);
    void decompress(const string &inputFileName, const string &outputFileName);
    std::unordered_map<char, int> buildFrequencyTable(const string& fileName);
    void encode(ifstream &inputFile, const std::unordered_map<char, string> &huffmanCodes, ofstream &outputFile);
    void HuffmanCodes(Node *node, const string &code, std::unordered_map<char, string> &huffmanCodes);
    void showTree(Node *node, ofstream &outputFile);
private:
    Node *root;
    Node *buildHuffmanTree(const unordered_map<char, int> &frequency);
    Node *buildHuffmanTreeFromFile(ifstream &inputFile);

};
#endif //HUFFMAN_H
