// Fill out your copyright notice in the Description page of Project Settings.

#include "DrivableTank.h"
#include "ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "TankPawnMovementComponent.h"


// Sets default values
ADrivableTank::ADrivableTank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	//GetMesh()->Set
	m_tankRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
	RootComponent = m_tankRootMesh;

	m_turretRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Turret"));
	m_turretRoot->SetupAttachment(RootComponent);
	m_turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	m_turretMesh->SetupAttachment(m_turretRoot);


	if (cubeMesh.Succeeded())
	{
		m_tankRootMesh->SetStaticMesh(cubeMesh.Object);
		m_turretMesh->SetStaticMesh(cubeMesh.Object);
	}

	m_turretRoot->RelativeLocation = FVector(0.0f, 45.0f, 110.0f);
	m_turretRoot->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_turretRoot->RelativeScale3D = FVector(0.5f, 1.5f, 0.5f);

	m_launchPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LaunchPoint"));
	m_launchPoint->SetupAttachment(m_turretMesh);

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_springArm->SetupAttachment(m_turretRoot);
	m_springArm->RelativeRotation = FRotator(-45.0f, 90.0f, 0.0f);
	m_springArm->RelativeLocation = FVector(-40.0f, 0.0f, 110.0f);
	m_springArm->TargetArmLength = 400.0f;
	m_springArm->bEnableCameraLag = true;
	m_springArm->CameraLagSpeed = 3.0f;

	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_camera->SetupAttachment(m_springArm, USpringArmComponent::SocketName);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MyMovementComponent = CreateDefaultSubobject<UTankPawnMovementComponent>(TEXT("MovementComponent"));
	MyMovementComponent->UpdatedComponent = RootComponent;
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

	//if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{

	}

	/*if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
		SetActorLocation(NewLocation);
	}*/

	//TODO press a key to launch a projectile

}

void ADrivableTank::Accelerate()
{
	UE_LOG(LogTemp, Warning, TEXT("Your message5AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
	// Move at 100 units per second right or left
	m_tankRootMesh->AddForce(m_tankRootMesh->GetForwardVector() * 100000);
	//CurrentVelocity = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f * m_tankRoot->GetForwardVector();
}

void ADrivableTank::TurnTankL()
{

	UE_LOG(LogTemp, Warning, TEXT("Your messageLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
	//m_tankRootMesh->AddTorque(FVector(0,0,1000));
	//m_turretRoot->GetComponentTransform.GetComponentRotation();
	//FRotator NewRotation = GetActorRotation();
	//FRotator NewRotation = m_tankRootMesh->GetComponentRotation();
	//NewRotation.Yaw += AxisValue;
	////SetActorRotation(NewRotation);
	//m_tankRootMesh->SetRelativeRotation(NewRotation);
	//m_turretRoot->GetComponentTransform().SetRotation(NewRotation);
}

void ADrivableTank::TurnTankR()
{

	UE_LOG(LogTemp, Warning, TEXT("Your messageRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR"));
	//m_tankRootMesh->AddTorque(FVector(0,0,1000));
	//m_turretRoot->GetComponentTransform.GetComponentRotation();
	//FRotator NewRotation = GetActorRotation();
	//FRotator NewRotation = m_tankRootMesh->GetComponentRotation();
	//NewRotation.Yaw += AxisValue;
	////SetActorRotation(NewRotation);
	//m_tankRootMesh->SetRelativeRotation(NewRotation);
	//m_turretRoot->GetComponentTransform().SetRotation(NewRotation);
}

void ADrivableTank::TurnTurretX(float AxisValue)
{
	//m_turretRoot->GetComponentTransform.GetComponentRotation();
	//FRotator NewRotation = GetActorRotation();
	
	FRotator NewRotation = m_turretRoot->GetComponentRotation();
	NewRotation.Yaw += AxisValue;
	//SetActorRotation(NewRotation);
	m_turretRoot->SetRelativeRotation(NewRotation);
	//m_turretRoot->GetComponentTransform().SetRotation(NewRotation);
}

// Called to bind functionality to input
void ADrivableTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Accelerate", IE_Repeat, this, &ADrivableTank::Accelerate);
	InputComponent->BindAction("Turn Tank L", IE_Repeat, this, &ADrivableTank ::TurnTankL);
	InputComponent->BindAction("Turn Tank R", IE_Repeat, this, &ADrivableTank ::TurnTankR);
	InputComponent->BindAxis("Turn Turret X", this, &ADrivableTank ::TurnTurretX);
}

UPawnMovementComponent* ADrivableTank::GetMovementComponent() const
{
	return MyMovementComponent;
}

