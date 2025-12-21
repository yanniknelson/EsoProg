#pragma once

#include "PietImageTokeniser.h"  // for PietImageTokeniser
#include "PietTextTokeniser.h"   // for PietTextTokeniser
#include "PietToken.h"           // for PietToken

#include <CRuntime.h>            // for CRuntime
#include <ELanguages.h>          // for ELanguages::Enum
#include <IRuntime.h>            // for SRuntimeSyncronisationStruct
#include <ITokeniser.h>          // for ITokeniser
#include <Stack.h>               // for Stack

#include <GLFW/glfw3.h>          // for GLuint
#include <gl/GL.h>               // for GLuint

#include <sstream>               // for std::ostringstream
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////
class PietRuntime : public CRuntime<PietToken>
{
    using TPietTokeniser = ITokeniser<PietToken>;

  public:
    
    PietRuntime(SRuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream);

    virtual ELanguages::Enum GetRuntimeLanguage() const override;
    virtual std::vector<std::string> GetSupportedFileTypes() const override;

    //////////////////////////////////////////////////////////////
    enum class ESourceType
    {
        Text,
        Image,
        Invalid
    };

    void SetImage(GLuint* pTexture, const unsigned char* imageData, const int imageWidth, const int imageHeight);
    void UnsetImage();

    void SetCodelSize(const int size);

    virtual void ResetImplementation() override;

    virtual void RenderWindows() override;
    virtual void CacheState() override;

  private:
    PietTextTokeniser m_textTokeniser;
    PietImageTokeniser m_imageTokeniser;

    TPietTokeniser* m_activeTokeniser = nullptr;

    Stack m_stack;
    Stack m_cachedStack;

    GLuint* m_pTexture = nullptr;
    float m_aspectRatio = 1.f;
    std::string m_codelSizeStr{ "1" };
    int m_codelSize{ 1 };

    bool m_bForceImage = false;
    ESourceType m_currentSourceType{ ESourceType::Text };

    void RenderImageDisplay();

    virtual void OnSourceSet() override;

    virtual void OnInput(int val) override;

    virtual bool ShouldEnd(const PietToken& token) override;

    virtual PietToken StepExecution_Internal() override;
};
