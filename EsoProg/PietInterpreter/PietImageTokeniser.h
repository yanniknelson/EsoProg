#pragma once

#include <strstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "PietTokeniser.h"

class PietImageTokeniser : public PietTokeniser
{
public:

	/// <summary>
	/// Get the next token
	/// </summary>
	/// <returns> The next token </returns>
	virtual const PietToken& Pop() override;

	PietImageTokeniser() {
		m_codels.push_back({ { 0,0 } });
	}

	void SetImage(const unsigned char* imageData, const int width, const int height);
	void UnsetImage();
	void SetCodelSize(const int size);
	void Reset();

	int GetInstructionNumber();

	void ToggleCodelChooser();
	void RotateDirectionPointer(const int times);

	enum class Direction : uint8_t
	{
		Up, Down, Left, Right, Count
	};

	static char* i_directionStrings[static_cast<int>(Direction::Count) + 1];
	static char* i_directionIcons[static_cast<int>(Direction::Count) + 1];

	struct Location
	{
		int x = 0;
		int y = 0;

		friend bool operator==(const Location& loc1, const Location& loc2)
		{
			return loc1.x == loc2.x && loc1.y == loc2.y;
		}

		friend bool operator!=(const Location& loc1, const Location& loc2)
		{
			return !(loc1 == loc2);
		}

		friend bool operator<(const Location& loc1, const Location& loc2)
		{
			return loc1.x < loc2.x || (loc1 .x == loc2.x && loc1.y < loc2.y);
		}

		friend std::ostream& operator<<(std::ostream& os, const Location& loc)
		{
			os << loc.x << ", " << loc.y;
			return os;
		}

		std::string toString() const 
		{
			return std::to_string(x) + "," + std::to_string(y);
		}
	};

	union RGB
	{
		uint32_t val = 0;
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};

		RGB(): val(0) {}

		RGB(const int init): val(init) {}
		
		RGB(const int initR, const int initG, const int initB): r(initR), g(initG), b(initB) {}

		friend std::ostream& operator<<(std::ostream& os, const RGB& col)
		{
			const int outint = (((col.r << 8) | col.g) << 8) | col.b;
			os << std::hex << outint << std::dec;
			return os;
		}

		friend bool operator==(const RGB& col1, const RGB& col2)
		{
			return col1.val == col2.val;
		}

		friend bool operator==(const int& col1, const RGB& col2)
		{
			return col1 == col2.val;
		}
		
		friend bool operator==(const RGB& col1, const int& col2)
		{
			return col1.val == col2;
		}

		friend bool operator!=(const RGB& col1, const RGB& col2)
		{
			return col1.val != col2.val;
		}

		friend bool operator!=(const int& col1, const RGB& col2)
		{
			return col1 != col2.val;
		}

