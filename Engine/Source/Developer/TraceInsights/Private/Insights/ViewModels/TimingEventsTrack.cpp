// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TimingEventsTrack.h"

#define LOCTEXT_NAMESPACE "TimingEventsTrack"

////////////////////////////////////////////////////////////////////////////////////////////////////
// FTimingEventsTrackLayout
////////////////////////////////////////////////////////////////////////////////////////////////////

void FTimingEventsTrackLayout::ForceNormalMode()
{
	bIsCompactMode = false;
	EventH = NormalLayoutEventH;
	EventDY = NormalLayoutEventDY;
	TimelineDY = NormalLayoutTimelineDY;
	MinTimelineH = NormalLayoutMinTimelineH;
	TargetMinTimelineH = NormalLayoutMinTimelineH;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FTimingEventsTrackLayout::ForceCompactMode()
{
	bIsCompactMode = true;
	EventH = CompactLayoutEventH;
	EventDY = CompactLayoutEventDY;
	TimelineDY = CompactLayoutTimelineDY;
	MinTimelineH = CompactLayoutMinTimelineH;
	TargetMinTimelineH = CompactLayoutMinTimelineH;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FTimingEventsTrackLayout::Update()
{
	constexpr float LayoutTransitionSpeed = 0.25f;

	if (bIsCompactMode)
	{
		if (EventH > CompactLayoutEventH)
		{
			EventH -= LayoutTransitionSpeed;
		}
		if (EventDY > CompactLayoutEventDY)
		{
			EventDY -= LayoutTransitionSpeed;
		}
		if (TimelineDY > CompactLayoutTimelineDY)
		{
			TimelineDY -= LayoutTransitionSpeed;
		}
	}
	else
	{
		if (EventH < NormalLayoutEventH)
		{
			EventH += LayoutTransitionSpeed;
		}
		if (EventDY < NormalLayoutEventDY)
		{
			EventDY += LayoutTransitionSpeed;
		}
		if (TimelineDY < NormalLayoutTimelineDY)
		{
			TimelineDY += LayoutTransitionSpeed;
		}
	}

	if (MinTimelineH > TargetMinTimelineH)
	{
		MinTimelineH -= LayoutTransitionSpeed;
	}
	else if (MinTimelineH < TargetMinTimelineH)
	{
		MinTimelineH += LayoutTransitionSpeed;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// FTimingEventsTrack
////////////////////////////////////////////////////////////////////////////////////////////////////

bool FTimingEventsTrack::bUseDownSampling = true;

////////////////////////////////////////////////////////////////////////////////////////////////////

FTimingEventsTrack::FTimingEventsTrack(uint64 InTrackId, const FName& InType, const FName& InSubType, const FString& InName)
	: FBaseTimingTrack(InTrackId)
	, Type(InType)
	, SubType(InSubType)
	, Name(InName)
	, Order(0)
	, NumLanes(0)
	, bIsCollapsed(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

FTimingEventsTrack::~FTimingEventsTrack()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FTimingEventsTrack::Reset()
{
	FBaseTimingTrack::Reset();

	NumLanes = 0;
	bIsCollapsed = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FTimingEventsTrack::UpdateHoveredState(float MouseX, float MouseY, const FTimingTrackViewport& Viewport)
{
	constexpr float HeaderWidth = 100.0f;
	constexpr float HeaderHeight = 14.0f;

	if (MouseY >= GetPosY() && MouseY < GetPosY() + GetHeight())
	{
		SetHoveredState(true);
		SetHeaderHoveredState(MouseX < HeaderWidth && MouseY < GetPosY() + HeaderHeight);
	}
	else
	{
		SetHoveredState(false);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
