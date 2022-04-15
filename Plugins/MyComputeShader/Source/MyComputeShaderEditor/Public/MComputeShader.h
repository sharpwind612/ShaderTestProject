#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Rendering/Texture2DResource.h"

class MComputeShader
{
    static void TestCS(ERHIFeatureLevel::Type FeatureLevel,
        int32 sizeX, int32 sizeY,
        FTexture2DResource* TmpUT,
        uint8 index,
        FTextureRenderTargetResource* resultTmpOut,
        FRHICommandListImmediate& CmdList);
public:  
    static  void RunMeowTestCS(ERHIFeatureLevel::Type FeatureLevel,
        int32 sizeX, int32 sizeY,
        FTexture2DResource* TmpUT,
        uint8 index,
        FTextureRenderTargetResource* resultTmpOut);
};