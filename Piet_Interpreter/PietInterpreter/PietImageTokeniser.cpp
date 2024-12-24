#include "PietImageTokeniser.h"

#include <deque>
#include <vector>
#include <map>

PietImageTokeniser::Direction PietImageTokeniser::i_clockwiseDirectionLookup[(int)Direction::Count] = { Direction::Right, Direction::Left, Direction::Up, Direction::Down };
char* PietImageTokeniser::i_directionStrings[static_cast<int>(Direction::Count) + 1] = {"Up", "Down", "Left", "Right", "Invalid"};

const PietToken& PietImageTokeniser::Pop()
{
	m_currentBlock = GetBlockInfo(m_currentBlock.m_endingCodel, m_currentBlock.m_endingDirectionPointer, m_currentBlock.m_endingCodelChooser);

	if (m_currentBlock.m_startingCodel == m_currentBlock.m_endingCodel)
	{
		m_tLastPopped.m_type = PietToken::TokenType::End;
		return m_tLastPopped;
	}

	PietColour nextCol = GetPietColourFromLocation(m_currentBlock.m_endingCodel);
	m_tLastPopped.m_type = ConvertColoursToInstruction(m_currentBlock.m_pietColour, nextCol);
	if (m_tLastPopped.m_type == PietToken::TokenType::Push)
	{
		m_tLastPopped.m_value = m_currentBlock.m_size;
	}

	return m_tLastPopped;
}

void PietImageTokeniser::SetImage(const unsigned char* imageData, const int width, const int height)
{
	Reset();
	m_imageData = imageData;
	m_imageWidth = width;
	m_imageHeight = height;
	m_instructionNumber = 1;
}

void PietImageTokeniser::UnsetImage()
{
	Reset();
	m_imageData = nullptr;
	m_imageWidth = 0;
	m_imageHeight = 0;
	m_instructionNumber = 1;
}

void PietImageTokeniser::SetCodelSize(const int size)
{
	m_codelSize = size;
}

void PietImageTokeniser::Reset()
{
	m_currentBlock = {};
}

int PietImageTokeniser::GetInstructionNumber()
{
	return m_instructionNumber;
}

void PietImageTokeniser::ToggleCodelChooser()
{
	PietImageTokeniser::ToggleCodelChooser(m_currentBlock);
}

void PietImageTokeniser::RotateDirectionPointer(const int times)
{
	PietImageTokeniser::RotateDirectionPointer(m_currentBlock, times);
}

PietImageTokeniser::Location PietImageTokeniser::GetCurrentBlockStartLocation() const
{
	return m_currentBlock.m_startingCodel;
}

PietImageTokeniser::Location PietImageTokeniser::GetCurrentBlockEndLocation() const
{
	return m_currentBlock.m_endingCodel;
}

PietImageTokeniser::Direction PietImageTokeniser::GetCurrentBlockStartDirectionPointer() const
{
	return m_currentBlock.m_startingDirectionPointer;
}

PietImageTokeniser::Direction PietImageTokeniser::GetCurrentBlockEndDirectionPointer() const
{
	return m_currentBlock.m_endingDirectionPointer;
}

PietImageTokeniser::Direction PietImageTokeniser::GetCurrentBlockStartCodelChoser() const
{
	return m_currentBlock.m_startingCodelChooser;
}

PietImageTokeniser::Direction PietImageTokeniser::GetCurrentBlockEndCodelChoser() const
{
	return m_currentBlock.m_endingCodelChooser;
}

PietImageTokeniser::PietColour PietImageTokeniser::RGBToPietColour(const PietImageTokeniser::RGB colour)
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

inline PietImageTokeniser::Location PietImageTokeniser::MoveInDirection(const Location loc, const Direction dir)
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

