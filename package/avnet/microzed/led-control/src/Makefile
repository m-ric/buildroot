
all: led-control

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
	
led-control: led-control.o
	$(CC) $^ -o $@ $(LDFLAGS) 

.PHONY: clean
clean:
	rm -f led-control
