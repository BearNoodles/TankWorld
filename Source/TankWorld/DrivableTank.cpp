// Fill out your copyright notice in the Description page of Project Settings.

#include "DrivableTank.h"
#include "ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
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

	m_wheelMeshArray = TArray<UStaticMeshComponent*>();
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL0")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL1")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL2")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL3")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR0")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR1")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR2")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR3")));


	/*m_wheelMeshL0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL0"));
	m_wheelMeshL1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL1"));
	m_wheelMeshL2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL2"));
	m_wheelMeshL3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL3"));
	m_wheelMeshR0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR0"));
	m_wheelMeshR1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR1"));
	m_wheelMeshR2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR2"));
	m_wheelMeshR3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR3"));*/

	//m_wheelMeshL0->SetSimulatePhysics(true);
	//m_wheelMeshL1->SetSimulatePhysics(true);
	//m_wheelMeshL2->SetSimulatePhysics(true);
	//m_wheelMeshL3->SetSimulatePhysics(true);
	//m_wheelMeshR0->SetSimulatePhysics(true);
	//m_wheelMeshR1->SetSimulatePhysics(true);
	//m_wheelMeshR2->SetSimulatePhysics(true);
	//m_wheelMeshR3->SetSimulatePhysics(true);

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


	m_constraintArray = TArray<UPhysicsConstraintComponent*>();

	m_constraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL0")));
	m_constraintArray[0]->ComponentName1.ComponentName = "WheelMeshL0";

	m_constraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL1")));
	m_constraintArray[1]->ComponentName1.ComponentName = "WheelMeshL1";

	m_constraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL2")));
	m_constraintArray[2]->ComponentName1.ComponentName = "WheelMeshL2";

	m_constraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintL3")));
	m_constraintArray[3]->ComponentName1.ComponentName = "WheelMeshL3";

	m_constraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR0")));
	m_constraintArray[4]->ComponentName1.ComponentName = "WheelMeshR0";

	m_constraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR1")));
	m_constraintArray[5]->ComponentName1.ComponentName = "WheelMeshR1";

	m_constraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR2")));
	m_constraintArray[6]->ComponentName1.ComponentName = "WheelMeshR2";

	m_constraintArray.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintR3")));
	m_constraintArray[7]->ComponentName1.ComponentName = "WheelMeshR3";

	for (int i = 0; i < m_wheelCount; i++)
	{
		m_constraintArray[i]->ComponentName2.ComponentName = "BodyMesh";
		m_constraintArray[i]->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
		m_constraintArray[i]->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
		m_constraintArray[i]->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 3.0f);
		m_constraintArray[i]->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
		m_constraintArray[i]->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
		m_constraintArray[i]->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
		m_constraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = 1;
		m_constraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = 100.1f;
		m_constraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 0.3;
		m_constraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.Restitution = 0.5f;
		m_constraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.ContactDistance = 5.0f;
		m_constraintArray[i]->SetDisableCollision(true);
	}

	m_constraintArray[0]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL0"));
	m_constraintArray[1]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL1"));
	m_constraintArray[2]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL2"));
	m_constraintArray[3]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL3"));
	m_constraintArray[4]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR0"));
	m_constraintArray[5]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR1"));
	m_constraintArray[6]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR2"));
	m_constraintArray[7]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR3"));

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
	//m_turretRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Turret"));
	m_turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	//m_turretMesh->SetupAttachment(m_tankRootMesh);
	m_turretMesh->SetupAttachment(m_tankRootMesh);

	if (sphereMesh.Succeeded())
	{
		m_turretMesh->SetStaticMesh(sphereMesh.Object);
		//m_wheelMeshL0->SetStaticMesh(sphereMesh.Object);
		//m_wheelMeshL1->SetStaticMesh(sphereMesh.Object);
		//m_wheelMeshL2->SetStaticMesh(sphereMesh.Object);
		//m_wheelMeshL3->SetStaticMesh(sphereMesh.Object);
		//m_wheelMeshR0->SetStaticMesh(sphereMesh.Object);
		//m_wheelMeshR1->SetStaticMesh(sphereMesh.Object);
		//m_wheelMeshR2->SetStaticMesh(sphereMesh.Object);
		//m_wheelMeshR3->SetStaticMesh(sphereMesh.Object);
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
		m_wheelMeshArray[i]->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	}
	m_wheelMeshArray[0]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL0"));
	m_wheelMeshArray[1]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL1"));
	m_wheelMeshArray[2]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL2"));
	m_wheelMeshArray[3]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL3"));
	m_wheelMeshArray[4]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR0"));
	m_wheelMeshArray[5]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR1"));
	m_wheelMeshArray[6]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR2"));
	m_wheelMeshArray[7]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR3"));

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

	//m_turretRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Turret"));
	m_barrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	//m_barrelMesh->SetupAttachment(m_turretMesh);
	m_barrelMesh->SetupAttachment(m_turretMesh);

	if (cylinderMesh.Succeeded())
	{
		m_barrelMesh->SetStaticMesh(cylinderMesh.Object);
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

	/*if (isAccelerating)
	{
		Accelerate();
	}
	else
	{
		StopAccelerate();
	}
	if (isTurningLeft)
	{
		TurnTankL();
	}
	else
	{
		StopTurnTankL();
	}
	if (isTurningRight)
	{
		TurnTankR();
	}
	else
	{
		StopTurnTankR();
	}*/

	//if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{

	}
	//CurrentVelocity += m_acceleration * m_tankRootMesh->GetForwardVector();
	//if(!CurrentVelocity.IsZero())
	//{
	//	AddActorLocalOffset(CurrentVelocity * DeltaTime);
	//}

	//m_tankRootMesh->AddForce(FVector(0.0f, 0.0f, -9.8f));
	//m_tankRootMesh->SetPhysicsLinearVelocity(m_tankRootMesh->GetComponentVelocity().Size() * m_tankRootMesh->GetForwardVector());
	//m_root->AddForce(m_acceleration);
	if (m_tankRootMesh->GetComponentVelocity().Size() < m_maxSpeed)
	{

		//m_tankRootMesh->AddForce(m_acceleration * m_tankRootMesh->GetForwardVector());
		//UE_LOG(LogTemp, Warning, TEXT("Your messageGOOOOOOOOOOOOOOOOOOOOOOO"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Your messageSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("Turning"));
	//AddActorLocalRotation(TurnAmount * DeltaTime);
	SetActorRelativeRotation(TurnAmount * DeltaTime * 100);

	TurnAmount.Yaw = 0;
	//TODO press a key to launch a projectile

}

