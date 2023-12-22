#include "PietImageTokeniser.h"

const PietToken& PietImageTokeniser::Pop()
{
	return m_tLastPopped;
}

void PietImageTokeniser::SetImage(const unsigned char* imageData, const int width, const int height)
{
	m_imageData = imageData;
	m_imageWidth = width;
	m_imageHeight = height;
	m_instructionNumber = 1;
}

int PietImageTokeniser::GetInstructionNumber()
{
	return m_instructionNumber;
}

const PietImageTokeniser::PietColour PietImageTokeniser::RGBToColourRep(const char R, const char G, const char B) const
{

	/*
	* The below table holds all the colours in the Piet Pallet, you have 6 Hues and 3 Brightnesses
	* and the special cases white and black.
	* |__________________________________________________________________________________|
	* | #FFC0C0   | #FFFFC0      | #C0FFC0     | #C0FFFF    | #C0C0FF    | #FFC0FF       |
	* | light red | light yellow | light green | light cyan | light blue | light magenta |
	* |__________________________________________________________________________________|
	* | #FF0000   | #FFFF00      | #00FF00     | #00FFFF    | #0000FF    | #FF00FF       |
	* | red       | yellow       | green       | cyan       | blue       | magenta       |
	* |__________________________________________________________________________________|
	* | #C00000   | #C0C000      | #00C000     | #00C0C0    | #0000C0    | #C000C0       |
	* | dark red  | dark yellow  | dark green  | dark cyan  | dark blue  | dark magenta  |
	* |__________________________________________________________________________________|
	* |                  #FFFFFF               |                #000000                  |
	* |                  white                 |                black                    |
	* |__________________________________________________________________________________|
	*/

	/* NOTE 1
	* For the Brightnesses, we can notice that all the light colours have both FF and C0 in the RGBs
	* all the standards have only FF and all the darks have only C0.
	*/

	/* NOTE 2
	* For the Hues if we look at the standard hues' hex values:
	* | #FF0000   | #FFFF00      | #00FF00     | #00FFFF    | #0000FF    | #FF00FF       |
	* If we replace all FF with 1 and all 00 with 0 we get
	* | 100 (4)   | 110 (6)      | 010 (4)     | 011 (3)    | 001 (1)    | 101 (5)       |
	* These are the numbers from 1 - 6 (with black being 0 and white being 7)
	* This means we can convert from the RGB to an index in a pre-ordered array of hues
	* 
	* This will only work directly for the standard hues. To make this work for light 
	* colours we can notice that the light hue hex values are the standard hue values with
	* all 00s replaced with C0s. Similarly for dark Hues, the 00s are left as is and the
	* FFs become C0s.
	*/

	static Hue hueTable[8]{ Hue::Black, Hue::Blue, Hue::Green, Hue::Cyan, Hue::Red , Hue::Magenta , Hue::Yellow , Hue::White };

	PietColour retVal;

	int hueIndx = 0;

	bool bHasFF = R == 0xFF || G == 0xFF || B == 0xFF;
	bool bHasC0 = R == 0xC0 || G == 0xC0 || B == 0xC0;

	if (bHasFF && bHasC0)
	{
		retVal.m_Lightness = Brightness::Light;
	}
	else if (bHasFF)
	{
		retVal.m_Lightness = Brightness::Standard;
	}
	else
	{
		retVal.m_Lightness = Brightness::Dark;
		// If the colour is dark all the hex FFs will be C0 (see NOTE 2)
		if (R == 0xC0)
		{
			hueIndx &= 0b100;
		}
		if (G == 0xC0)
		{
			hueIndx &= 0b010;
		}
		if (B == 0xC0)
		{
			hueIndx &= 0b001;
		}
	}

	// See NOTE 2
	if (R == 0xFF)
	{
		hueIndx &= 0b100;
	}
	if (G == 0xFF)
	{
		hueIndx &= 0b010;
	}
	if (B == 0xFF)
	{
		hueIndx &= 0b001;
	}
   
	retVal.m_hue = hueTable[hueIndx];

	return retVal;
}
