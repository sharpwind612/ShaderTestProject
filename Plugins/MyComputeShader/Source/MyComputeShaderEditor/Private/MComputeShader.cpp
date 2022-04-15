#include "MyComputeShaderEditor/Public/MComputeShader.h"
#include "MyComputeShader/Public/TestSimpleShader.h"
#include "RHICommandList.h"
#include "RHIDefinitions.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RenderGraphUtils.h"
#include "UnrealClient.h"

void MComputeShader::TestCS(ERHIFeatureLevel::Type FeatureLevel,
        int32 sizeX, int32 sizeY,
        FTexture2DResource* TmpUT,
        uint8 index,
        FTextureRenderTargetResource* resultTmpOut,
        FRHICommandListImmediate& CmdList)
{
    // 传进来的
    // ————————————————————————————————————————————————————
    FTexture2DRHIRef tmpTextureRT = resultTmpOut->GetRenderTargetTexture();
    FUnorderedAccessViewRHIRef tmpTexture = RHICreateUnorderedAccessView(tmpTextureRT);
    // ————————————————————————————————————————————————————
    
	TShaderMapRef<TestSimpleShader> TestShaderRef(GetGlobalShaderMap(FeatureLevel));
	{
		TestSimpleShader::FParameters TestShaderParameter;
		{
			TestShaderParameter.Index = index;
			TestShaderParameter.tmpTexture_input = TmpUT->GetTexture2DRHI();
			TestShaderParameter.tmpTexture_out = tmpTexture;
		}
		FComputeShaderUtils::Dispatch(CmdList, TestShaderRef, TestShaderParameter, FIntVector(sizeX / 8, sizeY / 8, 1));
	}
}


// 在GameThread获取FTextureRenderTargetResource
void MComputeShader::RunMeowTestCS(ERHIFeatureLevel::Type FeatureLevel,
        int32 sizeX, int32 sizeY,
        FTexture2DResource* TmpUT,
        uint8 index,
        FTextureRenderTargetResource* resultTmpOut)
{
    ENQUEUE_RENDER_COMMAND(TestShaderCmd)
    (
    [=](FRHICommandListImmediate& RHICmdList)
        {
            TestCS(FeatureLevel, sizeX, sizeY, TmpUT, index, resultTmpOut,RHICmdList);
        }
    );
}