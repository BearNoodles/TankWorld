// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreadSpline.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Camera/CameraComponent.h"

#include "DrivableTank.generated.h"

UCLASS()
class TANKWORLD_API ADrivableTank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrivableTank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);


private:

	UPROPERTY(EditAnywhere)
		USceneComponent* m_root;
		//UStaticMeshComponent* m_root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_tankRootMesh; 

	//UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> m_wheelMeshArray;


	UPROPERTY(EditAnywhere)
		UMaterial* m_wheelMat;

	//UPROPERTY(EditAnyWhere)
		//UMaterial* m_treadMat;
	UPROPERTY(EditAnyWhere)
		UMaterial* m_bodyMat;
	UPROPERTY(EditAnyWhere)
		UMaterial* m_turretMat;

	int m_wheelCount = 8;


	FVector m_treadPosition;
	FRotator m_treadRotation;

	//UPROPERTY(EditAnywhere)
		TArray<UPhysicsConstraintComponent*> m_wheelConstraintArray;

	//UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_turretMeshX;
	//UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_turretMeshY;

	//UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_barrelMesh;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* m_springArm;

	//UPROPERTY(EditAnywhere)
		UCameraComponent* m_camera;

	//UPROPERTY(EditAnywhere)
		USceneComponent* m_launchPoint;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> m_projectileType1;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> m_projectileType2;

	UPROPERTY(EditAnywhere)
		FString m_fireType;

	UFUNCTION(BluePrintCallable)
		FString GetFireType();

	AActor* m_loadedProjectile;

	bool m_isLoadedProjectile1;
	bool m_isLoadedProjectile2;

	bool m_fireReady;

	float m_loadTimeMax;
	float m_loadTimer;

	UPROPERTY(EditAnywhere)
		float m_loadTimeRate;


	UPROPERTY(EditAnywhere)
		AActor* m_projectile;

	int m_speed;

	UPROPERTY(EditAnywhere)
		int m_ammoMax;

	UPROPERTY(EditAnywhere)
		int m_ammoCurrent;
	
	UPROPERTY(EditAnywhere)
		float m_regenRate;

	float m_ammoTimer;

	int m_socketGap = 80;

	void Accelerate(float AxisValue);
	void TurnTank(float AxisValue);
	void Fire();
	void Load1();
	void Load2();
	void AimIn();
	void AimOut();

	float m_cameraLag;
	float m_fov;
	float m_nearFov;
	float m_farFov;
	float m_nearTargetSpringLength;
	float m_farTargetSpringLength;

	UPROPERTY(EditAnywhere)
		float m_zoomScale;

	FVector m_farCameraPosition;
	FVector m_nearCameraPosition;
	FRotator m_farCameraRotation;
	FRotator m_nearCameraRotation;


	void TurnTurretX(float AxisValue);
	void TurnTurretY(float AxisValue);

	void SetProjectileActor(AActor* object);

	FVector CurrentVelocity;
	float m_acceleration;

	UPROPERTY(EditAnywhere)
		float m_torque;

	UPROPERTY(EditAnywhere)
		float m_turnTorque;

	UPROPERTY(EditAnywhere)
		UPhysicalMaterial* m_physMat;

	float m_maxSpeed;
	FRotator TurnAmount;

	float m_turretStartHeight;
	float m_turretCurrentHeight;

	bool isAccelerating;
	bool isTurningLeft;
	bool isTurningRight;

	//class UTankPawnMovementComponent* MyMovementComponent;
	//virtual UPawnMovementComponent* GetMovementComponent() const override;

};
