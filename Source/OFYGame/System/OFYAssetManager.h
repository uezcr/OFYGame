// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/DataAsset.h"
#include "OFYAssetManagerStartupJob.h"

#include "OFYAssetManager.generated.h"

class UOFYGameData;
class UOFYPawnData;

struct FOFYBundles
{
	static const FName Equipped;
};

/**
 * UOFYAssetManager
 *
 * 资产管理器的游戏实现，重写功能并存储游戏特定的类型。
 * 预计大多数游戏都想覆盖AssetManager，因为它为游戏特定的加载逻辑提供了一个好地方。
 * 这个类是通过在DefaultEngine.ini中设置'AssetManagerClassName'来使用。
 */
UCLASS(Config = Game)
class UOFYAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UOFYAssetManager();

	// 返回AssetManager单例对象。
	static UOFYAssetManager& Get();

	// 返回由TSoftObjectPtr引用的资产，如果资产还没有被加载，这将同步加载它。
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// 返回TSoftClassPtr所引用的子类，如果资产还没有被加载，这将同步加载它。
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);
	
	// 记录当前加载的所有资产，并由资产管理器跟踪。
	static void DumpLoadedAssets();

	const UOFYGameData& GetGameData();
	const UOFYPawnData* GetDefaultPawnData() const;

protected:
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
	{
		if (TObjectPtr<UPrimaryDataAsset> const * pResult = GameDataMap.Find(GameDataClass::StaticClass()))
		{
			return *CastChecked<GameDataClass>(*pResult);
		}

		// Does a blocking load if needed
		return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
	}


	// 同步加载资产。
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();

	// 以线程安全的方式添加一个已加载的资产以保留在内存中。
	void AddLoadedAsset(const UObject* Asset);

	//~UAssetManager interface
	virtual void StartInitialLoading() override;

	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);


protected:
	
	// Global game data asset to use.
	UPROPERTY(Config)
	TSoftObjectPtr<UOFYGameData> OFYGameDataPath;
	
	// Loaded version of the game data
	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;
	
	// 如果在PlayerState上没有设置玩家PawnData，在生成玩家Pawn时使用的PawnData
	UPROPERTY(Config)
	TSoftObjectPtr<UOFYPawnData> DefaultPawnData;

private:
	// 刷新StartupJobs数组，处理所有的启动工作。
	void DoAllStartupJobs();

	void InitializeAbilitySystem();
	
	// 在加载过程中定期调用，可用于向加载屏幕提供状态。
	void UpdateInitialGameContentLoadPercent(float GameContentPercent);
	
	// 启动时要执行的任务列表。用来跟踪启动进度。
	TArray<FOFYAssetManagerStartupJob> StartupJobs;
	
private:
	
	// 资产管理器加载和跟踪的资产。
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// 在修改加载资产列表时用于范围锁定。
	FCriticalSection LoadedAssetsCritical;
	
};

template<typename AssetType>
AssetType* UOFYAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
TSubclassOf<AssetType> UOFYAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}