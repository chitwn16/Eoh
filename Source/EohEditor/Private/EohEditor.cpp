#include "EohEditor.h"

#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_GAME_MODULE(FEohEditorModule, EohEditor);

DEFINE_LOG_CATEGORY(EohEditor)

#define LOCTEXT_NAMESPACE "EohEditor"

void FEohEditorModule::StartupModule()
{
    UE_LOG(EohEditor, Warning, TEXT("Startup"));
}

void FEohEditorModule::ShutdownModule()
{
    UE_LOG(EohEditor, Warning, TEXT("Shutdown"));
}

#undef LOCTEXT_NAMESPACE