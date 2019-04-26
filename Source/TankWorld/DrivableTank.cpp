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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> turretMesh(TEXT("StaticMesh'/Game/StaticMeshes/Turret.Turret'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> barrelMesh(TEXT("StaticMesh'/Game/StaticMeshes/Barrel2.Barrel2'"));

	static ConstructorHelpers::FClassFinder<AActor> projectile1(TEXT("Blueprint'/Game/BluePrints/Projectile1.Projectile1_C'"));
	static ConstructorHelpers::FClassFinder<AActor> projectile2(TEXT("Blueprint'/Game/BluePrints/Projectile2.Projectile2_C'"));

	static ConstructorHelpers::FObjectFinder<UMaterial> patchMat(TEXT("Material'/Game/StarterContent/Materials/M_ColorGrid_LowSpec.M_ColorGrid_LowSpec'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> goldMat(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> clearMat(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'"));
	//GetMesh()->Set
	m_tankRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	m_tankRootMesh->SetupAttachment(m_root);
	m_tankRootMesh->SetSimulatePhysics(true);
	//RootComponent = m_tankRootMesh;

	if (projectile1.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Default Projectile1 Class Found"));
		m_projectileType1 = projectile1.Class;
	}
	if (projectile2.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Default Projectile2 Class Found"));
		m_projectileType2 = projectile2.Class;
	}

	if (bodyMesh.Succeeded())
	{
		m_tankRootMesh->SetStaticMesh(bodyMesh.Object);
	}
	/*if (goldMat.Succeeded())
	{
		m_bodyMat = goldMat.Object;
		m_tankRootMesh->SetMaterial(0, m_bodyMat); m_wheelMat = patchMat.Object;
		for (int i = 0; i < m_wheelCount; i++)
		{
			m_wheelMeshArray[i]->SetMaterial(0, m_wheelMat);
		}
	}*/

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



	for (int i = 0; i < m_wheelCount; i++)
	{
		m_wheelMeshArray[i]->SetSimulatePhysics(true);
	}
	
	if (patchMat.Succeeded())
	{
		/*m_wheelMat = patchMat.Object;
		for (int i = 0; i < m_wheelCount; i++)
		{
			m_wheelMeshArray[i]->SetMaterial(0, m_wheelMat);
		}*/
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
	m_turretMeshX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	m_turretMeshX->SetupAttachment(m_tankRootMesh);
	m_turretMeshY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh2"));
	m_turretMeshY->SetupAttachment(m_turretMeshX);
	if (sphereMesh.Succeeded())
	{
		m_turretMeshX->SetStaticMesh(sphereMesh.Object);
		m_turretMeshY->SetStaticMesh(sphereMesh.Object);
		for (int i = 0; i < m_wheelCount; i++)
		{
			m_wheelMeshArray[i]->SetStaticMesh(sphereMesh.Object);
		}
	}
	if (turretMesh.Succeeded())
	{
		m_turretMeshX->SetStaticMesh(turretMesh.Object);
		m_turretMeshY->SetStaticMesh(turretMesh.Object);
	}
	m_turretMeshY->SetVisibility(false);

	m_turretMeshX->RelativeLocation = FVector(0.0f, 0.0f, 20.0f);
	m_turretMeshX->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	//m_turretMeshX->SetWorldScale3D(FVector(1.75f, 1.75f, 1.75f));
	m_turretMeshX->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	m_turretMeshY->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	m_turretMeshY->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	//m_turretMeshY->SetWorldScale3D(FVector(1.75f, 1.75f, 1.75f));
	m_turretMeshY->SetWorldScale3D(FVector(1.0, 1.0f, 1.0f));

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


	m_barrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	m_barrelMesh->SetupAttachment(m_turretMeshY);

	if (barrelMesh.Succeeded())
	{
		m_barrelMesh->SetStaticMesh(barrelMesh.Object); 
		
		//for (int i = 0; i < m_wheelCount; i++)
		//{
		//	m_wheelMeshArray[i]->SetStaticMesh(cylinderMesh.Object);
		//}
	}


	m_barrelMesh->RelativeLocation = FVector(100.0f, 0.0f, 60.0f);
	m_barrelMesh->RelativeRotation = FRotator(-90.0f, 0.0f, 0.0f);
	m_barrelMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	m_launchPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LaunchPoint"));
	m_launchPoint->SetupAttachment(m_turretMeshX);

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_springArm->SetupAttachment(m_turretMeshY);
	m_springArm->RelativeRotation = FRotator(-20.0f, 0.0f, 0.0f);
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

	m_torque = 3000000;

	m_turnTorque = 6000000;

	m_turretStartHeight = m_turretMeshY->GetComponentRotation().Pitch;

	m_turretCurrentHeight = 0;

	m_regenRate = 1;

	m_loadTimer = 0;
	m_loadTimeMax = 1;
	m_loadTimeRate = 1;

	m_isLoadedProjectile1 = false;
	m_isLoadedProjectile2 = false;

	m_fireReady = false;

	m_ammoMax = 1;
	if (goldMat.Succeeded())
	{
		m_bodyMat = goldMat.Object;
		m_tankRootMesh->SetMaterial(0, m_bodyMat); 
		m_wheelMat = patchMat.Object;
		m_barrelMesh->SetMaterial(0, m_bodyMat);
		m_turretMeshX->SetMaterial(0, m_bodyMat);
		for (int i = 0; i < m_wheelCount; i++)
		{
			m_wheelMeshArray[i]->SetMaterial(0, m_bodyMat);
		}
	}

	m_fireType = "NONE";

}



// Called when the game starts or when spawned
void ADrivableTank::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Your message111111111111111111111111111"));
}

// Called every frame
void ADrivableTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//UE_LOG(LogTemp, Warning, TEXT("Ammo Count %f"), m_ammoCurrent);
	//Ammo regen if ammo is not max
	/*if (m_ammoCurrent < m_ammoMax)
	{
		m_ammoTimer += DeltaTime;
		if (m_ammoTimer > 1 / m_regenRate)
		{
			m_ammoCurrent++;
			m_ammoTimer = 0;
		}
	}*/

	if ((m_isLoadedProjectile1 || m_isLoadedProjectile2) && !m_fireReady)
	{
		if (m_loadTimer < m_loadTimeMax)
		{
			m_loadTimer += DeltaTime / m_loadTimeRate;
		}
		else
		{
			m_fireReady = true;
			m_loadTimer = 0;
		}
	}


	//TODO press a key to launch a projectile

}

