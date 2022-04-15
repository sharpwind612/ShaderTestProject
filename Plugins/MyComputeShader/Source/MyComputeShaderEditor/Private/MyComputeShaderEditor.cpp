#include "MyComputeShaderEditor.h"

DEFINE_LOG_CATEGORY(MyComputeShaderEditor);

#define LOCTEXT_NAMESPACE "FMyComputeShaderEditor"

void FMyComputeShaderEditor::StartupModule()
{
	UE_LOG(MyComputeShaderEditor, Warning, TEXT("MyComputeShaderEditor module has been loaded"));
}

void FMyComputeShaderEditor::ShutdownModule()
{
	UE_LOG(MyComputeShaderEditor, Warning, TEXT("MyComputeShaderEditor module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMyComputeShaderEditor, MyComputeShaderEditor)