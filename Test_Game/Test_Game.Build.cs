// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Test_Game : ModuleRules
{
	public Test_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Blutility", "UMGEditor", "MediaAssets", "MediaAssets" });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"Projects",
			"InputCore",
			"CoreUObject",
			"AnimGraph",
			"Slate",
			"SlateCore",
			"UnrealEd",
			"LevelEditor",
			"EditorStyle",
			"AssetTools",
			"Kismet",
			"Persona",
			"Slate",
			"MaterialUtilities",
			"MeshMergeUtilities",
			"MaterialBaking",
			"SlateCore",
			"DesktopPlatform",
			"ApplicationCore",
			"CinematicCamera",
			"MessageLog",
			"Json",
			"JsonUtilities",
			"FBX",
			"Blutility",
			"UMGEditor",
			"HTTP",
			"MovieSceneTools",
			"LevelSequenceEditor",
			"Sequencer",
			"SequencerWidgets",
			"MovieScene",
			"Foliage",
			"EditorScriptingUtilities",
			"MovieSceneTracks",
			"AlembicLib",
			"AlembicImporter",
			"AlembicLibrary",
			"NiagaraCore",
			"Niagara",
			"NiagaraShader",
			"VectorVM",
			"RHI",
			"AssetTags",
			"FoliageEdit",
			"HairStrandsCore",
			"StatsViewer",
			"GeometryCache",
			"GeometryCacheTracks",
			"MainFrame",
			"ToolMenus",
			"UMG", "PythonScriptPlugin",
			"MovieScene",
			"SequencerScriptingEditor",
			"GeometryCacheTracks",
			"LevelSequenceEditor",
			"BlueprintGraph","EditorFramework","Landscape", "Launch","MovieRenderPipelineEditor",
			"MeshDescription",
			"Engine",
			"UnrealEd",
			"ContentBrowser",
			"LevelSequence",
			"EditorScriptingUtilities",
			"Niagara",
			"NiagaraShader",
			"VectorVM",
			"RHI",
			"RenderCore",
			"NiagaraVertexFactories",
			"Sockets",
			"JsonUtilities",
			"UMG",
			"Blutility",
			"Slate",
			"GameplayAbilities"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
