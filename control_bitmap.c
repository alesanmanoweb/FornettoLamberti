#include <stdint.h>
#include <stdio.h>

#define range 64

/*
 * We want to avoid using floats but we still need some higher precision;
 * we use unsigned 32 values, shifted left 16 bits, the 16MSB will
 * represent the integer part, the 16LSB the fractional part.
 */
 
/*
 * Rounding algorithm, if the fractional part <= of the equivalent of 0.5
 * round down, otherwise round to the next integer up.
 */
uint32_t rnd_16(uint32_t n)
{
	uint32_t f = n & 0xffff;
	if(f <= 0x7fff)
	{
		return n >> 16;
	}
	return (n >> 16) + 1;
}

int main()
{
	// Opening a file to be written as 24-bit PPM image
	FILE *fp = fopen("bitmap.ppm", "wb");
	fprintf(fp, "P6\n%d %d\n255\n", range, range);

	// we only use two colors
	unsigned char white[3] = {0xff, 0xff, 0xff};
	unsigned char black[3] = {0x00, 0x00, 0x00};

	// each line is a different power level, from the lowest to the highest
	for(int i = 1; i < range; i++)
	{
		// Insteasd of using floats like in the comment below, we use shifted ints
		// float step = (float)range / i;
		uint32_t step = (range << 16) / i;
		uint32_t stepsum = 0;
		printf("Power level %02d, step size 0x%08x: ", i, step);
		int sum = 0;
		// We now cycle through a number "range" of waves and modulate our power output
		for(int j = 0; j < range; j++)
		{
			// If we determine we have to power the load, write a white pixel
			if(j == rnd_16(stepsum))
			{
				fwrite(black, sizeof(black), 1, fp);
				sum++;
				stepsum += step;
			}
			// Otherwise write a black pixel
			else
			{
				fwrite(white, sizeof(black), 1, fp);
			}
		}
		// Checking the actual powr level matches the desired value:
		printf("  sum %03d", sum);
		if(sum != i)
			printf(" -- error!! (%02d) --", i);
		printf("\n");
	}
}
