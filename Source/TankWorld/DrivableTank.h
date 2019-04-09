// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_wheelMesh0;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_wheelMesh1;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_wheelMesh2;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_wheelMesh3;

	UPROPERTY(EditAnyWhere)
		UPhysicsConstraintComponent* m_constraint0;
	UPROPERTY(EditAnyWhere)
		UPhysicsConstraintComponent* m_constraint1;
	UPROPERTY(EditAnyWhere)
		UPhysicsConstraintComponent* m_constraint2;
	UPROPERTY(EditAnyWhere)
		UPhysicsConstraintComponent* m_constraint3;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_turretMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_barrelMesh;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* m_springArm;

	UPROPERTY(EditAnywhere)
		UCameraComponent* m_camera;

	UPROPERTY(EditAnywhere)
		USceneComponent* m_launchPoint;

	int m_speed;
	int m_ammoMax;
	int m_ammoCurrent;
	float m_ammoTimer;
	float m_regenRate;

	void Accelerate(float AxisValue);
	void TurnTank(float AxisValue);

	/*void StartAccelerate();
	void StartTurnTankL();
	void StartTurnTankR();

	void StopAccelerate();
	void StopTurnTankL();
	void StopTurnTankR();*/

	void TurnTurretX(float AxisValue);

	FVector CurrentVelocity;
	float m_acceleration;
	float m_maxSpeed;
	FRotator TurnAmount;

	bool isAccelerating;
	bool isTurningLeft;
	bool isTurningRight;

	class UTankPawnMovementComponent* MyMovementComponent;
	virtual UPawnMovementComponent* GetMovementComponent() const override;

};
