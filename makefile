compressor.exe: main.c node.c node_vector.c word_vector.c huffman.c encode.c decode.c
	@cc main.c node.c node_vector.c word_vector.c huffman.c encode.c decode.c -o compressor.exe

debug: main.c node.c node_vector.c word_vector.c huffman.c encode.c decode.c
	@cc -ggdb main.c node.c node_vector.c word_vector.c huffman.c encode.c decode.c -o compressor.exe

test: compressor.exe
	@echo "1) Test na prostym pliku tekstowym zawierającym ciąg: abcda:"
	./compressor.exe -f ./test/simple.txt -d
	@echo "Kody poszczególnych znaków można odczytać ze skompresowanego pliku:"
	@sed '$$d' ./test/simple.huf
	@echo ""
	@echo "Sprawdźmy czy zdekompresowany plik jest taki sam jak oryginał dzięki poleceniu cmp:"
	@cmp --silent ./test/simple.txt ./test/simple_decoded.txt && echo 'cmp: Pliki simple.txt i simple_decoded.txt są identyczne' || echo 'cmp: Pliki simple.txt i simple_decoded.txt są różne'
	@echo ""
	@echo "2) Kompresja większego pliku tekstowego Lorem Ipsum:"
	./compressor.exe -f ./test/LoremIpsum.txt -s -d
	@echo ""
	@echo "Sprawdźmy czy zdekompresowany plik jest taki sam jak oryginał dzięki poleceniu cmp:"
	@cmp --silent ./test/LoremIpsum.txt ./test/LoremIpsum_decoded.txt && echo 'cmp: Pliki LoremIpsum.txt i LoremIpsum_decoded.txt są identyczne' || echo 'cmp: Pliki simple.txt i simple_decoded.txt są różne'
	@echo ""
	@echo "3) Kompresja zdjęcia z fretką:"
	./compressor.exe -f ./test/fretka_w_wannie.jpg -s -d
	@echo ""
	@echo "Sprawdźmy czy zdekompresowany plik jest taki sam jak oryginał dzięki poleceniu cmp:"
	@cmp --silent ./test/fretka_w_wannie.jpg ./test/fretka_w_wannie_decoded.jpg && echo 'cmp: Pliki fretka_w_wannie.jpg i fretka_w_wannie_decoded.jpg są identyczne' || echo 'cmp: Pliki simple.txt i simple_decoded.txt są różne'
	@echo ""
	@echo "4) Proszę również porównać zdekompresowane pliki z oryginałami samodzielnie."
	@echo "Mają one dopisek _decoded."
	@echo ""
	@tree -h -s ./test/

clear:
	@rm -f compressor.exe
	@rm -f ./test/simple.huf
	@rm -f ./test/simple_decoded.txt
	@rm -f ./test/LoremIpsum.huf
	@rm -f ./test/LoremIpsum_decoded.txt
	@rm -f ./test/fretka_w_wannie.huf
	@rm -f ./test/fretka_w_wannie_decoded.jpg

