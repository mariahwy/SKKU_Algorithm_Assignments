# Algorithm Assignments

## Overview
This repository contains assignments for the Algorithm course. The assignments focus on implementaion of Algorithms.

## PA1: Min-Max-Median Priority Queue
### Description
Task is to implement a Min-Max-Median Priority Queue in C using efficient data structures such as heaps to optimize performance.
Moreover, it is crucial to perform an experimental performance analysis of implementation to demonstrate its efficiency. 

### Implementation Details
I used a priority queue based on a min-max heap. Since both the min heap and max heap can only provide information about the minimum or maximum values, it is difficult to find the median.
Therefore, I stored smaller values in one min-max heap and larger values in another min-max heap, allowing for comparison to find the median. 

## PA2: k-DNA sequences alignment
### Description
Task is to implement multiple sequence alignment with k DNA sequences using Dynamic Programming.
The algorithm can be divided into 3 Part.
(1) Calculate the length of the LCS, 
(2) Compute the LCS by Backtracking,
(3) Align strings and print the results.

### Implementation Details
- Calculate the length of the LCS algorithm can be expressed in SRT BOT 
framework as follows.
- Compute the LCS by Backtracking.
- Align strings and print the results.

## PA3: File compression and decompression using Huffman code
### Description
The goal is to implement the algorithm that compresses and decompresses files using Huffman coding.
To compress a file, read text from the input file named ‘hw3_input.txt’, and calculate the frequencies of all ASCII characters in that file.
Then, construct a Huffman tree and output the header and compressed result to the output file named ‘hw3_output1.txt’.
To decompress the compressed file, read compressed result from the ‘hw3_output1.txt’, then print the Huffman code and uncompressed text file.

### Implementation Details
- Implementation of min heap.
- Encoding and compression.
- Reconstruction of tree and decompression.

---
This repository is maintained for educational purposes only, and plagiarism of the code may result in penalties.
