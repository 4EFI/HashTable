import matplotlib.pyplot as plt
import numpy as np

def DrawBar( file_name, x_size, y_size ):
    
    fig = plt.figure( figsize = (6, 4) )
    ax = fig.add_subplot()
    
    data = []

    with open( "res/output_" + file_name + ".csv", "r" ) as f:
        data = list( map( int, f.readline().split() ) )

    x = [i for i in range( len( data ) )]

    print( np.var( data ) )

    plt.xlim  ( -5, x_size )
    plt.ylim  (  0, y_size )
    #plt.xticks( [i for i in range( 0, x_size + 1, 100 )] )
    plt.xlabel( "Индекс списка" )
    plt.ylabel( "Количество элементов" )

    ax.bar( x, data, width = 1 )
    plt.grid()

    fig.savefig( "res/bar_" + file_name + ".png" )

x_size = 1001
y_size = 30

file_names = [ "const", "first_ascii", "len_word", "sum_ascii", "sum_ascii_5000", "rol",  "ror",  "bkdr", "crc32" ]
x_sizes    = [ 200,     200,           200,        x_size,      5000,             x_size, x_size, x_size, x_size    ]
y_sizes    = [ 6000,    1000,          1000,       y_size,      y_size,           y_size, 115,    y_size, y_size    ]

for i in range( len( file_names ) ):
    DrawBar( file_names[i], x_sizes[i], y_sizes[i] )
