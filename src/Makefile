eaproxy: control_lan.o eaproxy.o
	$(CC) $(LDFLAGS) control_lan.o eaproxy.o -lpthread -o eaproxy
control_lan.o: control_lan.c eaproxy.h
	$(CC) $(CFLAGS) -c control_lan.c
control_wan.o: control_wan.c eaproxy.h
	$(CC) $(CFLAGS) -c control_wan.c

clean:
	rm *.o eaproxy

