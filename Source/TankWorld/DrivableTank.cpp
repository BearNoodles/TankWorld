// Fill out your copyright notice in the Description page of Project Settings.

#include "DrivableTank.h"
#include "ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TankPawnMovementComponent.h"


// Sets default values
ADrivableTank::ADrivableTank()
{
	m_root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//m_root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = m_root;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> bodyMesh(TEXT("StaticMesh'/Game/StaticMeshes/Body2.Body2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> treadMesh(TEXT("StaticMesh'/Game/StaticMeshes/Tread2.Tread2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	static ConstructorHelpers::FObjectFinder<UMaterial> patchMat(TEXT("Material'/Game/StarterContent/Materials/M_ColorGrid_LowSpec.M_ColorGrid_LowSpec'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> goldMat(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'"));
	//GetMesh()->Set
	m_tankRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	m_tankRootMesh->SetupAttachment(m_root);
	m_tankRootMesh->SetSimulatePhysics(true);
	//RootComponent = m_tankRootMesh;

	if (bodyMesh.Succeeded())
	{
		m_tankRootMesh->SetStaticMesh(bodyMesh.Object);
	}
	if (goldMat.Succeeded())
	{
		m_bodyMat = goldMat.Object;
		m_tankRootMesh->SetMaterial(0, m_bodyMat);
	}

	m_tankRootMesh->SetAllMassScale(3.0f);

	m_tankRootMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	m_tankRootMesh->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_tankRootMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));


	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	FAttachmentTransformRules rules2 = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);

	m_wheelMeshArray = TArray<UStaticMeshComponent*>();
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL0")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL1")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL2")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL3")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR0")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR1")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR2")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR3")));

	if (treadMesh.Succeeded())
	{
		m_treadMesh = treadMesh.Object;
		
	}


	for (int i = 0; i < m_wheelCount; i++)
	{
		m_wheelMeshArray[i]->SetSimulatePhysics(true);
	}

	if (patchMat.Succeeded())
	{
		m_wheelMat = patchMat.Object;
		for (int i = 0; i < m_wheelCount; i++)
		{
			m_wheelMeshArray[i]->SetMaterial(0, m_wheelMat);
		}
	}


	m_wheelConstraintArray = TArray<UPhysicsConstraintComponent*>();

	m_wheelConstraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL0")));
	m_wheelConstraintArray[0]->ComponentName1.ComponentName = "WheelMeshL0";

	m_wheelConstraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL1")));
	m_wheelConstraintArray[1]->ComponentName1.ComponentName = "WheelMeshL1";

	m_wheelConstraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL2")));
	m_wheelConstraintArray[2]->ComponentName1.ComponentName = "WheelMeshL2";

	m_wheelConstraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL3")));
	m_wheelConstraintArray[3]->ComponentName1.ComponentName = "WheelMeshL3";

	m_wheelConstraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR0")));
	m_wheelConstraintArray[4]->ComponentName1.ComponentName = "WheelMeshR0";

	m_wheelConstraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR1")));
	m_wheelConstraintArray[5]->ComponentName1.ComponentName = "WheelMeshR1";

	m_wheelConstraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR2")));
	m_wheelConstraintArray[6]->ComponentName1.ComponentName = "WheelMeshR2";

	m_wheelConstraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR3")));
	m_wheelConstraintArray[7]->ComponentName1.ComponentName = "WheelMeshR3";

	for (int i = 0; i < m_wheelCount; i++)
	{
		m_wheelConstraintArray[i]->ComponentName2.ComponentName = "BodyMesh";
		m_wheelConstraintArray[i]->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
		m_wheelConstraintArray[i]->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
		m_wheelConstraintArray[i]->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 3.0f);
		m_wheelConstraintArray[i]->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
		m_wheelConstraintArray[i]->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
		m_wheelConstraintArray[i]->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = 1;
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = 100.1f;
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 0.3;
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.Restitution = 0.5f;
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.ContactDistance = 5.0f;
		m_wheelConstraintArray[i]->SetDisableCollision(true);
	}

	m_wheelConstraintArray[0]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL0"));
	m_wheelConstraintArray[1]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL1"));
	m_wheelConstraintArray[2]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL2"));
	m_wheelConstraintArray[3]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL3"));
	m_wheelConstraintArray[4]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR0"));
	m_wheelConstraintArray[5]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR1"));
	m_wheelConstraintArray[6]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR2"));
	m_wheelConstraintArray[7]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR3"));

