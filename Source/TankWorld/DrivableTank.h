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

	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* m_wheelMeshL0;
	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* m_wheelMeshL1;
	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* m_wheelMeshL2;
	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* m_wheelMeshL3;
	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* m_wheelMeshR0;
	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* m_wheelMeshR1;
	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* m_wheelMeshR2;
	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* m_wheelMeshR3;

	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> m_wheelMeshArray;

	UPROPERTY(EditAnyWhere)
		UMaterial* m_wheelMat;
	UPROPERTY(EditAnyWhere)
		UMaterial* m_bodyMat;

	int m_wheelCount = 8;

	//UPROPERTY(EditAnyWhere)
	//	UPhysicsConstraintComponent* m_constraintL0;
	//UPROPERTY(EditAnyWhere)
	//	UPhysicsConstraintComponent* m_constraintL1;
	//UPROPERTY(EditAnyWhere)
	//	UPhysicsConstraintComponent* m_constraintL2;
	//UPROPERTY(EditAnyWhere)
	//	UPhysicsConstraintComponent* m_constraintL3;
	//UPROPERTY(EditAnyWhere)
	//	UPhysicsConstraintComponent* m_constraintR0;
	//UPROPERTY(EditAnyWhere)
	//	UPhysicsConstraintComponent* m_constraintR1;
	//UPROPERTY(EditAnyWhere)
	//	UPhysicsConstraintComponent* m_constraintR2;
	//UPROPERTY(EditAnyWhere)
	//	UPhysicsConstraintComponent* m_constraintR3;

	UPROPERTY(EditAnywhere)
		TArray<UPhysicsConstraintComponent*> m_constraintArray;

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
	float m_torque;
	float m_turnTorque;
	float m_maxSpeed;
	FRotator TurnAmount;

	bool isAccelerating;
	bool isTurningLeft;
	bool isTurningRight;

	class UTankPawnMovementComponent* MyMovementComponent;
	virtual UPawnMovementComponent* GetMovementComponent() const override;

};
