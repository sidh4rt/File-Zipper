//header guards to prevent header files from being included multiple times
#ifndef HUFF_HPP
#define HUFF_HPP
#include <string>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

//defining Huffman Tree Node
struct Node {
    char data;
    unsigned freq;
    string code;
    Node *right, *left;
    //constructor
    Node() {
        left = right = NULL;
    }
};

class huff {
    private:
        vector <Node*> arr;
        fstream infile, outfile;
        string infileName, outfileName;
        Node *root;
        
        class Compare {
            public:
                bool operator() (Node* l, Node* r) {
                    return l -> freq > r -> freq;
                }
        };

        priority_queue <Node*, vector<Node*>, Compare> minHeap;
        
        //initializing a vector of tree nodes representing character's ASCII values and initializing its freq with 0
        void createArr();

        //reconstructing the Huffman tree while Decoding the file
        void buildTree(char, string&);

        //creating Min Heap of Nodes by freq of characters in input file
        void createMinHeap();

        //constructing the Huffman tree
        void createTree();

        //traversing the constructed tree to generate huffman codes of each present character
        void traverse(Node*, string);

        //to convert binary string to its equivalent decimal value
        int binToDec(string);
        
        //to convert a decimal number to its equivalent binary string
        string decToBin(int);
        
        //generating Huffman codes
        void createCodes();

        //saving Huffman Encoded file
        void saveEncodedFile();
        
        //saving Decoded file to obtain original File
        void saveDecodedFile();
        
        //reading the file to reconstruct the Huffman tree
        void getTree();

    public:
        //constructor
        huff (string infileName, string outfileName) {
            this->infileName = infileName;
            this->outfileName = outfileName;
            createArr();
        }
        //compressing input file
        void zip();
        //Decrompressing input file
        void unzip();
};
#endif