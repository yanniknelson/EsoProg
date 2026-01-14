#include "PietImageTokeniser.h"

#include "PietToken.h"          // for PietToken

#include <IconsFontAwesome7.h>  // for CON_FA_ARROW_UP, ICON_FA_ARROW_DOWN, ICON_FA_ARROW_LEFT, ICON_FA_ARROW_RIGHT,

#include <deque>
#include <map>
#include <numeric>              // for std::gcd
#include <ios>                  // for std::hex, std::dec
#include <ostream>
#include <string>
#include <vector>

PietImageTokeniser::EDirection PietImageTokeniser::s_clockwiseDirectionLookup[(int)EDirection::Count] = { EDirection::Right, EDirection::Left, EDirection::Up, EDirection::Down };
const char* PietImageTokeniser::s_directionStrings[static_cast<int>(EDirection::Count) + 1] = { "Up", "Down", "Left", "Right", "Invalid" };
const char* PietImageTokeniser::s_directionIcons[static_cast<int>(EDirection::Count) + 1] = { ICON_FA_ARROW_UP, ICON_FA_ARROW_DOWN, ICON_FA_ARROW_LEFT, ICON_FA_ARROW_RIGHT, "Invalid" };

//////////////////////////////////////////////////////////////
bool operator==(const PietImageTokeniser::SLocation& rLoc1, const PietImageTokeniser::SLocation& rLoc2)
{
    return rLoc1.x == rLoc2.x && rLoc1.y == rLoc2.y;
}

//////////////////////////////////////////////////////////////
bool operator!=(const PietImageTokeniser::SLocation& rLoc1, const PietImageTokeniser::SLocation& rLoc2)
{
    return !(rLoc1 == rLoc2);
}

//////////////////////////////////////////////////////////////
bool operator<(const PietImageTokeniser::SLocation& rLoc1, const PietImageTokeniser::SLocation& rLoc2)
{
    return rLoc1.x < rLoc2.x || (rLoc1.x == rLoc2.x && rLoc1.y < rLoc2.y);
}

//////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& rOS, const PietImageTokeniser::SLocation& rLoc)
{
    rOS << rLoc.x << ", " << rLoc.y;
    return rOS;
}

