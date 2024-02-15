# HuffmanCompressor
The Huffman File Compression project aims to provide an efficient file compression solution using the Huffman algorithm while maintaining lossless data integrity. The program, "compressor.exe," is designed to compress any type of file, although it is optimized for text files. Compressed files carry the ".huf" extension.

## Compressed file structure (.huf)
![Screenshot 2024-02-15 012016](https://github.com/milckywayy/HuffmanCompressor/assets/120181288/28a0b86f-895f-45ac-92be-5012ae08f6bd)

### File extension:
Original file extension is stored as plain text with '\n' as terminator.

### Dictionary:
The Huffman tree coding method using bits, where 0 represents a non-leaf node and 1 represents a leaf node followed by the next 8 bits encoding the character
- Node and Leaf Representation: Traverse the Huffman tree in preorder. For each non-leaf node, record a 0. For each leaf node, record a 1 followed immediately by the 8-bit ASCII code of the character represented by that leaf.

- Character Representation in Leaves: Upon encountering a leaf (marked by a 1), the next 8 bits are used to represent the ASCII code of the character stored in that leaf. For example, if the leaf represents the letter 'A', which has an ASCII code of 65, you write 01000001 immediately after the bit indicating a leaf.

- Building the Encoded Tree: By applying the above method, a bit sequence is created that represents the entire Huffman tree. This sequence contains all the information needed to reconstruct the original tree during decompression.


### Encoded Data:
The encoded data section contains the compressed data represented as a sequence of binary codes. The binary codes are concatenated to form a continuous stream of bits. 

### Encoded Data:
The encoded data section contains the compressed data represented as a sequence of binary codes. The binary codes are concatenated to form a continuous stream of bits. 

### Padding
In Huffman compression, the padding size (the number of extra bits added to the last byte to make it a full byte) is stored as a number ranging from 0 to 7. This number is encoded in the first 3 bits immediately following the file extension terminator ('\n'). The last byte of the compressed data is then filled with 'n' zeros, where 'n' is the padding size indicated by those 3 bits.

## Usage
    ./compressor.exe [-f FILEPATH] [-d] [-s] [-h]

Options:
- -f: Path of the file to be compressed/decompressed
- -d: Switch to decopression (Program compresses by default)
- -s: Display file size before and after compression/decompression
- -h: Display help message
        
To compress two documents, file1.txt and file2.txt, execute the following command in the terminal:  

    ./compressor.exe -f file1.txt -f file2.txt

Two compressed files, file1.huf and file2.huf, will be generated in the same directories as the original files.

## Error Codes
After program execution, it returns the following codes to indicate the result:

- 0 - Successful execution.
- 1 - Memory allocation error.  
- 2 - Error reading files.  
- 3 - Insufficient permissions to edit/create files.  
- 4 - Invalid parameters.
- 5 - Invalid file format.
