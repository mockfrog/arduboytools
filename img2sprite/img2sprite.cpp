/*
arduboytools
img2sprite
converts images to byte array for usage in arduboy programming
author: mockfrog
license: GNU GPL v3

build with: g++ img2sprite.cpp -o img2sprite $(Magick++-config --cppflags --cxxflags --ldflags --libs)
*/

#include <iostream>
#include <iomanip>
#include <Magick++.h>

using namespace std;
using namespace Magick;

int main(int argc, char** argv)
{
	InitializeMagick(*argv);

	if (argc < 2)
	{
		cerr << "usage: " << argv[0] << " imagefile ..." << endl;
		return(1);
	}

	for (int i = 1; i < argc; ++i)
	{
		Image image;
		image.read(argv[i]);
		Geometry size = image.size();

		string name = argv[i];
		int pos = name.rfind('.');
		if (pos != string::npos)
		{
			name = name.substr(0,pos);
		}
		cout << "/* file: " << argv[i] << " width: " << size.width() << " height: " << size.height() << " */" << endl;
		cout << "PROGMEM const unsigned char " << name << "[] = ";
		bool first(true);
		cout << "{";
		for (int y = 0; y < size.height(); y += 8)
		{
			for (int x = 0; x < size.width(); ++x)
			{
				PixelPacket* pp = image.getPixels(x, y, 1, 8);
				unsigned char b(0);
				for(int i = 0; i < 8; ++i)
				{
					if ( pp[i].red > 0 || pp[i].green > 0 || pp[i].blue > 0)
					{
						b |= (1 << i);
					}
				}
				if(!first)
				{
					cout << ", ";
				}

				cout << "0x"<< hex << setw(2) << setfill('0') << int(b) << dec;
				first = false;
			}
		}
		cout << "};" << endl;
	}

	return(0);
}
