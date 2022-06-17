# redis client makefile

BUILD   = ./build
OBJ     = main.o sysvar.o weather.o irrigation.o
OBJ_DIR = $(BUILD)/*.o
TARGET  = redisClient

# header file include directory
INCLUDES = -Isysvar -Iweather -Iirrigation
# library file directory
LIBB = -L/usr/local/lib/ -L/usr/lib/x86_64-linux-gnu/

# for library -l library name without lib prefix
$(TARGET): $(OBJ)
	$(CC) $(OBJ_DIR) $(INCLUDES) $(LIBB) -o $@ -lpthread -lhiredis

# -o is to indicate specific folder for obj file
# $< means main.c for example
main.o: main.c
	$(CC) $(INCLUDES) $(LIBB) -c $< -o $(BUILD)/$@

sysvar.o: sysvar/sysvar.c
	$(CC) $(INCLUDES) $(LIBB) -c $< -o $(BUILD)/$@

weather.o: weather/weather.c
	$(CC) $(INCLUDES) $(LIBB) -c $< -o $(BUILD)/$@

irrigation.o: irrigation/irrigation.c
	$(CC) $(INCLUDES) $(LIBB) -c $< -o $(BUILD)/$@


clean:
	rm -rf $(BUILD)/*.o $(TARGET)
