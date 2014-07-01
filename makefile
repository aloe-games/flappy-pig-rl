game: game.c error.h error.c
	gcc -Wall game.c error.c -o game -lm -lallegro -lallegro_image -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf

clean:
	rm -f game; echo "0" > assets/record.txt
