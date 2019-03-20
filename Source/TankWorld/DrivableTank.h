// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
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

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_tankRootMesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* m_turretRoot;

	//UPROPERTY(EditAnywhere)
		//UStaticMeshComponent* m_tankMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_turretMesh;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* m_springArm;

	UPROPERTY(EditAnywhere)
		UCameraComponent* m_camera;

	UPROPERTY(EditAnywhere)
		USceneComponent* m_launchPoint;

	int m_ammoMax;
	int m_ammoCurrent;
	float m_ammoTimer;
	float m_regenRate;

	void Accelerate();
	void TurnTankL();
	void TurnTankR();
	void TurnTurretX(float AxisValue);
	FVector CurrentVelocity;

	class UTankPawnMovementComponent* MyMovementComponent;
	virtual UPawnMovementComponent* GetMovementComponent() const override;

};
