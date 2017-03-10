// gcc brailleplot.c -o brailleplot -std=c99
// python -c "import math; print('\n'.join(['{} {}'.format(i, math.sin(i/20.0)) for i in range(0, 400)]))" | ./brailleplot
// open https://u.teknik.io/QWvJx.png
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

wchar_t to_braille(unsigned char byte) {
  return 10240 + (
  ((byte >> 7) & (1 << 0)) |
  ((byte >> 3) & (1 << 3)) |
  ((byte >> 4) & (1 << 1)) |
  ((byte >> 0) & (1 << 4)) |
  ((byte >> 1) & (1 << 2)) |
  ((byte << 3) & (1 << 5)) |
  ((byte << 5) & (1 << 6)) |
  ((byte << 7) & (1 << 7))
  );
}

unsigned char from_braille(wchar_t c) {
  char byte = (c - 10240) & 0xFF;
  return 
  ((byte << 7) & (1 << 7)) |
  ((byte << 3) & (1 << 6)) |
  ((byte << 4) & (1 << 5)) |
  ((byte << 0) & (1 << 4)) |
  ((byte << 1) & (1 << 3)) |
  ((byte >> 3) & (1 << 2)) |
  ((byte >> 5) & (1 << 1)) |
  ((byte >> 7) & (1 << 0));
}

wchar_t add_pixel(wchar_t c, int row, int col, int on) {
  unsigned char byte = from_braille(c);
  if (on) {
    return to_braille(byte | (1 << (2 * row + col)));
  } else {
    return to_braille(byte & (~(1 << (2 * row + col))));
  }
}

#define dot(x, y) buffer[(h - (int)(y) - 1) * w + (int)(x)] = 1

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");
  int h = 100, w = 300;
  if (argc == 3) {
    sscanf(argv[1], "%d", &h);
    sscanf(argv[2], "%d", &w);
  }
	h = h / 3 * 3;
	w = w / 2 * 2;
  char* buffer = (char*)calloc(h * w, sizeof(char));

  float(*plot)[2] = (float(*)[2])calloc(10, sizeof(float) * 2);
  int num_data = 0;
  int max_data = 10;
	int x, y;

	while (scanf("%f %f", &(plot[num_data][0]), &(plot[num_data][1])) == 2) {
    if (++num_data > max_data / 2) {
			max_data *=2;
      plot = realloc(plot, max_data * 2 * sizeof(float));
    }
  }

  float max_y = 0;
  float min_y = 0;
  float max_x = 0;
  float min_x = 0;
  for (int i = 0; i < num_data; ++i) {
    if (plot[i][1] < min_y) {
      min_y = plot[i][1];
		} else if (plot[i][1] > max_y) {
			max_y = plot[i][1];
		}
    if (plot[i][0] < min_x) {
      min_x = plot[i][0];
		} else if (plot[i][0] > max_x) {
			max_x = plot[i][0];
		}
  }

  for (int i = 0; i < num_data; ++i) {
    float scale_y = max_y - min_y;
    float scale_x = max_x - min_x;
    dot(((w - 1) * (plot[i][0] - min_x)) / scale_x, ((h - 1) * (plot[i][1] - min_y)) / scale_y);
  }

	for (int i = 0; i + 2 < h; i+=3) {
		for (int j = 0; j + 1 < w; j+=2) {
			wchar_t c = (wchar_t)10240;
			for (int _i = 0; _i < 3; ++_i) {
				for (int _j = 0; _j < 2; ++_j) {
					if (buffer[(i + _i) * w + (j + _j)]) {
						c = add_pixel(c, 2-_i, 1-_j, 1);
					}
				}
			}
			wprintf(L"%C", c);
		}
		wprintf(L"\n");
	}
  free(buffer);
  free(plot);
}
