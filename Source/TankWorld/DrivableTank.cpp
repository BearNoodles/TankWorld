// Fill out your copyright notice in the Description page of Project Settings.

#include "DrivableTank.h"

#include "ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
//#include "TankPawnMovementComponent.h"


// Sets default values
ADrivableTank::ADrivableTank()
{
	m_root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = m_root;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Constructor Helper for finding meshes,  materials and the 2 projectile actors in the project
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bodyMesh(TEXT("StaticMesh'/Game/StaticMeshes/Body2.Body2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> treadMesh(TEXT("StaticMesh'/Game/StaticMeshes/Tread2.Tread2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> turretMesh(TEXT("StaticMesh'/Game/StaticMeshes/Turret.Turret'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> barrelMesh(TEXT("StaticMesh'/Game/StaticMeshes/Barrel2.Barrel2'"));

	static ConstructorHelpers::FClassFinder<AActor> projectile1(TEXT("Blueprint'/Game/BluePrints/Projectile1.Projectile1_C'"));
	static ConstructorHelpers::FClassFinder<AActor> projectile2(TEXT("Blueprint'/Game/BluePrints/Projectile2.Projectile2_C'"));

	static ConstructorHelpers::FObjectFinder<UObject> bodyPhysMat(TEXT("PhysicalMaterial'/Game/Slidey.Slidey'"));

	//Creates the main body static mesh of the tank
	m_tankRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));

	//Attaches the body to the root
	m_tankRootMesh->SetupAttachment(m_root);

	//Turns on physics for the body
	m_tankRootMesh->SetSimulatePhysics(true);

	//Checks if first projectile was found successfully and sets its to the projectileType1 variable
	if (projectile1.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Default Projectile1 Class Found"));
		m_projectileType1 = projectile1.Class;
	}

	//Checks if second projectile was found successfully and sets its to the projectileType1 variable
	if (projectile2.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Default Projectile2 Class Found"));
		m_projectileType2 = projectile2.Class;
	}

	//Checks if the body static mesh was found and set the tank body mesh to this
	if (bodyMesh.Succeeded())
	{
		m_tankRootMesh->SetStaticMesh(bodyMesh.Object);
	}

	//Significantly increases the scale of the tanks body weight
	m_tankRootMesh->SetAllMassScale(27.0f);

	//Checks if the body physical material was found before setting it to the variable
	if (bodyPhysMat.Succeeded())
	{
		m_bodyPhysMat = (UPhysicalMaterial*)bodyPhysMat.Object;
	}

	//Positions, scales and rotates the tank body
	m_tankRootMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	m_tankRootMesh->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_tankRootMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	//Sets up the rules of attachment for the wheels, body and the physics constraints attaching them
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);

	//Fills a static mesh array with meshes to be used as wheels, named for later reference
	m_wheelMeshArray = TArray<UStaticMeshComponent*>();
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL0")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL1")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL2")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshL3")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR0")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR1")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR2")));
	m_wheelMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMeshR3")));


	//Sets each wheel mesh to have physics enabled
	for (int i = 0; i < m_wheelCount; i++)
	{
		m_wheelMeshArray[i]->SetSimulatePhysics(true);
	}
	

	//Creates array for the physics constraints and sets all of their first components to the appropriate wheels
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

	//for each wheel and constraint, sets up the attachment to the body
	for (int i = 0; i < m_wheelCount; i++)
	{
		//Name of the tank body
		m_wheelConstraintArray[i]->ComponentName2.ComponentName = "BodyMesh";
		
		//Locks movement in the x and y axis
		m_wheelConstraintArray[i]->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
		m_wheelConstraintArray[i]->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

		//Allows for limited movement in the z axis
		m_wheelConstraintArray[i]->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 3.0f);

		//Locks rotation on 2 axis'
		m_wheelConstraintArray[i]->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
		m_wheelConstraintArray[i]->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);

		//Allows wheels to rotate freely around this axis
		m_wheelConstraintArray[i]->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45);

		//Setting the constraint as soft causes the wheels to push back towards their original position when moved
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = 1;

		//Sets behaviour of wheels when moved from original position
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = 100.1f;
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 0.3;
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.Restitution = 0.5f;
		m_wheelConstraintArray[i]->ConstraintInstance.ProfileInstance.LinearLimit.ContactDistance = 5.0f;

		//Stops wheels from colliding with the tank body that they are attached to
		m_wheelConstraintArray[i]->SetDisableCollision(true);
	}

	//Attaches each constraint to the tank body at the named sockets
	m_wheelConstraintArray[0]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL0"));
	m_wheelConstraintArray[1]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL1"));
	m_wheelConstraintArray[2]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL2"));
	m_wheelConstraintArray[3]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL3"));
	m_wheelConstraintArray[4]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR0"));
	m_wheelConstraintArray[5]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR1"));
	m_wheelConstraintArray[6]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR2"));
	m_wheelConstraintArray[7]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR3"));

	//Creates the mesh components for the turret
	m_turretMeshX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	m_turretMeshX->SetupAttachment(m_tankRootMesh);

	//Attached to the first turret to inherit its rotation
	m_turretMeshY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh2"));
	m_turretMeshY->SetupAttachment(m_turretMeshX);

	//Checks if the sphere mesh can be loaded and sets each wheel as this mesh
	if (sphereMesh.Succeeded())
	{
		for (int i = 0; i < m_wheelCount; i++)
		{
			m_wheelMeshArray[i]->SetStaticMesh(sphereMesh.Object);
		}
	}

	//Checks if the turret mesh can be loaded and sets each turret as this mesh
	if (turretMesh.Succeeded())
	{
		m_turretMeshX->SetStaticMesh(turretMesh.Object);
		m_turretMeshY->SetStaticMesh(turretMesh.Object);
	}
	//Hides the second turret mesh from being seen
	m_turretMeshY->SetVisibility(false);

	//Positions, rotates and scales each turret
	m_turretMeshX->RelativeLocation = FVector(0.0f, 0.0f, 20.0f);
	m_turretMeshX->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_turretMeshX->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	m_turretMeshY->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	m_turretMeshY->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_turretMeshY->SetWorldScale3D(FVector(1.0, 1.0f, 1.0f));

	//Moves the wheel position and sets the wheel meshes scale to look more like a wheel
	for (int i = 0; i < m_wheelCount; i++)
	{
		m_wheelMeshArray[i]->RelativeLocation = FVector(0.0f, 0.0f, -50.0f);
		m_wheelMeshArray[i]->SetRelativeScale3D(FVector(1.0f, 0.5f, 1.0f));
	}

	//Attaches each wheel to the tank via the same socket as the constraint
	m_wheelMeshArray[0]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL0"));
	m_wheelMeshArray[1]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL1"));
	m_wheelMeshArray[2]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL2"));
	m_wheelMeshArray[3]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketL3"));
	m_wheelMeshArray[4]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR0"));
	m_wheelMeshArray[5]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR1"));
	m_wheelMeshArray[6]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR2"));
	m_wheelMeshArray[7]->AttachToComponent(m_tankRootMesh, rules, TEXT("socketR3"));

	//Creates the barrel mesh component
	m_barrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));

	//Attaches barrel to the second turret to inherit both turrets rotation
	m_barrelMesh->SetupAttachment(m_turretMeshY);

	//Checks if the barrel mesh can be loaded and sets the barrel as this mesh
	if (barrelMesh.Succeeded())
	{
		m_barrelMesh->SetStaticMesh(barrelMesh.Object); 
	}

	//Positions, rotates and scales the barrel
	m_barrelMesh->RelativeLocation = FVector(100.0f, 0.0f, 60.0f);
	m_barrelMesh->RelativeRotation = FRotator(-90.0f, 0.0f, 0.0f);
	m_barrelMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	//Creates the spring arm componenet to be used by the camera and attaches it to the second turret
	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_springArm->SetupAttachment(m_turretMeshY);

	//Sets up rotators for when the camera is zoomed and unzoomed
	m_farCameraRotation = FRotator(-20.0f, 0.0f, 0.0f);
	m_nearCameraRotation = FRotator(0.0f, 0.0f, 0.0f);
	m_springArm->RelativeRotation = m_farCameraRotation;

	//Sets up position vectors for when the camera is zoomed and unzoomed
	m_farCameraPosition = FVector(-40.0f, 0.0f, 110.0f);
	m_nearCameraPosition = FVector(-40.0f, 0.0f, 110.0f);
	m_springArm->RelativeLocation = m_farCameraPosition;

	//Sets up spring arm 
	m_nearTargetSpringLength = 180.0f;
	m_farTargetSpringLength = 400.0f;
	m_springArm->TargetArmLength = m_farTargetSpringLength;
	m_springArm->bEnableCameraLag = true;
	m_cameraLag = 3.0f;
	m_springArm->CameraLagSpeed = m_cameraLag;

	//Creates the camera component and attaches it to spring arm
	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_camera->SetupAttachment(m_springArm, USpringArmComponent::SocketName);

	//Forces player 1 to posses this tank point
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//The scale of the zoom when aiming in
	m_zoomScale = 3.0f;
	m_farFov = m_camera->FieldOfView;
	m_nearFov = m_farFov / m_zoomScale;

	//The torque at which to turn the wheels of the tank
	m_torque = 18000000;
	m_maxSpeed = 40.0f;

	//Turn torque needs to be greater than normal torque for when the tank is still or moving relatively slow
	m_turnTorque = m_torque * 1.5f;

	//how high the turret is aiming at the start
	m_turretStartHeight = m_turretMeshY->GetComponentRotation().Pitch;

	m_turretCurrentHeight = 0;
	
	//Timer variables for loading projectiles
	m_loadTimer = 0;
	m_loadTimeMax = 1;
	m_loadTimeRate = 1;

	//bools to check if either projectile is loaded
	m_isLoadedProjectile1 = false;
	m_isLoadedProjectile2 = false;

	//bool for checking if the tank is ready to fire
	m_fireReady = false;

	//Text to display on hud when each projectile type is loaded
	m_fireType1 = "Explosive";
	m_fireType2 = "Heavy";
	m_fireType = "NONE";

}


