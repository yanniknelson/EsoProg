#pragma once

#include "PietToken.h"   // for PietToken

#include <ITokeniser.h>  // for ITokeniser

#include <cstdint>       // for uint8_t
#include <iostream>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////
class PietImageTokeniser : public ITokeniser<PietToken>
{
  public:
    //////////////////////////////////////////////////////////////
    enum class EDirection : uint8_t
    {
        Up,
        Down,
        Left,
        Right,
        Count
    };

     //////////////////////////////////////////////////////////////
    struct SLocation
    {
        int x = 0;
        int y = 0;

        friend bool operator==(const SLocation& rLoc1, const SLocation& rLoc2);
        friend bool operator!=(const SLocation& rLoc1, const SLocation& rLoc2);
        friend bool operator<(const SLocation& rLoc1, const SLocation& rLoc2);

        friend std::ostream& operator<<(std::ostream& rOS, const SLocation& rLoc);

        std::string toString() const;
    };

    //////////////////////////////////////////////////////////////
    union SRGB
    {
        uint32_t val = 0;
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };

        SRGB() = default;
        SRGB(const int init);
        SRGB(const int initR, const int initG, const int initB);

        friend std::ostream& operator<<(std::ostream& rOS, const SRGB& rCol);

        friend bool operator==(const SRGB& rCol1, const SRGB& rCol2);
        friend bool operator==(const int& rCol1, const SRGB& rCol2);
        friend bool operator==(const SRGB& rCol1, const int& rCol2);
        friend bool operator!=(const SRGB& rCol1, const SRGB& rCol2);
        friend bool operator!=(const int& rCol1, const SRGB& rCol2);
        friend bool operator!=(const SRGB& rCol1, const int& rCol2);
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

    //////////////////////////////////////////////////////////////
    enum class EHue : uint8_t
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

    //////////////////////////////////////////////////////////////
    enum class EBrightness : uint8_t
    {
        Light,
        Standard,
        Dark,
        COUNT
    };

    //////////////////////////////////////////////////////////////
    struct SPietColour
    {
        EHue m_hue{ EHue::White };
        EBrightness m_brightness{ EBrightness::COUNT };

        friend std::ostream& operator<<(std::ostream& rOS, const SPietColour& rPietCol);
    };

    //////////////////////////////////////////////////////////////
    struct SCodelInfo
    {
        int m_size{ 0 };
        SLocation m_edgePositions[4][2] = { -1, -1 };
        SRGB m_rgbColour;
        SPietColour m_pietColour;

        SCodelInfo(const SLocation &rLoc = { 0, 0 });
    };

    //////////////////////////////////////////////////////////////
    struct SBlockInfo
    {
        SCodelInfo* m_pCodelInfo = nullptr;
        SLocation m_startingCodel{ -1, -1 };
        SLocation m_leavingCodel{ -1, -1 };
        SLocation m_endingCodel{ -1, -1 };

        EDirection m_startingDirectionPointer = EDirection::Count;
        EDirection m_endingDirectionPointer = EDirection::Count;
        EDirection m_startingCodelChooser = EDirection::Count;
        EDirection m_endingCodelChooser = EDirection::Count;

        SBlockInfo(SCodelInfo* pCodelInfo = nullptr, const SLocation& rLoc = { 0, 0 }, EDirection directionPointer = EDirection::Right, EDirection codelChooser = EDirection::Left);
        SBlockInfo(const SBlockInfo& rOther);

        friend std::ostream& operator<<(std::ostream& rOS, const SBlockInfo& block);
    };

  public:
    PietImageTokeniser();

    void SetImage(const unsigned char* pImageData, const int width, const int height);
    void UnsetImage();
    void SetCodelSize(const int size);
    virtual void ResetImplementation() override;

    int GetInstructionNumber();

    void ToggleCodelChooser();
    void RotateDirectionPointer(const int times);

    

    static const char* s_directionStrings[static_cast<int>(EDirection::Count) + 1];
    static const char* s_directionIcons[static_cast<int>(EDirection::Count) + 1];

    void CopyState();
    SLocation GetCachedBlockStartLocation() const;
    SLocation GetCachedBlockEndLocation() const;
    EDirection GetCachedBlockStartDirectionPointer() const;
    EDirection GetCachedBlockEndDirectionPointer() const;
    EDirection GetCachedBlockStartCodelChoser() const;
    EDirection GetCachedBlockEndCodelChoser() const;

  private:
    virtual PietToken Pop_Internal() override;

    static EDirection s_clockwiseDirectionLookup[(int)EDirection::Count];

    const unsigned char* m_pImageData{ nullptr };
    int m_imageWidth = 0;
    int m_imageHeight = 0;
    int m_instructionNumber = 1;
    int m_codelSize = 1;

    std::vector<SCodelInfo> m_codels;
    EDirection m_globalStartDirectionPointer{ EDirection::Right };
    EDirection m_globalStartCodelChooser{ EDirection::Left };
    SLocation m_globalStartCodel{ 0, 0 };

    SBlockInfo m_currentBlock;
    SBlockInfo m_cachedBlock;

    static SPietColour RGBToPietColour(const SRGB col);
    static inline SLocation MoveInDirection(const SLocation& rLoc, const EDirection dir);
    static void ToggleCodelChooser(SBlockInfo& rBlockInfo);
    static void RotateDirectionPointer(SBlockInfo& rBlockInfo, const int times);
    static PietToken::ETokenType::Enum ConvertColoursToInstruction(const SPietColour& rColour1, const SPietColour& rColour2);

    SRGB GetRGBFromLoation(const SLocation& rLoc) const;
    bool LocationIsSameColour(const SLocation& rLoc, const SRGB col) const;
    SPietColour GetPietColourFromLocation(const SLocation& rLoc) const;
    SLocation SlideAlongWhite(const SLocation& rLoc, const EDirection dir);
    bool IsValidLocation(const SLocation& rLoc) const;
    bool IsValidLocation(const SLocation& rLoc, const std::vector<std::vector<bool>>& visited) const;
    SLocation FindEndOfEdge(const SLocation& rEdgeLoc, const EDirection directionPointer, const EDirection chooser) const;
    void FindEndCodel(SBlockInfo& rBlockInfo);
    void FindEndWhiteCodel(SBlockInfo& rBlockInfo);
    SBlockInfo GetBlockInfo(const SLocation& rStartLocation, const EDirection startDirectionPointer, const EDirection StartCodelChooser);
};
