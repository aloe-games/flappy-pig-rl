all: 
	g++ *.cpp -Wall -Wno-write-strings -o game -lm -lallegro -lallegro_image -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf

run:
	./game

agent:
	./game --agent

clean:
	rm -f game *.o; echo "0" > assets/record.txt
