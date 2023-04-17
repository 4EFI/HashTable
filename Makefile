include SOURCES

#compiling & linking

OBJ_TMP = $(subst .cpp,.o,       $(SRC))
OBJ     = $(subst src,src/build, $(OBJ_TMP))

#compiling
                                             
link : $(OBJ) 
	$(CC) $(IFLAGS) $(LFLAGS) $(CFLAGS) $^ -o $(OUTPUT_FILE_NAME) $(LIBS)

src/build/%.o : src/%.cpp  	
	echo  $(@D)
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) $(FFLAGS) -c $^ -o $@	

clear :
	rm   -f ./src/build/*.o       