void ADrivableTank::Accelerate(float AxisValue)
{
	// Move at 100 units per second right or left
	//m_tankRootMesh->AddForce(m_tankRootMesh->GetForwardVector() * 1000000);
	//CurrentVelocity = FMath::Clamp(AxisValue, -1.0f, 1.0f) * m_speed * m_tankRootMesh->GetForwardVector();
	//CurrentVelocity = FMath::Clamp(AxisValue, -1.0f, 1.0f) * m_speed * m_tankRootMesh->GetForwardVector();
	//m_acceleration = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 15000;
	//m_acceleration = FMath::Clamp(AxisValue, -1.0f, 1.0f) * m_root->GetForwardVector() * 2500000;


	//m_acceleration = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1500000;

	for (int i = 0; i < m_wheelCount; i++)
	{
		if (m_wheelMeshArray[i]->GetPhysicsAngularVelocityInRadians().Size() < 20.0f)
		{
			FQuat myActorQuat = m_wheelMeshArray[i]->GetComponentQuat();
			m_wheelMeshArray[i]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);
		}
	}
	
	/*
	FQuat myActorQuatL0 = m_wheelMeshL0->GetComponentQuat();
	m_wheelMeshL0->AddTorqueInRadians(myActorQuatL0.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);

	FQuat myActorQuatL1 = m_wheelMeshL1->GetComponentQuat();
	m_wheelMeshL1->AddTorqueInRadians(myActorQuatL1.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);

	FQuat myActorQuatL2 = m_wheelMeshL2->GetComponentQuat();
	m_wheelMeshL2->AddTorqueInRadians(myActorQuatL2.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);

	FQuat myActorQuatL3 = m_wheelMeshL3->GetComponentQuat();
	m_wheelMeshL3->AddTorqueInRadians(myActorQuatL3.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);

	FQuat myActorQuatR0 = m_wheelMeshR0->GetComponentQuat();
	m_wheelMeshR0->AddTorqueInRadians(myActorQuatR0.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);

	FQuat myActorQuatR1 = m_wheelMeshR1->GetComponentQuat();
	m_wheelMeshR1->AddTorqueInRadians(myActorQuatR1.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);

	FQuat myActorQuatR2 = m_wheelMeshR2->GetComponentQuat();
	m_wheelMeshR2->AddTorqueInRadians(myActorQuatR2.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);

	FQuat myActorQuatR3 = m_wheelMeshR3->GetComponentQuat();
	m_wheelMeshR3->AddTorqueInRadians(myActorQuatR3.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);*/



	//m_wheelMesh0->AddTorqueInRadians(FVector(0, 5000000, 0));
	//m_wheelMesh1->AddTorqueInRadians(FVector(0, 5000000, 0));
	//m_wheelMesh2->AddTorqueInRadians(FVector(0, 5000000, 0));
	//m_wheelMesh3->AddTorqueInRadians(FVector(0, 5000000, 0));

	

	/*FRotator NewRotation2 = FRotator(0, AxisValue, 0);
	m_wheelMesh0->AddLocalRotation(NewRotation2);
	m_wheelMesh1->AddLocalRotation(NewRotation2);
	m_wheelMesh2->AddLocalRotation(NewRotation2);
	m_wheelMesh3->AddLocalRotation(NewRotation2);*/
	
}

