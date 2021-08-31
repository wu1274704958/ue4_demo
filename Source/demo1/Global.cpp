// Fill out your copyright notice in the Description page of Project Settings.


#include "Global.h"

DEFINE_LOG_CATEGORY(MyLog);

std::shared_ptr<Global> Global::self = nullptr;

Global::Global()
{

}

Global::~Global()
{
}

FStreamableManager& Global::GetStreamableManager()
{
	return StreamableManager;
}

std::shared_ptr<Global> Global::Ins()
{
	if (self == nullptr)
	{
		self = std::make_shared<Global>();
	}
	return self;
}
