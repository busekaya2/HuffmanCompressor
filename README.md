# HuffmanCompressor
The Huffman File Compression project aims to provide an efficient file compression solution using the Huffman algorithm while maintaining lossless data integrity. The program, "compressor.exe," is designed for Linux systems and can compress any type of file, although it is optimized for text files. Compressed files carry the ".huf" extension.

# File structure (.huf)
![o14FAxn - Imgur](https://github.com/milckywayy/HuffmanCompressor/assets/120181288/cb331160-08e9-4404-9ae7-5c03e35100ec)

Dictionary Section:

    The dictionary section is in plain text format and provides essential information for the decompression process.  
    The first line contains the original file's extension, enabling proper file naming after decompression.  
    The following lines consist of byte representations (in ASCII) and their corresponding binary codes.  
    Each line represents one byte(ASCII representation) and its binary code separated by a space.  
    The dictionary ends with a null character ('\0') to indicate its termination.  

Encoded Data Section:

    The encoded data section contains the compressed data represented as a sequence of binary codes.
    The binary codes are concatenated to form a continuous stream of bits.

End of File Marker:

    A unique end-of-file marker, represented by the binary code is added at the end of the encoded data.
    The marker indicates the end of the compressed data and helps the decompressor recognize when to stop reading the file.

# Usage
The program is invoked in the console using the command:  

    ./compressor.exe <arguments>

Available Arguments:

    -f: Allows users to specify the names/paths of files to be compressed.  
    -s: Displays the size of files before and after compression, enabling quick comparisons.  
    -d: Immediately decompresses the compressed file to verify the compression's success without data loss.  
    -h: Shows the program's user guide.
  
To compress two documents, file1.txt and file2.txt, execute the following command in the terminal:  

    ./compressor.exe -f file1.txt -f file2.txt

Two compressed files, file1.huf and file2.huf, will be generated in the same directories as the original files.

# Error Codes
After program execution, it returns the following codes to indicate the result:

    0: Successful execution.  
    1: Error reading files.  
    2: Insufficient permissions to edit/create files.  
    3: Memory allocation error.  
    4: Invalid parameters.
