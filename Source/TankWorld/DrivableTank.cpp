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
	//GetMesh()->Set
	m_tankRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	m_tankRootMesh->SetupAttachment(m_root);
	m_tankRootMesh->SetSimulatePhysics(true);
	//RootComponent = m_tankRootMesh;

	if (cubeMesh.Succeeded())
	{
		m_tankRootMesh->SetStaticMesh(bodyMesh.Object);
	}

	m_tankRootMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	m_tankRootMesh->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_tankRootMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));


	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);


	m_wheelMesh0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh0"));
	m_wheelMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh1"));
	m_wheelMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh2"));
	m_wheelMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh3"));

	m_wheelMesh0->SetSimulatePhysics(true);
	m_wheelMesh1->SetSimulatePhysics(true);
	m_wheelMesh2->SetSimulatePhysics(true);
	m_wheelMesh3->SetSimulatePhysics(true);

	m_constraint0 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Contraint0"));
	m_constraint0->ComponentName1.ComponentName = "WheelMesh0";
	m_constraint0->ComponentName2.ComponentName = "BodyMesh";
	m_constraint0->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint0->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint0->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint0->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	m_constraint0->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	m_constraint0->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	m_constraint0->SetDisableCollision(true);
	m_constraint0->AttachToComponent(m_tankRootMesh, rules, TEXT("socket0"));
	
	m_constraint1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Contraint1"));
	m_constraint1->ComponentName1.ComponentName = "WheelMesh1";
	m_constraint1->ComponentName2.ComponentName = "BodyMesh";
	m_constraint1->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint1->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint1->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	m_constraint1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	m_constraint1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	m_constraint1->SetDisableCollision(true);
	m_constraint1->AttachToComponent(m_tankRootMesh, rules, TEXT("socket1"));

	m_constraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Contraint2"));
	m_constraint2->ComponentName1.ComponentName = "WheelMesh2";
	m_constraint2->ComponentName2.ComponentName = "BodyMesh";
	m_constraint2->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint2->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint2->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	m_constraint2->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	m_constraint2->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	m_constraint2->SetDisableCollision(true);
	m_constraint2->AttachToComponent(m_tankRootMesh, rules, TEXT("socket2"));

	m_constraint3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Contraint3"));
	m_constraint3->ComponentName1.ComponentName = "WheelMesh3";
	m_constraint3->ComponentName2.ComponentName = "BodyMesh";
	m_constraint3->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint3->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint3->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	m_constraint3->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	m_constraint3->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);
	m_constraint3->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);
	m_constraint3->SetDisableCollision(true);
	m_constraint3->AttachToComponent(m_tankRootMesh, rules, TEXT("socket3"));

	//m_turretRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Turret"));
	m_turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	//m_turretMesh->SetupAttachment(m_tankRootMesh);
	m_turretMesh->SetupAttachment(m_tankRootMesh);

	if (sphereMesh.Succeeded())
	{
		m_turretMesh->SetStaticMesh(sphereMesh.Object);
		m_wheelMesh0->SetStaticMesh(sphereMesh.Object);
		m_wheelMesh1->SetStaticMesh(sphereMesh.Object);
		m_wheelMesh2->SetStaticMesh(sphereMesh.Object);
		m_wheelMesh3->SetStaticMesh(sphereMesh.Object);
	}

	m_turretMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	m_turretMesh->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_turretMesh->SetWorldScale3D(FVector(1.75f, 1.75f, 1.0f));


	m_wheelMesh0->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	//m_wheelMesh0->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	m_wheelMesh0->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	m_wheelMesh0->AttachToComponent(m_tankRootMesh, rules, TEXT("socket0"));

	m_wheelMesh1->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	//m_wheelMesh1->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	m_wheelMesh1->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	m_wheelMesh1->AttachToComponent(m_tankRootMesh, rules, TEXT("socket1"));

	m_wheelMesh2->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	//m_wheelMesh2->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	m_wheelMesh2->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	m_wheelMesh2->AttachToComponent(m_tankRootMesh, rules, TEXT("socket2"));

	m_wheelMesh3->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
	//m_wheelMesh3->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);
	m_wheelMesh3->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	m_wheelMesh3->AttachToComponent(m_tankRootMesh, rules, TEXT("socket3"));


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

	FQuat myActorQuat0 = m_wheelMesh0->GetComponentQuat();
	m_wheelMesh0->AddTorqueInRadians(myActorQuat0.RotateVector(FVector(10000000 * AxisValue, 10000000 * AxisValue, 10000000 * AxisValue)), NAME_None, false);

	FQuat myActorQuat1 = m_wheelMesh1->GetComponentQuat();
	m_wheelMesh1->AddTorqueInRadians(myActorQuat1.RotateVector(FVector(10000000 * AxisValue, 10000000 * AxisValue, 10000000 * AxisValue)), NAME_None, false);

	FQuat myActorQuat2 = m_wheelMesh2->GetComponentQuat();
	m_wheelMesh2->AddTorqueInRadians(myActorQuat2.RotateVector(FVector(10000000 * AxisValue, 10000000 * AxisValue, 10000000 * AxisValue)), NAME_None, false);

	FQuat myActorQuat3 = m_wheelMesh3->GetComponentQuat();
	m_wheelMesh3->AddTorqueInRadians(myActorQuat3.RotateVector(FVector(10000000 * AxisValue, 10000000 * AxisValue, 10000000 * AxisValue)), NAME_None, false);



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
	FRotator NewRotation2 = FRotator(0,AxisValue,0);
	//NewRotation2.Yaw += AxisValue;
	//TurnAmount = FRotator(0,0,0);
	//TurnAmount.Yaw = AxisValue;
	//SetActorRotation(NewRotation);
	//AddActorLocalRotation(NewRotation);
	//m_turretRoot->GetComponentTransform().SetRotation(NewRotation);
	//m_root->SetWorldRotation(NewRotation2);
	
	//m_root->SetRelativeRotation(NewRotation2);
	m_tankRootMesh->AddLocalRotation(NewRotation2);
	//m_root->AddLocalRotation(NewRotation2);
	//AddActorLocalRotation(NewRotation2);
}
//
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
	FRotator NewRotation = FRotator(0, AxisValue, 0);
	//NewRotation.Yaw += AxisValue;
	//SetActorRotation(NewRotation);
	//m_turretMesh->SetRelativeRotation(NewRotation);
	m_turretMesh->AddLocalRotation(NewRotation);
	//m_turretRoot->GetComponentTransform().SetRotation(NewRotation);
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