		friend bool operator!=(const RGB& col1, const int& col2)
		{
			return col1.val != col2;
		}
	};

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
		Black,
		White,
		COUNT
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
		Hue m_hue{ Hue::White };
		Brightness m_brightness{ Brightness::COUNT };

		friend std::ostream& operator<<(std::ostream& os, const PietColour& pietCol)
		{
			static const std::string m_hueStrings[8] = { "Red", "Yellow", "Green", "Cyan", "Blue", "Magenta", "Black", "White" };
			static const std::string m_brightnessStrings[3] = { "Light", "", "Dark" };

			os << m_brightnessStrings[(int)pietCol.m_brightness] << (((int)pietCol.m_brightness == 1) ? "" : " ") << m_hueStrings[(int)pietCol.m_hue];
			return os;
		}
	};

	struct CodelInfo
	{
		int m_size{ 0 };
		Location m_edgePositions[4][2] = { -1, -1 };
		RGB m_rgbColour;
		PietColour m_pietColour;

		CodelInfo(Location loc = { 0,0 })
		{
			for (int dir = 0; dir < (int)Direction::Count; dir++)
			{
				m_edgePositions[dir][0] = loc;
				m_edgePositions[dir][1] = loc;
			}
		}
	};

	struct BlockInfo
	{
		CodelInfo* m_pCodelInfo = nullptr;
		Location m_startingCodel{ -1, -1 };
		Location m_leavingCodel{ -1, -1 };
		Location m_endingCodel{ -1, -1 };

		Direction m_startingDirectionPointer = Direction::Count;
		Direction m_endingDirectionPointer = Direction::Count;
		Direction m_startingCodelChooser = Direction::Count;
		Direction m_endingCodelChooser = Direction::Count;

		BlockInfo(CodelInfo* codelInfo = nullptr, Location loc = { 0,0 }, Direction directionPointer = Direction::Right, Direction codelChooser = Direction::Left)
			: m_pCodelInfo(codelInfo)
			, m_startingDirectionPointer(directionPointer)
			, m_endingDirectionPointer(directionPointer)
			, m_startingCodelChooser(codelChooser)
			, m_endingCodelChooser(codelChooser)
			, m_startingCodel(loc)
			, m_leavingCodel(loc)
			, m_endingCodel(loc)
		{}

		BlockInfo(const BlockInfo& rOther)
			: m_pCodelInfo(rOther.m_pCodelInfo)
			, m_startingDirectionPointer(rOther.m_startingDirectionPointer)
			, m_endingDirectionPointer(rOther.m_endingDirectionPointer)
			, m_startingCodelChooser(rOther.m_startingCodelChooser)
			, m_endingCodelChooser(rOther.m_endingCodelChooser)
			, m_startingCodel(rOther.m_startingCodel)
			, m_leavingCodel(rOther.m_leavingCodel)
			, m_endingCodel(rOther.m_endingCodel)
		{}

		friend std::ostream& operator<<(std::ostream& os, const BlockInfo& block)
		{
			os << "Block Start: " << block.m_startingCodel << " Block Leave: " << block.m_leavingCodel << " Block End: " << block.m_endingCodel << std::endl;
			if (block.m_pCodelInfo)
			{
				os	<< "\tCodel Size: " << block.m_pCodelInfo->m_size << std::endl 
					<< "\tCodel Colour: " << block.m_pCodelInfo->m_rgbColour << " (" << block.m_pCodelInfo->m_pietColour << ")" << std::endl
					<< "\tTop Edge: Left - " << block.m_pCodelInfo->m_edgePositions[static_cast<int>(Direction::Up)][0] << " Right -" << block.m_pCodelInfo->m_edgePositions[static_cast<int>(Direction::Up)][1] << std::endl
					<< "\tBottom Edge:  Left - " << block.m_pCodelInfo->m_edgePositions[static_cast<int>(Direction::Down)][0] << " Right -" << block.m_pCodelInfo->m_edgePositions[static_cast<int>(Direction::Down)][1] << std::endl
					<< "\tLeft Edge: Left - " << block.m_pCodelInfo->m_edgePositions[static_cast<int>(Direction::Left)][0] << " Right -" << block.m_pCodelInfo->m_edgePositions[static_cast<int>(Direction::Left)][1] << std::endl
					<< "\tRight Edge: Left - " << block.m_pCodelInfo->m_edgePositions[static_cast<int>(Direction::Right)][0] << " Right -" << block.m_pCodelInfo->m_edgePositions[static_cast<int>(Direction::Right)][1];
			}
			else
			{
				os << "\tBlock has no Codel Info";
			}
			return os;
		}
	};

	void CopyState();
	Location GetCachedBlockStartLocation() const;
	Location GetCachedBlockEndLocation() const;
	Direction GetCachedBlockStartDirectionPointer() const;
	Direction GetCachedBlockEndDirectionPointer() const;
	Direction GetCachedBlockStartCodelChoser() const;
	Direction GetCachedBlockEndCodelChoser() const;

