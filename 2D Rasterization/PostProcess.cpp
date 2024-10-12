#include "PostProcess.h"


namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer)
	{

		std::for_each(buffer.begin(), buffer.end(), [](color_t& color)
			{
				color.r = 255 - color.r;
				color.g = 255 - color.g;
				color.b = 255 - color.b;
			});

	}
	void Monochrome(std::vector<color_t>& buffer)
	{

		std::for_each(buffer.begin(), buffer.end(), [](color_t& color)
			{

				int gray = (color.r + color.g + color.b)/3;
				color.r = gray;
				color.g = gray;
				color.b = gray;
				
			});
	}
	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo)
	{


		std::for_each(buffer.begin(), buffer.end(), [ro, go , bo](color_t& color)
			{

				
				color.r = std::min(std::max(color.r + ro, 0), 255);
				color.g = std::min(std::max(color.g + go, 0), 255);
				color.b = std::min(std::max(color.b + bo, 0), 255);

			});


	}
	void Brightness(std::vector<color_t>& buffer, int brightness)
	{
		std::for_each(buffer.begin(), buffer.end(), [brightness](color_t& color)
			{

				
				color.r = std::min(255, std::max(0, color.r + brightness));
				color.g = std::min(255, std::max(0, color.g + brightness));
				color.b = std::min(255, std::max(0, color.b + brightness));
				

			});
	}
	void Noise(std::vector<color_t>& buffer, uint8_t noise)
	{

		std::for_each(buffer.begin(), buffer.end(), [noise](color_t& color)
			{

				int offset = (rand() % ((noise * 2) + 1)) - noise;
				color.r += offset;
				color.g += offset;
				color.b += offset;

			});


	}
	void Threshold(std::vector<color_t>& buffer, uint8_t threshold)
	{
		std::for_each(buffer.begin(), buffer.end(), [threshold](color_t& color)
			{
				int average = (color.r + color.g + color.b) / 3;

				if (average > threshold)
				{
					color.r = 0;
					color.g = 0;
					color.b = 0;
				}
				else if (average < threshold)
				{
					color.r = 255;
					color.g = 255;
					color.b = 255;
				}
			
			});
	}
	void Posterize(std::vector<color_t>& buffer, uint8_t levels)
	{
		uint8_t level = 255 / levels;
		std::for_each(buffer.begin(), buffer.end(), [level](color_t& color)
			{




				color.r = (color.r/ level) * level;
				color.g = (color.g/ level) * level;
				color.b = (color.b/ level) * level;

			});
	}
	void BoxBlur(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;
		
		int k[3][3] =
		{
			{ 1, 1, 1},
			{ 1, 1, 1},
			{ 1, 1, 1}
		};
		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			if (x < 1 || x + 1 >= width ||y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x+ ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;

				}
			}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(r / 9);
		color.g = static_cast<uint8_t>(g / 9);
		color.b = static_cast<uint8_t>(b / 9);
		}

	}
}
