#include "huff.hpp"

void huff::createArr() {
    for (int i = 0; i < 128; i++) {
        arr.push_back(new Node());
        arr[i] -> data = i;
        arr[i] -> freq = 0;
    }
}

void huff::buildTree(char code, string& path) {
    Node* curr = root;
    for (int i = 0; i < path.length(); i++) {
        if (path[i] == '1') {
            if (curr -> right == NULL) {
                curr -> right = new Node();
            }
            curr = curr->right;
        }
        if (path[i] == '0') {
            if (curr -> left == NULL) {
                curr -> left = new Node();
            }
            curr = curr->left;
        }
    }
    curr -> data = code;
}

void huff::createMinHeap() {
    char id;
    infile.open(infileName, ios::in);
    infile.get(id);
    //incrementing frequency of characters that appear in the input file
    while (!infile.eof()) {
        arr[id] -> freq++;
        infile.get(id);
    }
    infile.close();
    //pushing the nodes which appear in the file into the priority queue(Min Heap)
    for (int i = 0; i < 128; i++) {
        if (arr[i] -> freq > 0) {
            minHeap.push(arr[i]);
        }
    }
}