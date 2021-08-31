// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <memory>
#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"

DECLARE_LOG_CATEGORY_EXTERN(MyLog,Log,All);

/**
 * 
 */
class DEMO1_API Global
{
public:
	Global();
	~Global();

	FStreamableManager& GetStreamableManager();

	static std::shared_ptr<Global> Ins();
private:
	static std::shared_ptr<Global> self;

	FStreamableManager StreamableManager;

};
