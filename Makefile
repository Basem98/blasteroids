# Development version with debugging info
de_blasteroids.o: blasteroids.c
	gcc -c -Wall -Wextra -fsanitize=address -g blasteroids.c -o de_blasteroids.o
	
de_spaceship.o: spaceship.c
	gcc -c -Wall -Wextra -fsanitize=address -g spaceship.c -o de_spaceship.o
	
de_asteroid.o: asteroid.c
	gcc -c -Wall -Wextra -fsanitize=address -g asteroid.c -o de_asteroid.o

de_blast.o: blast.c
	gcc -c -Wall -Wextra -fsanitize=address -g blast.c -o de_blast.o

de_blasteroids: de_blasteroids.o de_spaceship.o de_asteroid.o de_blast.o
	gcc -Wall -Wextra -fsanitize=address -g de_blasteroids.o de_spaceship.o de_asteroid.o de_blast.o -lm -o de_blasteroids $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)


# Production version
blasteroids.o: blasteroids.c
	gcc -c blasteroids.c -o blasteroids.o
	
spaceship.o: spaceship.c
	gcc -c spaceship.c -o spaceship.o

asteroid.o: asteroid.c
	gcc -c asteroid.c -o asteroid.o

blast.o: blast.c
	gcc -c blast.c -o blast.o


blasteroids: blasteroids.o spaceship.o asteroid.o blast.o
	gcc blasteroids.o spaceship.o asteroid.o blast.o -lm -o blasteroids $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)
