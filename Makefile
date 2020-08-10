blasteroids.o: blasteroids.c
	gcc -c blasteroids.c -o blasteroids.o

blasteroids: blasteroids.o
	gcc blasteroids.o -o blasteroids $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)
