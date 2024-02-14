compressor.exe: main.c ./node/node.c ./vector/word_vector.c ./huffman/huffman_tree.c ./huffman/read_codes.c ./huffman/encode.c ./huffman/decode.c ./heap/heap_min.c ./map/hashmap.c ./utils/bit_operations.c ./utils/file_operations.c ./utils/path_handler.c
	gcc main.c ./node/node.c ./vector/word_vector.c ./huffman/huffman_tree.c ./huffman/read_codes.c ./huffman/encode.c ./huffman/decode.c ./heap/heap_min.c ./map/hashmap.c ./utils/bit_operations.c ./utils/file_operations.c ./utils/path_handler.c -o ../build/compressor.exe

debug: main.c ./node/node.c ./vector/word_vector.c ./huffman/huffman_tree.c ./huffman/read_codes.c ./huffman/encode.c ./huffman/decode.c ./heap/heap_min.c ./map/hashmap.c ./utils/bit_operations.c ./utils/file_operations.c ./utils/path_handler.c
	gcc -ggdb main.c ./node/node.c ./vector/word_vector.c ./huffman/huffman_tree.c ./huffman/read_codes.c ./huffman/encode.c ./huffman/decode.c ./heap/heap_min.c ./map/hashmap.c ./utils/bit_operations.c ./utils/file_operations.c ./utils/path_handler.c -o ../build/debug.exe

test: compressor.exe

	../build/compressor.exe -f ../test/simple.txt -f ../test/one_unique_byte.txt -f ../test/LoremIpsum.txt -f ../test/fretka_w_wannie.jpg
	../build/compressor.exe -f ../test/simple.huf -f ../test/one_unique_byte.huf -f ../test/LoremIpsum.huf -f ../test/fretka_w_wannie.huf -d

clean:
	rm ../build/debug.exe

	rm ../test/simple.huf
	rm ../test/one_unique_byte.huf
	rm ../test/LoremIpsum.huf
	rm ../test/fretka_w_wannie.huf