private:

	static Direction i_clockwiseDirectionLookup[(int)Direction::Count];

	PietToken tLastPopped{ PietToken::TokenType::End };
	const unsigned char* m_imageData{ nullptr };
	int m_imageWidth = 0;
	int m_imageHeight = 0;
	int m_instructionNumber = 1;
	int m_codelSize = 1;

	std::vector<CodelInfo> m_codels;
	Direction m_globalStartDirectionPointer{ Direction::Right };
	Direction m_globalStartCodelChooser{ Direction::Left };
	Location m_globalStartCodel{ 0, 0 };

	BlockInfo m_currentBlock;
	BlockInfo m_cachedBlock;

	/// <summary>
	/// Convert from RGB value to Hue Brightness pair (using struct for readability)
	/// </summary>
	/// <param name="RGB - "> RGB colour info </param>
	static PietColour RGBToPietColour(const RGB col);

	/// <summary>
	/// Returns a location directly neighbouring the provided location in which ever direction specified
	/// </summary>
	/// <param name="loc - ">The intial location</param>
	/// <param name="dir - ">The direction of desired neighbing location</param>
	/// <returns>the neighbouring location in specified direction</returns>
	static inline Location MoveInDirection(const Location loc, const Direction dir);

	/// <summary>
	/// Wrapper for getting m_topEdge/m_bottomEdge/m_leftEdge/m_rightEdge from a BlockInfo struct
	/// </summary>
	/// <param name="block - ">The block whose edge we desire</param>
	/// <param name="edge - ">The edge we desire (Up -> Top etc...)</param>
	/// <returns>A location on the desired edge</returns>
	//static inline Location GetEdge(const BlockInfo & block, const Direction edge);

	static void ToggleCodelChooser(BlockInfo& blockinfo);

	static void RotateDirectionPointer(BlockInfo& blockinfo, const int times);

	static PietToken::TokenType ConvertColoursToInstruction(const PietColour colour1, const PietColour colour2);

	/// <summary>
	/// Query the image data
	/// </summary>
	/// <param name="loc - ">The pixel coordinates whose data we desire</param>
	/// <returns>The RGB data of the desired pixel</returns>
	RGB GetRGBFromLoation(const Location& loc) const;

	bool LocationIsSameColour(const Location& loc, const RGB col) const;

	/// <summary>
	/// Query the image data
	/// </summary>
	/// <param name="loc - ">The pixel coordinates whose data we desire</param>
	/// <returns>The Piet Colour of the desired pixel</returns>
	PietColour GetPietColourFromLocation(const Location& loc) const;

	Location SlideAlongWhite(const Location loc, const Direction dir);

	/// <summary>
	/// Check the location is within the image
	/// </summary>
	/// <param name="loc - ">The location to be verified</param>
	/// <returns>The validy of the location</returns>
	bool isValidLocation(const Location& loc) const;

	/// <summary>
	/// Check the location is within the image and if the location has been visited already
	/// </summary>
	/// <param name="loc - ">The location to be verified</param>
	/// <param name="visited - ">A 2D vector of bools representing the visited locations</param>
	/// <returns>Will return true if the location is valid and has not been visited</returns>
	bool isValidLocation(const Location& loc, const std::vector<std::vector<bool>>& visited) const;


	/// <summary>
	/// Will find the end point of a block's edge
	/// </summary>
	/// <param name="EdgeLoc - ">Any location on the edge</param>
	/// <param name="directionPointer - ">Direction facing outwards from the edge</param>
	/// <param name="chooser - ">Direction, relative to the outward direction, we wish to search for the edge</param>
	/// <returns>The location marking the end of the edge</returns>
	Location FindEndOfEdge(const Location EdgeLoc, const Direction directionPointer, const Direction chooser) const;
	
	/// <summary>
	/// Will find the end codel for a block
	/// </summary>
	/// <param name="block - ">The block we with to find the end codel of</param>
	void FindEndCodel(BlockInfo& block);

	/// <summary>
	/// Will find the end point of a white block's edge
	/// </summary>
	/// <param name="EdgeLoc - ">Any location on the edge</param>
	/// <param name="directionPointer - ">Direction facing outwards from the edge</param>
	/// <param name="chooser - ">Direction, relative to the outward direction, we wish to search for the edge</param>
	/// <returns>The location marking the end of the edge</returns>
	void FindEndWhiteCodel(BlockInfo& block);

	/// <summary>
	/// Finds the size, colour, edges, leaving location, end location, end direction pointer, end codel cooder of the block in whcih startLocation resides
	/// </summary>
	/// <param name="startLocation - ">A location within the block we want information about</param>
	/// <param name="startDirectionPointer - ">The initial edge we'd like to leave the block from (will affect the end direction pointer, end codel chooser, leaving location and end location)</param>
	/// <param name="StartCodelChooser - ">The end of the leaving edge we wish to leave from (will affect the end direction pointer, end codel chooser, leaving location and end location)</param>
	/// <returns>The populated Info for the requested block</returns>
	BlockInfo GetBlockInfo(const Location startLocation, const Direction startDirectionPointer, const Direction StartCodelChooser);

};