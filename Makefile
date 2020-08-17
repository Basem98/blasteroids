blasteroids.o: blasteroids.c
	gcc -c blasteroids.c -o blasteroids.o
	
spaceship.o: spaceship.c
	gcc -c spaceship.c -o spaceship.o

blasteroids: blasteroids.o spaceship.o
	gcc blasteroids.o spaceship.o -lm -o blasteroids $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)
