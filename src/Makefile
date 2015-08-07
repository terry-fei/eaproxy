eaproxy: control_lan.o control_wan.o handle_packet.o eaproxy.o
	$(CC) $(LDFLAGS) control_lan.o eaproxy.o -lpthread -o eaproxy
control_lan.o: control_lan.c eaproxy.h
	$(CC) $(CFLAGS) -c control_lan.c
control_wan.o: control_wan.c eaproxy.h
	$(CC) $(CFLAGS) -c control_wan.c
handle_packet.o: handle_packet.c eaproxy.h
	$(CC) $(CFLAGS) -c handle_packet.c

clean:
	rm *.o eaproxy