// Called when the game starts or when spawned
void ADrivableTank::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Your message111111111111111111111111111"));
}

void ADrivableTank::OnConstruction(const FTransform& Transform)
{
	m_turnTorque = m_torque * 1.5f;
	if (m_bodyMat != NULL)
	{
		m_tankRootMesh->SetMaterial(0, m_bodyMat);
	}
	if (m_wheelMat != NULL)
	{
		for (int i = 0; i < m_wheelCount; i++)
		{
			m_wheelMeshArray[i]->SetMaterial(0, m_wheelMat);
		}
	}
	if (m_turretMat != NULL)
	{
		m_turretMeshX->SetMaterial(0, m_turretMat);
	}
	if (m_barrelMat != NULL)
	{
		m_barrelMesh->SetMaterial(0, m_barrelMat);
	}
	if (m_bodyPhysMat != NULL)
	{
		//Sets tanks body to the low friction material
		m_tankRootMesh->SetPhysMaterialOverride(m_bodyPhysMat);
	}
}

// Called every frame
void ADrivableTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//checks if a projectile is loaded but tank is not ready to fire
	if ((m_isLoadedProjectile1 || m_isLoadedProjectile2) && !m_fireReady)
	{
		//increases the timer if the timer has not hit the max value yet
		if (m_loadTimer < m_loadTimeMax)
		{
			m_loadTimer += DeltaTime / m_loadTimeRate;
		}
		//sets the tank ready to fire and resets timer
		else
		{
			m_fireReady = true;
			m_loadTimer = 0;
			if (m_isLoadedProjectile1)
			{
				m_fireType = m_fireType1;
			}
			else if (m_isLoadedProjectile2)
			{
				m_fireType = m_fireType2;
			}
			else
			{
				m_fireType = "ERROR";
			}
		}
	}

}

