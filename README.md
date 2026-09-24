# Huffman File Compression

A C++ implementation of lossless file compression and decompression using the **Huffman Coding algorithm**.

The project builds a Huffman tree from character frequencies, generates variable-length binary codes, and stores the encoded data in a compressed binary file. The original file can be reconstructed through the decompression process.

## Features

* Lossless file compression
* File decompression
* Huffman tree construction
* Binary bit-level encoding
* Huffman code generation
* Support for empty and single-character files
* Command-line interface
* Original file reconstruction without data loss

## Data Structures & Algorithms

### Data Structures

* Binary Tree
* Min Heap using priority_queue
* Hash Map using unordered_map
* Vector

### Algorithms

* Frequency Analysis
* Huffman Coding
* Greedy Algorithm
* Tree Traversal
* Binary Encoding and Decoding

## How It Works

## Compression


Input File
    ↓
Calculate Character Frequencies
    ↓
Build Min Heap
    ↓
Construct Huffman Tree
    ↓
Generate Huffman Codes
    ↓
Convert Codes to Binary
    ↓
Compressed File


Characters that occur more frequently receive shorter binary codes, while less frequent characters receive longer codes.

## Decompression


Compressed File
    ↓
Read Huffman Tree
    ↓
Read Encoded Binary Data
    ↓
Traverse Huffman Tree
    ↓
Reconstruct Original File


## Complexity

Let:

* n = number of bytes in the input file
* k = number of unique bytes


 Frequency calculation     O(n)      
 Huffman tree construction O(k log k)
 Code generation           O(k)
 Encoding                  o(n)       
 Decoding                  o(n)

Space complexity is approximately:O(n + k)


## Project Structure





bash
g++ -std=c++17 main.cpp -o huffman


Run:


./huffman


### Windows

```bash
g++ -std=c++17 main.cpp -o huffman.exe
```

Run:
bash
huffman.exe


## Usage

After running the program:


1. Compress
2. Decompress
3. Exit
Choice:


### Compress a file

Select:




Then provide:


Input file: sample.txt
Output file: sample.huff

The program creates a compressed binary file:

text
sample.huff


### Decompress a file

Select:

text
2


Then provide:

text
Compressed file: sample.huff
Output file: restored.txt


The program reconstructs the original content in:

text
restored.txt


## Example

Suppose sample.txt contains:


Huffman coding is a lossless compression algorithm.


Compress:

text
sample.txt → sample.huff


Then decompress:

sample.huff → restored.txt

The contents of 'restored.txt' will match the original file.

## Key Concepts Demonstrated

This project demonstrates practical implementation of:

* Greedy algorithms
* Binary trees
* Priority queues
* Hash maps
* Recursion
* Bit manipulation
* Binary file handling
* Memory management
* Time and space complexity analysis





