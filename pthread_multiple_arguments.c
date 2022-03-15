#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Global variables
typedef struct waitinglist
{
	int vehicle_id;
	int vehicle_type;
	int direction;
	struct  waitinglist *next;
} waitingvehiclelist;

typedef struct movinglist
{
	int vehicle_id;
	int vehicle_type;
	int direction;
	struct  movinglist *next;
} movingvehiclelist;

typedef struct pmstr
{
	int vehicle_id;
	int vehicle_type;
	int direction;
} pmstr_t;

struct waitinglist *pw; //pointer to the waiting list
struct movinglist *pm;  //pointer to the moving list

int waitingcarsouth, waitingcarnorth, waitingtrucksouth, waitingtrucknorth;
int movingcar, movingtruck;
int currentmovingdir, previousmovingdir; //0:north, 1: south
pthread_mutex_t lock;
pthread_cond_t TruckNorthMovable, TruckSouthMovable, CarNorthMovable, CarSouthMovable;



void *vehicle_routine(void *pmstrpara); //vehicle_type: 0 for truck, 1 for car;
                                           //direction: 0 for north, 1 for south;
void vehicle_arrival(pmstr_t *pmstrpara);
void waitinglistinsert(int vehicle_id,int vehicle_type, int direction);
void waitinglistdelete(int vehicle_id);
void movinglistinsert(int vehicle_id, int vehicle_type, int direction);
void movinglistdelete(int vehicle_id);

void printmoving();
void printwaiting();