void ADrivableTank::Accelerate(float AxisValue)
{

	for (int i = 0; i < m_wheelCount; i++)
	{
		if (m_wheelMeshArray[i]->GetPhysicsAngularVelocityInRadians().Size() < 40.0f)
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
	m_turretMeshX->AddLocalRotation(NewRotation);
}

void ADrivableTank::TurnTurretY(float AxisValue)
{
	FRotator NewRotation = FRotator(AxisValue, 0, 0);

	if (m_turretCurrentHeight + AxisValue < -15 || m_turretCurrentHeight + AxisValue > 30)
	{
		return;
	}

	m_turretCurrentHeight += AxisValue;
	
	m_turretMeshY->AddRelativeRotation(NewRotation);
}

void ADrivableTank::Fire()
{
	if (!m_fireReady)
	{
		return;
	}
	else
	{

	}
	FName name = *FString::Printf(TEXT("LaunchPoint"));
	FVector Location = m_barrelMesh->GetSocketLocation(name);
	FRotator Rotation = m_turretMeshY->GetComponentRotation();
	FActorSpawnParameters SpawnInfo;
	if (m_isLoadedProjectile1)
	{
		GetWorld()->SpawnActor<AActor>(m_projectileType1, Location, Rotation);
		m_isLoadedProjectile1 = false;
	}
	else
	{
		GetWorld()->SpawnActor<AActor>(m_projectileType2, Location, Rotation);
		m_isLoadedProjectile2 = false;
	}

	m_fireReady = false;

	m_fireType = "NONE";
}

void ADrivableTank::Load1()
{
	if (m_isLoadedProjectile1 || m_isLoadedProjectile2)
	{
		return;
	}

	m_fireType = "Explosive";
	m_isLoadedProjectile1 = true;

}
void ADrivableTank::Load2()
{
	if (m_isLoadedProjectile1 || m_isLoadedProjectile2)
	{
		return;
	}

	m_fireType = "Heavy";
	m_isLoadedProjectile2 = true;
}

void ADrivableTank::SetProjectileActor(AActor* object)
{
	m_projectile = object;
}

// Called to bind functionality to input
void ADrivableTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Accelerate", this, &ADrivableTank::Accelerate);
	InputComponent->BindAxis("Turn Tank", this, &ADrivableTank::TurnTank);

	InputComponent->BindAxis("Turn Turret X", this, &ADrivableTank ::TurnTurretX);
	InputComponent->BindAxis("Turn Turret Y", this, &ADrivableTank ::TurnTurretY);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ADrivableTank::Fire);
	InputComponent->BindAction("Load1", IE_Pressed, this, &ADrivableTank::Load1);
	InputComponent->BindAction("Load2", IE_Pressed, this, &ADrivableTank::Load2);
}

FString ADrivableTank::GetFireType()
{
	return  m_fireType;
}

UPawnMovementComponent* ADrivableTank::GetMovementComponent() const
{
	return MyMovementComponent;
}


