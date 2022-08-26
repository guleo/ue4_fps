// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Runtime/UMG/Public/Components/ListView.h>
#include "PlayerList.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FPS_API UPlayerList : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "player")
	int32 nIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "player")
	FString sPlayername;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "player")
	int32 nScore;


	//UFUNCTION(BlueprintCallable, Category = "player")
	//void SetPlayerView(UListView* listView);
	
	//UListView* m_listView;
};
