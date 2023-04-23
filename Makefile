include SOURCES

#compiling & linking

OBJ_TMP = $(subst .cpp,.o,       $(SRC))
OBJ     = $(subst src,src/build, $(OBJ_TMP))

#compiling
                                             
link : $(OBJ) src/build/BKDR_hash_function.o
	$(CC) $(IFLAGS) $(LFLAGS) $(CFLAGS) $^ -o $(OUTPUT_FILE_NAME) $(LIBS)

src/build/%.o : src/%.cpp  	
	echo  $(@D)
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) $(FFLAGS) -c $^ -o $@	

src/build/BKDR_hash_function.o : src/BKDR_hash_function.asm
	nasm -f elf64 -o $@	$^

clear :
	rm   -f ./src/build/*.o       