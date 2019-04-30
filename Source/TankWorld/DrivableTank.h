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
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);


private:
	//Root component and meshes which make up tank
	USceneComponent* m_root;
	UStaticMeshComponent* m_tankRootMesh; 
	TArray<UStaticMeshComponent*> m_wheelMeshArray;
	TArray<UPhysicsConstraintComponent*> m_wheelConstraintArray;
	UStaticMeshComponent* m_turretMeshX;
	UStaticMeshComponent* m_turretMeshY;
	UStaticMeshComponent* m_barrelMesh;

	//Spring arm and camera
	USpringArmComponent* m_springArm;
	UCameraComponent* m_camera;

	//Materials and physics material
	UPROPERTY(EditAnywhere)
		UMaterial* m_wheelMat;
	UPROPERTY(EditAnyWhere)
		UMaterial* m_bodyMat;
	UPROPERTY(EditAnyWhere)
		UMaterial* m_turretMat;
	UPROPERTY(EditAnyWhere)
		UMaterial* m_barrelMat;
	UPROPERTY(EditAnywhere)
		UPhysicalMaterial* m_bodyPhysMat;

	int m_wheelCount = 8;

	UPROPERTY(EditAnywhere)
		float m_massScale;

	//2 Projectile type which can be set
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> m_projectileType1;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> m_projectileType2;

	//FireType strinf for displaying which type of projectile is loaded on the HUD
	FString m_fireType;
	UPROPERTY(EditAnywhere)
		FString m_fireType1;
	UPROPERTY(EditAnywhere)
		FString m_fireType2;

	//Torque for turning the wheels
	UPROPERTY(EditAnywhere)
		float m_torque;
	float m_turnTorque;

	UPROPERTY(EditAnywhere)
		float m_maxSpeed;

	//Turrets starting aim height
	float m_turretStartHeight;
	float m_turretCurrentHeight;

	//bools to check which projectile is loaded
	bool m_isLoadedProjectile1;
	bool m_isLoadedProjectile2;

	//bool to check if the tank is ready to fire
	bool m_fireReady;

	//timer floats for loading projectiles
	float m_loadTimeMax;
	float m_loadTimer;

	//how fast the tank loads projectiles
	UPROPERTY(EditAnywhere)
		float m_loadTimeRate;

	//variables for camera effects
	float m_cameraLag;
	float m_fov;
	float m_nearFov;
	float m_farFov;
	float m_nearTargetSpringLength;
	float m_farTargetSpringLength;

	//Zoom amount when aiming in
	UPROPERTY(EditAnywhere)
		float m_zoomScale;

	//cameras position and rotation values for each zoom
	FVector m_farCameraPosition;
	FVector m_nearCameraPosition;
	FRotator m_farCameraRotation;
	FRotator m_nearCameraRotation;

	//Input functions bound to keys
	void Accelerate(float AxisValue);
	void TurnTank(float AxisValue);
	void TurnTurretX(float AxisValue);
	void TurnTurretY(float AxisValue);
	void Fire();
	void Load1();
	void Load2();
	void AimIn();
	void AimOut();

	//Returns which fire type is currently loaded
	UFUNCTION(BluePrintCallable)
		FString GetFireType();

};