#pragma region MyRegion


	//m_constraintL0 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL0"));
	//m_constraintL0->ComponentName1.ComponentName = "WheelMeshL0";
	//m_constraintL0->ComponentName2.ComponentName = "BodyMesh";
	//m_constraintL0->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL0->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL0->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL0->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintL0->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintL0->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	//m_constraintL0->SetDisableCollision(true);
	//m_constraintL0->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL0"));
	//
	//m_constraintL1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL1"));
	//m_constraintL1->ComponentName1.ComponentName = "WheelMeshL1";
	//m_constraintL1->ComponentName2.ComponentName = "BodyMesh";
	//m_constraintL1->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL1->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL1->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintL1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintL1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	//m_constraintL1->SetDisableCollision(true);
	//m_constraintL1->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL1"));
	//
	//m_constraintL2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL2"));
	//m_constraintL2->ComponentName1.ComponentName = "WheelMeshL2";
	//m_constraintL2->ComponentName2.ComponentName = "BodyMesh";
	//m_constraintL2->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL2->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL2->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintL2->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintL2->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	//m_constraintL2->SetDisableCollision(true);
	//m_constraintL2->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL2"));
	//
	//m_constraintL3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL3"));
	//m_constraintL3->ComponentName1.ComponentName = "WheelMeshL3";
	//m_constraintL3->ComponentName2.ComponentName = "BodyMesh";
	//m_constraintL3->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL3->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL3->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintL3->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintL3->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintL3->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	//m_constraintL3->SetDisableCollision(true);
	//m_constraintL3->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL3"));
	//
	//m_constraintR0 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR0"));
	//m_constraintR0->ComponentName1.ComponentName = "WheelMeshR0";
	//m_constraintR0->ComponentName2.ComponentName = "BodyMesh";
	//m_constraintR0->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR0->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR0->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR0->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintR0->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintR0->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	//m_constraintR0->SetDisableCollision(true);
	//m_constraintR0->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR0"));
	//			
	//m_constraintR1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR1"));
	//m_constraintR1->ComponentName1.ComponentName = "WheelMeshR1";
	//m_constraintR1->ComponentName2.ComponentName = "BodyMesh";
	//m_constraintR1->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR1->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR1->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintR1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintR1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	//m_constraintR1->SetDisableCollision(true);
	//m_constraintR1->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR1"));
	//			
	//m_constraintR2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR2"));
	//m_constraintR2->ComponentName1.ComponentName = "WheelMeshR2";
	//m_constraintR2->ComponentName2.ComponentName = "BodyMesh";
	//m_constraintR2->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR2->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR2->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintR2->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintR2->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	//m_constraintR2->SetDisableCollision(true);
	//m_constraintR2->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR2"));
	//			
	//m_constraintR3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR3"));
	//m_constraintR3->ComponentName1.ComponentName = "WheelMeshR3";
	//m_constraintR3->ComponentName2.ComponentName = "BodyMesh";
	//m_constraintR3->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR3->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR3->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//m_constraintR3->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintR3->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	//m_constraintR3->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	//m_constraintR3->SetDisableCollision(true);
	//m_constraintR3->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR3"));

