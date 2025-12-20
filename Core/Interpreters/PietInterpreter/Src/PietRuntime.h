#pragma once

#include "PietImageTokeniser.h"
#include "PietTextTokeniser.h"
#include "PietToken.h"

#include <CRuntime.h>
#include <ELanguages.h>
#include <IRuntime.h>
#include <ITokeniser.h>
#include <Stack.h>

#include <GLFW/glfw3.h>

#include <sstream>
#include <string>
#include <vector>

class PietRuntime : public CRuntime<PietToken>
{
    using TPietTokeniser = ITokeniser<PietToken>;

  public:
    PietRuntime(RuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : CRuntime(rSync, rOutputStream, rExecutionhistoryStream)
    {
        m_activeTokeniser = (TPietTokeniser*)&m_textTokeniser;
    };

    virtual ELanguages::Enum GetRuntimeLanguage() const override
    {
        return ELanguages::Piet;
    }
    virtual std::vector<std::string> GetSupportedFileTypes() const override
    {
        return { ".txt", ".jpg", ".png", ".gif", ".ppm" };
    }

    enum class SourceType
    {
        Text,
        Image,
        Invalid
    };

    void SetImage(GLuint* pTexture, const unsigned char* imageData, const int imageWidth, const int imageHeight)
    {
        m_currentSourceType = SourceType::Image;
        m_pTexture = pTexture;
        m_aspectRatio = (float)imageHeight / (float)imageWidth;
        m_imageTokeniser.SetImage(imageData, imageWidth, imageHeight);
        m_activeTokeniser = &m_imageTokeniser;
    }

    void UnsetImage()
    {
        m_currentSourceType = SourceType::Invalid;
        m_activeTokeniser = (TPietTokeniser*)&m_textTokeniser;
        m_pTexture = nullptr;
        m_aspectRatio = 1.f;
        m_imageTokeniser.UnsetImage();
    }

    void SetCodelSize(const int size);

    virtual void ResetImplementation() override
    {
        m_stack.Clear();
        m_textTokeniser.SetTextStream(m_code);
        m_imageTokeniser.Reset();
    };

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
    SourceType m_currentSourceType{ SourceType::Text };

    void RenderImageDisplay();

    virtual void OnSourceSet() override;

    virtual void OnInput(int val) override;

    virtual bool ShouldEnd(const PietToken& token) override;

    virtual PietToken StepExecution_Internal() override;
};