int main(void)
{
	int option;
	int i,j;
	float carprob;
	int vehicle_type, direction, vehicle_id;

	pthread_t vehicle[30];
	pmstr_t *pmthread;


	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&TruckNorthMovable, NULL);
	pthread_cond_init(&TruckSouthMovable, NULL);
	pthread_cond_init(&CarNorthMovable, NULL);
	pthread_cond_init(&CarSouthMovable, NULL);


	waitingcarnorth=0;
	waitingcarsouth=0;
	waitingtrucknorth=0;
	waitingtrucksouth=0;
	movingcar=0;
	movingtruck=0;
	option=-1;
	pw = NULL; //waiting list
	pm = NULL; //moving list

	fprintf(stderr,"***************************************************************\n");
	fprintf(stderr,"Please select one Schedules from the following six options:\n");
	fprintf(stderr,"1. 10 : DELAY(10) : 10\n");
	fprintf(stderr,"   car/truck probability: [1.0, 0.0]\n");
	fprintf(stderr,"2. 10 : DELAY(10) : 10\n");
	fprintf(stderr,"   car/truck probability: [0.0, 1.0]\n");
	fprintf(stderr,"3. 20\n");
	fprintf(stderr,"   car/truck probability: [0.65, 0.35]\n");
	fprintf(stderr,"4. 10 : DELAY(25) : 10 : DELAY(25) : 10\n");
	fprintf(stderr,"   car/truck probability: [0.5, 0.5]\n");
	fprintf(stderr,"5. 10 : DELAY(3) : 10 : DELAY(10): 10\n");
	fprintf(stderr,"   car/truck probability: [0.65, 0.35]\n");
	fprintf(stderr,"6. 20 : DELAY(15) : 10\n");
	fprintf(stderr,"   car/truck probability: [0.75, 0.25]\n");
	do
	{
		fprintf(stderr,"\nPlease select [1-6]:");
		scanf("%d", &option);
	}while((option<0) || (option>6));

	fprintf(stderr,"***************************************************************\n");

	switch (option)
	{
		case 1: carprob = 1;    break;
		case 2: carprob = 0;    break;
		case 3: carprob = 0.65; break;
		case 4: carprob = 0.5;  break;
		case 5: carprob = 0.65; break;
		case 6: carprob = 0.75; break;
		default: carprob = 0.5;
	}

	srand((unsigned int)time((time_t *)NULL));

	if(option==1) // 20 vehicles
	{

        pthread_mutex_lock(&lock);
		for (j=0; j<=9; j++)
		{

            //call rand() to decide vehicle type and direction
            
            srand(time(NULL));
            double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);

            
		}
		pthread_mutex_unlock(&lock);


	    sleep(10);//delay (10)



		pthread_mutex_lock(&lock);
		for (j=10; j<=19; j++)
		{
			double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);
		}
		pthread_mutex_unlock(&lock);


		for (j=0; j<=19; j++)
			pthread_join(vehicle[j], NULL);

	} //end of option 1

	else if(option==2) // 20 vehicles
	{

        pthread_mutex_lock(&lock);
		for (j=0; j<=9; j++)
		{


            //call rand() to decide vehicle type and direction
            
            
            srand(time(NULL));
            double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);

            
		}
		pthread_mutex_unlock(&lock);


	    sleep(10);//delay (10)



		pthread_mutex_lock(&lock);
		for (j=10; j<=19; j++)
		{
			double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);
		}
		pthread_mutex_unlock(&lock);


		for (j=0; j<=19; j++)
			pthread_join(vehicle[j], NULL);
} //end of option 2


	else if(option==3) // 20 vehicles
	{
        pthread_mutex_lock(&lock);
		for (j=0; j<=9; j++)
		{


            //call rand() to decide vehicle type and direction
            
            
            srand(time(NULL));
            double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);

            
		}
		pthread_mutex_unlock(&lock);


	    sleep(10);//delay (10)



		pthread_mutex_lock(&lock);
		for (j=10; j<=19; j++)
		{
			double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);
		}
		pthread_mutex_unlock(&lock);


		for (j=0; j<=19; j++)
			pthread_join(vehicle[j], NULL);
	} // end of option3

	else if(option==4) // 30 vehicles
	{

        pthread_mutex_lock(&lock);
		for (j=0; j<=9; j++)
		{


            //call rand() to decide vehicle type and direction
            
            
            srand(time(NULL));
            double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);

            
		}
		pthread_mutex_unlock(&lock);


	    sleep(10);//delay (10)



		pthread_mutex_lock(&lock);
		for (j=10; j<=19; j++)
		{
			double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);
		}
		pthread_mutex_unlock(&lock);

	    sleep(10);//delay (10)



		pthread_mutex_lock(&lock);
		for (j=20; j<=29; j++)
		{
			double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);
		}
		pthread_mutex_unlock(&lock);

		for (j=0; j<=29; j++)
			pthread_join(vehicle[j], NULL);

	} // end of option4

	else if(option==5) // 30 vehicles
	{
		
        pthread_mutex_lock(&lock);
		for (j=0; j<=9; j++)
		{


            //call rand() to decide vehicle type and direction
            
            
            srand(time(NULL));
            double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);

            
		}
		pthread_mutex_unlock(&lock);


	    sleep(10);//delay (10)



		pthread_mutex_lock(&lock);
		for (j=10; j<=19; j++)
		{
			double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);
		}
		pthread_mutex_unlock(&lock);

	    sleep(10);//delay (10)



		pthread_mutex_lock(&lock);
		for (j=20; j<=29; j++)
		{
			double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);
		}
		pthread_mutex_unlock(&lock);

		for (j=0; j<=29; j++)
			pthread_join(vehicle[j], NULL);
	} // end of option5

	else //option6: 30 vehicles
	{
		
        pthread_mutex_lock(&lock);
		for (j=0; j<=9; j++)
		{


            //call rand() to decide vehicle type and direction
            
            
            srand(time(NULL));
            double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);

            
		}
		pthread_mutex_unlock(&lock);


	    sleep(10);//delay (10)



		pthread_mutex_lock(&lock);
		for (j=10; j<=19; j++)
		{
			double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);
		}
		pthread_mutex_unlock(&lock);

	    sleep(10);//delay (10)



		pthread_mutex_lock(&lock);
		for (j=20; j<=29; j++)
		{
			double r = (rand() % 100) / 100;
            int direct = rand() % 2;
            
            //generate pmstr_t struct to save the vehicle type, direction, and id
            pmstr_t args;
            direct = rand() % 2;
            args.vehicle_id = j;
            args.direction = direct;
            if(r <= carprob){
                args.vehicle_type = 1;
            }
            else args.vehicle_type = 0;            

            //call vehicle_arrival()
            vehicle_arrival(&args);
            
            //create a pthread to represent the vehicle, vehicle_routine() is the start function of a pthread
            pthread_create(&vehicle[j], NULL, vehicle_routine , (void *)&args);
		}
		pthread_mutex_unlock(&lock);

		for (j=0; j<=29; j++)
			pthread_join(vehicle[j], NULL);
	} // end of option6



	fprintf(stderr,"\nFinished execution.\n");



} // end of main function

void *vehicle_routine(pmstr_t *pmstrpara)
{
	char *strdir;


	if (pmstrpara->vehicle_type) //car
	{
		pthread_mutex_lock(&lock);
		//Try to cross
		
		//while (this vehicle cannot cross) {
		//     wait for proper moving signal
		//}


		//Now begin accrossing
		//update global variables
		//print out proper message

		pthread_mutex_unlock(&lock);

		sleep(2);

		//now leaving
		pthread_mutex_lock(&lock);

		//update global variables
		//send out signals to wake up vehicle(s) accordingly
    	fprintf(stderr,"\nCar #%d exited the bridge.\n", pmstrpara->vehicle_id);

		pthread_mutex_unlock(&lock);


	}


	else //truck
	{
		pthread_mutex_lock(&lock);
		//Try to cross
		//while (this vehicle cannot cross) {
		//     wait for proper moving signal
		//}


		//Now begin accrossing
		//update global variables
		//print out proper message


		sleep(2);	//delay (2)

		pthread_mutex_lock(&lock);
		//update global variables
		//send out signals to wake up vehicle(s) accordingly
		fprintf(stderr,"\nTruck #%d exited the bridge.\n", pmstrpara->vehicle_id);

		pthread_mutex_unlock(&lock);


	}



} // end of thread routine


