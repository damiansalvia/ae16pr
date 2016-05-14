include ../../environment

# Se asignan valores por defecto
cant=5 
tasa=0.0 
nomb=ej1
gene=0

all: MainLan MainSeq

clean:
	rm -f MainLan MainSeq MainWan *.o *% *~

MainLan: prAE.req.o prAE.pro.o MainLan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainWan: prAE.req.o prAE.pro.o MainWan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainSeq: prAE.req.o prAE.pro.o MainSeq.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

LAN:
	$(RUN) -np 10 ./MainLan Config.cfg

WAN:
	$(RUN) -v  -p4pg pgfileWan MainWan
	
SEQ:
	./MainSeq prAE.cfg $(cant) $(tasa) $(nomb) $(gene) res/solucion.out