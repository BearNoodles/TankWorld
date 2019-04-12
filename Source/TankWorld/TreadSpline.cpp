// Fill out your copyright notice in the Description page of Project Settings.


#include "TreadSpline.h"
#include "ConstructorHelpers.h"



// Sets default values
ATreadSpline::ATreadSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = m_root;

	m_spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	m_spline->SetupAttachment(RootComponent);

	m_splineMeshes = TArray<USplineMeshComponent*>();
	//m_splineMeshes->SetupAttachment(RootComponent);

	//m_root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));

	//m_spline->AddSplinePoint(FVector(0, 0, 0), ESplineCoordinateSpace::Local);
	//m_spline->AddSplinePoint(FVector(1, 0, 0), ESplineCoordinateSpace::Local);

	//m_staticMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> treadMesh(TEXT("StaticMesh'/Game/StaticMeshes/treadMesh.treadMesh'"));

	static ConstructorHelpers::FObjectFinder<UMaterial> goldMat(TEXT("Material'/Game/StarterContent/Materials/M_ColorGrid_LowSpec.M_ColorGrid_LowSpec'"));

	if (treadMesh.Succeeded())
	{
		m_staticMesh = treadMesh.Object;
	}

	if (goldMat.Succeeded())
	{
		m_splineMeshMaterial = goldMat.Object;
	}

	m_sectionLength = 40;
}

void ATreadSpline::OnConstruction(const FTransform& Transform)
{

	
	for (int i = 0; i < m_spline->GetNumberOfSplinePoints() - 1; i++)
	for (int i = 0; i < (int)(m_spline->GetSplineLength() / m_sectionLength); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"));
		//USplineMeshComponent* SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("Spline"));
		//USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>();
		m_splineMeshes.Add(NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass()));
		//m_splineMeshes.Add(SplineMesh);
		//USplineMeshComponent* SplineMesh = ConstructObject<USplineMeshComponent>(USplineMeshComponent::StaticClass(), this);

		m_splineMeshes.Last()->SetStaticMesh(m_staticMesh);

		m_splineMeshes.Last()->SetMaterial(0, m_splineMeshMaterial);

		m_splineMeshes.Last()->SetForwardAxis(ESplineMeshAxis::X);

		m_splineMeshes.Last()->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		m_splineMeshes.Last()->SetMobility(EComponentMobility::Movable);
		//SplineMesh->AttachParent = m_spline;
		m_splineMeshes.Last()->SetupAttachment(m_spline);

		m_splineMeshes.Last()->SetSimulatePhysics(true);
		m_splineMeshes.Last()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		//m_splineMeshes.Last()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		//Set the color!
		//UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(m_splineMeshMaterial, NULL);
		//dynamicMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(mSegments[i].mColor));

		m_splineMeshes.Last()->bCastDynamicShadow = false;

		//SplineMesh->SetMaterial(0, dynamicMat);


		FVector pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd;

		//pointLocationStart = m_spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		//pointLocationEnd = m_spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		//pointTangentStart = m_spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		//pointTangentEnd = m_spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);


		pointLocationStart = m_spline->GetLocationAtDistanceAlongSpline(i * m_sectionLength, ESplineCoordinateSpace::Local);
		pointLocationEnd = m_spline->GetLocationAtDistanceAlongSpline((i + 1) * m_sectionLength, ESplineCoordinateSpace::Local);
		pointTangentStart = m_spline->GetTangentAtDistanceAlongSpline(i * m_sectionLength, ESplineCoordinateSpace::Local).GetClampedToMaxSize(m_sectionLength);
		pointTangentEnd = m_spline->GetTangentAtDistanceAlongSpline((i + 1) * m_sectionLength, ESplineCoordinateSpace::Local).GetClampedToMaxSize(m_sectionLength);

		//m_spline->GetLocalLocationAndTangentAtSplinePoint(i, pointLocationStart, pointTangentStart);
		//m_spline->GetLocalLocationAndTangentAtSplinePoint(i + 1, pointLocationEnd, pointTangentEnd);

		m_splineMeshes.Last()->SetStartAndEnd(pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd);

		//TODO
		//CREATE PHYSICS CONSTRAINT ALONGSIDE EACH SPLINE MESH ATTACHING IT TO THE PREVIOUS ONE AND THEN ATTACH THE LAST ONE TO THE FIRST
		//SETUP SOCKETS ON THE TREAD MESH

		//SplineMesh->SetStaticMesh(m_staticMesh);
		//
		//SplineMesh->SetForwardAxis(ESplineMeshAxis::X);
		//
		//SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		//SplineMesh->SetMobility(EComponentMobility::Movable);
		////SplineMesh->AttachParent = m_spline;
		//SplineMesh->SetupAttachment(m_spline);
		//
		////Set the color!
		////UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(mSplineMeshMaterial, NULL);
		////dynamicMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(mSegments[i].mColor));
		//
		//SplineMesh->bCastDynamicShadow = false;
		//
		////SplineMesh->SetMaterial(0, dynamicMat);
		//
		////Width of the mesh 
		////SplineMesh->SetStartScale(FVector2D(50, 50));
		////SplineMesh->SetEndScale(FVector2D(50, 50));
		//
		//FVector pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd;
		//
		//pointLocationStart = m_spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		//pointLocationEnd = m_spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		//pointTangentStart = m_spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		//pointTangentEnd = m_spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		////m_spline->GetLocalLocationAndTangentAtSplinePoint(i, pointLocationStart, pointTangentStart);
		////m_spline->GetLocalLocationAndTangentAtSplinePoint(i + 1, pointLocationEnd, pointTangentEnd);
		//
		//SplineMesh->SetStartAndEnd(pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd);
	
	}
	
	RegisterAllComponents();
}

// Called when the game starts or when spawned
void ATreadSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreadSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

