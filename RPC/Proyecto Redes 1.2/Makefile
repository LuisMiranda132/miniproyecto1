.c.o:
	gcc -c -Wall $<
 
all:	scdax_cli scdax_svc
 
scdax_cli:	scdaxcliente.o scdax_clnt.o scdax_xdr.o
	gcc -o client scdaxcliente.o scdax_clnt.o scdax_xdr.o 
 
scdax_svc:	scdaxservidor.o scdax_svc.o scdax_xdr.o
	gcc -o server scdaxservidor.o scdax_svc.o scdax_xdr.o 
 
scdax_clnt.o:	scdax_clnt.c
	gcc -o scdax_clnt.o -c scdax_clnt.c 
 
scdax_xdr.o:	scdax_xdr.c
	gcc -o scdax_xdr.o -c scdax_xdr.c 
 
scdax_svc.o:	scdax_svc.c
	gcc -o scdax_svc.o -c scdax_svc.c 
 
scdax.h:	scdax.x
	rpcgen -C scdax.x
 
clean :
	rm server client *.o scdax_svc.c scdax_xdr.c scdax_clnt.c scdax.h
 
scdaxcliente.c:	scdax.h
scdaxservidor.c:	scdax.h