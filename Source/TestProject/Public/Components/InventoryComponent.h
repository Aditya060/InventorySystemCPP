// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tuples.h"
#include "Components/ActorComponent.h"
#include "Inventory/FSlotStructure.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	bool EquipFromInventory(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlotStructure> Inventory;
	
	virtual void InitInventory(int32 NumberSlots = 32);
	
	UFUNCTION()
	bool AddItem(FName ID, uint8 Amount);

	UFUNCTION()
	bool AddItemToInventory(FSlotStructure& ContentToAdd);

	UFUNCTION()
	FSlotStructure GetItemFromInventory(const uint8& InventorySlot);

	UFUNCTION()
	FSlotStructure GetEmptySlot();
	FSlotStructure GetItemFromItemDB(FName Name);

	UFUNCTION()
	virtual bool MoveInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);
	
	UFUNCTION()
	void AddItemToIndex(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);

	UFUNCTION()
	void UseInventoryItem(const uint8& InventorySlot);

private:
	UPROPERTY()
	UDataTable* ItemDB;
	
	FReturnTupleBoolInt HasPartialStack(const FSlotStructure& ContentToAdd);
	
	UFUNCTION()
	bool CreateStack(FSlotStructure& ContentToAdd);
	UFUNCTION()
	bool AddToStack(FSlotStructure& ContentToAdd, const int8& Index);


	UFUNCTION()
	void UseConsumableItem(const uint8& InventorySlot, FSlotStructure& InventoryItem);

	UFUNCTION()
	void RemoveFromItemAmount(FSlotStructure& InventoryItem, const uint8& AmountToRemove, bool& WasFullAmountRemoved, uint8& AmountRemoved);

	UFUNCTION()
	void RemoveItem(TArray<FSlotStructure> OutInventory, const uint8& InventorySlot);
};