#pragma endregion
	m_turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	m_turretMesh->SetupAttachment(m_tankRootMesh);

	if (sphereMesh.Succeeded())
	{
		m_turretMesh->SetStaticMesh(sphereMesh.Object);
		for (int i = 0; i < m_wheelCount; i++)
		{
			m_wheelMeshArray[i]->SetStaticMesh(sphereMesh.Object);
		}
	}

	m_turretMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	m_turretMesh->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_turretMesh->SetWorldScale3D(FVector(1.75f, 1.75f, 1.0f));

	for (int i = 0; i < m_wheelCount; i++)
	{
		m_wheelMeshArray[i]->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
		//m_wheelMeshArray[i]->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
		m_wheelMeshArray[i]->SetRelativeScale3D(FVector(1.0f, 0.5f, 1.0f));
	}
	m_wheelMeshArray[0]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL0"));
	m_wheelMeshArray[1]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL1"));
	m_wheelMeshArray[2]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL2"));
	m_wheelMeshArray[3]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL3"));
	m_wheelMeshArray[4]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR0"));
	m_wheelMeshArray[5]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR1"));
	m_wheelMeshArray[6]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR2"));
	m_wheelMeshArray[7]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR3"));


	treadMeshes0 = TArray<UStaticMeshComponent*>();
	treadMeshes1 = TArray<UStaticMeshComponent*>();
	treadMeshes2 = TArray<UStaticMeshComponent*>();
	treadMeshes3 = TArray<UStaticMeshComponent*>();
	treadMeshes4 = TArray<UStaticMeshComponent*>();
	treadMeshes5 = TArray<UStaticMeshComponent*>();
	treadMeshes6 = TArray<UStaticMeshComponent*>();
	treadMeshes7 = TArray<UStaticMeshComponent*>();

	for (int i = 0; i < 8; i++)
	{
		FName name = *FString::Printf(TEXT("Baesh0%i"),i);
		treadMeshes0.Add(CreateDefaultSubobject<UStaticMeshComponent>(name));
		name = *FString::Printf(TEXT("Baesh1%i"), i);
		treadMeshes1.Add(CreateDefaultSubobject<UStaticMeshComponent>(name));
		name = *FString::Printf(TEXT("Baesh2%i"), i);
		treadMeshes2.Add(CreateDefaultSubobject<UStaticMeshComponent>(name));
		name = *FString::Printf(TEXT("Baesh3%i"), i);
		treadMeshes3.Add(CreateDefaultSubobject<UStaticMeshComponent>(name));
		name = *FString::Printf(TEXT("Baesh4%i"), i);
		treadMeshes4.Add(CreateDefaultSubobject<UStaticMeshComponent>(name));
		name = *FString::Printf(TEXT("Baesh5%i"), i);
		treadMeshes5.Add(CreateDefaultSubobject<UStaticMeshComponent>(name));
		name = *FString::Printf(TEXT("Baesh6%i"), i);
		treadMeshes6.Add(CreateDefaultSubobject<UStaticMeshComponent>(name));
		name = *FString::Printf(TEXT("Baesh7%i"), i);
		treadMeshes7.Add(CreateDefaultSubobject<UStaticMeshComponent>(name)); 
		
		SetupTreads(i);
	}
	//meshTest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMe"));
	//meshTest->SetStaticMesh(m_treadMesh);
#pragma region MyRegion


	//m_wheelMeshL0->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	////m_wheelMeshL0->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	//m_wheelMeshL0->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	//m_wheelMeshL0->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL0"));
	//
	//m_wheelMeshL1->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	////m_wheelMeshL1->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	//m_wheelMeshL1->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	//m_wheelMeshL1->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL1"));
	//
	//m_wheelMeshL2->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	////m_wheelMeshL2->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	//m_wheelMeshL2->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	//m_wheelMeshL2->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL2"));
	//
	//m_wheelMeshL3->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	////m_wheelMeshL3->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	//m_wheelMeshL3->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	//m_wheelMeshL3->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL3"));
	//
	//m_wheelMeshR0->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	////m_wheelMeshR0->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	//m_wheelMeshR0->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	//m_wheelMeshR0->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR0"));
	//
	//m_wheelMeshR1->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	////m_wheelMeshR1->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	//m_wheelMeshR1->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	//m_wheelMeshR1->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR1"));
	//
	//m_wheelMeshR2->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	////m_wheelMeshR2->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	//m_wheelMeshR2->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	//m_wheelMeshR2->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR2"));
	//
	//m_wheelMeshR3->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	////m_wheelMeshR3->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	//m_wheelMeshR3->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	//m_wheelMeshR3->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR3"));

#pragma endregion
	//meshTest->AttachToComponent(m_wheelMeshArray[0], rules2, NAME_None);




	//m_turretRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Turret"));
	m_barrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	//m_barrelMesh->SetupAttachment(m_turretMesh);
	m_barrelMesh->SetupAttachment(m_turretMesh);

	if (cylinderMesh.Succeeded())
	{
		m_barrelMesh->SetStaticMesh(cylinderMesh.Object); 
		
		//for (int i = 0; i < m_wheelCount; i++)
		//{
		//	m_wheelMeshArray[i]->SetStaticMesh(cylinderMesh.Object);
		//}
	}


	m_barrelMesh->RelativeLocation = FVector(0.0f, 0.0f, 80.0f);
	m_barrelMesh->RelativeRotation = FRotator(-90.0f, 0.0f, 0.0f);
	m_barrelMesh->SetWorldScale3D(FVector(0.4f, 0.4f, 2.5f));

	m_launchPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LaunchPoint"));
	m_launchPoint->SetupAttachment(m_turretMesh);

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_springArm->SetupAttachment(m_turretMesh);
	m_springArm->RelativeRotation = FRotator(-15.0f, 0.0f, 0.0f);
	m_springArm->RelativeLocation = FVector(-40.0f, 0.0f, 110.0f);
	m_springArm->TargetArmLength = 400.0f;
	m_springArm->bEnableCameraLag = true;
	m_springArm->CameraLagSpeed = 3.0f;

	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_camera->SetupAttachment(m_springArm, USpringArmComponent::SocketName);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MyMovementComponent = CreateDefaultSubobject<UTankPawnMovementComponent>(TEXT("MovementComponent"));
	MyMovementComponent->UpdatedComponent = RootComponent;

	m_speed = 300;

	m_maxSpeed = 500;

	m_torque = 2000000;

	m_turnTorque = 4000000;

	static ConstructorHelpers::FObjectFinder<UClass> treadFinder(TEXT("Blueprint'/Game/BluePrints/TreadSplineBP.TreadSplineBP_C'"));
	//CreateTreads();
}



