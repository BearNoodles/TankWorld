// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "TreadSpline.generated.h"

UCLASS()
class TANKWORLD_API ATreadSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATreadSpline();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(EditAnywhere)
		USceneComponent* m_root;

	UPROPERTY(EditAnywhere)
		USplineComponent* m_spline;

	UPROPERTY(EditAnywhere)
		TArray<USplineMeshComponent*> m_splineMeshes;

	UPROPERTY(EditAnywhere)
		TArray<UPhysicsConstraintComponent*>  m_splineConstraints;

	UPROPERTY(EditAnywhere)
		UStaticMesh* m_staticMesh;
	
	UPROPERTY(EditAnywhere)
		UMaterial* m_splineMeshMaterial;

	int m_sectionLength;

	
	
	
	
};
