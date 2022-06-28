﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WindowLayout.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UWindowLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void ToggleWindow();
	
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	AMyPlayerController* PlayerController;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WindowTitle;
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void OnButtonQuitClicked();
	
	UPROPERTY(meta = (BindWidget))	
	class UBorder* TopBorder;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UFUNCTION()
	virtual void SetTitleToWindow(FText Text);
	
	virtual FEventReply RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);
	
	//virtual void MyFunction(FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	
private:
	UFUNCTION()
	virtual void InitializeSlots();

	// Creates the Widgets that will belong to the parent Widget.
	UFUNCTION()
	virtual void CreateChildWidgets();

	/**
	 * Set an index to an array of Child Widgets.
	 *
	 * @param IndexStart The first index that it will added to the array.
	 */
	UFUNCTION()
	virtual void SetIndexToChilds(uint8& IndexStart);

	
	UPROPERTY()
	class UDragWidget* DragWidget;
	
	FVector2D DragOffset;
};