// Called when the game starts or when spawned
void ADrivableTank::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Your message111111111111111111111111111"));
}

// Called every frame
void ADrivableTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Ammo regen if ammo is not max
	if (m_ammoCurrent < m_ammoMax)
	{
		m_ammoTimer += DeltaTime;
		if (m_ammoTimer > 1 / m_regenRate)
		{
			m_ammoCurrent++;
			m_ammoTimer = 0;
		}
	}

	UpdateTreads();

	if (m_tankRootMesh->GetComponentVelocity().Size() < m_maxSpeed)
	{

		//m_tankRootMesh->AddForce(m_acceleration * m_tankRootMesh->GetForwardVector());
		//UE_LOG(LogTemp, Warning, TEXT("Your messageGOOOOOOOOOOOOOOOOOOOOOOO"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Your messageSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS"));
	}

	SetActorRelativeRotation(TurnAmount * DeltaTime * 100);

	TurnAmount.Yaw = 0;
	//TODO press a key to launch a projectile

}

void ADrivableTank::UpdateTreads()
{
	for (int i = 0; i < 8; i++)
	{
		
		//UE_LOG(LogTemp, Warning, TEXT("Vel %f"), (m_tankRootMesh->GetComponentLocation().X - m_wheelMeshArray[0]->GetComponentLocation().X));
		UE_LOG(LogTemp, Warning, TEXT("Vel %f"), (m_tankRootMesh->GetComponentLocation().Z - m_wheelMeshArray[0]->GetComponentLocation().Z));
		//treadMeshes0[i]->AddLocalOffset(FVector(0, 0, m_wheelMeshArray[0]->RelativeLocation.Z));
		//treadMeshes0[i]->SetRelativeLocationAndRotation(FVector(treadMeshes0[i]->GetComponentLocation().X, treadMeshes0[i]->GetComponentLocation().Y, 
			//treadMeshes0[i]->GetComponentLocation().Z), FRotator(0, 0, 0));// +
			//(m_tankRootMesh->GetComponentLocation().Z - m_wheelMeshArray[0]->GetComponentLocation().Z)), FRotator(0,0,0));
	}
}

void ADrivableTank::Accelerate(float AxisValue)
{

	for (int i = 0; i < m_wheelCount; i++)
	{
		if (m_wheelMeshArray[i]->GetPhysicsAngularVelocityInRadians().Size() < 20.0f)
		{
			FQuat myActorQuat = m_wheelMeshArray[i]->GetComponentQuat();
			m_wheelMeshArray[i]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);
		}
	}
	
}

void ADrivableTank::TurnTank(float AxisValue)
{
	
	float tankVel = m_tankRootMesh->GetComponentVelocity().Size();
	//UE_LOG(LogTemp, Warning, TEXT("Vel %f"), tankVel);
	//Loop over 4 sets of 2 wheels 
	for (int i = 4; i < 8; i++)
	{
		//if the wheels arent spinning fast enough then spin them
		if (m_wheelMeshArray[i]->GetPhysicsAngularVelocityInRadians().Size() < 10.0f || tankVel > 200)
		{
			//Left Wheel
			FQuat myActorQuat = m_wheelMeshArray[i]->GetComponentQuat();
			m_wheelMeshArray[i]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_turnTorque * AxisValue, 0)), NAME_None, false);

			//Right Wheel
			FQuat myActorQuat2 = m_wheelMeshArray[i - 4]->GetComponentQuat();
			m_wheelMeshArray[i - 4]->AddTorqueInRadians(myActorQuat2.RotateVector(FVector(0, m_turnTorque * -AxisValue, 0)), NAME_None, false);
		}


	}
}


