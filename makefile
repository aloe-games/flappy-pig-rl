all: 
	g++ *.cpp -Wall -Wno-write-strings -o game -lm -lallegro -lallegro_image -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf

run:
	./game

clean:
	rm -f game *.o; echo "0" > assets/record.txt
