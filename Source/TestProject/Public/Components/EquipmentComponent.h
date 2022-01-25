// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "EquipmentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UEquipmentComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitInventory(int32 NumberSlots) override;

	virtual void ToggleWindow() override;
	virtual void InitializeLayout() override;
	virtual void RefreshWidgetUI() override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Default")
	class UProfileLayout* W_ProfileLayout;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfEquipmentSlots;
	
};