void ADrivableTank::TurnTurretX(float AxisValue)
{
	FRotator NewRotation = FRotator(0, AxisValue, 0);
	m_turretMesh->AddLocalRotation(NewRotation);
}



// Called to bind functionality to input
void ADrivableTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Accelerate", this, &ADrivableTank::Accelerate);
	InputComponent->BindAxis("Turn Tank", this, &ADrivableTank::TurnTank);

	InputComponent->BindAxis("Turn Turret X", this, &ADrivableTank ::TurnTurretX);
}

UPawnMovementComponent* ADrivableTank::GetMovementComponent() const
{
	return MyMovementComponent;
}


void ADrivableTank::SetupTreads(int i)
{
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	FName name;
	treadMeshes0[i]->SetStaticMesh(m_treadMesh);

	treadMeshes0[i]->RelativeLocation = FVector(-(m_socketGap / 2) + i * 10.0f, 0.0f, 0.0f);
	treadMeshes0[i]->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	treadMeshes0[i]->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	name = *FString::Printf(TEXT("socketL00"));
	treadMeshes0[i]->AttachToComponent(m_tankRootMesh, rules, name);


	treadMeshes1[i]->SetStaticMesh(m_treadMesh);

	treadMeshes1[i]->RelativeLocation = FVector(-(m_socketGap / 2) + i * 10.0f, 0.0f, 0.0f);
	treadMeshes1[i]->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	treadMeshes1[i]->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	name = *FString::Printf(TEXT("socketL01"));
	treadMeshes1[i]->AttachToComponent(m_tankRootMesh, rules, name);


	treadMeshes2[i]->SetStaticMesh(m_treadMesh);

	treadMeshes2[i]->RelativeLocation = FVector(-(m_socketGap / 2) + i * 10.0f, 0.0f, 0.0f);
	treadMeshes2[i]->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	treadMeshes2[i]->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	name = *FString::Printf(TEXT("socketL02"));
	treadMeshes2[i]->AttachToComponent(m_tankRootMesh, rules, name);


	treadMeshes3[i]->SetStaticMesh(m_treadMesh);

	treadMeshes3[i]->RelativeLocation = FVector(-(m_socketGap / 2) + i * 10.0f, 0.0f, 0.0f);
	treadMeshes3[i]->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	treadMeshes3[i]->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	name = *FString::Printf(TEXT("socketL03"));
	treadMeshes3[i]->AttachToComponent(m_tankRootMesh, rules, name);


	treadMeshes4[i]->SetStaticMesh(m_treadMesh);

	treadMeshes4[i]->RelativeLocation = FVector(-(m_socketGap / 2) + i * 10.0f, 0.0f, 0.0f);
	treadMeshes4[i]->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	treadMeshes4[i]->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	name = *FString::Printf(TEXT("socketR00"));
	treadMeshes4[i]->AttachToComponent(m_tankRootMesh, rules, name);


	treadMeshes5[i]->SetStaticMesh(m_treadMesh);

	treadMeshes5[i]->RelativeLocation = FVector(-(m_socketGap / 2) + i * 10.0f, 0.0f, 0.0f);
	treadMeshes5[i]->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	treadMeshes5[i]->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	name = *FString::Printf(TEXT("socketR01"));
	treadMeshes5[i]->AttachToComponent(m_tankRootMesh, rules, name);


	treadMeshes6[i]->SetStaticMesh(m_treadMesh);

	treadMeshes6[i]->RelativeLocation = FVector(-(m_socketGap / 2) + i * 10.0f, 0.0f, 0.0f);
	treadMeshes6[i]->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	treadMeshes6[i]->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	name = *FString::Printf(TEXT("socketR02"));
	treadMeshes6[i]->AttachToComponent(m_tankRootMesh, rules, name);


	treadMeshes7[i]->SetStaticMesh(m_treadMesh);

	treadMeshes7[i]->RelativeLocation = FVector(-(m_socketGap / 2) + i * 10.0f, 0.0f, 0.0f);
	treadMeshes7[i]->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	treadMeshes7[i]->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	name = *FString::Printf(TEXT("socketR03"));
	treadMeshes7[i]->AttachToComponent(m_tankRootMesh, rules, name);


}

