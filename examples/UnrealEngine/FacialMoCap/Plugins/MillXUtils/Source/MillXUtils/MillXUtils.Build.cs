namespace UnrealBuildTool.Rules
{
	public class MillXUtils : ModuleRules
	{
		public MillXUtils(ReadOnlyTargetRules Target) : base(Target)
        {
            PublicIncludePaths.AddRange(new string[] {});
            PrivateIncludePaths.AddRange(new string[] {});

            PublicIncludePathModuleNames.AddRange(
                new string[] {
                    "Core",
                });

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                });

            PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                    "Engine",
                });


		}
	}
}