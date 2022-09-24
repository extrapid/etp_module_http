CFF= -c  -O -fPIC -I etp_log
etp_module_http.so:sdk.o main.o handClient.o connectClient.o init.o cJSON.o str.o addone.o log.o
	g++ sdk.o main.o handClient.o cJSON.o connectClient.o init.o str.o log.o addone.o -ldl -lpthread -lm  -fPIC  -shared -o etp_module_http.so

main.o:main.cpp ThreadInfo.h cJSON.h addone.h etp_log/log.h
	g++ $(CFF)  main.cpp -o main.o
	

handClient.o:handClient.cpp ThreadInfo.h cJSON.h str.h define.h addone.h etp_log/log.h 
	g++ $(CFF) handClient.cpp -o handClient.o

connectClient.o:connectClient.cpp ThreadInfo.h
	g++ $(CFF) connectClient.cpp -o connectClient.o

init.o:init.cpp cJSON.h ThreadInfo.h addone.h
	g++ $(CFF) init.cpp -o init.o

addone.o:addone.h define.h str.h addone.cpp
	g++ $(CFF) addone.cpp -o addone.o

str.o:str.h define.h str.cpp
	g++ $(CFF) str.cpp -o str.o

log.o:etp_log/log.h etp_log/log.cpp
	g++ $(CFF) etp_log/log.cpp -o log.o
cJSON.o:cJSON.cpp cJSON.h
	g++ $(CFF) cJSON.cpp -o cJSON.o 
sdk.o:etp_sdk/modulesdk.cpp etp_sdk/modulesdk.h etp_sdk/moduletype.h
	g++ $(CFF) etp_sdk/modulesdk.cpp -o sdk.o

clean:
	rm *.o

