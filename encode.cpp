#include <iostream>
#include "huff.hpp"
using namespace std;

int main (int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Failed to detect Files..";
		exit(1);
	}
    huff f (argv[1], argv[2]);
    f.zip();
    cout << "Compressed Successfully!" << endl;
    return 0;
}