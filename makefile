all: game

game: game.o error.o
	g++ game.o error.o -o game -lm -lallegro -lallegro_image -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf

game.o: game.cpp error.h
	g++ -c game.cpp -Wall -Wno-write-strings

error.o: error.cpp error.h
	g++ -c error.cpp -Wall -Wno-write-strings

clean:
	rm -f game game.o error.o; echo "0" > assets/record.txt
