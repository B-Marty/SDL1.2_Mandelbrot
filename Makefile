

all: mandelbrot

mandelbrot : mandelbrot.c		
	gcc mandelbrot.c `sdl-config --cflags --libs` -o $@

clean:	
	rm -fr *.o mandelbrot