void vehicle_arrival(pmstr_t *pmstrpara)
{
	if(pmstrpara->vehicle_type)
	{
		if (pmstrpara->direction)
			waitingcarsouth++;
		else
			waitingcarnorth++;
	}
	else
	{
		if (pmstrpara->direction)
			waitingtrucksouth++;
		else
			waitingtrucknorth++;
	}
	waitinglistinsert(pmstrpara->vehicle_id,pmstrpara->vehicle_type,pmstrpara->direction);

	if(pmstrpara->vehicle_type)
	{
		if (pmstrpara->direction)
			fprintf(stderr,"\nCar #%d (southbound) arrived.\n",pmstrpara->vehicle_id);
		else
			fprintf(stderr,"\nCar #%d (northbound) arrived.\n",pmstrpara->vehicle_id);
	}
	else
	{
		if (pmstrpara->direction)
			fprintf(stderr,"\nTruck #%d (southbound) arrived.\n",pmstrpara->vehicle_id);
		else
			fprintf(stderr,"\nTruck #%d (northbound) arrived.\n",pmstrpara->vehicle_id);
	}
} // end of vehicle_arrival

void waitinglistinsert(int vehicle_id,int vehicle_type, int direction)
{
	struct waitinglist *p;
	p=(struct waitinglist *)malloc(sizeof(struct waitinglist));
	p->vehicle_id = vehicle_id;
	p->vehicle_type = vehicle_type;
	p->direction = direction;
	p->next = pw;
	pw = p;
}

void waitinglistdelete(int vehicle_id)
{
	struct waitinglist *p, *pprevious;
	p = pw;
	pprevious = p;
	while(p)
	{
		if ((p->vehicle_id)==(vehicle_id))
			break;
		else
		{
			pprevious = p;
			p=p->next;
		}
	}

	if(p==pw)
		pw=p->next;
	else
		pprevious->next=p->next;

	free(p);
}

void movinglistinsert(int vehicle_id, int vehicle_type, int direction)
{
	struct movinglist *p;
	p=(struct movinglist *)malloc(sizeof(struct movinglist));
	p->vehicle_id = vehicle_id;
	p->vehicle_type = vehicle_type;
	p->direction = direction;
	p->next = pm;
	pm = p;

}

void movinglistdelete(int vehicle_id)
{
	struct movinglist *p, *pprevious;
	p = pm;
	pprevious = p;
	while(p)
	{
		if ((p->vehicle_id)==(vehicle_id))
			break;
		else
		{
			pprevious = p;
			p=p->next;
		}
	}

	if(p==pm)
		pm=p->next;
	else
		pprevious->next=p->next;

	free(p);
}

void printmoving()
{
	struct movinglist *p;
	p = pm;
	fprintf(stderr,"Vehicles on the bridge: [");
	while(p)
	{
		if (p->vehicle_type)
			fprintf(stderr,"Car #%d,",p->vehicle_id);
		else
			fprintf(stderr,"Truck #%d,",p->vehicle_id);
		p=p->next;
	}

	fprintf(stderr,"]\n");
	fprintf(stderr,"Now %d cars are moving.\n", movingcar);
	fprintf(stderr,"Now %d trucks are moving.\n", movingtruck);
	//fprintf(stderr,"Current moving direction: %d.\n", currentmovingdir);

}
void printwaiting()
{
	struct waitinglist *p;
	p = pw;
	fprintf(stderr,"Waiting Vehicles (northbound): [");
	while(p)
	{
		if(p->direction==0)
		{
			if (p->vehicle_type)
		 		fprintf(stderr,"Car #%d,",p->vehicle_id);
			else
				fprintf(stderr,"Truck #%d,",p->vehicle_id);
		}
		p=p->next;
	}

	fprintf(stderr,"]\n");

	p = pw;
	fprintf(stderr,"Waiting Vehicles (Southbound): [");
	while(p)
	{
		if(p->direction)
		{
			if (p->vehicle_type)
		 		fprintf(stderr,"Car #%d,",p->vehicle_id);
			else
				fprintf(stderr,"Truck #%d,",p->vehicle_id);
		}
		p=p->next;
	}

	fprintf(stderr,"]\n");

	//fprintf(stderr,"Now %d cars (south) are waiting.\n", waitingcarsouth);
	//fprintf(stderr,"Now %d cars (north) are waiting.\n", waitingcarnorth);
	//fprintf(stderr,"Now %d trucks (south) are waiting.\n", waitingtrucksouth);
	//fprintf(stderr,"Now %d trucks (north) are waiting.\n", waitingtrucknorth);
}
