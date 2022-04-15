// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyComputeShader.h"
#include "Interfaces/IPluginManager.h"
#define LOCTEXT_NAMESPACE "FMyComputeShaderModule"

void FMyComputeShaderModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString ShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("MyComputeShader"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/MyComputerShader/Shaders"), ShaderDir);
}

void FMyComputeShaderModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyComputeShaderModule, MyComputeShader)