// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(YourLog, Log, All);

void AMyGameModeBase::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(YourLog, Warning, TEXT("=-= InitGame"));
	UE_LOG(YourLog, Warning, TEXT("=-= MapName=%s Options=%s Error=%s"), *MapName, *Options, *ErrorMessage);
}

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = nullptr;
	SpectatorClass = nullptr;
}
