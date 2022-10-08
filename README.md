# Text-File Zipper

A file zipper which can be used to compress text-files and decompress them.
Huffman coding is implemented, which is a lossless data compression algorithm.
In computer science and information theory, a Huffman code is a particular type of optimal 
prefix code that is commonly used for lossless data compression.


## Features

With this application, the user can perform the following actions:

- Encode: Compresses the given input file.
- Decode: Decompresses Huffman coded file back to its original state.
## Compression 

`compress()`: Steps followed to compress the input file :

- `createMinHeap()`: Reads the input file and keeps the frequency of characters in the file. Also creates a Min Heap structure based on the frequency of each character using priority queue as a data structure that stores nodes and uses its frequency as a comparing parameter.

- `createTree()`: Generates the Huffman tree by copying the Min Heap created earlier, keeping the original. It pops the two nodes with the least frequency. It also assigns these two as left and right nodes to a new node with a frequency which is the sum of the two popped nodes and pushes this node back to the Min Heap. This process is continued until Min Heap has a size equal to 1.

- `createCodes()`: Traverses the complete Huffman tree and assigns codes in binary format to each and every node.

- `saveEncodedFile()`: Saves the Huffman encoded file to an output file.

## Decompression

`decompress()`: Steps followed to decompress the Huffman encoded file :

- `getTree()`: Reads the Min Heap kept at the start of the file and recreates the Huffman tree by attaching one node at a time. MinHeapSize is the first value, next {MinHeapSize * (1+16)} contains character data and 16 decimal values representing 128 bits of binary Huffman code.

- `saveDecodedFile()`: Reads the entire Encoded input File charachters and count0 by ignoring the first (MinHeapSize * (1 + 16)) of the file. The decimal values are converted to huffman codes and the resulting character is appended to the output file by traversing the reconstructed huffman tree. The final count0 number of '0's are ignored since they were extra bits added while saving the encoded file.


## To run the project 

For compressing the text file 
```bash
  g++ encode.cpp huff.cpp -o main
  ./main testFile.txt compressedFile.huf
```

For decompressing the text file

```bash
  g++ decode.cpp huff.cpp -o main
 ./main compressedFile.huf decompressedFile.txt
```



## To learn more about Huffman Coding

 - [Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding)
 - [Huffman Coding | Greedy Algo-3](https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/)
