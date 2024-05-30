linux: 
	g++ *.cpp -Wall -Wno-write-strings -o game -lallegro -lallegro_image -lallegro_font -lallegro_ttf

macos:
	g++ *.cpp -Wall -Wno-write-strings -o game -lallegro -lallegro_image -lallegro_font -lallegro_ttf -I/opt/homebrew/include/ -L/opt/homebrew/lib/ -lallegro_main

play:
	./game

agent:
	./game --agent