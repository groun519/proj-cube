// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Project_Cube/ProjectCube_GameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeProjectCube_GameModeBase() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	PROJECT_CUBE_API UClass* Z_Construct_UClass_AProjectCube_GameModeBase();
	PROJECT_CUBE_API UClass* Z_Construct_UClass_AProjectCube_GameModeBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_Project_Cube();
// End Cross Module References
	void AProjectCube_GameModeBase::StaticRegisterNativesAProjectCube_GameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AProjectCube_GameModeBase);
	UClass* Z_Construct_UClass_AProjectCube_GameModeBase_NoRegister()
	{
		return AProjectCube_GameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AProjectCube_GameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AProjectCube_GameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Project_Cube,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AProjectCube_GameModeBase_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AProjectCube_GameModeBase_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "ProjectCube_GameModeBase.h" },
		{ "ModuleRelativePath", "ProjectCube_GameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AProjectCube_GameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AProjectCube_GameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AProjectCube_GameModeBase_Statics::ClassParams = {
		&AProjectCube_GameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AProjectCube_GameModeBase_Statics::Class_MetaDataParams), Z_Construct_UClass_AProjectCube_GameModeBase_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_AProjectCube_GameModeBase()
	{
		if (!Z_Registration_Info_UClass_AProjectCube_GameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AProjectCube_GameModeBase.OuterSingleton, Z_Construct_UClass_AProjectCube_GameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AProjectCube_GameModeBase.OuterSingleton;
	}
	template<> PROJECT_CUBE_API UClass* StaticClass<AProjectCube_GameModeBase>()
	{
		return AProjectCube_GameModeBase::StaticClass();
	}
	AProjectCube_GameModeBase::AProjectCube_GameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AProjectCube_GameModeBase);
	AProjectCube_GameModeBase::~AProjectCube_GameModeBase() {}
	struct Z_CompiledInDeferFile_FID_Users_groun_Documents_github_repositorys_proj_cube_Project_Cube_Source_Project_Cube_ProjectCube_GameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_groun_Documents_github_repositorys_proj_cube_Project_Cube_Source_Project_Cube_ProjectCube_GameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AProjectCube_GameModeBase, AProjectCube_GameModeBase::StaticClass, TEXT("AProjectCube_GameModeBase"), &Z_Registration_Info_UClass_AProjectCube_GameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AProjectCube_GameModeBase), 560842397U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_groun_Documents_github_repositorys_proj_cube_Project_Cube_Source_Project_Cube_ProjectCube_GameModeBase_h_2855394077(TEXT("/Script/Project_Cube"),
		Z_CompiledInDeferFile_FID_Users_groun_Documents_github_repositorys_proj_cube_Project_Cube_Source_Project_Cube_ProjectCube_GameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_groun_Documents_github_repositorys_proj_cube_Project_Cube_Source_Project_Cube_ProjectCube_GameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
