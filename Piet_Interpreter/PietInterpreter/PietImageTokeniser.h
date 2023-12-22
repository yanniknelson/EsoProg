#include <strstream>
#include <iostream>

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

	int GetInstructionNumber();

private:

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

	enum class Hue : uint8_t
	{
		Red,
		Yellow, 
		Green, 
		Cyan, 
		Blue, 
		Magenta, 
		COUNT,
		Black,
		White
	};
	
	enum class Brightness : uint8_t
	{
		Light,
		Standard,
		Dark,
		COUNT
	};

	struct PietColour
	{
		Hue m_hue{ Hue::COUNT };
		Brightness m_Lightness{ Brightness::COUNT };
	};

	/// <summary>
	/// Convert from RGB value to Hue Brightness pair (using struct for readability)
	/// </summary>
	/// <param name="R - "> Red value </param>
	/// <param name="G - "> Green value </param>
	/// <param name="B - "> Blue value </param>
	/// <returns> Hue/Brightness pair </returns>
	const PietColour RGBToColourRep(const char R,const char G,const char B) const;

	PietToken tLastPopped{ PietToken::TokenType::End };
	const unsigned char* m_imageData{ nullptr };
	int m_imageWidth = 0;
	int m_imageHeight = 0;
	int m_instructionNumber = 1;

};