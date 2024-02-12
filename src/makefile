compressor.exe: main.c ./node/node.c ./vector/node_vector.c ./vector/word_vector.c ./huffman/huffman.c ./huffman/encode.c ./huffman/decode.c ./heap/heap_min.c ./utils/bit_operations.c ./utils/file_operations.c
	gcc main.c ./node/node.c ./vector/node_vector.c ./vector/word_vector.c ./huffman/huffman.c ./huffman/encode.c ./huffman/decode.c ./heap/heap_min.c ./utils/bit_operations.c ./utils/file_operations.c -o ../build/compressor.exe

debug: main.c ./node/node.c ./vector/node_vector.c ./vector/word_vector.c ./huffman/huffman.c ./huffman/encode.c ./huffman/decode.c ./heap/heap_min.c ./utils/bit_operations.c ./utils/file_operations.c
	gcc -ggdb main.c ./node/node.c ./vector/node_vector.c ./vector/word_vector.c ./huffman/huffman.c ./huffman/encode.c ./huffman/decode.c ./heap/heap_min.c ./utils/bit_operations.c ./utils/file_operations.c -o ../build/debug.exe

test: compressor.exe
	../build/compressor.exe -f ../test/simple.txt -d

clean:
	rm ../build/compressor.exe
	rm ../build/debug.exe
	rm ../build/.exe

	rm ../test/simple.huf
	rm ../test/simple_decoded.txt
	rm ../test/one_unique_byte.huf
	rm ../test/one_unique_byte_decoded.txt
	rm ../test/LoremIpsum.huf
	rm ../test/LoremIpsum_decoded.txt
	rm ../test/fretka_w_wannie.huf
	rm ../test/fretka_w_wannie_decoded.txt