//////////////////////////////////////////////////////////////
std::string PietImageTokeniser::SLocation::toString() const
{
    return std::to_string(x) + "," + std::to_string(y);
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SRGB::SRGB(const int init)
    : val(init)
{
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SRGB::SRGB(const int initR, const int initG, const int initB, const int initA /*= 0xFF*/)
    : r(initR), g(initG), b(initB), a(initA)
{
}

//////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& rOS, const PietImageTokeniser::SRGB& rCol)
{
    const int outint = (((rCol.r << 8) | rCol.g) << 8) | rCol.b;
    rOS << std::hex << outint << std::dec;
    return rOS;
}

//////////////////////////////////////////////////////////////
bool operator==(const PietImageTokeniser::SRGB& rCol1, const PietImageTokeniser::SRGB& rCol2)
{
    return rCol1.val == rCol2.val;
}

//////////////////////////////////////////////////////////////
bool operator==(const int& rCol1, const PietImageTokeniser::SRGB& rCol2)
{
    return rCol1 == rCol2.val;
}

//////////////////////////////////////////////////////////////
bool operator==(const PietImageTokeniser::SRGB& rCol1, const int& rCol2)
{
    return rCol1.val == rCol2;
}

//////////////////////////////////////////////////////////////
bool operator!=(const PietImageTokeniser::SRGB& rCol1, const PietImageTokeniser::SRGB& rCol2)
{
    return rCol1.val != rCol2.val;
}

//////////////////////////////////////////////////////////////
bool operator!=(const int& rCol1, const PietImageTokeniser::SRGB& rCol2)
{
    return rCol1 != rCol2.val;
}

//////////////////////////////////////////////////////////////
bool operator!=(const PietImageTokeniser::SRGB& rCol1, const int& rCol2)
{
    return rCol1.val != rCol2;
}

//////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& rOS, const PietImageTokeniser::SPietColour& rPietCol)
{
    static const std::string m_hueStrings[8] = { "Red", "Yellow", "Green", "Cyan", "Blue", "Magenta", "Black", "White" };
    static const std::string m_brightnessStrings[3] = { "Light", "", "Dark" };

    rOS << m_brightnessStrings[(int)rPietCol.m_brightness] << (((int)rPietCol.m_brightness == 1) ? "" : " ") << m_hueStrings[(int)rPietCol.m_hue];
    return rOS;
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SCodelInfo::SCodelInfo(const SLocation& rLoc /*= { 0, 0 }*/)
{
    for (int dir = 0; dir < (int)EDirection::Count; dir++)
    {
        m_edgePositions[dir][0] = rLoc;
        m_edgePositions[dir][1] = rLoc;
    }
}

// clang-format off
//////////////////////////////////////////////////////////////
PietImageTokeniser::SBlockInfo::SBlockInfo(SCodelInfo* pCodelInfo /*= nullptr*/, const SLocation& rLoc /*= { 0, 0 }*/, EDirection directionPointer /*= EDirection::Right*/, EDirection codelChooser /*= EDirection::Left*/)
    : m_pCodelInfo(pCodelInfo)
    , m_startingDirectionPointer(directionPointer)
    , m_endingDirectionPointer(directionPointer)
    , m_startingCodelChooser(codelChooser)
    , m_endingCodelChooser(codelChooser)
    , m_startingCodel(rLoc)
    , m_leavingCodel(rLoc)
    , m_endingCodel(rLoc)
{
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SBlockInfo::SBlockInfo(const SBlockInfo& rOther)
    : m_pCodelInfo(rOther.m_pCodelInfo)
    , m_startingDirectionPointer(rOther.m_startingDirectionPointer)
    , m_endingDirectionPointer(rOther.m_endingDirectionPointer)
    , m_startingCodelChooser(rOther.m_startingCodelChooser)
    , m_endingCodelChooser(rOther.m_endingCodelChooser)
    , m_startingCodel(rOther.m_startingCodel)
    , m_leavingCodel(rOther.m_leavingCodel)
    , m_endingCodel(rOther.m_endingCodel)
{
}
// clang-format on

//////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& rOS, const PietImageTokeniser::SBlockInfo& block)
{
    rOS << "Block Start: " << block.m_startingCodel << " Block Leave: " << block.m_leavingCodel << " Block End: " << block.m_endingCodel << std::endl;
    if (block.m_pCodelInfo)
    {
        rOS << "\tCodel Size: " << block.m_pCodelInfo->m_size << std::endl
           << "\tCodel Colour: " << block.m_pCodelInfo->m_rgbColour << " (" << block.m_pCodelInfo->m_pietColour << ")" << std::endl
           << "\tTop Edge: Left - " << block.m_pCodelInfo->m_edgePositions[static_cast<int>(PietImageTokeniser::EDirection::Up)][0] << " Right -" << block.m_pCodelInfo->m_edgePositions[static_cast<int>(PietImageTokeniser::EDirection::Up)][1] << std::endl
           << "\tBottom Edge:  Left - " << block.m_pCodelInfo->m_edgePositions[static_cast<int>(PietImageTokeniser::EDirection::Down)][0] << " Right -" << block.m_pCodelInfo->m_edgePositions[static_cast<int>(PietImageTokeniser::EDirection::Down)][1] << std::endl
           << "\tLeft Edge: Left - " << block.m_pCodelInfo->m_edgePositions[static_cast<int>(PietImageTokeniser::EDirection::Left)][0] << " Right -" << block.m_pCodelInfo->m_edgePositions[static_cast<int>(PietImageTokeniser::EDirection::Left)][1] << std::endl
           << "\tRight Edge: Left - " << block.m_pCodelInfo->m_edgePositions[static_cast<int>(PietImageTokeniser::EDirection::Right)][0] << " Right -" << block.m_pCodelInfo->m_edgePositions[static_cast<int>(PietImageTokeniser::EDirection::Right)][1];
    }
    else
    {
        rOS << "\tBlock has no Codel Info";
    }
    return rOS;
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::PietImageTokeniser()
{
    m_codels.push_back({ { 0, 0 } });
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::ResetImplementation()
{
    m_currentBlock = {};
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::SetImage(const unsigned char* pImageData, const int width, const int height)
{
    Reset();
    m_pImageData = pImageData;
    m_imageWidth = width;
    m_imageHeight = height;
    m_instructionNumber = 1;
    SetCodelSize(CalcCodelSize());
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::UnsetImage()
{
    Reset();
    m_pImageData = nullptr;
    m_imageWidth = 0;
    m_imageHeight = 0;
    m_instructionNumber = 1;
}

//////////////////////////////////////////////////////////////
int PietImageTokeniser::GetCodelSize() const
{
    return m_codelSize;
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::SetCodelSize(const int size)
{
    m_codelSize = size;
}

//////////////////////////////////////////////////////////////
int PietImageTokeniser::GetInstructionNumber()
{
    return m_instructionNumber;
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::ToggleCodelChooser()
{
    PietImageTokeniser::ToggleCodelChooser(m_currentBlock);
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::RotateDirectionPointer(const int times)
{
    PietImageTokeniser::RotateDirectionPointer(m_currentBlock, times);
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::CopyState()
{
    m_cachedBlock = m_currentBlock;
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SLocation PietImageTokeniser::GetCachedBlockStartLocation() const
{
    return m_cachedBlock.m_startingCodel;
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SLocation PietImageTokeniser::GetCachedBlockEndLocation() const
{
    return m_cachedBlock.m_endingCodel;
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::EDirection PietImageTokeniser::GetCachedBlockStartDirectionPointer() const
{
    return m_cachedBlock.m_startingDirectionPointer;
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::EDirection PietImageTokeniser::GetCachedBlockEndDirectionPointer() const
{
    return m_cachedBlock.m_endingDirectionPointer;
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::EDirection PietImageTokeniser::GetCachedBlockStartCodelChoser() const
{
    return m_cachedBlock.m_startingCodelChooser;
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::EDirection PietImageTokeniser::GetCachedBlockEndCodelChoser() const
{
    return m_cachedBlock.m_endingCodelChooser;
}

//////////////////////////////////////////////////////////////
PietToken PietImageTokeniser::Pop_Internal()
{
    if (m_pImageData == nullptr)
    {
        return PietToken::ETokenType::End;
    }

    m_currentBlock = GetBlockInfo(m_currentBlock.m_endingCodel, m_currentBlock.m_endingDirectionPointer, m_currentBlock.m_endingCodelChooser);

    if (m_currentBlock.m_startingCodel == m_currentBlock.m_endingCodel)
    {
        return PietToken::ETokenType::End;
    }

    SPietColour nextCol = GetPietColourFromLocation(m_currentBlock.m_endingCodel);
    PietToken::ETokenType::Enum type = ConvertColoursToInstruction(m_currentBlock.m_pCodelInfo->m_pietColour, nextCol);
    if (type == PietToken::ETokenType::Push)
    {
        return PietToken(type, m_currentBlock.m_pCodelInfo->m_size);
    }

    return PietToken(type);
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SPietColour PietImageTokeniser::RGBToPietColour(const PietImageTokeniser::SRGB colour)
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
	* This means we can convert from the SRGB to an index in a pre-ordered array of hues
	*
	* This will only work directly for the standard hues. To make this work for light
	* colours we can notice that the light hue hex values are the standard hue values with
	* all 00s replaced with C0s. Similarly for dark Hues, the 00s are left as is and the
	* FFs become C0s.
	*/

    static EHue hueTable[8]{ EHue::Black, EHue::Blue, EHue::Green, EHue::Cyan, EHue::Red, EHue::Magenta, EHue::Yellow, EHue::White };

    SPietColour retVal;

    int hueIndx = 0;

    bool bHasFF = colour.r == 0xFF || colour.g == 0xFF || colour.b == 0xFF;
    bool bHasC0 = colour.r == 0xC0 || colour.g == 0xC0 || colour.b == 0xC0;
    
    // If the colour is dark all the hex FFs will be C0 (see NOTE 2)
    const uint8_t highBits = bHasFF ? 0xFF : 0xC0;
    
    retVal.m_brightness = bHasFF ? (bHasC0 ? EBrightness::Light : EBrightness::Standard) : EBrightness::Dark;

    // See NOTE 2
    if (colour.r == highBits)
    {
        hueIndx |= 0b100;
    }
    if (colour.g == highBits)
    {
        hueIndx |= 0b010;
    }
    if (colour.b == highBits)
    {
        hueIndx |= 0b001;
    }

    retVal.m_hue = hueTable[hueIndx];

    return retVal;
}

//////////////////////////////////////////////////////////////
inline PietImageTokeniser::SLocation PietImageTokeniser::MoveInDirection(const SLocation& rLoc, const EDirection dir)
{
    switch (dir)
    {
    case (EDirection::Up):
    {
        return SLocation{ rLoc.x, rLoc.y - 1 };
        break;
    }
    case (EDirection::Down):
    {
        return SLocation{ rLoc.x, rLoc.y + 1 };
        break;
    }
    case (EDirection::Left):
    {
        return SLocation{ rLoc.x - 1, rLoc.y };
        break;
    }
    case (EDirection::Right):
    {
        return SLocation{ rLoc.x + 1, rLoc.y };
        break;
    }
    }

    return rLoc;
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::ToggleCodelChooser(SBlockInfo& rBlockinfo)
{
    if (rBlockinfo.m_endingCodelChooser == EDirection::Left)
    {
        rBlockinfo.m_endingCodelChooser = EDirection::Right;
    }
    else
    {
        rBlockinfo.m_endingCodelChooser = EDirection::Left;
    }
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::RotateDirectionPointer(SBlockInfo& rBlockinfo, const int times)
{
    for (int rotation = 0; rotation < times % 4; rotation++)
    {
        rBlockinfo.m_endingDirectionPointer = s_clockwiseDirectionLookup[(int)rBlockinfo.m_endingDirectionPointer];
    }
}

//////////////////////////////////////////////////////////////
PietToken::ETokenType::Enum PietImageTokeniser::ConvertColoursToInstruction(const SPietColour& rColour1, const SPietColour& rColour2)
{
    // clang-format off
	static const PietToken::ETokenType::Enum conversionTable[6][3] = { {PietToken::ETokenType::Unrecognised_Token, PietToken::ETokenType::Push, PietToken::ETokenType::Pop}
														, {PietToken::ETokenType::Add, PietToken::ETokenType::Subtract, PietToken::ETokenType::Multiply}
														, {PietToken::ETokenType::Divide, PietToken::ETokenType::Modulo, PietToken::ETokenType::Not}
														, {PietToken::ETokenType::Greater, PietToken::ETokenType::Pointer, PietToken::ETokenType::Switch}
														, {PietToken::ETokenType::Duplicate, PietToken::ETokenType::Roll, PietToken::ETokenType::Input_Val}
														, {PietToken::ETokenType::Input_Char, PietToken::ETokenType::Output_Val, PietToken::ETokenType::Output_Char} };
    // clang-format on

    // White acts as a reset for the colour, moving into it 'clears' the current colour,
    // so we can't execute when we move into or out of it, thus return NOP
    if (rColour2.m_hue == EHue::White)
    {
        return PietToken::ETokenType::EnterSlide;
    }

    if (rColour1.m_hue == EHue::White)
    {
        return PietToken::ETokenType::ExitSlide;
    }

    int hueDiffIndx = (int)rColour2.m_hue - (int)rColour1.m_hue;
    if (hueDiffIndx < 0)
    {
        hueDiffIndx += (int)EHue::COUNT - 2;
    }

    int brightnessDiffIndx = (int)rColour2.m_brightness - (int)rColour1.m_brightness;
    if (brightnessDiffIndx < 0)
    {
        brightnessDiffIndx += (int)EBrightness::COUNT;
    }

    return conversionTable[hueDiffIndx][brightnessDiffIndx];
}

//////////////////////////////////////////////////////////////
int PietImageTokeniser::CalcCodelSize() const
{
    int minWidth = m_imageWidth;
    int startOfCurrentBlock = 0;
    const SRGB* pImageData = reinterpret_cast<const SRGB*>(m_pImageData);
    for (int xCoord = 1; xCoord < m_imageWidth; xCoord++)
    {
        if (pImageData[startOfCurrentBlock] != pImageData[xCoord])
        {
            const int sizeOfBlock = xCoord - startOfCurrentBlock;
            minWidth = std::min(minWidth, sizeOfBlock);
            if (minWidth == 1)
            {
                return 1;
            }
            startOfCurrentBlock = xCoord;
        }
    }

    int minHeight = m_imageHeight;
    startOfCurrentBlock = 0;
    for (int yCoord = 1; yCoord < m_imageWidth; yCoord++)
    {
        if (pImageData[startOfCurrentBlock * m_imageWidth] != pImageData[yCoord * m_imageWidth])
        {
            const int sizeOfBlock = yCoord - startOfCurrentBlock;
            minHeight = std::min(minHeight, sizeOfBlock);
            if (minHeight == 1)
            {
                return 1;
            }
            startOfCurrentBlock = yCoord;
        }
    }

    return std::gcd(minWidth, minHeight);
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SRGB PietImageTokeniser::GetRGBFromLoation(const PietImageTokeniser::SLocation& rLoc) const
{
    if (rLoc.y < m_imageHeight && rLoc.y > -1 && rLoc.x < m_imageWidth && rLoc.x > -1)
    {
        return reinterpret_cast<const SRGB*>(m_pImageData)[(m_imageWidth * rLoc.y + rLoc.x)];
    }

    return BLACK;
}

//////////////////////////////////////////////////////////////
bool PietImageTokeniser::LocationIsSameColour(const SLocation& rLoc, const SRGB col) const
{
    if (rLoc.y < m_imageHeight && rLoc.y > -1 && rLoc.x < m_imageWidth && rLoc.x > -1)
    {
        return reinterpret_cast<const SRGB*>(m_pImageData)[(m_imageWidth * rLoc.y + rLoc.x)] == col;
    }

    return col == BLACK;
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SPietColour PietImageTokeniser::GetPietColourFromLocation(const PietImageTokeniser::SLocation& rLoc) const
{
    return RGBToPietColour(GetRGBFromLoation(rLoc));
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SLocation PietImageTokeniser::SlideAlongWhite(const SLocation& rLoc, const EDirection dir)
{
    SLocation currLocation = rLoc;
    while (true)
    {
        const SLocation nextLocation = MoveInDirection(currLocation, dir);
        if (LocationIsSameColour(nextLocation, WHITE))
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

//////////////////////////////////////////////////////////////
bool PietImageTokeniser::IsValidLocation(const SLocation& rLoc) const
{
    if (rLoc.x < 0 || rLoc.x > m_imageWidth - 1 || rLoc.y < 0 || rLoc.y > m_imageHeight - 1)
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////
bool PietImageTokeniser::IsValidLocation(const SLocation& rLoc, const std::vector<std::vector<bool>>& visited) const
{
    return IsValidLocation(rLoc) && !visited[rLoc.x][rLoc.y];
}

//////////////////////////////////////////////////////////////
PietImageTokeniser::SLocation PietImageTokeniser::FindEndOfEdge(const SLocation& rEdgeLoc, const EDirection directionPointer, const EDirection chooser) const
{
    // clang-format off
	// Since the chooser direction is relative to the direciton pointer, we can use them to find the global real direction
	static PietImageTokeniser::EDirection searchDirLookup[4][2] = { /*Up*/{/*Left*/EDirection::Left, /*Right*/EDirection::Right}
																 , /*Down*/{/*Left*/EDirection::Right, /*Right*/EDirection::Left}
																 , /*Left*/{/*Left*/EDirection::Down, /*Right*/EDirection::Up}
																 , /*Right*/{/*Left*/EDirection::Up, /*Right*/EDirection::Down} };
    // clang-format on

    PietImageTokeniser::EDirection SearchDir = searchDirLookup[(int)directionPointer][(int)chooser - (int)EDirection::Left];

    // The edge ends when the colour changes so we check the block ahead of us before we move, if its a different colour
    // (or no longer valid) we return our current location otherwise we move to it
    const SRGB blockCol = GetRGBFromLoation(rEdgeLoc);

    PietImageTokeniser::SLocation focus = rEdgeLoc;

    while (IsValidLocation(MoveInDirection(focus, SearchDir)) && LocationIsSameColour(MoveInDirection(focus, SearchDir), blockCol))
    {
        focus = MoveInDirection(focus, SearchDir);
    }

    return focus;
}

//////////////////////////////////////////////////////////////
void PietImageTokeniser::FindEndCodel(SBlockInfo& rBlockInfo)
{
    int attempts = 0;
    bool switchCodelChooser = true;
    PietImageTokeniser::SLocation currentCorner = rBlockInfo.m_startingCodel;
    while (attempts < 8)
    {
        // To find the end codel we go to the edge furthest in the direction we're pointing, go to the end of that edge chosen by the codel chooser
        // then try to move in the direction we're facing, if the location is valid and the colour isn't black then that's the next block and our end location
        rBlockInfo.m_leavingCodel = rBlockInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(rBlockInfo.m_endingDirectionPointer)][static_cast<int>(rBlockInfo.m_endingCodelChooser) - static_cast<int>(EDirection::Left)];
        rBlockInfo.m_endingCodel = MoveInDirection(rBlockInfo.m_leavingCodel, rBlockInfo.m_endingDirectionPointer);

        if (IsValidLocation(rBlockInfo.m_endingCodel) && !LocationIsSameColour(rBlockInfo.m_endingCodel, BLACK))
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

//////////////////////////////////////////////////////////////
void PietImageTokeniser::FindEndWhiteCodel(SBlockInfo& rBlockInfo)
{
    SLocation currLocation = rBlockInfo.m_startingCodel;
    EDirection currDirectionPointer = rBlockInfo.m_startingDirectionPointer;
    EDirection currCodelChoser = rBlockInfo.m_startingCodelChooser;

    std::map<SLocation, bool[4]> pathTrace;
    while (true)
    {
        const SLocation nextLocation = SlideAlongWhite(currLocation, currDirectionPointer);
        const SLocation nextBlockLoc = MoveInDirection(nextLocation, currDirectionPointer);
        if (LocationIsSameColour(nextBlockLoc, BLACK))
        {
            currCodelChoser = currCodelChoser == EDirection::Left ? EDirection::Right : EDirection::Left;
            currDirectionPointer = s_clockwiseDirectionLookup[(int)currDirectionPointer];
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

//////////////////////////////////////////////////////////////
PietImageTokeniser::SBlockInfo PietImageTokeniser::GetBlockInfo(const SLocation& rStartLocation, const EDirection startDirectionPointer, const EDirection StartCodelChooser)
{
    m_codels[0] = SCodelInfo(rStartLocation);
    SBlockInfo retInfo(&m_codels[0], rStartLocation, startDirectionPointer, StartCodelChooser);

    retInfo.m_pCodelInfo->m_rgbColour = GetRGBFromLoation(retInfo.m_startingCodel);
    retInfo.m_pCodelInfo->m_pietColour = RGBToPietColour(retInfo.m_pCodelInfo->m_rgbColour);

    if (retInfo.m_pCodelInfo->m_pietColour.m_hue == EHue::White)
    {
        FindEndWhiteCodel(retInfo);
        return retInfo;
    }

    std::vector<std::vector<bool>> visited(m_imageWidth, std::vector<bool>(m_imageHeight, false));
    visited[retInfo.m_startingCodel.x][retInfo.m_startingCodel.y] = true;
    std::deque<PietImageTokeniser::SLocation> ToVisit = { retInfo.m_startingCodel };

    // Run through all the pixels of the shape using floodfill
    // count the pixles to find the size, find the highest/lowest/leftmost/rightmost locations in the block
    // we stop flooding when we detect a change in colour
    while (!ToVisit.empty())
    {
        const PietImageTokeniser::SLocation focus = ToVisit.front();
        ToVisit.pop_front();
        if (LocationIsSameColour(focus, retInfo.m_pCodelInfo->m_rgbColour))
        {
            retInfo.m_pCodelInfo->m_size++;

            // if the current pixel is on the top edge check if it's our new left or right corner of said edge
            if (focus.y == retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Up)][0].y)
            {
                // dir up cc left (the objective top left corner)
                if (focus.x < retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Up)][0].x)
                {
                    retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Up)][0] = focus;
                }

                // dir up cc right (the objective top right corner)
                if (focus.x > retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Up)][1].x)
                {
                    retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Up)][1] = focus;
                }
            }
            // otherwise if the current pixel is higher than the top edge then it is our new top edge (both corners)
            else if (focus.y < retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Up)][0].y)
            {
                retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Up)][0] = focus;
                retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Up)][1] = focus;
            }

            // if the current pixel is on the bottom edge check if it's our new left or right corner of said edge
            if (focus.y == retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Down)][0].y)
            {
                // dir down cc left (the objective bottom right corner)
                if (focus.x > retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Down)][0].x)
                {
                    retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Down)][0] = focus;
                }

                // dir down cc right (the objective bottom left corner)
                if (focus.x < retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Down)][1].x)
                {
                    retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Down)][1] = focus;
                }
            }
            // otherwise if the current pixel is lower than the bottom edge then it is our new bottom edge (both corners)
            else if (focus.y > retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Down)][0].y)
            {
                retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Down)][0] = focus;
                retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Down)][1] = focus;
            }

            // if the current pixel is on the left edge check if it's our new left or right corner of said edge
            if (focus.x == retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Left)][0].x)
            {
                // dir left cc left (the objective bottom left corner)
                if (focus.y > retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Left)][0].y)
                {
                    retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Left)][0] = focus;
                }

                // dir down cc right (the objective top left corner)
                if (focus.y < retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Left)][1].y)
                {
                    retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Left)][1] = focus;
                }
            }
            // otherwise if the current pixel is lower than the bottom edge then it is our new left edge (both corners)
            else if (focus.x < retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Left)][0].x)
            {
                retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Left)][0] = focus;
                retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Left)][1] = focus;
            }

            // if the current pixel is on the right edge check if it's our new left or right corner of said edge
            if (focus.x == retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Right)][0].x)
            {
                // dir right cc left (the objective top right corner)
                if (focus.y < retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Right)][0].y)
                {
                    retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Right)][0] = focus;
                }

                // dir right cc right (the objective bottom right corner)
                if (focus.y > retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Right)][1].y)
                {
                    retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Right)][1] = focus;
                }
            }
            // otherwise if the current pixel is lower than the bottom edge then it is our new right edge (both corners)
            else if (focus.x > retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Right)][0].x)
            {
                retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Right)][0] = focus;
                retInfo.m_pCodelInfo->m_edgePositions[static_cast<int>(EDirection::Right)][1] = focus;
            }

            for (int dir = 0; dir < (int)EDirection::Count; dir++)
            {
                if (IsValidLocation(MoveInDirection(focus, (EDirection)dir), visited))
                {
                    ToVisit.push_back(MoveInDirection(focus, (EDirection)dir));
                    visited[ToVisit.back().x][ToVisit.back().y] = true;
                }
            }
        }
    }

    // The size of the codels will only affect the size of the blocks not the shape (i.e. the end codel)
    // as such we only need to re-adjust the block size using the codel size (1 codel has a size of nxn pixels).
    retInfo.m_pCodelInfo->m_size /= m_codelSize * m_codelSize;

    // We now have all the information we need to find the end codel for this block
    FindEndCodel(retInfo);

    return retInfo;
}