//Accelerate checks the current angular velocity of each wheel and if its below the set maximum then applies torque to it
void ADrivableTank::Accelerate(float AxisValue)
{
	//Moves all wheels at same time
	for (int i = 0; i < m_wheelCount; i++)
	{
		//Checks if each wheel is moving fast enoug halready
		if (m_wheelMeshArray[i]->GetPhysicsAngularVelocityInRadians().Size() < m_maxSpeed)
		{
			//Adds torque to the wheel
			FQuat myActorQuat = m_wheelMeshArray[i]->GetComponentQuat();
			m_wheelMeshArray[i]->AddTorqueInRadians(myActorQuat.RotateVector(FVector(0, m_torque * AxisValue, 0)), NAME_None, false);
		}
	}
	
}

//Turn tank turns each set of wheels (left and right) to rotate in different direction
void ADrivableTank::TurnTank(float AxisValue)
{
	//Gets the current velocity of the tank
	float tankVel = m_tankRootMesh->GetComponentVelocity().Size();

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

//Turns the turretx meshs yaw value
void ADrivableTank::TurnTurretX(float AxisValue)
{
	FRotator NewRotation = FRotator(0, AxisValue, 0);
	m_turretMeshX->AddLocalRotation(NewRotation);
}

//Turns the turrety meshs pitch value if it is within certain bounds
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

//Fires the loaded projectile if ready
void ADrivableTank::Fire()
{
	//Returns the function if no projectile is ready to fire
	if (!m_fireReady)
	{
		return;
	}

	//Sets the launch point as the barrels LaunchPoint socket
	FName name = *FString::Printf(TEXT("LaunchPoint"));
	FVector Location = m_barrelMesh->GetSocketLocation(name);

	//Sets the rotation is the rotation of the TurretY
	FRotator Rotation = m_turretMeshY->GetComponentRotation();
	FActorSpawnParameters SpawnInfo;

	//Checks which projectile is loaded
	if (m_isLoadedProjectile1)
	{
		//spawns the projectile with the given location and rotation
		GetWorld()->SpawnActor<AActor>(m_projectileType1, Location, Rotation);
		m_isLoadedProjectile1 = false;
	}
	else
	{
		//spawns the projectile with the given location and rotation
		GetWorld()->SpawnActor<AActor>(m_projectileType2, Location, Rotation);
		m_isLoadedProjectile2 = false;
	}

	//resets fire ready bool and fire type
	m_fireReady = false;

	m_fireType = "NONE";
}

void ADrivableTank::Load1()
{
	//checks if a projectile is already loaded and returns function if true
	if (m_isLoadedProjectile1 || m_isLoadedProjectile2)
	{
		return;
	}

	//sets the loaded projectile to true and updates the firetype display
	m_fireType = "Loading";
	m_isLoadedProjectile1 = true;

}
void ADrivableTank::Load2()
{
	//checks if a projectile is already loaded and returns function if true
	if (m_isLoadedProjectile1 || m_isLoadedProjectile2)
	{
		return;
	}

	//sets the loaded projectile to true and updates the firetype display
	m_fireType = "Loading";
	m_isLoadedProjectile2 = true;
}

void ADrivableTank::AimIn()
{
	//changes the length of the spring arm
	m_springArm->TargetArmLength = m_nearTargetSpringLength;

	//stops the camera lag effect
	m_springArm->CameraLagSpeed = 0;

	//changes spring arm location and position
	m_springArm->RelativeLocation = m_nearCameraPosition;
	m_springArm->RelativeRotation = m_nearCameraRotation;

	//lowers the field of view to give a zoom in effect
	m_camera->FieldOfView = m_farFov / m_zoomScale;
}

void ADrivableTank::AimOut()
{
	//changes the length of the spring arm
	m_springArm->TargetArmLength = m_farTargetSpringLength;

	//gives the camera lag effect
	m_springArm->CameraLagSpeed = m_cameraLag;

	//changes spring arm location and position
	m_springArm->RelativeLocation = m_farCameraPosition;
	m_springArm->RelativeRotation = m_farCameraRotation;

	//raises the field of view to zoom back out
	m_camera->FieldOfView = m_farFov;
}


// Called to bind functionality to input
void ADrivableTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//uses unreals input manager to bind appropriate functions to key presses
	InputComponent->BindAxis("Accelerate", this, &ADrivableTank::Accelerate);
	InputComponent->BindAxis("Turn Tank", this, &ADrivableTank::TurnTank);
	
	InputComponent->BindAxis("Turn Turret X", this, &ADrivableTank ::TurnTurretX);
	InputComponent->BindAxis("Turn Turret Y", this, &ADrivableTank ::TurnTurretY);
	
	InputComponent->BindAction("Fire", IE_Pressed, this, &ADrivableTank::Fire);
	InputComponent->BindAction("Load1", IE_Pressed, this, &ADrivableTank::Load1);
	InputComponent->BindAction("Load2", IE_Pressed, this, &ADrivableTank::Load2);
	InputComponent->BindAction("AimIn", IE_Pressed, this, &ADrivableTank::AimIn);
	InputComponent->BindAction("AimOut", IE_Released, this, &ADrivableTank::AimOut);
}

FString ADrivableTank::GetFireType()
{
	//Returns the fire tpye for the hud to display
	return  m_fireType;
}


