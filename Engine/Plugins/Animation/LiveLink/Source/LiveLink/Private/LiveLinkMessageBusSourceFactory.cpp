// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "LiveLinkMessageBusSourceFactory.h"

#include "ILiveLinkClient.h"
#include "LiveLinkMessageBusSource.h"
#include "LiveLinkMessageBusFinder.h"

#include "Features/IModularFeatures.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "LiveLinkMessageBusSourceFactory"

FText ULiveLinkMessageBusSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("SourceDisplayName", "Message Bus Source");
}

FText ULiveLinkMessageBusSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("SourceTooltip", "Creates a connection to a Message Bus based Live Link Source");
}

TSharedPtr<SWidget> ULiveLinkMessageBusSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
	return SNew(SLiveLinkMessageBusSourceFactory)
		.OnSourceSelected(FOnLiveLinkMessageBusSourceSelected::CreateUObject(this, &ULiveLinkMessageBusSourceFactory::OnSourceSelected, InOnLiveLinkSourceCreated));
}

TSharedPtr<ILiveLinkSource> ULiveLinkMessageBusSourceFactory::CreateSource(const FString& ConnectionString) const
{
	FString Name;
	if (!FParse::Value(*ConnectionString, TEXT("Name="), Name))
	{
		return TSharedPtr<ILiveLinkSource>();
	}

	const double TimeOffset = 0.0;
	return MakeShared<FLiveLinkMessageBusSource>(FText::FromString(Name), FText::GetEmpty(), FMessageAddress(), TimeOffset);
}

void ULiveLinkMessageBusSourceFactory::OnSourceSelected(FProviderPollResultPtr SelectedSource, FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
	if (SelectedSource.IsValid())
	{
#if WITH_EDITOR
		bool bDoesAlreadyExist = false;
		{
			ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
			TArray<FGuid> Sources = LiveLinkClient.GetSources();
			for (FGuid SourceGuid : Sources)
			{
				if (LiveLinkClient.GetSourceType(SourceGuid).ToString() == SelectedSource->Name)
				{
					bDoesAlreadyExist = true;
					break;
				}
			}
		}

		if (bDoesAlreadyExist)
		{
			if (EAppReturnType::No == FMessageDialog::Open(EAppMsgType::YesNo, EAppReturnType::Yes, LOCTEXT("AddSourceWithSameName", "This provider name already exist. Are you sure you want to add a new one?")))
			{
				return;
			}
		}
#endif

		TSharedPtr<FLiveLinkMessageBusSource> SharedPtr = MakeShared<FLiveLinkMessageBusSource>(FText::FromString(SelectedSource->Name), FText::FromString(SelectedSource->MachineName), SelectedSource->Address, SelectedSource->MachineTimeOffset);
		FString ConnectionString = FString::Printf(TEXT("Name=\"%s\""), *SelectedSource->Name);
		InOnLiveLinkSourceCreated.ExecuteIfBound(SharedPtr, MoveTemp(ConnectionString));
	}
}

#undef LOCTEXT_NAMESPACE