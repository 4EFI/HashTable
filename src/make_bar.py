import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure( figsize = (12, 8) )
ax = fig.add_subplot()

post_file_name = "ror"

data = []

with open( "res/output_" + post_file_name + ".csv", "r" ) as f:
    data = list( map( int, f.readline().split() ) )

x = [i for i in range( len( data ) )]

xmax = 1000

plt.xlim  ( 0, xmax )
plt.xticks( [i for i in range( 0, xmax + 1, 50 )] )
plt.xlabel( "Индекс списка" )
plt.ylabel( "Количество элементов" )

ax.bar( x, data )

fig.savefig( "res/bar_" + post_file_name + ".png" )

plt.grid()
plt.show()