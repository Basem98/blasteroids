de_blasteroids.o: blasteroids.c
	gcc -c -g blasteroids.c -o de_blasteroids.o
	
de_spaceship.o: spaceship.c
	gcc -c -g spaceship.c -o de_spaceship.o
	
de_blast.o: blast.c
	gcc -c -g blast.c -o de_blast.o

de_blasteroids: de_blasteroids.o de_spaceship.o de_blast.o
	gcc -g de_blasteroids.o de_spaceship.o de_blast.o -lm -o de_blasteroids $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)


blasteroids.o: blasteroids.c
	gcc -c blasteroids.c -o blasteroids.o
	
spaceship.o: spaceship.c
	gcc -c spaceship.c -o spaceship.o
	
blast.o: blast.c
	gcc -c blast.c -o blast.o


blasteroids: blasteroids.o spaceship.o blast.o
	gcc blasteroids.o spaceship.o blast.o -lm -o blasteroids $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)
