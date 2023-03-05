compressor.exe: main.c node.c node_vector.c word_vector.c huffman.c encode.c decode.c
	@cc main.c node.c node_vector.c word_vector.c huffman.c encode.c decode.c -o compressor.exe


debug: main.c node.c node_vector.c word_vector.c huffman.c encode.c decode.c
	@cc -ggdb main.c node.c node_vector.c word_vector.c huffman.c encode.c decode.c -o compressor.exe

test: compressor.exe
	@echo "Test na prostym pliku tekstowym zawierającym ciąg: abcda:"
	./compressor.exe -f ./test/simple.txt
	@echo "Zawartość wygenerowanego pliku z kodami poszczególnych znaków:"
	@cat ./test/simple.key
	@echo ""
	@echo "Kompresja większego pliku Lorem Ipsum:"
	./compressor.exe -f ./test/LoremIpsum.txt -s -d
	@echo ""
	@echo "Kompresja zdjęcia z fretką: (trwa chwilę przez dekompresję)"
	./compressor.exe -f ./test/fretka_w_wannie.jpg -s -d
	@echo ""
	@echo "Proszę porównać zdekompresowane pliki z oryginałami by potwierdzić poprawność kompresji."
	@echo "Mają one dopisek _decoded."
	@tree -h -s ./test/

clear:
	@rm -f compressor.exe
	@rm -f ./test/simple.huf
	@rm -f ./test/simple.key
	@rm -f ./test/LoremIpsum.huf
	@rm -f ./test/LoremIpsum.key
	@rm -f ./test/LoremIpsum_decoded.txt
	@rm -f ./test/fretka_w_wannie.huf
	@rm -f ./test/fretka_w_wannie.key
	@rm -f ./test/fretka_w_wannie_decoded.jpg

