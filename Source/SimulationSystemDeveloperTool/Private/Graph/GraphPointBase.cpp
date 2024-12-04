// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GraphPointBase.h"

#include "SimulationSystemDeveloperSettings.h"


void AGraphPointBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	auto MeshConfig = GetMutableDefault<USimulationSystemDeveloperSettings>()->ShapeMesh;
	if(MeshConfig.IsValid())
	{
		Mesh->SetStaticMesh(MeshConfig.LoadSynchronous());
		auto MaterialConfig = GetMutableDefault<USimulationSystemDeveloperSettings>()->ShapeMaterial;
		if(MaterialConfig.IsValid())
		{
			Mesh->SetMaterial(0, MaterialConfig.LoadSynchronous());
		}
	}
}

// Sets default values
AGraphPointBase::AGraphPointBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeLocation({0.0f, 0.0f, GetMutableDefault<USimulationSystemDeveloperSettings>()->PointZOffset});
	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void AGraphPointBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGraphPointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGraphPointBase::SetVertexID(FSimVertexID NewID)
{
	ID = NewID;
}

FSimVertexID AGraphPointBase::GetVertexID()
{
	return ID;
}

