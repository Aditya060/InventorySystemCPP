// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUD.h"
#include "WorldActor.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryComponent"));
	// EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AMyPlayerController::Interact);
	InputComponent->BindAction("ToggleProfile", IE_Pressed, this, &AMyPlayerController::ToggleProfile);
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AMyPlayerController::ToggleInventory);
	InputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AMyPlayerController::ToggleMenu);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CharacterReference = Cast<AMyCharacter>(GetPawn());
	InventoryComponent->CharacterReference = CharacterReference;
	InventoryComponent->ControllerReference = this;


	InventoryComponent->Server_InitInventory(32);
	
	HUD_Reference = Cast<AMyHUD>(GetHUD());
	
	if (AMyHUD* HUD_Reference2 = Cast<AMyHUD>(GetHUD()))
	{
		HUDLayoutReference = HUD_Reference2->HUDLayoutReference;

		if (HUD_Reference2 && HUDLayoutReference)
		{
			InventoryComponent->InitializeInventoryManagerUI(HUDLayoutReference->InventoryUI);
		}
	}
}

void AMyPlayerController::UI_UseInventoryItem_Implementation(const uint8& InventorySlot)
{
	InventoryComponent->UseInventoryItem(InventorySlot);
	
	HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	HUD_Reference->RefreshWidgetUILayout(ELayout::Equipment);
}

void AMyPlayerController::UI_MoveInventoryItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	if (InventoryComponent->MoveInventoryItem(FromInventorySlot, ToInventorySlot))
	{
		HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	}
}

void AMyPlayerController::UI_DropInventoryItem_Implementation(const uint8& InventorySlot)
{
	IInventoryInterface::UI_DropInventoryItem_Implementation(InventorySlot);

	InventoryComponent->Server_DropItemFromInventory_Implementation(InventorySlot);
	RefreshWidgets();
}

void AMyPlayerController::UI_EquipInventoryItem_Implementation(const uint8& FromInventorySlot,
                                                               const uint8& ToInventorySlot)
{
	IInventoryInterface::UI_EquipInventoryItem_Implementation(FromInventorySlot, ToInventorySlot);

	InventoryComponent->Server_EquipFromInventory_Implementation(FromInventorySlot, ToInventorySlot);
	RefreshWidgets();
}

void AMyPlayerController::UI_UnEquipInventoryItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	IInventoryInterface::UI_UnEquipInventoryItem_Implementation(FromInventorySlot, ToInventorySlot);

	InventoryComponent->Server_UnEquipFromInventory_Implementation(FromInventorySlot, ToInventorySlot);
	RefreshWidgets();
}

void AMyPlayerController::Server_OnActorUsed_Implementation(AActor* Actor)
{
	OnActorUsed(Actor);
}
void AMyPlayerController::OnActorUsed(AActor* Actor)
{
	if (HasAuthority())
	{
		if (IsValid(Actor))
		{
			if(AWorldActor* WorldActor = Cast<AWorldActor>(Actor))
			{
				WorldActor->OnActorUsed_Implementation(this);

				InventoryComponent->Server_RefreshInventorySlots();
			}
		}
	}
}

void AMyPlayerController::ToggleProfile()
{
	HUD_Reference->ToggleWindow(ELayout::Equipment);
	
	if (HUD_Reference->IsAnyWidgetVisible())
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AMyPlayerController::ToggleInventory()
{
	HUD_Reference->ToggleWindow(ELayout::Inventory);

	if (HUD_Reference->IsAnyWidgetVisible())
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AMyPlayerController::ToggleMenu()
{
	if(InventoryComponent->AddItem(TEXT("Cardboard_Chest"), 1))
	{
		InventoryComponent->AddItem(TEXT("Cardboard_Boots"), 1);
		InventoryComponent->AddItem(TEXT("Cardboard_Gloves"), 1);
		
		HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
		PrintInventory();
	}
}

void AMyPlayerController::Interact()
{
	if (CharacterReference->UsableActorsInsideRange.Num() > 0)
	{
		AActor* Actor = CharacterReference->UsableActorsInsideRange[0];
		if (AWorldActor* WorldActor = Cast<AWorldActor>(Actor))
		{
			Server_OnActorUsed(Actor);

			InventoryComponent->AddItem(WorldActor->ID, WorldActor->Amount);
			//PrintInventory();
			//GetWorld()->DestroyActor(WorldActor);
		}
		
		HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	}
}
UUserWidget* AMyPlayerController::GetInteractWidget()
{
	return HUD_Reference->GetInteractWidget();
}
void AMyPlayerController::RefreshWidgets()
{
	HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	HUD_Reference->RefreshWidgetUILayout(ELayout::Equipment);
}

void AMyPlayerController::AddItemToInventoryAndToIndex(TArray<FSlotStructure> Inventory, FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	Inventory[InventorySlot] = ContentToAdd;
}

FSlotStructure AMyPlayerController::GetItemFrom(TArray<FSlotStructure> Inventory, const int8& SlotIndex)
{
	return Inventory[SlotIndex];
}

void AMyPlayerController::PrintInventory()
{
	for (int i = 0; i < InventoryComponent->NumberOfSlots; i++)
	{
		FText a = InventoryComponent->Inventory[i].ItemStructure.Name;
		uint8 b = InventoryComponent->Inventory[i].Amount;
		//uint8 c = W_InventoryLayout->InventorySlotsArray[i]->InventorySlotIndex;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s, Amount %i"),*a.ToString(), b));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s , Amount %i, Index: %i"), *a.ToString(), b, c));
	}
}