default: clean 2310A 2310B 2310dealer

2310A: playerInstance.o site.o playerStack.o path.o messageUtils.o player.o 2310A.o
	gcc -Wall -pedantic -std=gnu99 -g -o 2310A 2310A.o playerInstance.o site.o player.o messageUtils.o path.o playerStack.o

2310B: playerInstance.o site.o playerStack.o path.o messageUtils.o player.o 2310B.o
	gcc -Wall -pedantic -std=gnu99 -g -o 2310B 2310B.o playerInstance.o site.o player.o messageUtils.o path.o playerStack.o

2310dealer: site.o playerStack.o path.o messageUtils.o player.o deck.o dealerUtils.o 2310dealer.o
	gcc -Wall -pedantic -std=gnu99 -g -o 2310dealer 2310dealer.o dealerUtils.o deck.o site.o player.o messageUtils.o path.o playerStack.o

2310A.o:
	gcc -Wall -pedantic -std=gnu99 -g -c 2310A.c

2310B.o:
	gcc -Wall -pedantic -std=gnu99 -g -c 2310B.c

2310dealer.o:
	gcc -Wall -pedantic -std=gnu99 -g -c 2310dealer.c

player.o:
	gcc -Wall -pedantic -std=gnu99 -g -c player.c

messageUtils.o:
	gcc -Wall -pedantic -std=gnu99 -g -c messageUtils.c

path.o:
	gcc -Wall -pedantic -std=gnu99 -g -c path.c

playerStack.o:
	gcc -Wall -pedantic -std=gnu99 -g -c playerStack.c

site.o:
	gcc -Wall -pedantic -std=gnu99 -g -c site.c

playerInstance.o:
	gcc -Wall -pedantic -std=gnu99 -g -c playerInstance.c

deck.o:
	gcc -Wall -pedantic -std=gnu99 -g -c deck.c

dealerUtils.o:
	gcc -Wall -pedantic -std=gnu99 -g -c dealerUtils.c

clean:
	$(RM) 2310A 2310B 2310dealer *.o *~s4581991@moss:~/csse2310/ass3$
