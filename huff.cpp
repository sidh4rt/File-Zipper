#include "huff.hpp"

void huff::createArr() {
    for (int i=0; i<128; i++) {
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
            curr = curr -> left;
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

void huff::createTree() {
    //creating Huffman tree with min heap created earlier
    Node *left, *right;
    priority_queue <Node*, vector<Node*>, Compare> tPQ(minHeap);
    while (tPQ.size() != 1) {
        left = tPQ.top();
        tPQ.pop();
        right = tPQ.top();
        tPQ.pop();
        root = new Node();
        root -> freq = left -> freq + right -> freq;
        root -> left = left;
        root -> right = right;
        tPQ.push(root);
    }
}

void huff::traverse(Node* r, string s) {
    if (r -> left == NULL && r -> right == NULL) {
        r -> code = s;
        return;
    }
    traverse(r -> left, s + '0');
    traverse(r -> right, s + '1');
}

int huff::binToDec(string inS) {
    int res = 0;
    for (auto c : inS) {
        res = res * 2 + c - '0';
    }
    return res;
}

string huff::decToBin(int inNum) {
    string temp = "", ans = "";
    while (inNum > 0) {
        temp += (inNum % 2 + '0');
        inNum /= 2;
    }
    ans.append(8 - temp.size(), '0');
    for (int i=temp.size()-1; i>=0; i--) {
        ans += temp[i];
    }
    return ans;
}

void huff::createCodes() {
    //traversing the Huffman tree and assigning specific codes to every character
    traverse(root, "");
}

void huff::saveEncodedFile() {
    //saving encoded (.huf) file
    infile.open(infileName, ios::in);
    outfile.open(outfileName, ios::out | ios::binary);
    string in = "";
    string s = "";
    char id;
    //saving the meta data (huffman tree)
    in += (char)minHeap.size();
    priority_queue <Node*, vector<Node*>, Compare> tPQ(minHeap);
    while (!tPQ.empty()) {
        Node* curr = tPQ.top();
        in += curr -> data;
        //saving 16 decimal values representing code of curr->data
        s.assign(127 - curr -> code.size(), '0');
        s += '1';
        s += curr -> code;
        //saving decimal values of every 8-bit binary code 
        in += (char)binToDec(s.substr(0, 8));
        for (int i=0; i<15; i++) {
            s = s.substr(8);
            in += (char)binToDec(s.substr(0, 8));
        }
        tPQ.pop();
    }
    s.clear();
    //saving codes of every charachter appearing in input file
    infile.get(id);
    while (!infile.eof()) {
        s += arr[id] -> code;
        //saving decimal values of every 8-bit binary code
        while (s.size() > 8) {
            in += (char)binToDec(s.substr(0, 8));
            s = s.substr(8);
        }
        infile.get(id);
    }
    //if bits remaining are less than 8, append 0's
    int cnt = 8 - s.size();
	if (s.size() < 8) {
		s.append(cnt, '0');
	}
	in += (char)binToDec(s);	
    //append count of appended 0's
    in += (char)cnt;
    //write the in string to the output file
	outfile.write(in.c_str(), in.size());
	infile.close();
	outfile.close();
}

void huff::saveDecodedFile() {
    infile.open(infileName, ios::in | ios::binary);
    outfile.open(outfileName, ios::out);
    unsigned char size;
    infile.read(reinterpret_cast<char*>(&size), 1);
    //reading count at the end of the file which is number of bits appended to make final value 8-bit
    infile.seekg(-1, ios::end);
    char count0;
    infile.read(&count0, 1);
    //ignoring the meta data (huffman tree) (1 + 17 * size) and reading remaining file
    infile.seekg(1 + 17 * size, ios::beg);

    vector<unsigned char> text;
    unsigned char textseg;
    infile.read(reinterpret_cast<char*>(&textseg), 1);
    while (!infile.eof()) {
        text.push_back(textseg);
        infile.read(reinterpret_cast<char*>(&textseg), 1);
    }

    Node *curr = root;
    string path;
    for (int i=0; i<text.size()-1; i++) {
        //converting decimal number to its equivalent 8-bit binary code
        path = decToBin(text[i]);
        if (i == text.size() - 2) {
            path = path.substr(0, 8 - count0);
        }
        //traversing huffman tree and appending resultant data to the file
        for (int j=0; j<path.size(); j++) {
            if (path[j] == '0') {
                curr = curr -> left;
            }
            else {
                curr = curr -> right;
            }
            if (curr -> left == NULL && curr -> right == NULL) {
                outfile.put(curr -> data);
                curr = root;
            }
        }
    }
    infile.close();
    outfile.close();
}

void huff::getTree() {
    infile.open(infileName, ios::in | ios::binary);
    //reading size of MinHeap
    unsigned char size;
    infile.read(reinterpret_cast<char*>(&size), 1);
    root = new Node();
    //next size * (1 + 16) characters contain (char)data and (string)code[in decimal]
    for(int i=0; i<size; i++) {
        char aCode;
        unsigned char hCodeC[16];
        infile.read(&aCode, 1);
        infile.read(reinterpret_cast<char*>(hCodeC), 16);
        //converting decimal characters into their binary equivalent to obtain code
        string hCodeStr = "";
        for (int i=0; i<16; i++) {
            hCodeStr += decToBin(hCodeC[i]);
        }
        //removing padding by ignoring first (127 - curr -> code.length()) '0's and next '1' character
        int j = 0;
        while (hCodeStr[j] == '0') {
            j++;
        }
        hCodeStr = hCodeStr.substr(j+1);
        //adding node with aCode data and hCodeStr string to the huffman tree
        buildTree(aCode, hCodeStr);
    }
    infile.close();
}

void huff::zip() {
    createMinHeap();
    createTree();
    createCodes();
    saveEncodedFile();
}

void huff::unzip() {
    getTree();
    saveDecodedFile();
}