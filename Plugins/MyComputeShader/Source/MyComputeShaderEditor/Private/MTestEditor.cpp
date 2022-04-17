// Fill out your copyright notice in the Description page of Project Settings.


#include "MTestEditor.h"
#include "ImageUtils.h"
#include "MComputeShader.h"
#include "Misc/FileHelper.h"
#include "IImageWrapper.h"  
#include "IImageWrapperModule.h"  
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"  
#include "Interfaces/IPluginManager.h"

// 新建Texture
UTexture2D* UMTestEditor::MCreatNewTexture(TArray<FColor>& TexColors, uint32 sizex, uint32 sizey)
{
	UTexture2D* Texture = UTexture2D::CreateTransient(sizex, sizey, PF_B8G8R8A8);

	uint8* MipData = static_cast<uint8*>(Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	for (size_t y = 0; y < sizey; y++)
	{
		uint8* DestPtr = &MipData[(sizey - 1 - y) * sizex * sizeof(FColor)];
		FColor* SrcPtr = const_cast<FColor*>(&TexColors[(sizey - 1 - y) * sizex]);
		for (size_t x = 0; x < sizex; x++)
		{
			*DestPtr++ = SrcPtr->B;
			*DestPtr++ = SrcPtr->G;
			*DestPtr++ = SrcPtr->R;
			*DestPtr++ = SrcPtr->A;
			SrcPtr++;
		}
	}

	Texture->PlatformData->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();

	return Texture;
}

// 导出UTexture
bool UMTestEditor::MExportUTexture2D(UTexture2D* tex, const FString& path)
{
	FTexture2DMipMap& mipmaps = tex->PlatformData->Mips[0];
	unsigned char* Data = (unsigned char*)mipmaps.BulkData.Lock(LOCK_READ_WRITE);
	int texturex = tex->PlatformData->SizeX;
	int texturey = tex->PlatformData->SizeY;
	TArray<FColor> colors;


	for (int32 y = 0; y < texturey; y++)
	{
		for (int32 x = 0; x < texturex; x++)
		{
			FColor bColor;
			bColor.B = Data[(y * texturex + x) * 4 + 0];
			bColor.G = Data[(y * texturex + x) * 4 + 1];
			bColor.R = Data[(y * texturex + x) * 4 + 2];
			bColor.A = Data[(y * texturex + x) * 4 + 3];
			colors.Add(bColor);
		}
	}
	mipmaps.BulkData.Unlock();
	tex->UpdateResource();

	TArray<uint8> texData;
	FImageUtils::CompressImageArray(texturex, texturey, colors, texData);

	return FFileHelper::SaveArrayToFile(texData, *path);
}

// 根据图片文件格式后缀名获得ImageWrapperPtr
static TSharedPtr<IImageWrapper> GetImageWrapperPtrByUrlExtention(const FString& ImageUrl)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	if (ImageUrl.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (ImageUrl.EndsWith(".jpg") || ImageUrl.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (ImageUrl.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	return nullptr;
}

// 根据图片文件路径加载图片
UTexture2D* UMTestEditor::LoadImageFromLocalUrl(const FString& ImageUrl)
{
	UTexture2D* LoadTexture = nullptr;
	// 判断文件路径是否有效
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImageUrl))
	{
		UE_LOG(LogTemp, Warning, TEXT("Image url is not exist!"));
		return nullptr;
	}

	TArray<uint8> RawFileData;
	// 将图片文件转换成uint8数据
	if (FFileHelper::LoadFileToArray(RawFileData, *ImageUrl))
	{
		// 获取图片的IImageWrapperPtr
		IImageWrapperPtr ImageWrapper = GetImageWrapperPtrByUrlExtention(ImageUrl);
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
		{
			// 载入图片数据
			TArray<uint8> UncompressedRGBA;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedRGBA))
			{
				// 创建一个空白纹理
				LoadTexture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

				// 改写纹理数据
				void* TextureData = LoadTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedRGBA.GetData(), UncompressedRGBA.Num());
				LoadTexture->PlatformData->Mips[0].BulkData.Unlock();

				// 更新纹理数据
				LoadTexture->UpdateResource();
			}
		}
	}
	return LoadTexture;
}

// 新建RT
UTextureRenderTarget2D* UMTestEditor::MCreatNewTexRT(uint32 sizex, uint32 sizey, FLinearColor clearColor)
{
	UTextureRenderTarget2D* result = NewObject<UTextureRenderTarget2D>(GWorld);
	check(result);
	result->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	result->ClearColor = clearColor;
	result->bAutoGenerateMips = 1;
	result->InitAutoFormat(sizex, sizey);
	result->UpdateResourceImmediate(true);
	return result;
}

// 获取RT的Texture
UTexture2D* UMTestEditor::MGetRTTexture(UTextureRenderTarget2D* result)
{
	UTexture2D* resultTex = result->ConstructTexture2D(GWorld, "", EObjectFlags::RF_NoFlags, CTF_DeferCompression);
	resultTex->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	resultTex->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	resultTex->SRGB = 1;
	resultTex->UpdateResource();
	return resultTex;
}


void UMTestEditor::MTest()
{
	UE_LOG(LogTemp, Display, TEXT("开始测试"));
	// 设置图片宽高
	int32 sizeX = 256;
	int32 sizeY = 256;
	int32 outputSizeX = 1024;
	int32 outputSizeY = 1024;
	// 设置input图片颜色数组
	TArray<FColor> TexColors;
	for (size_t i = 0; i < sizeX * sizeY; ++i)
	{
		TexColors.Add(FColor::Red);
	}
	FString BaseDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("MyComputeShader"))->GetBaseDir(), TEXT("Image"));
	UTexture2D* TmpTexture = LoadImageFromLocalUrl(FPaths::Combine(BaseDir, TEXT("rawImage.jpg")));

	// 新建Texture  红色
	//UTexture2D* TmpTexture = MCreatNewTexture(TexColors, sizeX, sizeY);

	// 获取Texture Resource
	FTexture2DResource* TmpUT = static_cast<FTexture2DResource*>(TmpTexture->Resource);

	// 新建RT  蓝色
	UTextureRenderTarget2D* resultTmp = MCreatNewTexRT(outputSizeX, outputSizeY, FLinearColor::Blue);
	// 获取RT Resource
	FTextureRenderTargetResource* resultTmpOut = resultTmp->GameThread_GetRenderTargetResource();

	// 执行ComputeShader测试
	MComputeShader::RunMeowTestCS(GWorld->Scene->GetFeatureLevel(), sizeX, sizeY, TmpUT, 1 /* G通道赋值*/, resultTmpOut);
	// 获取返回的RT Texture
	UTexture2D* resultTex = MGetRTTexture(resultTmp);

	// 导出查看结果
	// 红色
	MExportUTexture2D(TmpTexture, FPaths::Combine(BaseDir, TEXT("input.png")));
	// 黄色
	MExportUTexture2D(resultTex, FPaths::Combine(BaseDir, TEXT("output.png")));

	UE_LOG(LogTemp, Display, TEXT("测试完成"));
}