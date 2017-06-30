// Fill out your copyright notice in the Description page of Project Settings.

#include "BlobmentsV1.h"
#include "CurrentLandingDecal.h"


ACurrentLandingDecal::ACurrentLandingDecal()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAssetPotential(TEXT("Material'/Game/TopDownCPP/Blueprints/M_PotentialLandingDecal'"));//M_Cursor_Decal.M_PotentialLandingDecal
	if (DecalMaterialAssetPotential.Succeeded())
	{
		SetDecalMaterial(DecalMaterialAssetPotential.Object);
	}
}




