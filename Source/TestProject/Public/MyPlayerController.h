// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryManagerComponent.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API AMyPlayerController : public APlayerController, public IInventoryInterface
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	/* Interface */
	virtual void UI_UseInventoryItem_Implementation(const uint8& InventorySlot) override;
	virtual void UI_MoveInventoryItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_DropInventoryItem_Implementation(const uint8& InventorySlot) override;
	virtual void UI_EquipInventoryItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_UnEquipInventoryItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	/* Ends Interface */
	
	FSlotStructure GetItemFrom(TArray<FSlotStructure> Inventory, const int8& SlotIndex);
	
	void AddItemToInventoryAndToIndex(TArray<FSlotStructure> Inventory, FSlotStructure& ContentToAdd, const uint8& InventorySlot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMyHUD* HUD_Reference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UHUDLayout* HUDLayoutReference;

	UPROPERTY()
	UMainLayout* MainHUD;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Default", OverrideNativeName="InventoryComponent"))
	UInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Default", OverrideNativeName="EquipmentComponent"))
	UEquipmentComponent* PlayerInventoryComponent;

	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleProfile();
	
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleMenu();
	
	UFUNCTION()
	void RefreshWidgets();

	UFUNCTION(Server, Reliable)
	void Server_OnActorUsed(AActor* Actor);

	// Server Drops an Actor on World
	UFUNCTION(Server, Reliable)
	void Server_OnActorDropped(FSlotStructure LocalSlot);
	
	UFUNCTION()
	UUserWidget* GetInteractWidget();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	UPROPERTY(VisibleAnywhere, Category="Character")
	AMyCharacter* CharacterReference;

	UFUNCTION(BlueprintCallable, Category="Character")
	void Interact();
	
	UFUNCTION()
	void PrintInventory();
	
	UFUNCTION()
	void OnActorUsed(AActor* Actor);

	UFUNCTION()
	void OnActorDropped(FSlotStructure LocalSlot);
};