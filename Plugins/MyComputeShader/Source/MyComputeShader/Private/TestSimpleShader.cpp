#include "TestSimpleShader.h"
#include "GlobalShader.h"
// IMPLEMENT_SHADER_TYPE 这个宏一定要写cpp里面
IMPLEMENT_SHADER_TYPE(, TestSimpleShader, TEXT("/Plugins/MyComputeShader/Shaders/TestCS.usf"), TEXT("CSTest"), SF_Compute)