/*inline PietImageTokeniser::Location PietImageTokeniser::GetEdge(const BlockInfo& block, const Direction edge)
{
	switch (edge)
	{
	case (Direction::Up):
	{
		return block.m_topEdge;
		break;
	}
	case (Direction::Down):
	{
		return block.m_bottomEdge;
		break;
	}
	case (Direction::Left):
	{
		return block.m_leftEdge;
		break;
	}
	case (Direction::Right):
	{
		return block.m_rightEdge;
		break;
	}
	}
	return { -1,-1 };
}*/

void PietImageTokeniser::ToggleCodelChooser(BlockInfo& blockInfo)
{
	if (blockInfo.m_endingCodelChooser == Direction::Left)
	{
		blockInfo.m_endingCodelChooser = Direction::Right;
	}
	else
	{
		blockInfo.m_endingCodelChooser = Direction::Left;
	}
}

void PietImageTokeniser::RotateDirectionPointer(BlockInfo& blockInfo, const int times)
{
	for (int rotation = 0; rotation < times % 4; rotation++)
	{
		blockInfo.m_endingDirectionPointer = i_clockwiseDirectionLookup[(int)blockInfo.m_endingDirectionPointer];
	}
}

PietToken::TokenType PietImageTokeniser::ConvertColoursToInstruction(const PietColour colour1, const PietColour colour2)
{
	static const PietToken::TokenType conversionTable[6][3] = { {PietToken::TokenType::NOP, PietToken::TokenType::Push, PietToken::TokenType::Pop}
														, {PietToken::TokenType::Add, PietToken::TokenType::Subtract, PietToken::TokenType::Multiply}
														, {PietToken::TokenType::Divide, PietToken::TokenType::Modulo, PietToken::TokenType::Not}
														, {PietToken::TokenType::Greater, PietToken::TokenType::Pointer, PietToken::TokenType::Switch}
														, {PietToken::TokenType::Duplicate, PietToken::TokenType::Roll, PietToken::TokenType::Input_Val}
														, {PietToken::TokenType::Input_Char, PietToken::TokenType::Output_Val, PietToken::TokenType::Output_Char} };

	// White acts as a reset for the colour, moving into it 'clears' the current colour,
	// so we can't execute when we move into or out of it, thus return NOP
	if (colour1.m_hue == Hue::White || colour2.m_hue == Hue::White)
	{
		return PietToken::TokenType::NOP;
	}

	int hueDiffIndx = (int)colour2.m_hue - (int)colour1.m_hue;
	if (hueDiffIndx < 0)
	{
		hueDiffIndx += (int)Hue::COUNT-2;
	}

	int brightnessDiffIndx = (int)colour2.m_brightness - (int)colour1.m_brightness;
	if (brightnessDiffIndx < 0)
	{
		brightnessDiffIndx += (int)Brightness::COUNT;
	}

	return conversionTable[hueDiffIndx][brightnessDiffIndx];
}

PietImageTokeniser::RGB PietImageTokeniser::GetRGBFromLoation(const PietImageTokeniser::Location& loc) const
{
	RGB retCol;

	if (loc.y < m_imageHeight && loc.y > -1 && loc.x < m_imageWidth && loc.x > -1)
	{
		int indx = (m_imageWidth * loc.y + loc.x) * 4;

		retCol.r = m_imageData[indx];
		retCol.g = m_imageData[indx + 1];
		retCol.b = m_imageData[indx + 2];
	}

	return retCol;
}

PietImageTokeniser::PietColour PietImageTokeniser::GetPietColourFromLocation(const PietImageTokeniser::Location& loc) const
{
	return RGBToPietColour(GetRGBFromLoation(loc));
}