void ADrivableTank::TurnTank(float AxisValue)
{
	//m_root->AddTorque(FVector(0, 10000000 , 10000000 * AxisValue));
	//m_root->AddTorqueInRadians(FVector(0, 10000000 , 10 * AxisValue));
	//m_root->AddAngularImpulse(FVector(0, 0, 1000000 * AxisValue));

	//FQuat myActorQuat = GetActorQuat();
	//m_root->AddTorque(myActorQuat.RotateVector(FVector(0.0f, 0.0f, 10000000 * AxisValue)), NAME_None, false);

	//m_turretRoot->GetComponentTransform.GetComponentRotation();
	//FRotator NewRotation = GetActorRotation();
	//FRotator NewRotation2 = m_root->GetComponentRotation();
	//FRotator NewRotation2 = FRotator(0,0,0);
	//FRotator NewRotation2 = FRotator(0,AxisValue,0);
	//NewRotation2.Yaw += AxisValue;
	//TurnAmount = FRotator(0,0,0);
	//TurnAmount.Yaw = AxisValue;
	//SetActorRotation(NewRotation);
	//AddActorLocalRotation(NewRotation);
	//m_turretRoot->GetComponentTransform().SetRotation(NewRotation);
	//m_root->SetWorldRotation(NewRotation2);
	
	//m_root->SetRelativeRotation(NewRotation2);
	//m_root->AddLocalRotation(NewRotation2);
	//AddActorLocalRotation(NewRotation2);


	//FRotator NewRotation2 = FRotator(0, AxisValue, 0);
	//m_tankRootMesh->AddLocalRotation(NewRotation2);

	/*for (int i = 0; i < 4; i++)
	{
		if (m_wheelMeshArray[i]->GetPhysicsAngularVelocityInRadians().Size() < 10.0f)
		{
			FQuat myActorQuat = m_wheelMeshArray[i]->GetComponentQuat();
			m_wheelMeshArray[i]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_turnTorque * -AxisValue, 0)), NAME_None, false);
		}
		else if (m_wheelMeshArray[i]->GetPhysicsAngularVelocityInRadians().X < 0.0f && AxisValue > 0)
		{
			FQuat myActorQuat = m_wheelMeshArray[i]->GetComponentQuat();
			m_wheelMeshArray[i]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_turnTorque * -AxisValue, 0)), NAME_None, false);
		}
		else
		{
			continue;
		}
	}*/
	float tankVel = m_tankRootMesh->GetComponentVelocity().Size();
	UE_LOG(LogTemp, Warning, TEXT("Vel %f"), tankVel);
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

		//else if (tankVel > 200)
		//{
		//	if (AxisValue < 0)
		//	{
		//		//Left Wheel spin backwards
		//		FQuat myActorQuat = m_wheelMeshArray[i]->GetComponentQuat();
		//		m_wheelMeshArray[i]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_turnTorque * AxisValue, 0)), NAME_None, false);
		//	}
		//	else if (AxisValue > 0)
		//	{
		//		//Right Wheel spin backwards
		//		FQuat myActorQuat = m_wheelMeshArray[i - 4]->GetComponentQuat();
		//		m_wheelMeshArray[i - 4]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_turnTorque * -AxisValue, 0)), NAME_None, false);
		//	}
		//}

		////If A key is down and left wheel is spinning forward, slow it down
		//else if (AxisValue < 0 && m_wheelMeshArray[i]->GetPhysicsAngularVelocityInRadians().X < 0.0f)
		//{
		//	FQuat myActorQuat = m_wheelMeshArray[i]->GetComponentQuat();
		//	m_wheelMeshArray[i]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_turnTorque * 3 * AxisValue, 0)), NAME_None, false);
		//}
		////If D key is down and right wheel is spinning forward, slow it down
		//else if(AxisValue > 0 && m_wheelMeshArray[i - 4]->GetPhysicsAngularVelocityInRadians().X < 0.0f)
		//{
		//	FQuat myActorQuat = m_wheelMeshArray[i - 4]->GetComponentQuat();
		//	m_wheelMeshArray[i - 4]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_turnTorque * 3 * -AxisValue, 0)), NAME_None, false);
		//}

	}
}


//void ADrivableTank::TurnTankR()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Your messageRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR"));
//	//m_tankRootMesh->AddTorque(FVector(0,0,1000));
//	//m_turretRoot->GetComponentTransform.GetComponentRotation();
//	//FRotator NewRotation = GetActorRotation();
//	//FRotator NewRotation = m_tankRootMesh->GetComponentRotation();
//	//NewRotation.Yaw += AxisValue;
//	////SetActorRotation(NewRotation);
//	//m_tankRootMesh->SetRelativeRotation(NewRotation);
//	//m_turretRoot->GetComponentTransform().SetRotation(NewRotation);
//}

void ADrivableTank::TurnTurretX(float AxisValue)
{
	//m_turretRoot->GetComponentTransform.GetComponentRotation();
	//FRotator NewRotation = GetActorRotation();
	
	//FRotator NewRotation = m_turretMesh->GetComponentRotation();
	//FRotator NewRotation = FRotator(0, AxisValue, 0);
	//NewRotation.Yaw += AxisValue;
	//SetActorRotation(NewRotation);
	//m_turretMesh->SetRelativeRotation(NewRotation);
	//m_turretMesh->AddLocalRotation(NewRotation);
	//m_turretRoot->GetComponentTransform().SetRotation(NewRotation);

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

