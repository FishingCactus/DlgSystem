// Copyright Csaba Molnar, Daniel Butum. All Rights Reserved.
#pragma once

#include "IDlgSystemEditorModule.h"
#include "Templates/SharedPointer.h"
#include "AssetTypeCategories.h"
#include "IAssetTypeActions.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Docking/WorkspaceItem.h"


class FSpawnTabArgs;
class UK2Node;
class UDlgDialogue;
struct FGraphPanelNodeFactory;
struct FGraphPanelPinFactory;
class FExtender;

// Implementation of the DlgSystemEditor Module
class DLGSYSTEMEDITOR_API FDlgSystemEditorModule : public IDlgSystemEditorModule
{
	typedef FDlgSystemEditorModule Self;
public:
	FDlgSystemEditorModule();

	//
	// IModuleInterface interface
	//

	void StartupModule() override;
	void ShutdownModule() override;

	EAssetTypeCategories::Type GetAssetCategory() const override { return DlgSystemAssetCategoryBit; }


	//
	// Own functions
	//

	// Create the menu extenders
	static TSharedRef<FExtender> CreateFileMenuExtender(
		TSharedRef<FUICommandList> Commands,
		const TArray<TSharedPtr<FUICommandInfo>>& AdditionalMenuEntries = {}
	);
	static TSharedRef<FExtender> CreateHelpMenuExtender(TSharedRef<FUICommandList> Commands);
	static void MapActionsForFileMenuExtender(TSharedRef<FUICommandList> Commands);
	static void MapActionsForHelpMenuExtender(TSharedRef<FUICommandList> Commands);

	/***
	* Pops up a class picker dialog to choose the class that is a child of the Classprovided.
	*
	* @param	TitleText		The title of the class picker dialog
	* @param	OutChosenClass  The class chosen (if this function returns false, this will be null) by the the user
	* @param	Class		    The children of this class we are displaying and prompting the user to choose from.
	*
	* @return true if OK was pressed, false otherwise
	*/
	static bool PickChildrenOfClass(const FText& TitleText, UClass*& OutChosenClass, UClass* Class);

	// Gets all child classes of ParentClass, NOTE: this is super slow, use with care
	// DOES NOT INCLUDE BLUEPRINT
	static TArray<UClass*> GetAllNativeChildClasses(const UClass* ParentClass);

	// Save all the dialogues.
	// @return True on success or false on failure.
	static bool SaveAllDialogues();

	// Deletes all teh dialogues text files
	// @return True on success or false on failure.
	static bool DeleteAllDialoguesTextFiles();

private:
	// Handle clicking on save all dialogues.
	static void HandleOnSaveAllDialogues();

	// Handle clicking on delete all dialogues text files.
	static void HandleOnDeleteAllDialoguesTextFiles();

	// Handle on post engine init event
	void HandleOnPostEngineInit();

	// Handle PIE events
	void HandleOnBeginPIE(bool bIsSimulating);
	void HandleOnPostPIEStarted(bool bIsSimulating);
	void HandleOnEndPIEHandle(bool bIsSimulating);

	// Extend the Menus of the editor
	void ExtendMenu();

private:
	// The submenu type of the dialog system
	EAssetTypeCategories::Type DlgSystemAssetCategoryBit;

	// All registered asset type actions. Cached here so that we can unregister them during shutdown.
	TArray<TSharedPtr<IAssetTypeActions>> RegisteredAssetTypeActions;

	// All registered custom class layouts for the details panel. Cached here so that we can unregister them during shutdown.
	TArray<FName> RegisteredCustomClassLayouts;

	// All registered custom property layouts for the details panel. Cached here so that we can unregister them during shutdown.
	TArray<FName> RegisteredCustomPropertyTypeLayout;

	// The factory of how the nodes look.
	TSharedPtr<FGraphPanelNodeFactory> DialogueGraphNodeFactory;

	// The factory of how the pins look.
	TSharedPtr<FGraphPanelPinFactory> DialogueGraphPinFactory;

	// Level Editor commands bound from this plugin.
	TSharedPtr<FUICommandList> LevelMenuEditorCommands;

	// The Tools Dialogue category.
	TSharedPtr<FWorkspaceItem> ToolsDialogueCategory;

	// Handlers
	FDelegateHandle OnPostEngineInitHandle;
	FDelegateHandle OnBeginPIEHandle;
	FDelegateHandle OnPostPIEStartedHandle; // after BeginPlay() has been called
	FDelegateHandle OnEndPIEHandle;

};
