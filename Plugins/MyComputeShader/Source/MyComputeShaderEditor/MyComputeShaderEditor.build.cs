using UnrealBuildTool;
 
public class MyComputeShaderEditor : ModuleRules
{
	public MyComputeShaderEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd", "RenderCore", "RHI", "MyComputeShader" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Core", "RenderCore", "MyComputeShader" });

		//PublicIncludePaths.AddRange(new string[] {"MyComputeShaderEditor/Public"});
		//PrivateIncludePaths.AddRange(new string[] {"MyComputeShaderEditor/Private"});
	}
}