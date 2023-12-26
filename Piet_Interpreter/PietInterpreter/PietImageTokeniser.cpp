#include "PietImageTokeniser.h"

#include <deque>
#include <vector>

const std::string PietImageTokeniser::m_hueStrings[(int)PietImageTokeniser::Hue::COUNT] = {"Red", "Yellow", "Green", "Cyan", "Blue", "Magenta", "Black", "White"};
const std::string PietImageTokeniser::m_brightnessStrings[(int)PietImageTokeniser::Brightness::COUNT] = { "Light", "", "Dark"};

const PietToken& PietImageTokeniser::Pop()
{
	return m_tLastPopped;
}

void PietImageTokeniser::SetImage(const unsigned char* imageData, const int width, const int height)
{
	Reset();
	m_imageData = imageData;
	m_imageWidth = width;
	m_imageHeight = height;
	m_instructionNumber = 1;

	Location previous = { -1,-1 };
	while (previous != m_currentCodel)
	{
		std::cout << m_currentCodel << std::endl;
		previous = m_currentCodel;
		m_currentCodel = GetNextCodel();
	}
}

void PietImageTokeniser::Reset()
{
	m_direction = Direction::Right;
	m_codelChooser = Direction::Left;
	m_currentCodel = { 0, 0 };
}

int PietImageTokeniser::GetInstructionNumber()
{
	return m_instructionNumber;
}

PietImageTokeniser::PietColour PietImageTokeniser::RGBToColourRep(const PietImageTokeniser::RGB colour)
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

	bool bHasFF = colour.r == 0xFF || colour.g == 0xFF || colour.b == 0xFF;
	bool bHasC0 = colour.r == 0xC0 || colour.g == 0xC0 || colour.b == 0xC0;

	if (bHasFF && bHasC0)
	{
		retVal.m_brightness = Brightness::Light;
	}
	else if (bHasFF)
	{
		retVal.m_brightness = Brightness::Standard;
	}
	else
	{
		retVal.m_brightness = Brightness::Dark;
		// If the colour is dark all the hex FFs will be C0 (see NOTE 2)
		if (colour.r == 0xC0)
		{
			hueIndx |= 0b100;
		}
		if (colour.g == 0xC0)
		{
			hueIndx |= 0b010;
		}
		if (colour.b == 0xC0)
		{
			hueIndx |= 0b001;
		}
	}

	// See NOTE 2
	if (colour.r == 0xFF)
	{
		hueIndx |= 0b100;
	}
	if (colour.g == 0xFF)
	{
		hueIndx |= 0b010;
	}
	if (colour.b == 0xFF)
	{
		hueIndx |= 0b001;
	}
   
	retVal.m_hue = hueTable[hueIndx];

	return retVal;
}

PietImageTokeniser::RGB PietImageTokeniser::GetRGBFromLoation(const PietImageTokeniser::Location& loc) const
{
	RGB retCol;

	if (loc. y < m_imageHeight && loc.x < m_imageWidth)
	{
		int indx = (m_imageWidth * loc.y + loc.x) * 4;

		retCol.r = m_imageData[indx];
		retCol.g = m_imageData[indx + 1];
		retCol.b = m_imageData[indx + 2];
	}

	return retCol;
}

PietImageTokeniser::Location PietImageTokeniser::MoveInDirection(Location loc, Direction dir) const
{
	switch (dir)
	{
	case (Direction::Up):
	{
		return Location{ loc.x, loc.y - 1 };
		break;
	}
	case (Direction::Down):
	{
		return Location{ loc.x, loc.y + 1 };
		break;
	}
	case (Direction::Left):
	{
		return Location{ loc.x - 1, loc.y };
		break;
	}
	case (Direction::Right):
	{
		return Location{ loc.x + 1, loc.y };
		break;
	}
	}

	return loc;
}

bool PietImageTokeniser::isValidLocation(const Location& loc) const
{
	if (loc.x < 0 || loc.x > m_imageWidth - 1 || loc.y < 0 || loc.y > m_imageHeight - 1)
	{
		return false;
	}
	return true;
}

bool PietImageTokeniser::isValidLocation(const Location& loc, const std::vector<std::vector<bool>>& visited) const
{
	return isValidLocation(loc) && !visited[loc.x][loc.y];
}

