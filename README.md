# brailleplot

A screen emulated in braille unicode characters.  Send coordinates through `stdin` and it prints to `stdout`.

# Build

    make
    
# Use

Pipe `x y` coordinates separated by newlines.

    python -c "print '\n'.join(['{} {}'.format(i, i**3) for i in range(-100,100)])" | ./brailleplot 100 280
    
![](https://u.teknik.io/R38sf.png)

# TODO
 - write todo
