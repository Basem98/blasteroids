blasteroids.o: blasteroids.c
	gcc -c -g blasteroids.c -o blasteroids.o
	
spaceship.o: spaceship.c
	gcc -c -g spaceship.c -o spaceship.o
	
blast.o: blast.c
	gcc -c -g blast.c -o blast.o

blasteroids: blasteroids.o spaceship.o blast.o
	gcc blasteroids.o spaceship.o blast.o -lm -o blasteroids $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)

blasto: blasteroids.o spaceship.o blast.o
	gcc -g blasteroids.o spaceship.o blast.o -lm -o blasto $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)
