


warmup2: queue.o packet.o bucket.o helper.o warmup2.o list.o
	gcc -Wall -o warmup2 queue.o packet.o bucket.o helper.o list.o warmup2.o  -pthread -D_POSIX_C_SOURCE=199506L

warmup2.o: queue.o packet.o bucket.o helper.o warmup2.c warmup2.h list.o
	gcc -Wall -c -o warmup2.o warmup2.c 

queue.o: queue.c queue.h list.o helper.o
	gcc -Wall -c -o queue.o queue.c

bucket.o: tokenBucket.c tokenBucket.h helper.o list.o
	gcc -Wall -c -o bucket.o tokenBucket.c 

packet.o: packet.c packet.h helper.o list.o
	gcc -Wall -c -o packet.o packet.c 

helper.o: helper.c helper.h list.o
	gcc -Wall -c -o helper.o helper.c

list.o: my402list.c my402list.h cs402.h
	gcc -Wall -c -o list.o my402list.c

clean: 
	rm *.o warmup2
	