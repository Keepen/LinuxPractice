all:fifo fifo_write shm_write shm_read
shm_write:shm_write.c
	gcc $^ -o $@
shm_read:shm_read.c
	gcc $^ -o $@
fifo_write:fifo_write.c
	gcc $^ -o $@
fifo:fifo.c
	gcc $^ -o $@
mypipe:mypipe.c
	gcc $^ -o $@
pipe2:pipe2.c
	gcc $^ -o $@
pipe:pipe.c
	gcc $^ -o $@
