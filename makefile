game: game.c error.h error.c allegro_wrapper.h allegro_wrapper.c
	gcc -Wall game.c error.c allegro_wrapper.c -o game -lm -lallegro -lallegro_image -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf

clean:
	rm -f game; echo "0" > assets/record.txt
