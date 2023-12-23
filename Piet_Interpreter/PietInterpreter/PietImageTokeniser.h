#include <strstream>
#include <iostream>
#include <iomanip>

#include "PietTokeniser.h"

class PietImageTokeniser : PietTokeniser
{
public:

	/// <summary>
	/// Get the next token
	/// </summary>
	/// <returns> The next token </returns>
	virtual const PietToken& Pop() override;

	PietImageTokeniser() {}

	void SetImage(const unsigned char* imageData, const int width, const int height);
	void Reset();

	int GetInstructionNumber();

	/*
	* Bellow is a table representing the Piet colour pallette
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

	static enum class Hue : uint8_t
	{
		Red,
		Yellow,
		Green,
		Cyan,
		Blue,
		Magenta,
		Black,
		White,
		COUNT
	};

	static const std::string m_hueStrings[(int)Hue::COUNT];


	static enum class Brightness : uint8_t
	{
		Light,
		Standard,
		Dark,
		COUNT
	};

	static const std::string m_brightnessStrings[(int)Brightness::COUNT];

	struct PietColour
	{
		Hue m_hue{ Hue::COUNT };
		Brightness m_brightness{ Brightness::COUNT };
	};

	class RGB;

	/// <summary>
	/// Convert from RGB value to Hue Brightness pair (using struct for readability)
	/// </summary>
	/// <param name="RGB - "> RGB colour info </param>
	static PietColour RGBToColourRep(const RGB col);

	class RGB
	{
	public:
		unsigned char r = 0.f;
		unsigned char g = 0.f;
		unsigned char b = 0.f;

		friend std::ostream& operator<<(std::ostream& os, const RGB& col)
		{
			const int outint = (((col.r << 8) | col.g) << 8) | col.b;
			PietImageTokeniser::PietColour pietCol = PietImageTokeniser::RGBToColourRep(col);
			os << std::hex << outint << "(" << PietImageTokeniser::m_brightnessStrings[(int)pietCol.m_brightness] << " " << PietImageTokeniser::m_hueStrings[(int)pietCol.m_hue] << ")";
			return os;
		}

		friend bool operator==(const RGB& col1, const RGB& col2)
		{
			return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b;
		}

	};

private:	

	PietToken tLastPopped{ PietToken::TokenType::End };
	const unsigned char* m_imageData{ nullptr };
	int m_imageWidth = 0;
	int m_imageHeight = 0;
	int m_instructionNumber = 1;

	enum class Direction
	{
		Up, Right, Down, Left
	};

	struct Location
	{
		int x = 0;
		int y = 0;
	};

	Direction m_direction{ Direction::Right };
	Direction m_codelChooser{Direction::Left};
	Location m_currentCodel{ 0, 0 };

	RGB GetRGBFromLoation(const Location& loc) const;

};