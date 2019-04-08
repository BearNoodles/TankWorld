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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	//GetMesh()->Set
	m_tankRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	m_tankRootMesh->SetupAttachment(m_root);
	//RootComponent = m_tankRootMesh;

	if (cubeMesh.Succeeded())
	{
		//m_tankRootMesh->SetStaticMesh(cubeMesh.Object);
	}

	m_tankRootMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	m_tankRootMesh->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_tankRootMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	//m_turretRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Turret"));
	m_turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	//m_turretMesh->SetupAttachment(m_tankRootMesh);
	m_turretMesh->SetupAttachment(m_tankRootMesh);

	if (cylinderMesh.Succeeded())
	{
		m_turretMesh->SetStaticMesh(sphereMesh.Object);
	}
	

	m_turretMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	m_turretMesh->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_turretMesh->SetWorldScale3D(FVector(1.75f, 1.75f, 1.0f));

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

	/*if(!CurrentVelocity.IsZero())
	{
		AddActorLocalOffset(CurrentVelocity * DeltaTime);
	}*/

	//m_root->AddForce(m_acceleration);
	m_tankRootMesh->AddForce(m_acceleration);

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
	CurrentVelocity = FMath::Clamp(AxisValue, -1.0f, 1.0f) * m_speed * m_tankRootMesh->GetForwardVector();
	m_acceleration = FMath::Clamp(AxisValue, -1.0f, 1.0f) * m_tankRootMesh->GetForwardVector() * 1500000;
	//m_acceleration = FMath::Clamp(AxisValue, -1.0f, 1.0f) * m_root->GetForwardVector() * 2500000;
	
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

