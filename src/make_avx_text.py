
input_file  = open( "res/hamlet.txt",     "r" )
output_file = open( "res/hamlet_avx.txt", "w" )

text = input_file.read()

i = 0
while( True ):
    is_break      = 0
    curr_word_len = 0

    for j in range( 0, 32 ):
        if( i >= len( text ) ):
            is_break = True
            break

        if( not text[i].isalpha() ):
            i += 1
            break
        
        output_file.write( text[i] )

        i += 1
        curr_word_len += 1


    if( curr_word_len != 0 ):
        for j in range( curr_word_len, 32 ):
            output_file.write( '\0' )
    
    if( is_break ): break

input_file.close()
output_file.close()