int PietImageTokeniser::GetSizeOfCurrentBlock() const
{
	std::vector<std::vector<bool>> visited(m_imageWidth, std::vector<bool>(m_imageHeight, false));

	int count = 0;
	const RGB blockCol = GetRGBFromLoation(m_currentCodel);
	visited[m_currentCodel.x][m_currentCodel.y] = true;

	std::deque<PietImageTokeniser::Location> ToVisit = { m_currentCodel };

	while (!ToVisit.empty())
	{
		const PietImageTokeniser::Location focus = ToVisit.front();
		ToVisit.pop_front();
		if (GetRGBFromLoation(focus) == blockCol)
		{
			count++;


			for (int dir = 0; dir < (int)Direction::Count; dir++)
			{
				if (isValidLocation(MoveInDirection(focus, (Direction)dir), visited))
				{
					ToVisit.push_back(MoveInDirection(focus, (Direction)dir));
					visited[ToVisit.back().x][ToVisit.back().y] = true;
				}
			}
		}
	}

	return count;
}

PietImageTokeniser::Location PietImageTokeniser::GetNextCodel()
{
	static Direction clockwiseDirectionLookup[(int)Direction::Count] = {Direction::Right, Direction::Left, Direction::Up, Direction::Down};

	int attempts = 0;
	bool switchCodelChooser = true;
	PietImageTokeniser::Location currentCorner = m_currentCodel;
	while (attempts < 8)
	{
		if (switchCodelChooser)
		{
			currentCorner = FindEndOfEdge(FindFurthestEdge(m_currentCodel, m_direction), m_direction, m_codelChooser);
		}
		else
		{
			currentCorner = FindEndOfEdge(currentCorner, m_direction, m_codelChooser);
		}
		
		PietImageTokeniser::Location nxt = MoveInDirection(currentCorner, m_direction);
		if (isValidLocation(nxt) && GetRGBFromLoation(nxt) != 0)
		{
			return nxt;
		}

		if (switchCodelChooser)
		{
			if (m_codelChooser == Direction::Left)
			{
				m_codelChooser = Direction::Right;
			}
			else
			{
				m_codelChooser = Direction::Left;
			}
		}
		else
		{
			m_direction = clockwiseDirectionLookup[(int)m_direction];
		}

		switchCodelChooser =! switchCodelChooser;
		attempts++;
	}


	return m_currentCodel;
}

PietImageTokeniser::Location PietImageTokeniser::FindFurthestEdge(Location currentLoc, Direction direction) const
{
	std::vector<std::vector<bool>> visited(m_imageWidth, std::vector<bool>(m_imageHeight, false));

	const RGB blockCol = GetRGBFromLoation(currentLoc);
	visited[currentLoc.x][currentLoc.y] = true;

	PietImageTokeniser::Location currentWaveFront = currentLoc;

	std::deque<PietImageTokeniser::Location> ToVisit = { currentLoc };

	while (!ToVisit.empty())
	{
		const PietImageTokeniser::Location focus = ToVisit.front();
		ToVisit.pop_front();
		if (GetRGBFromLoation(focus) == blockCol)
		{

			switch (direction)
			{
			case (Direction::Up):
			{
				if ( focus.y < currentWaveFront.y)
				{
					currentWaveFront = focus;
				}
				break;
			}
			case (Direction::Down):
			{
				if (focus.y > currentWaveFront.y)
				{
					currentWaveFront = focus;
				}
				break;
			}
			case (Direction::Left):
			{
				if (focus.x < currentWaveFront.x)
				{
					currentWaveFront = focus;
				}
				break;
			}
			case (Direction::Right):
			{
				if (focus.x > currentWaveFront.x)
				{
					currentWaveFront = focus;
				}
				break;
			}
			}

			for (int dir = 0; dir < (int)Direction::Count; dir++)
			{
				if (isValidLocation(MoveInDirection(focus, (Direction)dir), visited))
				{
					ToVisit.push_back(MoveInDirection(focus, (Direction)dir));
					visited[ToVisit.back().x][ToVisit.back().y] = true;
				}
			}
		}
	}

	return currentWaveFront;
}

PietImageTokeniser::Location PietImageTokeniser::FindEndOfEdge(Location EdgeLoc, Direction direction, Direction chooser) const
{
	static PietImageTokeniser::Direction searchDirLookup[4][2] = { /*Up*/{/*Left*/Direction::Left, /*Right*/Direction::Right} 
																 , /*Down*/{/*Left*/Direction::Right, /*Right*/Direction::Left}
																 , /*Left*/{/*Left*/Direction::Down, /*Right*/Direction::Up}
																 , /*Right*/{/*Left*/Direction::Up, /*Right*/Direction::Down} };

	PietImageTokeniser::Direction SearchDir = searchDirLookup[(int)direction][(int)chooser - (int)Direction::Left];

	const RGB blockCol = GetRGBFromLoation(EdgeLoc);

	PietImageTokeniser::Location focus = EdgeLoc;

	while (isValidLocation(MoveInDirection(focus, SearchDir)) ? GetRGBFromLoation(MoveInDirection(focus, SearchDir)) == blockCol : false)
	{
		focus = MoveInDirection(focus, SearchDir);
	}

	return focus;
}