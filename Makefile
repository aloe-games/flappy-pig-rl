linux: 
	g++ -Wno-c++11-extensions *.cpp -o game -lallegro -lallegro_image -lallegro_font -lallegro_ttf

macos:
	g++ -Wno-c++11-extensions *.cpp -o game -lallegro -lallegro_image -lallegro_font -lallegro_ttf -I/opt/homebrew/include/ -L/opt/homebrew/lib/ -lallegro_main

play:
	./game

agent:
	./game --agent