PietImageTokeniser::Location PietImageTokeniser::SlideAlongWhite(const Location loc, const Direction dir)
{
	Location currLocation = loc;
	while (true)
	{
		const Location nextLocation = MoveInDirection(currLocation, dir);
		if (GetPietColourFromLocation(nextLocation).m_hue == Hue::White)
		{
			currLocation = nextLocation;
		}
		else
		{
			break;
		}
	}
	return currLocation;
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

PietImageTokeniser::Location PietImageTokeniser::FindEndOfEdge(const Location EdgeLoc, const Direction directionPointer, const Direction chooser) const
{
	// Since the chooser direction is relative to the direciton pointer, we can use them to find the global real direction
	static PietImageTokeniser::Direction searchDirLookup[4][2] = { /*Up*/{/*Left*/Direction::Left, /*Right*/Direction::Right}
																 , /*Down*/{/*Left*/Direction::Right, /*Right*/Direction::Left}
																 , /*Left*/{/*Left*/Direction::Down, /*Right*/Direction::Up}
																 , /*Right*/{/*Left*/Direction::Up, /*Right*/Direction::Down} };

	PietImageTokeniser::Direction SearchDir = searchDirLookup[(int)directionPointer][(int)chooser - (int)Direction::Left];

	// The edge ends when the colour changes so we check the block ahead of us before we move, if its a different colour 
	// (or no longer valid) we return our current location otherwise we move to it
	const RGB blockCol = GetRGBFromLoation(EdgeLoc);

	PietImageTokeniser::Location focus = EdgeLoc;

	while (isValidLocation(MoveInDirection(focus, SearchDir)) ? GetRGBFromLoation(MoveInDirection(focus, SearchDir)) == blockCol : false)
	{
		focus = MoveInDirection(focus, SearchDir);
	}

	return focus;
}

void PietImageTokeniser::FindEndCodel(BlockInfo& rBlockInfo)
{
	int attempts = 0;
	bool switchCodelChooser = true;
	PietImageTokeniser::Location currentCorner = rBlockInfo.m_startingCodel;
	while (attempts < 8)
	{
		// To find the end codel we go to the edge furthest in the direction we're pointing, go to the end of that edge chosen by the codel chooser
		// then try to move in the direction we're facing, if the location is valid and the colour isn't black then that's the next block and our end location
		rBlockInfo.m_leavingCodel = rBlockInfo.m_edgePositions[static_cast<int>(rBlockInfo.m_endingDirectionPointer)][static_cast<int>(rBlockInfo.m_endingCodelChooser) - static_cast<int>(Direction::Left)];//FindEndOfEdge(GetEdge(rBlockInfo, rBlockInfo.m_endingDirectionPointer), rBlockInfo.m_endingDirectionPointer, rBlockInfo.m_endingCodelChooser);
		rBlockInfo.m_endingCodel = MoveInDirection(rBlockInfo.m_leavingCodel, rBlockInfo.m_endingDirectionPointer);

		if (isValidLocation(rBlockInfo.m_endingCodel) && GetRGBFromLoation(rBlockInfo.m_endingCodel) != 0)
		{
			rBlockInfo.m_endingDirectionPointer;
			rBlockInfo.m_endingCodelChooser;
			return;
		}

		// If that wasn't the right location then we flip the codel chooser or the direction pointer (first fail we flip codel chooser, second direction, third codel chooser again etc...)
		// once we've done this 8 times we've exhuated all possible exit points there is no end point and the program should end.
		if (switchCodelChooser)
		{
			ToggleCodelChooser(rBlockInfo);
		}
		else
		{
			RotateDirectionPointer(rBlockInfo, 1);
		}

		switchCodelChooser = !switchCodelChooser;
		attempts++;
	}

	//Set the ending codel back to the startingCodel to indicate we can't leave this block
	rBlockInfo.m_endingCodel = rBlockInfo.m_startingCodel;
}

void PietImageTokeniser::FindEndWhiteCodel(BlockInfo& rBlockInfo)
{
	Location currLocation = rBlockInfo.m_startingCodel;
	Direction currDirectionPointer = rBlockInfo.m_startingDirectionPointer;
	Direction currCodelChoser = rBlockInfo.m_startingCodelChooser;

	std::map<Location, bool[4]> pathTrace;
	while (true)
	{
		const Location nextLocation = SlideAlongWhite(currLocation, currDirectionPointer);
		const Location nextBlockLoc = MoveInDirection(nextLocation, currDirectionPointer);
		if (GetPietColourFromLocation(nextBlockLoc).m_hue == Hue::Black)
		{
			currCodelChoser = currCodelChoser == Direction::Left ? Direction::Right : Direction::Left;
			currDirectionPointer = i_clockwiseDirectionLookup[(int)currDirectionPointer];
			currLocation = nextLocation;
		}
		else
		{
			currLocation = nextBlockLoc;
			break;
		}

		if (pathTrace.find(nextLocation) != pathTrace.end())
		{
			if (pathTrace[nextLocation][(int)currDirectionPointer])
			{
				rBlockInfo.m_endingCodel = rBlockInfo.m_startingCodel;
				return;
			}
			else
			{
				pathTrace[nextLocation][(int)currDirectionPointer] = true;
			}
		}	
		else
		{
			pathTrace[nextLocation][0] = false;
			pathTrace[nextLocation][1] = false;
			pathTrace[nextLocation][2] = false;
			pathTrace[nextLocation][3] = false;
			pathTrace[nextLocation][(int)currDirectionPointer] = true;
		}
	}

	rBlockInfo.m_endingCodel = currLocation;
	rBlockInfo.m_endingDirectionPointer = currDirectionPointer;
	rBlockInfo.m_endingCodelChooser = currCodelChoser;

}

PietImageTokeniser::BlockInfo PietImageTokeniser::GetBlockInfo(const Location startLocation, const Direction startDirectionPointer, const Direction StartCodelChooser)
{
	BlockInfo retInfo(startLocation, startDirectionPointer, StartCodelChooser);

	retInfo.m_rgbColour = GetRGBFromLoation(retInfo.m_startingCodel);
	retInfo.m_pietColour = RGBToPietColour(retInfo.m_rgbColour);

	if (retInfo.m_pietColour.m_hue == Hue::White)
	{
		FindEndWhiteCodel(retInfo);
		return retInfo;
	}

	std::vector<std::vector<bool>> visited(m_imageWidth, std::vector<bool>(m_imageHeight, false));
	visited[retInfo.m_startingCodel.x][retInfo.m_startingCodel.y] = true;
	std::deque<PietImageTokeniser::Location> ToVisit = { retInfo.m_startingCodel };

	// Run through all the pixels of the shape using floodfill
	// count the pixles to find the size, find the highest/lowest/leftmost/rightmost locations in the block
	// we stop flooding when we detect a change in colour
	while (!ToVisit.empty())
	{
		const PietImageTokeniser::Location focus = ToVisit.front();
		ToVisit.pop_front();
		if (GetRGBFromLoation(focus) == retInfo.m_rgbColour)
		{
			retInfo.m_size++;

			// if the current pixel is on the top edge check if it's our new left or right corner of said edge
			if (focus.y == retInfo.m_edgePositions[static_cast<int>(Direction::Up)][0].y)
			{
				// dir up cc left (the objective top left corner)
				if (focus.x < retInfo.m_edgePositions[static_cast<int>(Direction::Up)][0].x)
				{
					retInfo.m_edgePositions[static_cast<int>(Direction::Up)][0] = focus;
				}

				// dir up cc right (the objective top right corner)
				if (focus.x > retInfo.m_edgePositions[static_cast<int>(Direction::Up)][1].x)
				{
					retInfo.m_edgePositions[static_cast<int>(Direction::Up)][1] = focus;
				}
			}
			// otherwise if the current pixel is higher than the top edge then it is our new top edge (both corners)
			else if (focus.y < retInfo.m_edgePositions[static_cast<int>(Direction::Up)][0].y)
			{
				retInfo.m_edgePositions[static_cast<int>(Direction::Up)][0] = focus;
				retInfo.m_edgePositions[static_cast<int>(Direction::Up)][1] = focus;
			}
			
			// if the current pixel is on the bottom edge check if it's our new left or right corner of said edge
			if (focus.y == retInfo.m_edgePositions[static_cast<int>(Direction::Down)][0].y)
			{
				// dir down cc left (the objective bottom right corner)
				if (focus.x > retInfo.m_edgePositions[static_cast<int>(Direction::Down)][0].x)
				{
					retInfo.m_edgePositions[static_cast<int>(Direction::Down)][0] = focus;
				}

				// dir down cc right (the objective bottom left corner)
				if (focus.x < retInfo.m_edgePositions[static_cast<int>(Direction::Down)][1].x)
				{
					retInfo.m_edgePositions[static_cast<int>(Direction::Down)][1] = focus;
				}
			}
			// otherwise if the current pixel is lower than the bottom edge then it is our new bottom edge (both corners)
			else if (focus.y > retInfo.m_edgePositions[static_cast<int>(Direction::Down)][0].y)
			{
				retInfo.m_edgePositions[static_cast<int>(Direction::Down)][0] = focus;
				retInfo.m_edgePositions[static_cast<int>(Direction::Down)][1] = focus;
			}
			
			// if the current pixel is on the left edge check if it's our new left or right corner of said edge
			if (focus.x == retInfo.m_edgePositions[static_cast<int>(Direction::Left)][0].x)
			{
				// dir left cc left (the objective bottom left corner)
				if (focus.y > retInfo.m_edgePositions[static_cast<int>(Direction::Left)][0].y)
				{
					retInfo.m_edgePositions[static_cast<int>(Direction::Left)][0] = focus;
				}

				// dir down cc right (the objective top left corner)
				if (focus.y < retInfo.m_edgePositions[static_cast<int>(Direction::Left)][1].y)
				{
					retInfo.m_edgePositions[static_cast<int>(Direction::Left)][1] = focus;
				}
			}
			// otherwise if the current pixel is lower than the bottom edge then it is our new left edge (both corners)
			else if (focus.x < retInfo.m_edgePositions[static_cast<int>(Direction::Left)][0].x)
			{
				retInfo.m_edgePositions[static_cast<int>(Direction::Left)][0] = focus;
				retInfo.m_edgePositions[static_cast<int>(Direction::Left)][1] = focus;
			}
			
			// if the current pixel is on the right edge check if it's our new left or right corner of said edge
			if (focus.x == retInfo.m_edgePositions[static_cast<int>(Direction::Right)][0].x)
			{
				// dir right cc left (the objective top right corner)
				if (focus.y < retInfo.m_edgePositions[static_cast<int>(Direction::Right)][0].y)
				{
					retInfo.m_edgePositions[static_cast<int>(Direction::Right)][0] = focus;
				}

				// dir right cc right (the objective bottom right corner)
				if (focus.y > retInfo.m_edgePositions[static_cast<int>(Direction::Right)][1].y)
				{
					retInfo.m_edgePositions[static_cast<int>(Direction::Right)][1] = focus;
				}
			}
			// otherwise if the current pixel is lower than the bottom edge then it is our new right edge (both corners)
			else if (focus.x > retInfo.m_edgePositions[static_cast<int>(Direction::Right)][0].x)
			{
				retInfo.m_edgePositions[static_cast<int>(Direction::Right)][0] = focus;
				retInfo.m_edgePositions[static_cast<int>(Direction::Right)][1] = focus;
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

	// The size of the codels will only affect the size of the blocks not the shape (i.e. the end codel)
	// as such we only need to re-adjust the block size using the codel size (1 codel has a size of nxn pixels).
	retInfo.m_size /= m_codelSize * m_codelSize;

	
	// We now have all the information we need to find the end codel for this block
	FindEndCodel(retInfo);

	return retInfo;
}