// Sets default values
AContainerActor::AContainerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AContainerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AContainerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}