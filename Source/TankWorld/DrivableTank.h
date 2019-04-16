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
	
	UPROPERTY(EditAnywhere)
		UStaticMesh* m_treadMesh;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ATreadSpline> treadBP;

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

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* meshTest;

	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> treadMeshes0;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> treadMeshes1;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> treadMeshes2;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> treadMeshes3;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> treadMeshes4;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> treadMeshes5;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> treadMeshes6;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> treadMeshes7;

	UPROPERTY(EditAnywhere)
		UMaterial* m_wheelMat;

	UPROPERTY(EditAnyWhere)
		UMaterial* m_treadMat;
	UPROPERTY(EditAnyWhere)
		UMaterial* m_bodyMat;

	int m_wheelCount = 8;


	FVector m_treadPosition;
	FRotator m_treadRotation;

	UPROPERTY(EditAnywhere)
		TArray<UPhysicsConstraintComponent*> m_wheelConstraintArray;

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

	int m_socketGap = 80;

	void Accelerate(float AxisValue);
	void TurnTank(float AxisValue);

	void SetupTreads(int i);

	void UpdateTreads();

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
